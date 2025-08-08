#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "body.h"
#include "list.h"
#include "vector.h"
#include "scene.h"
#include <stdbool.h>

/**
 * Represents the status of a collision between two shapes.
 * The shapes are either not colliding, or they are colliding along some axis.
 */
typedef struct {
  /** Whether the two shapes are colliding */
  bool collided;
  /**
   * If the shapes are colliding, the axis they are colliding on.
   * This is a unit vector pointing from the first shape towards the second.
   * Normal impulses are applied along this axis.
   * If collided is false, this value is undefined.
   */
  vector_t axis;
} collision_info_t;

/**
 * Computes the status of the collision between two bodies.
 *
 * @param body1 the first body
 * @param body2 the second body
 * @return whether the shapes are colliding, and if so, the collision axis.
 * The axis should be a unit vector pointing from shape1 towards shape2.
 */
collision_info_t find_collision(body_t *body1, body_t *body2);

/**
 * Determines whether the bottom of the user collides with a platform
 *
 * @param platforms_list the list of the platforms in the scene
 * @param scene the scene in the state
 * @param user the doodler
 * @return whether the doodler bottom collides with the top of a platform
 */
bool find_collision_with_user_bottom(list_t *platforms_list, scene_t *scene, body_t *user);

/**
 * Handles user bounce physics when collides with platform
 *
 * @param user the doodler
 * @return void
 */
void user_bounce(body_t *user);

#endif // #ifndef __COLLISION_H__
