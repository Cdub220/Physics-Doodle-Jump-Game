#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "asset.h"
#include "asset_cache.h"
#include "sdl_wrapper.h"

/**
 * Returns the body of a platform.
 *
 * @param w width of platform
 * @param h height of platform
 * @param center vector coordinate of center of platform
 * @param platform_info what type of platform - steady, moving, breaking
 * @return body_t of the platform
 */
body_t *make_platform(size_t w, size_t h, vector_t center, const char *platform_info);

/**
 * Returns the info of a platform. Selects platform info between steady, moving, and breaking
 *
 * @return the info of the platform
 */
 const char *platform_select();

/**
 * Creates initial platforms when the game starts.
 * 
 * @param state the state 
 * @return void
 */
void platforms_init(list_t *platforms_list, scene_t *scene);

/**
 * Creates platforms to replace the platforms that go off the bottom of the screen when the screen moves.
 *
 * @param state the state 
 * @return void
 */
void screen_move_platforms_create(list_t *platforms_list, scene_t *scene, int16_t score);

/**
 * Removes platforms that leave the screen when the screen moves up.
 *
 * @param state the state 
 * @return void
 */
void remove_platform(list_t *platforms_list, scene_t *scene);

/**
 * Bounces the platforms off the sides of the screen when they reach them.
 *
 * @param state the state 
 * @return void
 */
void platforms_bounce_off_wall(scene_t *scene);
