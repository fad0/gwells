//
//  globals.h
//  astro
//
//  Created by Dirk Reese on 2/17/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef globals_h
#define globals_h

//// Header file
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint score = 0;
GLuint scoreLevel = 0;
GLboolean win = false;
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame

GLfloat xPan = 0.0f;  // For panning left and right
glm::vec3 surfaceScaleFactor = glm::vec3(1.0f, 1.0f, 1.0f);

//extern const GLuint floorPoints = 17;
//extern const GLuint floorPoints = 8;
//extern const GLuint floorPoints = 13;
extern const GLuint floorPoints = 15 * 3;
extern const GLuint skyPoints = 24 * 3;
const GLuint crossPoints = 12;
extern const GLuint gatePoints = 2;


// Bonus Ship scores
extern const GLuint firstBonusShipScore = 4;
extern const GLuint secondBonusShipScore = firstBonusShipScore + 20; // 40
extern const GLuint thirdBonusShipScore = secondBonusShipScore + 30; // 70
extern const GLuint winScore = 300;
extern const GLuint launchAttackScore[4] = {60, 120, 200, 270};


#endif /* globals_h */
