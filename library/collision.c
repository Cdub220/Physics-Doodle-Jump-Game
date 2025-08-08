#include "collision.h"
#include "body.h"
#include "constants.h"
#include "platforms.h"
#include "scene.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>


/**
 * Returns a list of vectors representing the edges of a shape.
 *
 * @param shape the list of vectors representing the vertices of a shape
 * @return a list of vectors representing the edges of the shape
 */
static list_t *get_edges(list_t *shape) {
  list_t *edges = list_init(list_size(shape), free);

  for (size_t i = 0; i < list_size(shape); i++) {
    vector_t *vec = malloc(sizeof(vector_t));
    assert(vec);
    *vec =
        vec_subtract(*(vector_t *)list_get(shape, i % list_size(shape)),
                     *(vector_t *)list_get(shape, (i + 1) % list_size(shape)));
    list_add(edges, vec);
  }

  return edges;
}

/**
 * Returns a vector containing the maximum and minimum length projections given
 * a unit axis and shape.
 *
 * @param shape the list of vectors representing the vertices of a shape
 * @param unit_axis the unit axis to project eeach vertex on
 * @return a vector in the form (max, min) where `max` is the maximum projection
 * length and `min` is the minimum projection length.
 */
static vector_t get_max_min_projections(list_t *shape, vector_t unit_axis) {
  // TODO: copy and paste your implementation from last week!
  double min = __DBL_MAX__;
  double max = -__DBL_MAX__;

  size_t vertexs = list_size(shape);
  for (size_t i = 0; i < vertexs; i++) {
    vector_t v = *(vector_t *)list_get(shape, i);
    double proj = vec_dot(v, unit_axis);

    if (proj < min) {
      min = proj;
    }
    if (proj > max) {
      max = proj;
    }
  }
  return (vector_t){max, min};
}

/**
 * Determines whether two convex polygons intersect.
 * The polygons are given as lists of vertices in counterclockwise order.
 * There is an edge between each pair of consecutive vertices,
 * and one between the first vertex and the last vertex.
 *
 * @param shape1 the first shape
 * @param shape2 the second shape
 * @return whether the shapes are colliding
 */
static collision_info_t compare_collision(list_t *shape1, list_t *shape2,
                                          double *min_overlap) {

  collision_info_t info = {.collided = false, .axis = {0, 0}};
  list_t *edges1 = get_edges(shape1);

  for (size_t i = 0; i < list_size(edges1); i++) {
    vector_t edge = *(vector_t *)list_get(edges1, i);
    vector_t axis = {-edge.y, edge.x};

    double len = vec_get_length(axis);

    vector_t unit_axis = {axis.x / len, axis.y / len};

    vector_t proj1 = get_max_min_projections(shape1, unit_axis);
    vector_t proj2 = get_max_min_projections(shape2, unit_axis);

    double overlap = fmin(proj1.x, proj2.x) - fmax(proj1.y, proj2.y);

    if (overlap <= 0) {
      list_free(edges1);
      return info;
    }

    if (overlap < *min_overlap) {
      *min_overlap = overlap;
      info.axis = unit_axis;
    }
  }
  list_free(edges1);
  info.collided = true;
  return info;
}

collision_info_t find_collision(body_t *body1, body_t *body2) {
  list_t *shape1 = body_get_shape(body1);
  list_t *shape2 = body_get_shape(body2);

  double c1_overlap = __DBL_MAX__;
  double c2_overlap = __DBL_MAX__;

  collision_info_t collision1 = compare_collision(shape1, shape2, &c1_overlap);
  collision_info_t collision2 = compare_collision(shape2, shape1, &c2_overlap);

  list_free(shape1);
  list_free(shape2);

  if (!collision1.collided) {
    return collision1;
  }

  if (!collision2.collided) {
    return collision2;
  }

  if (c1_overlap < c2_overlap) {
    return collision1;
  }
  return collision2;
}


/**
 * Determines whether the bottom of the user collides with a platform
 *
 * @param platforms_list the list of the platforms in the scene
 * @param scene the scene in the state
 * @param user the doodler
 * @return whether the doodler bottom collides with the top of a platform
 */
bool find_collision_with_user_bottom(list_t *platforms_list, scene_t *scene, body_t *user) {
  // body_t *user = user;
  vector_t user_center = body_get_centroid(user);
  double user_bot_y  = user_center.y - OUTER_RADIUS;
  double user_left_x = user_center.x - OUTER_RADIUS/2.0;
  double user_right_x = user_center.x + OUTER_RADIUS/2.0;

  size_t n = scene_bodies(scene);
  for (size_t i = 1; i < n; i++) {
    double body_y_velocity = body_get_velocity(user).y;
    body_t *platform = scene_get_body(scene, i);
    vector_t center_of_platform = body_get_centroid(platform);
    double plat_top_y = center_of_platform.y + PLATFORM_HEIGHT/2.0;
    double plat_left_x = center_of_platform.x - PLATFORM_WIDTH/2.0;
    double plat_right_x = center_of_platform.x + PLATFORM_WIDTH/2.0;

    if (user_left_x > plat_left_x - 5 && user_right_x < plat_right_x + 5 && fabs(plat_top_y - user_bot_y) <= 5 && body_y_velocity < 0) {
      if (strcmp(body_get_info(platform), STEADY_PLATFORM_INFO) == 0 || strcmp(body_get_info(platform), MOVING_PLATFORM_INFO) == 0) {
        SDL_play_sound(PLATFORM_BOUNCE_SOUND_PATH);
        return true;
      } 
      else if (strcmp(body_get_info(platform), BREAKING_PLATFORM_INFO) == 0) {
        asset_remove_body(platform);
        body_t *broken = make_platform(PLATFORM_WIDTH, PLATFORM_HEIGHT, center_of_platform, BREAKING_PLATFORM_INFO);
        list_add(platforms_list, broken);
        scene_add_body(scene, broken);
        asset_make_image_with_body(PLATFORM_BROKE, broken);
        SDL_play_sound(BREAKING_PLATFORM_SOUND_PATH);
        return false;
      }
    }
  }
  return false;
}


/**
 * Handles user bounce physics when collides with platform
 *
 * @param user the doodler
 * @return void
 */
void user_bounce(body_t *user){
  double x_velocity = body_get_velocity(user).x;
  body_set_velocity(user, (vector_t){x_velocity, DOODLER_BOUNCE_VELO_Y});
}
