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
#include "villain.h"

#include "platforms.h"
#include "game_util.h"
#include "constants.h"
#include "player_util.h"


struct state {
  body_t *user;
  scene_t *scene;
  int16_t score;

  list_t *platforms_list;
  body_t *villain;

  bool game_over;
  double timer;
};

void on_key(char key, key_event_type_t type, double held_time, void *state) {
  state_t *s = (state_t *)state;
  body_t *user = s->user;
  double y_velocity = body_get_velocity(user).y;
  if (type == KEY_PRESSED) {
    switch (key) {
      case LEFT_ARROW:
        body_set_velocity(user, (vector_t){-150 - DOODLE_LR_VELO * held_time, y_velocity});
        break;
      case RIGHT_ARROW:
        body_set_velocity(user, (vector_t){150 + DOODLE_LR_VELO * held_time, y_velocity});
        break;
      default: break;
    }
  }
  else if (type == KEY_RELEASED) {
    switch (key) {
      case LEFT_ARROW:
        body_set_velocity(user, (vector_t){0, y_velocity});
        break;
      case RIGHT_ARROW:
        body_set_velocity(user, (vector_t){0, y_velocity});
        break;
      default: break;
    }
  }
}


void update_score(state_t *state){
  char score[32];  
  sprintf(score, "Score: %d", state->score);

  TTF_Font *font = TTF_OpenFont(FONT, 30);
  SDL_Color color = {0, 0, 0};
  SDL_Surface *message = TTF_RenderText_Solid(font, score, color);
  SDL_Rect *font_rect = sdl_get_rect(FONT_POSITION.x, FONT_POSITION.y, FONT_SIZE.x, FONT_SIZE.y);
  sdl_render_text(message, font_rect);

  SDL_FreeSurface(message);
  free(font_rect);
  TTF_CloseFont(font);
}

void calculate_score(state_t *state){
  body_t *start_dot = scene_get_body(state->scene, 1);
  double height = body_get_centroid(start_dot).y;
  state->score = height * -1;
  
  update_score(state);
}

void reset_game(state_t *state){

  // Reset Score
  body_t *starting_dot = scene_get_body(state->scene, 1);
  body_set_centroid(starting_dot, VEC_ZERO);
  state->score = 0;

  // Resets User Position
  body_set_centroid(state->user, START_POS);

  //Removes Villain & Bullets 
  if (state->villain != NULL){

    for (ssize_t i = scene_bodies(state->scene) - 1; i >= 0; i--){
      body_t *body = scene_get_body(state->scene, i);
      void *info = body_get_info(body);
      if(info != NULL && strcmp((char *)info, BULLET_INFO) == 0){
        scene_remove_body(state->scene, i);
        asset_remove_body(body);
      }
    }
    scene_remove_body(state->scene, body_index_in_scene(state->scene, state->villain));
    asset_remove_body(state->villain);
    state->villain = NULL;
  }
  // Reset Plateforms
  ssize_t platform_count = list_size(state->platforms_list);
  for (ssize_t i = 0; i < platform_count; i++) {
    body_t *platform = list_get(state->platforms_list, i);
      scene_remove_body(state->scene, body_index_in_scene(state->scene, platform));
      list_remove(state->platforms_list, i);
      platform_count--;
      i--;
  }

  platforms_init(state->platforms_list, state->scene);
}


void gameover_screen(state_t *state){
  sdl_clear();
  double delta = time_since_last_tick();
  state->timer += delta;

  char over_top[32];
  char over_bottom[32];  

  sprintf(over_top, "GAME OVER");
  sprintf(over_bottom, "Score: %d", state->score);

  TTF_Font *font = TTF_OpenFont(GAMEOVER_FONT, 30);
  SDL_Color color = {0, 0, 0};

  SDL_Surface *message_top = TTF_RenderText_Solid(font, over_top, color);
  SDL_Surface *message_bottom = TTF_RenderText_Solid(font, over_bottom, color);

  SDL_Rect *font_rect_top = sdl_get_rect(MAX.x / 4.4, MAX.y/3.5, FONT_SIZE.x * 3, FONT_SIZE.y * 3);
  SDL_Rect *font_rect_bottom = sdl_get_rect(MAX.x / 4.4, (MAX.y/3.5) + (FONT_SIZE.y * 3), FONT_SIZE.x * 3, FONT_SIZE.y * 3);

  sdl_render_text(message_top, font_rect_top);
  sdl_render_text(message_bottom, font_rect_bottom);

  sdl_show();

  if (state->timer > 3.0){
    reset_game(state);
    state->game_over = false;
  }
  
  SDL_FreeSurface(message_top);
  SDL_FreeSurface(message_bottom);

  free(font_rect_top);
  free(font_rect_bottom);

  TTF_CloseFont(font);
}

