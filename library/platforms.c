#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "asset.h"
#include "asset_cache.h"
#include "game_util.h"
#include "constants.h"


/**
 * Returns the body of a platform.
 *
 * @param w width of platform
 * @param h height of platform
 * @param center vector coordinate of center of platform
 * @param platform_info what type of platform - steady, moving, breaking
 * @return body_t of the platform
 */
body_t *make_platform(size_t w, size_t h, vector_t center, const char *platform_info) {
  list_t *c = list_init(4, free);
  vector_t *v1 = malloc(sizeof(vector_t));
  *v1 = (vector_t){0, 0};
  list_add(c, v1);

  vector_t *v2 = malloc(sizeof(vector_t));
  *v2 = (vector_t){w, 0};
  list_add(c, v2);

  vector_t *v3 = malloc(sizeof(vector_t));
  *v3 = (vector_t){w, h};
  list_add(c, v3);

  vector_t *v4 = malloc(sizeof(vector_t));
  *v4 = (vector_t){0, h};
  list_add(c, v4);
  body_t *obstacle = body_init_with_info(c, 1, OBS_COLOR, (void *)platform_info, NULL);
  body_set_centroid(obstacle, center);
  return obstacle;
}

/**
 * Returns the info of a platform
 *
 * @return the info of the platform
 */
const char *platform_select() {
  float r = (float)rand() / (float)RAND_MAX;
  if (r < PCT_MOVING) {
    return MOVING_PLATFORM_INFO;
  }
  else if (r < PCT_MOVING + PCT_STEADY) {
    return STEADY_PLATFORM_INFO;
  }
  else {
    return BREAKING_PLATFORM_INFO;
  }
}

/**
 * Creates initial platforms when the game starts.
 * 
 * @param state the state 
 * @return void
 */
void platforms_init(list_t *platforms_list, scene_t *scene) {
  body_t *first_platform = make_platform(PLATFORM_WIDTH, PLATFORM_HEIGHT, FIRST_PLATFORM_LOC, STEADY_PLATFORM_INFO);
  scene_add_body(scene, first_platform);
  asset_make_image_with_body(STEADY_PLATFORM_PATH, first_platform);
  list_add(platforms_list, first_platform);

  for (size_t i = 0; i < NUM_PLATFORM_CHANNELS; i++) {
    size_t y_min = CHANNEL_HEIGHT * i;
    size_t y_max = y_min + CHANNEL_HEIGHT;
    for (size_t i = 0; i < PLATFORMS_PER_CHANNEL; i++) {
      size_t x_position = (size_t)(((rand() / (double)RAND_MAX) * (MAX.x - PLATFORM_WIDTH)) + 0.5*PLATFORM_WIDTH);;
      size_t y_position = (size_t)(y_min + (rand() / (double)RAND_MAX) * (y_max - y_min));
      const char *platform_type = platform_select();
      body_t *platform = make_platform(PLATFORM_WIDTH, PLATFORM_HEIGHT, (vector_t){x_position, y_position}, platform_type);
      scene_add_body(scene, platform);
      if (strcmp(platform_type, STEADY_PLATFORM_INFO) == 0) {
        asset_make_image_with_body(STEADY_PLATFORM_PATH, platform);
      } 
      else if (strcmp(platform_type, BREAKING_PLATFORM_INFO) == 0) {
        asset_make_image_with_body(BREAKING_PLATFORM_PATH, platform);
      } 
      else if (strcmp(platform_type, MOVING_PLATFORM_INFO) == 0) {
        asset_make_image_with_body(MOVING_PLATFORM_PATH, platform);
        body_set_velocity(platform, BASE_OBJ_VEL);
      }
      list_add(platforms_list, platform);
    }
  }
}

/**
 * Creates platforms to replace the platforms that go off the bottom of the screen when the screen moves.
 *
 * @param state the state 
 * @return void
 */
void screen_move_platforms_create(list_t *platforms_list, scene_t *scene, int16_t score) {
  double max_platform_y = MIN.y;
  size_t n = list_size(platforms_list);
  for (size_t i = 0; i < n; i++) {
    body_t *platform = list_get(platforms_list, i);
    size_t plat_y_coord = body_get_centroid(platform).y;
    if (plat_y_coord > max_platform_y) {
      max_platform_y = plat_y_coord;
    }
  }
  size_t num_platforms = list_size(platforms_list);
  if (num_platforms < TOTAL_PLATFORMS) {
    size_t NUM_NEW_PLATFORMS = TOTAL_PLATFORMS - num_platforms;
    for (size_t i = 0; i < NUM_NEW_PLATFORMS; i++) {
      const char *plat_info = platform_select();
      size_t x_position = (size_t)(((rand() / (double)RAND_MAX) * (MAX.x - PLATFORM_WIDTH)) + 0.5*PLATFORM_WIDTH);
      size_t y_position = (size_t)(max_platform_y + (rand() / (double)RAND_MAX) * (MAX.y - max_platform_y));
      body_t *new_platform = make_platform(PLATFORM_WIDTH, PLATFORM_HEIGHT, (vector_t){x_position, y_position}, plat_info);
      scene_add_body(scene, new_platform);

      if (strcmp(plat_info, STEADY_PLATFORM_INFO) == 0) {
        asset_make_image_with_body(STEADY_PLATFORM_PATH, new_platform);
      } else if (strcmp(plat_info, BREAKING_PLATFORM_INFO) == 0) {
        asset_make_image_with_body(BREAKING_PLATFORM_PATH, new_platform);
      } else if (strcmp(plat_info, MOVING_PLATFORM_INFO) == 0) {
        asset_make_image_with_body(MOVING_PLATFORM_PATH, new_platform);
        body_set_velocity(new_platform, vec_multiply((double)(score / 2000) ,BASE_OBJ_VEL));
      }
      list_add(platforms_list, new_platform);
    }
  }
}

/**
 * Removes platforms that leave the screen when the screen moves up.
 *
 * @param state the state 
 * @return void
 */
void remove_platform(list_t *platforms_list, scene_t *scene) {
  ssize_t platform_count = list_size(platforms_list);
  for (ssize_t i = 0; i < platform_count; i++) {
    body_t *platform = list_get(platforms_list, i);
    double y = body_get_centroid(platform).y;
    if (y <= MIN.y) {
      scene_remove_body(scene, body_index_in_scene(scene, platform));
      list_remove(platforms_list, i);
      platform_count--;
      i--;
    } 
  }

}

/**
 * Bounces the platforms off the sides of the screen when they reach them.
 *
 * @param state the state 
 * @return void
 */
void platforms_bounce_off_wall(scene_t *scene) {
  size_t n = scene_bodies(scene);
  for (size_t i = 0; i < n; i++) {
    body_t *body = scene_get_body(scene, i);
    if (body_get_info(body) && strcmp(body_get_info(body), MOVING_PLATFORM_INFO) == 0) {
      if (body_get_centroid(body).x + PLATFORM_WIDTH/2.0 >= MAX.x || body_get_centroid(body).x - PLATFORM_WIDTH/2.0 <= MIN.x) {
        vector_t vel = body_get_velocity(body);
        body_set_velocity(body, vec_multiply(-1, vel));
      }
    }
  }
}
