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

// vertical frame for doodle jump
const vector_t MIN = {0, 0};
const vector_t MAX = {500, 750};
const double SCREEN_MOVE_THRESHOLD = 0.55 * MAX.y;

// doodler start coords
const vector_t START_POS = {MAX.x/2, 0.40 * MAX.y};
const vector_t RESET_POS = {250, 300};

// velocities for doodler
const vector_t DOODLER_START_VELO = {0, -40};
const double DOODLER_BOUNCE_VELO_Y = 400;
double DOODLE_LR_VELO = 250;
const double ACC = 300;

// doodler constants
const double OUTER_RADIUS = 20;
const double INNER_RADIUS = 20;
const size_t USER_NUM_POINTS = 20;
const color_t USER_COLOR = (color_t){0.1, 0.9, 0.2};
const char *USER_PATH = "assets/doodle.png";


// villain constants
const double VILLAIN_RADIUS = 40;
const size_t VILLAIN_NUM_POINTS = 20;
const vector_t VILLAIN_START_POS = {MAX.x/2 , MAX.y - 50 };
const color_t VILLAIN_COLOR = (color_t){0, 0, 1};
const char *VILLAIN_PATH = "assets/villain.png";
const char *VILLAIN_INFO = "villain";
const vector_t HOVER_RIGHT = {100, 0};
const vector_t HOVER_LEFT = {-100,0};


//bullet constants
const double BULLET_RADIUS = 15;
const double BULLET_NUM_POINTS = 20;
const color_t BULLET_COLOR = (color_t){0, 0.5, 1};
const char *BULLET_INFO = "bullet";
const char *BULLET_PATH = "assets/bullet.png";
const vector_t BULLET_VELOCITY = {0, -50};


// velocity stuff for platforms
const vector_t BASE_OBJ_VEL = {50, 0};
const double EXTRA_VEL_MULT = 10;
const double VEL_MULT_PROB = 0.2;

// platform info
const size_t PLATFORM_HEIGHT = 20;
const size_t PLATFORM_WIDTH = 60;

const char *STEADY_PLATFORM_INFO = "steady";
const char *STEADY_PLATFORM_PATH = "assets/steady_platform.png";
const char *MOVING_PLATFORM_INFO = "moving";
const char *MOVING_PLATFORM_PATH = "assets/moving_platform.png";
const char *BREAKING_PLATFORM_INFO = "breaking";
const char *BREAKING_PLATFORM_PATH = "assets/breaking_platform.png";
const char *BROKEN_PLATFORM_INFO = "broken";
const char *BROKEN_PLATFORM_PATH = "assets/broken_platform.png";
const char *PLATFORM_BROKE = "assets/plateform-broke.png";

// platform init stuff
const size_t NUM_PLATFORM_CHANNELS = 4;
const size_t CHANNEL_HEIGHT = MAX.y / NUM_PLATFORM_CHANNELS;
const double PCT_MOVING = 0.3;
const double PCT_STEADY = 0.6;
const double PCT_BREAKING = 0.1;
const size_t TOTAL_PLATFORMS = 24;
const size_t PLATFORMS_PER_CHANNEL = TOTAL_PLATFORMS/NUM_PLATFORM_CHANNELS;
const vector_t FIRST_PLATFORM_LOC = {MAX.x/2, 0.3 * MAX.y - OUTER_RADIUS};
const color_t OBS_COLOR = (color_t){0.2, 0.2, 0.3};


// background
const char *BACKGROUND_PATH = "assets/background.png";

// Font constants
const vector_t FONT_POSITION = {0, 0};
const vector_t FONT_SIZE = {100, 30};
const char *GAMEOVER_FONT = "assets/Roboto-Regular.ttf";
const char *FONT = "assets/Doodle-Font.ttf";

//Music constants
const char *BACKGROUND_MUSIC_PATH = "assets/background_music.wav";
const char *BULLET_SOUND_PATH = "assets/bullet.wav";
const char *BREAKING_PLATFORM_SOUND_PATH = "assets/breaking_platform.wav";
const char *PLATFORM_BOUNCE_SOUND_PATH = "assets/platform_bounce.wav";
const char *VILLAIN_SPAWN_SOUND_PATH = "assets/villain_spawn.wav";
const char *GAME_OVER_SOUND_PATH = "assets/game_over.wav";
const char *USER_DEATH_SOUND_PATH = "assets/user_death.wav";