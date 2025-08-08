#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "asset.h"
#include "asset_cache.h"
#include "sdl_wrapper.h"
#include "game_util.h"
#include "constants.h"

/**
 * Starting dot used to calulate the distance traveled for the score
 *
 * @param center center/ where the dot is
 * @return body_t of the starting dot
 */
body_t *start_dot(vector_t center);

/**
 * make the doodler body
 *
 * @param outer_radius outer radius in constructing the body of the doodler
 * @param inner_radius outer radius in constructing the body of the doodler
 * @param center the center of the doodler body
 * @return body_t of the starting dot
 */
body_t *make_user(double outer_radius, double inner_radius, vector_t center);

/**
 * wrap body if off screen
 *
 * @param body body that needs to be wrapped
 * @return void
 */
void wrap_edges(body_t *body);

/**
 * resets user to starting position
 *
 * @param body user body
 * @return void
 */
void reset_user(body_t *body);

/**
 * resets user handler
 *
 * @param body1 
 * @param body2
 * @param axis
 * @param aux
 * @return void
 */
void reset_user_handler(body_t *body1, body_t *body2, vector_t axis, void *aux, double force_const);

/**
 * resets the player
 *
 * @param scene scene of the game
 * @return void
 */
void player_reset(scene_t *scene);
