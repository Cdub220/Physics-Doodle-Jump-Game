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
#include"state.h"
#include "villain.h"

/**
 * Creates the body for the villain inside of the scene with VILLAIN_INFO
 * 
 * @param radius the radius of the body of the villain
 * @param center the center of the body of the villain inside of the scene
 * @return a pointer to the newly allocated villain body 
 */
body_t *make_villain(double radius, vector_t center){

  list_t *villain_shape  = list_init(VILLAIN_NUM_POINTS, free);

  for (size_t i = 0; i < VILLAIN_NUM_POINTS; i++){
    double angle = 2 * M_PI * i / VILLAIN_NUM_POINTS;
    vector_t *v = malloc(sizeof(vector_t));
    *v = (vector_t){center.x + radius * cos(angle),
                    center.y + radius * sin(angle)};
    list_add(villain_shape, v);
  }
  
  body_t *villain_body = body_init_with_info(villain_shape, 1, VILLAIN_COLOR, 
                        (void *)VILLAIN_INFO, NULL);
  return villain_body;
}

/**
 * Initializes the villain body inside of the scene along
 * with the asset of the villain image HAS TO BE CHANGED 
 * 
 * @param state the state of where we want to initalize the villain body.
 * @param villain a double pointer to the newley created villain body.
 */
void villain_init(scene_t *scene, body_t **villain){
    *villain = make_villain(VILLAIN_RADIUS, VILLAIN_START_POS);
    body_set_velocity(*villain, HOVER_RIGHT);
    scene_add_body(scene, *villain);
    asset_make_image_with_body(VILLAIN_PATH, *villain);
    SDL_play_sound(VILLAIN_SPAWN_SOUND_PATH);
}

/**
 * Initalizes a hover effect from left to right on the 
 * designated villain body within the bounds of the scene.
 * 
 * @param villain a double pointer to the designated villain body.
 */
void villain_hover(body_t **villain){
    vector_t villain_centroid = body_get_centroid(*villain);
    double villain_right_bound  = villain_centroid.x + VILLAIN_RADIUS;
    double villain_left_bound = villain_centroid.x - VILLAIN_RADIUS;

    if (villain_right_bound > MAX.x){
        body_set_velocity(*villain, HOVER_LEFT);
    }
    else if (villain_left_bound < MIN.x){
        body_set_velocity(*villain, HOVER_RIGHT);
    }
}

/**
 * Creates the body for the bulltet outputted by the villain 
 * relative to his position inside of the scene.
 * 
 * @param radius the desired radius of the body of the bullet
 * @param center the center position of the body of the bullet
 * @return a pointer to the newly allocated bullet body
 */
body_t *make_bullet(double radius, vector_t center){
  list_t *bullet_shape  = list_init(BULLET_NUM_POINTS, free);

    for (size_t i = 0; i < BULLET_NUM_POINTS; i++){
      double angle = 2 * M_PI * i / BULLET_NUM_POINTS;
      vector_t *v = malloc(sizeof(vector_t));
      *v = (vector_t){center.x + radius * cos(angle),
                      center.y + radius * sin(angle)};
      list_add(bullet_shape, v);
    }
    
    body_t *bullet_body = body_init_with_info(bullet_shape, 1, BULLET_COLOR, 
                          (void *)BULLET_INFO, NULL);
    return bullet_body;
}

/**
 * Creates an asset to place on the bullet body and shoots
 * the bullet by creating a downward velocity relative to the 
 * score of the game.
 * 
 * @param scene the scene of the game the bullet is in 
 * @param villain the villain of the game
 * @param score the score of the game 
 * 
 */
void villain_shoot_bullet(scene_t *scene, body_t *villain, uint16_t score){
    vector_t villain_center = body_get_centroid(villain);
    vector_t bullet_pos = {villain_center.x, villain_center.y - VILLAIN_RADIUS - BULLET_RADIUS};
    body_t *bullet = make_bullet(BULLET_RADIUS, bullet_pos);

    double multiplier = 1.0;

    if (score > 10000) {
        multiplier = 5.0;
    }
    else if (score >= 8000) {
        multiplier = 4.0;
    }
    else if (score >= 6000) {
        multiplier = 3.0;
    }
    else if (score >= 4000) {
        multiplier = 2.0;
    }
    else if (score >= 2000) {
        multiplier = 1.0;
    }

    vector_t final_velocity = vec_multiply(multiplier, BULLET_VELOCITY);
    body_set_velocity(bullet, final_velocity);
    scene_add_body(scene, bullet);
    asset_make_image_with_body(BULLET_PATH, bullet);
    SDL_play_sound(BULLET_SOUND_PATH);
}

/**
 * Scans the scene for any bullet bodies that have gone
 * outside of the bounds of the screen and remvoes them
 * from the scene
 * 
 * @param scene the scene of the game
 */
void remove_offscreen_bullets(scene_t *scene){
    for (ssize_t i = scene_bodies(scene) - 1; i >= 0; i--) {
        body_t *body = scene_get_body(scene, i);
        void *info = body_get_info(body);

        if (info != NULL && strcmp((char *)info, BULLET_INFO) == 0) {
            vector_t bullet_pos = body_get_centroid(body);
            if (bullet_pos.y - BULLET_RADIUS < MIN.y) {
                scene_remove_body(scene, i);
                asset_remove_body(body);
            }
        }
    }
}

/**
 * Scans the scene for a collision between any of the bullets
 * in the scene and the user.
 * 
 * @param scene the scene that we are scanning for collisions
 * @param user the body of the user we are looking at for collisions
 * 
 * @return a boolean value of either true of false indicating the
 * detection of a collision
 */
bool check_villain_bullet_collision(scene_t *scene, body_t *user){
    for (size_t i = 1; i < scene_bodies(scene); i++){
        body_t *body = scene_get_body(scene, i);
        void *info = body_get_info(body);

        if (info != NULL && (strcmp((char *)info, BULLET_INFO) == 0)){
        collision_info_t collision_info = find_collision(user, body);

        if (collision_info.collided == true){
            SDL_play_sound(USER_DEATH_SOUND_PATH);
            return true;
            }
        }
    }
    return false;
}

/**
 * Checks the condition of the villain in every 
 * time the function is called and utilizes the
 * neccessary helper fucntions on the villain.abort
 * 
 * @param villain a double pointer to the villain of the state
 * @param score the current score of the game
 * @param scene the scene of the game 
 * @param dt the rate at which the game is changing
 * 
 */
void update_villain(body_t **villain, uint16_t score, scene_t *scene, double dt){
    static double bullet_cooldown = 0.0;
    if (*villain == NULL && score >= 2000){
        villain_init(scene, villain);
    }

    if (*villain != NULL){
        remove_offscreen_bullets(scene);
        villain_hover(villain);
        bullet_cooldown += dt;

        if (bullet_cooldown >= 3.0){
            villain_shoot_bullet(scene, *villain, score);
            bullet_cooldown = 0.0;
        }
    }
}
