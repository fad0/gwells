//
//  constants.h
//  astro
//
//  Created by Dirk Reese on 2/14/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <GL/glew.h>
#include <glfw3.h>

//// Header file
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Properties
extern const GLuint WIDTH = 800, HEIGHT = 800;
// Screen behavior parameters
extern const GLfloat x_modulo = 1.0; // Wrap the scene in the x-dimension

extern const GLuint maxPhotons = 5;

extern const GLfloat smallShape = 0.025f;

// Ship consts
extern const GLuint initialShipCount = 10;
extern const GLuint totalExplodePieces = 10;
extern const GLfloat explodeDuration = 2.0f;
extern const GLfloat explodeSize = 0.5f;

// Score triggers for attacker ship
extern const GLuint attackerPoints = 1;

extern const glm::vec2 centerOfGravity = glm::vec2(0.0f, 0.0f);
//extern const glm::vec2 centerOfGravity = glm::vec2(0.0f, -1000.0f);
extern const GLfloat gravityStrength = 0.000003f;

// gunner scale
extern const GLfloat gunnerScale = 0.1f;
extern const glm::vec2 gunnerScaleVec = glm::vec2(gunnerScale, gunnerScale);

const GLuint floorPoints = 15 * 3;
const GLuint skyPoints = 24 * 3;
const GLuint crossPoints = 12;
const GLuint gatePoints = 2;

//
const GLfloat gateFadeDelay = 2.0;


#endif /* constants_h */
