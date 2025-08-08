#ifndef __VILLAIN_H__
#define __VILLAIN_H__

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
#include "state.h"

/**
 * Creates the body for the villain inside of the scene with VILLAIN_INFO
 * 
 * @param radius the radius of the body of the villain
 * @param center the center of the body of the villain inside of the scene
 * @return a pointer to the newly allocated villain body 
 */
body_t *make_villain(double radius, vector_t center);

/**
 * Initializes the villain body inside of the scene along
 * with the asset of the villain image HAS TO BE CHANGED 
 * 
 * @param state the state of where we want to initalize the villain body.
 * @param villain a double pointer to the newley created villain body.
 */
void villain_init(scene_t *scene, body_t **villain);

/**
 * Initalizes a hover effect from left to right on the 
 * designated villain body within the bounds of the scene.
 * 
 * @param villain a double pointer to the designated villain body.
 */
void villain_hover(body_t **villain);

/**
 * Creates the body for the bulltet outputted by the villain 
 * relative to his position inside of the scene.
 * 
 * @param radius the desired radius of the body of the bullet
 * @param center the center position of the body of the bullet
 * @return a pointer to the newly allocated bullet body
 */
body_t *make_bullet(double radius, vector_t center);

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
void villain_shoot_bullet(scene_t *scene, body_t *villain, uint16_t score);

/**
 * Scans the scene for any bullet bodies that have gone
 * outside of the bounds of the screen and remvoes them
 * from the scene
 * 
 * @param scene the scene of the game
 */
void remove_offscreen_bullets(scene_t *scene);

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
bool check_villain_bullet_collision(scene_t *scene, body_t *user);

/**
 * Checks the condition of the villain in every 
 * time the function is called and utilizes the
 * neccessary helper fucntions on the villain.abort
 * 
 * @param villain a double pointer to the villain of the state
 * @param score the current score of the game
 * @param scene the scene of the game 
 * @param dt the rate at which the time of the game is changing
 * 
 */
void update_villain(body_t **villain, uint16_t score, scene_t *scene, double dt);

#endif // __VILLAIN_H__