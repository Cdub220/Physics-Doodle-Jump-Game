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

/**
 * Returns the body of a platform.
 *
 * @param scene the scene of the game
 * @param body the body
 * @return the index of the body
 */
ssize_t body_index_in_scene(scene_t *scene, body_t *body);

/**
 * Moves screen down when user reaches certain threshold
 *
 * @param scene the scene of the game
 * @param user the user body
 * @return void
 */
void screen_move(body_t *user, scene_t *scene);
