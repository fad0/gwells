//
//  spaceship.h
//  astro
//
//  Created by Dirk Reese on 2/15/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//
#ifndef SPACESHIP_H
#define SPACESHIP_H

// Header file
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "vbos.h"
#include "shader.h"
//#include "gunner.h"

#include <cmath>

static const GLuint maxPhotons = 5;

static const GLfloat smallShape = 0.025f;

// Ship consts
static const GLuint initialShipCount = 10;
static const GLuint totalExplodePieces = 10;
static const GLfloat explodeDuration = 2.0f;
static const GLfloat explodeSize = 0.5f;

//static const glm::vec2 centerOfGravity = glm::vec2(0.0f, 0.0f);
//extern const glm::vec2 centerOfGravity = glm::vec2(0.0f, -1000.0f);

// gunner scale
static const GLfloat gunnerScale = 0.07f;
static const glm::vec2 gunnerScaleVec = glm::vec2(gunnerScale, gunnerScale);
static const GLfloat gunnerHeight = 0.45f * gunnerScale;

//!!!!!!!!!!!REMOVE LATER!!!!!!!!!!!!!!!!!!!!!
const GLfloat xPan = 0.0f;


// Used as abstraction to stay away from window-system specific input methods
enum Ship_Action {
    CLOCKWISE,
    COUNTERCLOCKWISE,
    BURN,
    NOBURN,
    PHOTON,
    NOPHOTON
};

class Spaceship
{
    
    // SpaceShip parameters
    // Location
    
    
    
    GLfloat aspeed = 0.0f;
    
    GLfloat xaspeed = 0.0f;
    GLfloat yaspeed = 0.0f;
    GLfloat previous_xaspeed = 0.0f;
    GLfloat previous_yaspeed = 0.0f;
    GLfloat accelerationFactor = 0.0002f;
    GLint accelerationCounter = 0;
    const GLuint maxAccelerationCounter = 50;
    const GLfloat attackerRate = 0.20f;
    GLfloat explodePiecesDir[totalExplodePieces]; // array of random directions for exploding pieces
    
    // photon gun variables
    GLuint pQueue = 0;
    GLfloat photonSpeed = 1.5f; // how fast photons move
    GLfloat photonDuration = 2.0f;  // how long they last
    GLfloat photonTimer = 0.0f;  // determines how quickly photons can be repeatedly fired
//    const GLfloat photonRate = 0.50f; // determines how quickly photons can be repeatedly fired
    GLboolean firePhoton = false;
//    GLboolean enablePhotonSound = false;
    
public:
//    GLfloat gravityStrength = 0.000002f;
//    glm::vec2 centerOfGravity = glm::vec2(0.0f, 0.0f);
//    GLfloat gravityStrength1 = 0.00000f;
//    glm::vec2 centerOfGravity1 = glm::vec2(0.0f, 0.0f);
    
    std::vector<glm::vec2> centerOfGravity;
    std::vector<GLfloat> gravityStrength;
    
    
    float Direction;
    GLint DirAngle = 90;
    GLint GunRotation;
    glm::vec2 Position;
    GLfloat xpos;
    GLfloat ypos;
    GLfloat lastxpos;
    GLfloat lastypos;
    
    glm::vec2 Shape;
    GLboolean Draw;
    GLboolean explodeShip = false;
    GLboolean latchExplodeShip = false;
    GLfloat explodeTime = 0.0f;
    GLuint shipsLeft = initialShipCount;
    GLboolean playBell = false;
    glm::vec4 photons[maxPhotons]; // 0-xpos, 1-ypos, 2-direction, 3-duration
    
    
    GLboolean playSiren = false;
    GLuint incrementer = 0;
    GLuint randomNumber = 1;

    Spaceship (glm::vec2 position = glm::vec2(0.0f, 0.0f), GLint dirangle = 90, GLboolean draw = true, glm::vec2 shape = glm::vec2(0.04f, 0.04f))
    {
        xpos = 0.0f;
        ypos = 0.0f;
        Position = position;
        DirAngle = dirangle;
        Direction = glm::radians((float) dirangle);
        Draw = draw;
        Shape = shape;
    }
    
    Spaceship (GLfloat Xpos, GLfloat Ypos)
    {
        xpos = Xpos;
        ypos = Ypos;
        Shape = gunnerScaleVec;
    }