bool check_game_over(state_t *state){
  if (body_get_centroid(state->user).y < 0){
    state->game_over = true;
    state->timer = 0;
    return true;
  }

  else if (check_villain_bullet_collision(state->scene, state->user)){
    state->game_over = true;
    state->timer = 0;
    return true;
  }
  return false;
}

state_t *emscripten_init() {
  asset_cache_init();
  sdl_init(MIN, MAX);

  state_t *state = malloc(sizeof(state_t));
  state->score = 0;
  srand(time(NULL));
  state->scene = scene_init();

  state->game_over = false;
  state->timer = 0;

  // Creates user and initial velocity
  body_t *user = make_user(OUTER_RADIUS, INNER_RADIUS, VEC_ZERO);
  body_set_centroid(user, START_POS);
  state->user = user;
  scene_add_body(state->scene, user);

  // Create starting position dot
  body_t *starting_dot = start_dot(VEC_ZERO);
  scene_add_body(state->scene, starting_dot);

  // Assigns images to their related bodies
  asset_make_image(BACKGROUND_PATH, (SDL_Rect){MIN.x, MIN.y, MAX.x, MAX.y});
  asset_make_image_with_body(USER_PATH, user);

  // init platform list
  state->platforms_list = list_init(TOTAL_PLATFORMS, NULL);

  // init platforms
  platforms_init(state->platforms_list, state->scene);

  sdl_on_key(on_key);
  TTF_Init();
  //initalize background music
  Mix_Init(0);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
  SDL_play_music(BACKGROUND_MUSIC_PATH);

  //initalize villain to NULL
  state->villain = NULL;

  return state;
}

bool emscripten_main(state_t *state) {
  // Shows game over screen
  static bool game_over_sound_played = false;

  if (state->game_over == true){
    if(!game_over_sound_played){
      SDL_play_sound(GAME_OVER_SOUND_PATH);
      game_over_sound_played = true;
    }
    gameover_screen(state);
    return false;
  } else {
    game_over_sound_played = false;
  }
  double dt = time_since_last_tick();

  // apply gravity + most recent velocity
  vector_t user_velocity = body_get_velocity(state->user);                     
  body_set_velocity(state->user, (vector_t){user_velocity.x, user_velocity.y - ACC * dt});

  // advance all physics in scene
  scene_tick(state->scene, dt);
 
  //updates villain conditions relative to the game 
  update_villain(&(state->villain), state->score, state->scene, dt);

  if (find_collision_with_user_bottom(state->platforms_list, state->scene, state->user)) {
    user_bounce(state->user);
  }

  screen_move(state->user, state->scene);
  remove_platform(state->platforms_list, state->scene);
  screen_move_platforms_create(state->platforms_list, state->scene, state->score);
  
  platforms_bounce_off_wall(state->scene);

  // User wrap edges
  wrap_edges(state->user);

  //Check if game is over
  if (check_game_over(state) == true){
    return false;
  }

  sdl_clear();
  sdl_render_scene(state->scene);
  list_t *body_assets = asset_get_asset_list();
  
  for (size_t i = 0; i < list_size(body_assets); i++) {
    asset_render(list_get(body_assets, i));
  }

  calculate_score(state);

  sdl_show();
  return false;
}

void emscripten_free(state_t *state) {
  list_free(asset_get_asset_list());
  scene_free(state->scene);
  asset_cache_destroy();
  free(state);
}