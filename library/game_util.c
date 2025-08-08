#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "asset.h"
#include "asset_cache.h"
#include "collision.h"
#include "forces.h"
#include "sdl_wrapper.h"
#include "constants.h"
#include "scene.h"

/**
 * Returns the body of a platform.
 *
 * @param scene the scene of the game
 * @param body the body
 * @return the index of the body
 */
ssize_t body_index_in_scene(scene_t *scene, body_t *body) {
  for (ssize_t i = 0; i < scene_bodies(scene); i++) {
    if (scene_get_body(scene, i) == body) {
      return i;
    }
  }
  return -1;
}

/**
 * Moves screen down when user reaches certain threshold
 *
 * @param scene the scene of the game
 * @param user the user body
 * @return void
 */
void screen_move(body_t *user, scene_t *scene) {
  double old_body_centroid_y = body_get_centroid(user).y;
  if (old_body_centroid_y > SCREEN_MOVE_THRESHOLD) {
    // double new_body_centroid_y = body_get_centroid(state->user).y;
    double y_dist = fabs(old_body_centroid_y - SCREEN_MOVE_THRESHOLD);
    for (size_t i = 1; i < scene_bodies(scene); i++) {
      body_t *object = scene_get_body(scene, i);
      void *info = body_get_info(object);
 
      if (info == NULL || strcmp((char *)info, VILLAIN_INFO) != 0 ){
        vector_t current = body_get_centroid(object);
        body_set_centroid(object, (vector_t){current.x, current.y - y_dist});
      }
    }
    vector_t doodler_curr_centroid = body_get_centroid(user);
    vector_t doodler_new_centroid = (vector_t){doodler_curr_centroid.x, doodler_curr_centroid.y - y_dist};
    body_set_centroid(user, doodler_new_centroid);
  }
}