    void init(GLfloat Xpos, GLfloat Ypos, GLint gunRotation)
    {
        xpos = Xpos;
        ypos = Ypos;
        Position = glm::vec2(xpos, ypos);
        lastxpos = xpos;
        lastypos = ypos;
        xaspeed = 0;
        yaspeed = 0;
        DirAngle = 90;
        GunRotation = gunRotation;
        Direction = glm::radians((float) DirAngle);
        Draw = true;
        explodeShip = false;
        explodeTime = 0;
        for (int i=0; i < maxPhotons; i++) photons[i] = glm::vec4(0.0f, 0.0f, 0.0f, -1.0f);
    }

    void update(Vbos * vbos_p, Shader * shader, Shader * pShader, std::vector<Spaceship> * Guns_p, GLfloat deltaTime)
    {
        // Draw remaining ships in upper left corner
        shader->use();
//        std::cout << "In the loop\n";
        glBindVertexArray(vbos_p->sVAO);
        for (int i=0; i < shipsLeft; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-0.95f + (float)i * 0.07, 0.95f, 0.0f));
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.0f));
            model = glm::rotate(model, glm::radians((float)90.0), glm::vec3(0.0f, 0.0f, 1.0f));
            shader->setMat4("model", model);
            shader->setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        glBindVertexArray(0);
        
        if (Draw == true) {
            // Calculate ships new position
            // Acceleration
            if (accelerationCounter > maxAccelerationCounter) accelerationCounter = maxAccelerationCounter;
            aspeed = accelerationFactor * deltaTime * deltaTime * (accelerationCounter * accelerationCounter);
            // gravity
            GLfloat xGravity = 0;
            GLfloat yGravity = 0;
            
            for (int i=0; i < centerOfGravity.size(); i++) {
                if (gravityStrength[i] != 0.0f) {
                    GLfloat gravityDir = atan2((centerOfGravity[i].y - lastypos), (centerOfGravity[i].x - lastxpos));
                    GLfloat gravityDist = sqrt(pow((centerOfGravity[i].x - lastxpos), 2) + pow((centerOfGravity[i].y - lastypos), 2));
                    if (gravityDist != 0.0f) {
                        xGravity += (gravityStrength[i]/gravityDist) * cos(gravityDir);
                        yGravity += (gravityStrength[i]/gravityDist) * sin(gravityDir);
                    }
                }
            }

            // Speed
            xaspeed += aspeed * cos(Direction) + xGravity;
            yaspeed += aspeed * sin(Direction) + yGravity;

            lastxpos = xpos;
            lastypos = ypos;
            xpos += xaspeed;
            ypos += yaspeed;

//            std::cout << "xpos = " << xpos << "  ypos = " << ypos << std::endl;
            Position = glm::vec2(xpos, ypos);
            shader->use();
            glBindVertexArray(vbos_p->sVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(Position, 0.0f));
            model = glm::scale(model, glm::vec3(Shape, 0.0f));
            model = glm::rotate(model, Direction, glm::vec3(0.0f, 0.0f, 1.0f));
            //        model = glm::rotate(model, glm::radians((float)dirAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            shader->setMat4("model", model);
            shader->setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 3);
            // draw the rocket flame
            if (accelerationCounter > 0) {
                glBindVertexArray(vbos_p->fVAO);
                shader->setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            
            updatePhotons2(firePhoton, true, vbos_p, pShader, false, 0.25f, DirAngle, deltaTime);

            for (Spaceship &gunna : *Guns_p) {
                for ( int j=0; j < maxPhotons; j++) {
                    if (gunna.photons[j].w > 0)  {
                        if (gunna.photons[j].x + xPan > xpos - Shape.x/2 and gunna.photons[j].x + xPan < xpos + Shape.x/2 and gunna.photons[j].y > ypos - Shape.y/2 and gunna.photons[j].y < ypos + Shape.y/2) {
                            Draw = false;
                            gunna.photons[j].w = -0.01f;  // photon destroyed in the collision
                            explodeShip = true;
                            playBell = true;
                        }
                    }
                }
            }
                
        } else if (explodeShip == true) {
            if (playBell == true) {
//                engine->play2D("/Users/dirk/games/media/Explosion+11.wav");
                for (int i=0; i < totalExplodePieces; i++) explodePiecesDir[i] = GLfloat(rand() % 360);   // direction of each piece
                playBell = false;
                //                std::cout << "\nI'm in!\n";
            }
            
            for (int i=0; i < totalExplodePieces; i++) {
                pShader->use();
                glBindVertexArray(vbos_p->pVAO);
                glEnable(GL_PROGRAM_POINT_SIZE);
                float xpiece = xpos + explodeTime * explodeSize * cos(glm::radians((float)explodePiecesDir[i]));
                float ypiece = ypos + explodeTime * explodeSize * sin(glm::radians((float)explodePiecesDir[i]));
                //                if (i == 0) std::cout << "xpieces = " << xpiece << "\n";
                //                if (i == 0) std::cout << "ypieces = " << ypiece << "\n";
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(xpiece, ypiece, 0.0f));
                pShader->setMat4("model", model);
                //                shader.setFloat("aPointSize", 10.0f);
                pShader->setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
                pShader->setFloat("aPointSize", 8.0f);
                glDrawArrays(GL_POINTS, 0, 1);
                glDisable(GL_PROGRAM_POINT_SIZE);
                glBindVertexArray(0);
            }
            explodeTime += deltaTime;
            if (explodeTime > explodeDuration) {
                explodeShip = false;
            }
        }
    }
    
    void updateGunner(Vbos * vbos_p, Shader * shader, Shader * pShader, Spaceship * ship_p, GLfloat deltaTime)
    {
        //        std::cout << "init: " << Axpos << "\n";
        if (Draw == true) {
            if (playSiren == true) {
                //                ISound* snd = engine->play2D("/Users/dirk/games/media/sirenwawa.wav", true, false, true);
                for (int i=0; i < totalExplodePieces; i++) explodePiecesDir[i] = GLfloat(rand() % 360);   // direction of each piece
                playSiren = false;
            }
            
            GLfloat xposTranslated = xpos + xPan;
            GLfloat photonAngle = GLfloat(rand() % 90 + 45) + GunRotation;
//            GLfloat photonRate = ((rand() % 4)/10) + 0.5f;
            
            updatePhotons(true, false, vbos_p, pShader, true, 0.1f, photonAngle, deltaTime);
            
            shader->use();
            glBindVertexArray(vbos_p->gunnerVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(xposTranslated, ypos, 0.0f));
            model = glm::scale(model, glm::vec3(gunnerScaleVec, 0.0f));
            model = glm::rotate(model, glm::radians((GLfloat) GunRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            //        glm::mat4 view = glm::mat4(1.0f);
            // note that we're translating the scene in the reverse direction of where we want to move
            //        view = glm::translate(view, glm::vec3(xPan, 0.0f, 0.0f));
            shader->setMat4("model", model);
            //        shader.setMat4("view", view);
            shader->setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(vbos_p->cannonVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            
            for ( int j=0; j < maxPhotons; j++) {
                if (ship_p->photons[j].w > 0)  {
                    if (ship_p->photons[j].x > xposTranslated - Shape.x/2 and ship_p->photons[j].x < xposTranslated + Shape.x/2 and ship_p->photons[j].y > ypos - Shape.y/2 and ship_p->photons[j].y < ypos + Shape.y/2) {
                        Draw = false;
                        ship_p->photons[j].w = -0.01f;  // photon destroyed in the collision
//                        score += attackerPoints; // each attacker destroyed is worth 10 points
//                        scoreLevel += attackerPoints;
                        playBell = true;
                        explodeShip = true;
                        //                        snd->stop();
                        //                        engine->stop("/Users/dirk/games/media/sirenwawa.wav");
                    }
                }
            }
        } else if (explodeShip == true) {
            if (playBell == true) {
//                engine->play2D("/Users/dirk/games/media/Explosion+11.wav");
                for (int i=0; i < totalExplodePieces; i++) explodePiecesDir[i] = GLfloat(rand() % 360);   // direction of each piece
                playBell = false;
                //                std::cout << "\nI'm in!\n";
            }
            // Clear attacker photons queue or they can still cause ship to explode even though they are visable.
            for (int i=0; i < maxPhotons; i++) photons[i][3] = -1.0;
            // give attacker time to explode, although it's not needed...
            explodeTime += deltaTime;
            if (explodeTime > explodeDuration) {
                explodeShip = false;
            }
        }
        
    }
    
    void updatePhotons2(GLboolean shootPhoton, GLboolean enablePhotonSound , Vbos * vbos_p, Shader * pShader, GLboolean panPhotons, GLfloat photonRate, GLfloat photonAngle, GLfloat deltaTime)
    {
        // store existing photons
        if (shootPhoton == true and photonTimer >= photonRate) {
            photons[pQueue][0] = xpos;
            photons[pQueue][1] = ypos;
            photons[pQueue][2] = photonAngle;
            photons[pQueue][3] = photonDuration;
            pQueue = ++pQueue % maxPhotons;
            photonTimer = 0;
            if (enablePhotonSound == true) {
                //                engine->play2D("/Users/dirk/games/media/Gun+Silencer.wav");
            }
        }
        // loop timer for phonton fire rate
        photonTimer += deltaTime;
//        std::cout << "photonTimer = " << photonTimer << std::endl;
        
        
        // calculate new postion of each existing photon and draw it
        for ( int i=0; i < maxPhotons; i++) {
            if (photons[i][3] > 0) {
                
                photons[i][0] += photonSpeed * deltaTime * cos(glm::radians((float) photons[i][2]));
                photons[i][1] += photonSpeed * deltaTime * sin(glm::radians((float) photons[i][2]));
                pShader->use();
                glBindVertexArray(vbos_p->pVAO);
                glEnable(GL_PROGRAM_POINT_SIZE);
                glm::mat4 model = glm::mat4(1.0f);
                //        model = glm::translate(model, glm::vec3(photons[pcounter][0], photons[pcounter][0], 0.0f));
                if (panPhotons == true) {
                    model = glm::translate(model, glm::vec3(photons[i][0] + xPan, photons[i][1], 0.0f));
                } else model = glm::translate(model, glm::vec3(photons[i][0], photons[i][1], 0.0f));
                
                pShader->setMat4("model", model);
                //                    shader.setFloat("aPointSize", 4.0f);
                pShader->setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
                pShader->setFloat("aPointSize", 5.0f);
                glDrawArrays(GL_POINTS, 0, 1);
                glDisable(GL_PROGRAM_POINT_SIZE);
                glBindVertexArray(0);
                photons[i][3] -= deltaTime;
            }
        }
    }

    
    void updatePhotons(GLboolean shootPhoton, GLboolean enablePhotonSound , Vbos * vbos_p, Shader * pShader, GLboolean panPhotons, GLfloat photonRate, GLfloat photonAngle, GLfloat deltaTime)
    {
        
        // store existing photons
        if (shootPhoton == true and photonTimer >= photonRate) {
            if ( incrementer < randomNumber) {
                photonTimer = 0;
                incrementer++;
            } else {
                photons[pQueue][0] = xpos;
                photons[pQueue][1] = ypos;
                photons[pQueue][2] = photonAngle;
                photons[pQueue][3] = photonDuration;
                pQueue = ++pQueue % maxPhotons;
                photonTimer = 0;
                incrementer = 0;
                randomNumber = rand() % 10 + 1;
                if (enablePhotonSound == true) {
                    //                engine->play2D("/Users/dirk/games/media/Gun+Silencer.wav");
                    
                }
            }
        }
        // loop timer for phonton fire rate
        photonTimer += deltaTime;
//        std::cout << "photonTimer = " << photonTimer << std::endl;
        
        
        // calculate new postion of each existing photon and draw it
        for ( int i=0; i < maxPhotons; i++) {
            if (photons[i][3] > 0) {
                
                photons[i][0] += photonSpeed * deltaTime * cos(glm::radians((float) photons[i][2]));
                photons[i][1] += photonSpeed * deltaTime * sin(glm::radians((float) photons[i][2]));
                pShader->use();
                glBindVertexArray(vbos_p->pVAO);
                glEnable(GL_PROGRAM_POINT_SIZE);
                glm::mat4 model = glm::mat4(1.0f);
                //        model = glm::translate(model, glm::vec3(photons[pcounter][0], photons[pcounter][0], 0.0f));
                if (panPhotons == true) {
                    model = glm::translate(model, glm::vec3(photons[i][0] + xPan, photons[i][1], 0.0f));
                } else model = glm::translate(model, glm::vec3(photons[i][0], photons[i][1], 0.0f));
                
                pShader->setMat4("model", model);
                //                    shader.setFloat("aPointSize", 4.0f);
                pShader->setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
                pShader->setFloat("aPointSize", 5.0f);
                glDrawArrays(GL_POINTS, 0, 1);
                glDisable(GL_PROGRAM_POINT_SIZE);
                glBindVertexArray(0);
                photons[i][3] -= deltaTime;
            }
        }
    }


    
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Ship_Action direction, float deltaTime)
    {
//        float velocity = MovementSpeed * deltaTime;
        if (direction == CLOCKWISE)
            Direction = glm::radians((float) --DirAngle);
        if (direction == COUNTERCLOCKWISE)
            Direction = glm::radians((float) ++DirAngle);
        if (direction == BURN)
            accelerationCounter++;
        if (direction == NOBURN)
            accelerationCounter = 0;
        if (direction == PHOTON)
            firePhoton = true;
        if (direction == NOPHOTON)
            firePhoton = false;
    }
    
};

#endif /* spaceship_h */
