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
 * Starting dot used to calulate the distance traveled for the score
 *
 * @param center center/ where the dot is
 * @return body_t of the starting dot
 */
body_t *start_dot(vector_t center){
  const double DOT_RADIUS = 1.0;
  const size_t NUM_POINTS = 12;

  list_t *c = list_init(NUM_POINTS, free);
  for (size_t i = 0; i < NUM_POINTS; i++){
    double angle = 2 * M_PI * i / NUM_POINTS;
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){center.x + DOT_RADIUS * cos(angle),
                    center.y + DOT_RADIUS * sin(angle)};
    list_add(c, v);
  }
  body_t *dot = body_init(c, 1, USER_COLOR);
  return dot;
}

/**
 * make the doodler body
 *
 * @param outer_radius outer radius in constructing the body of the doodler
 * @param inner_radius outer radius in constructing the body of the doodler
 * @param center the center of the doodler body
 * @return body_t of the starting dot
 */
body_t *make_user(double outer_radius, double inner_radius, vector_t center) {
  center.y += inner_radius;
  list_t *c = list_init(USER_NUM_POINTS, free);
  for (size_t i = 0; i < USER_NUM_POINTS; i++) {
    double angle = 2 * M_PI * i / USER_NUM_POINTS;
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){center.x + inner_radius * cos(angle),
                    center.y + outer_radius * sin(angle)};
    list_add(c, v);
  }
  body_t *user = body_init(c, 1, USER_COLOR);
  return user;
}

/**
 * wrap body if off screen
 *
 * @param body body that needs to be wrapped
 * @return void
 */
void wrap_edges(body_t *body) {
  vector_t centroid = body_get_centroid(body);
  if (centroid.x > MAX.x) {
    body_set_centroid(body, (vector_t){MIN.x, centroid.y});
  } else if (centroid.x < MIN.x) {
    body_set_centroid(body, (vector_t){MAX.x, centroid.y});
  }
}

/**
 * resets user to starting position
 *
 * @param body user body
 * @return void
 */
void reset_user(body_t *body) { body_set_centroid(body, START_POS); }

/**
 * resets user handler
 *
 * @param body1 
 * @param body2
 * @param axis
 * @param aux
 * @return void
 */
void reset_user_handler(body_t *body1, body_t *body2, vector_t axis, void *aux,
                        double force_const) {
  reset_user(body1);
}

/**
 * resets the player
 *
 * @param scene scene of the game
 * @return void
 */
void player_reset(scene_t *scene) {
  body_t *player = scene_get_body(scene, 0);
  reset_user(player);
}
