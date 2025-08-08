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

// vertical frame for doodle jump
extern const vector_t MIN;
extern const vector_t MAX;
extern const double SCREEN_MOVE_THRESHOLD;


// doodler start coords
extern const vector_t START_POS;
extern const vector_t RESET_POS;

// velocities for doodler
extern const vector_t DOODLER_START_VELO;
extern const double DOODLER_BOUNCE_VELO_Y;
extern double DOODLE_LR_VELO;
extern const double ACC;

// doodler constants
extern const double OUTER_RADIUS;
extern const double INNER_RADIUS;
extern const size_t USER_NUM_POINTS;
extern const color_t USER_COLOR;
extern const char *USER_PATH;


// villain constants
extern const double VILLAIN_RADIUS;
extern const size_t VILLAIN_NUM_POINTS;
extern const vector_t VILLAIN_START_POS;
extern const color_t VILLAIN_COLOR;
extern const char *VILLAIN_PATH;
extern const char *VILLAIN_INFO;
extern const vector_t HOVER_RIGHT;
extern const vector_t HOVER_LEFT;


//bullet constants
extern const double BULLET_RADIUS;
extern const double BULLET_NUM_POINTS;
extern const color_t BULLET_COLOR;
extern const char *BULLET_INFO;
extern const char *BULLET_PATH;
extern const vector_t BULLET_VELOCITY;


// velocity stuff for platforms
extern const vector_t BASE_OBJ_VEL;
extern const double EXTRA_VEL_MULT;
extern const double VEL_MULT_PROB;

// platform info
extern const size_t PLATFORM_HEIGHT;
extern const size_t PLATFORM_WIDTH;

extern const char *STEADY_PLATFORM_INFO;
extern const char *STEADY_PLATFORM_PATH;
extern const char *MOVING_PLATFORM_INFO;
extern const char *MOVING_PLATFORM_PATH;
extern const char *BREAKING_PLATFORM_INFO;
extern const char *BREAKING_PLATFORM_PATH;
extern const char *BROKEN_PLATFORM_INFO;
extern const char *BROKEN_PLATFORM_PATH;
extern const char *PLATFORM_BROKE;

// platform init stuff
extern const size_t NUM_PLATFORM_CHANNELS;
extern const size_t CHANNEL_HEIGHT;
extern const double PCT_MOVING;
extern const double PCT_STEADY;
extern const double PCT_BREAKING;
extern const size_t TOTAL_PLATFORMS;
extern const size_t PLATFORMS_PER_CHANNEL;
extern const vector_t FIRST_PLATFORM_LOC;
extern const color_t OBS_COLOR;


// background
extern const char *BACKGROUND_PATH;

// Font constants
extern const vector_t FONT_POSITION;
extern const vector_t FONT_SIZE;
extern const char *GAMEOVER_FONT;
extern const char *FONT;

//Music Constants
extern const char *BACKGROUND_MUSIC_PATH;
extern const char *BULLET_SOUND_PATH;
extern const char *BREAKING_PLATFORM_SOUND_PATH;
extern const char *PLATFORM_BOUNCE_SOUND_PATH;
extern const char *VILLAIN_SPAWN_SOUND_PATH;
extern const char *GAME_OVER_SOUND_PATH;
extern const char *USER_DEATH_SOUND_PATH;