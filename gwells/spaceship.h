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


static const GLfloat photonSpeed = 1.0f; // how fast photons move
static const GLfloat photonDuration = 2.0f;  // how long they last
static const GLfloat photonMaxDistance = 2.828f; // screen diagonal is 2 * sqrt(2)

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
static const GLfloat gunnerHeight = 0.45f * gunnerScale;  // Photon starting point in the gunner.
// this can't be (0,0) because gunners are on surfaces, which destroy photons.


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
    
    GLfloat photonTimer = 0.0f;  // determines how quickly photons can be repeatedly fired
//    const GLfloat photonRate = 0.50f; // determines how quickly photons can be repeatedly fired
    GLboolean firePhoton = false;
    
//    GLboolean enablePhotonSound = false;
    GLboolean isGunner;  // enables random firing of photons for all gunners
    
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
    
    static const GLuint maxPhotons = 5;
    static const GLuint photonParams = 10;
    GLfloat photons[maxPhotons][photonParams];
    GLfloat ShortestPhotonSegment[maxPhotons] = {photonMaxDistance, photonMaxDistance, photonMaxDistance, photonMaxDistance, photonMaxDistance};
    // Inside vector holds photon params : 0-xpos, 1-ypos, 2-direction, 3-duration,
    // 4-start xpos, 5-start ypos, 6-end xpos (virtual), 7-end ypos (virtual), 8-new photon indicator,
    // 9-photonMaxDistance = 2 * sqrt(2) -> this will be replaced by distane to nearest collision surface
    
    std::vector<GLfloat> GunnerVertexCoords; // transformed and rotated
    GLfloat gunner[12];
    GLuint gunnerVBO, gunnerVAO;

    
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
    
//    Spaceship (GLfloat Xpos, GLfloat Ypos)
//    {
//        xpos = Xpos;
//        ypos = Ypos;
//        Shape = gunnerScaleVec;
//    }
    
    void initPhotons() {
        for (int i=0; i < maxPhotons; i++) {
            photons[i][0] = 0;
            photons[i][1] = 0;
            photons[i][2] = 0;
            photons[i][3] = 0;
            photons[i][4] = 0;  //  start xpos, not updated as photon moves
            photons[i][5] = 0;  //  start ypos, not updated as photon moves
            photons[i][6] = 0; // endpoint of photon virtual line segment
            photons[i][7] = 0; // endpoint of photon virtual line segment
            photons[i][8] = 1.0; // 1.0 indicates a new photon for the collision calculations
            photons[i][9] = photonMaxDistance;  // for surfaces
        }
    }

    void init(GLfloat Xpos, GLfloat Ypos, GLint gunRotation )
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
        initPhotons();

    }
    
    void initGunner(GLfloat Xpos, GLfloat Ypos, GLint gunRotation, std::vector<GLfloat> GunnerPolarCoords )
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
        Direction = glm::radians((float) gunRotation);
        Draw = true;
        explodeShip = false;
        explodeTime = 0;
        initPhotons();
        

//        GunnerVertexCoords.clear();
        for (int j=0; j < GunnerPolarCoords.size(); j+=2) {
            // Calculate ship vertices rotated positions and scale
            GLfloat theta = GunnerPolarCoords[(j + 1)] + this->Direction;
            
            GLfloat x1_rot = GunnerPolarCoords[(j + 0)] * cos(theta) * gunnerScale;
            GLfloat y1_rot = GunnerPolarCoords[(j + 0)] * sin(theta) * gunnerScale;
            
            // Push into new vector
            GunnerVertexCoords.push_back(x1_rot + this->xpos);
            GunnerVertexCoords.push_back(y1_rot + this->ypos);
        }
//        printf("GunnerVertexCoords.size() = %lu\n", GunnerVertexCoords.size());
//        for (int i=0; i < GunnerVertexCoords.size(); i+=2) {
//            printf("%4.5f, %4.5f\n", GunnerVertexCoords[i], GunnerVertexCoords[i+1]);
//        }
        for (int i=0; i < 12; i++) {
            GLuint index = i % 8;
            gunner[i] = GunnerVertexCoords[index];
        }
        
        for (int i=0; i < maxPhotons; i++)
            printf("ShortestPhotonSegment[%d] = %4.5f\n", i, ShortestPhotonSegment[i]);

//        for (int i=0; i < 12; i+=2) {
//            printf("%4.5f, %4.5f\n", gunner[i], gunner[i+1]);
//        }
        
        glGenVertexArrays(1, &gunnerVAO);
        glGenBuffers(1, &gunnerVBO);
        glBindVertexArray(gunnerVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gunnerVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gunner), gunner, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

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
            
            updatePhotons(firePhoton, true, vbos_p, pShader, 0.25f, Direction, deltaTime, false);

            for (Spaceship &gunna : *Guns_p) {
                for ( int j=0; j < maxPhotons; j++) {
                    if (gunna.photons[j][3] > 0)  {
                        if (gunna.photons[j][0] > xpos - Shape.x/2 and gunna.photons[j][0] < xpos + Shape.x/2 and gunna.photons[j][1] > ypos - Shape.y/2 and gunna.photons[j][1] < ypos + Shape.y/2) {
                            Draw = false;
                            gunna.photons[j][3] = 0.0f;  // photon destroyed in the collision
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
    
    void updateGunner(Vbos * vbos_p, Shader * shader, Shader * pShader, std::vector<Spaceship> * ship_p, GLfloat deltaTime)
    {
        //        std::cout << "init: " << Axpos << "\n";
        if (Draw == true) {
            if (playSiren == true) {
                //                ISound* snd = engine->play2D("/Users/dirk/games/media/sirenwawa.wav", true, false, true);
                for (int i=0; i < totalExplodePieces; i++) explodePiecesDir[i] = GLfloat(rand() % 360);   // direction of each piece
                playSiren = false;
            }
            
            updatePhotons(true, false, vbos_p, pShader, 0.1f, GunRotation, deltaTime, true);
            
            shader->use();
            glBindVertexArray(vbos_p->gunnerVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(xpos, ypos, 0.0f));
            model = glm::scale(model, glm::vec3(gunnerScaleVec, 0.0f));
            model = glm::rotate(model, glm::radians((GLfloat) GunRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            //        glm::mat4 view = glm::mat4(1.0f);
            shader->setMat4("model", model);
            //        shader.setMat4("view", view);
            shader->setVec3("aColor", glm::vec3(0.7f, 0.0f, 0.2f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(vbos_p->cannonVAO);
            shader->setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            shader->use();
            glBindVertexArray(gunnerVAO);
            model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(xpos, ypos, 0.0f));
//            model = glm::scale(model, glm::vec3(gunnerScaleVec, 0.0f));
//            model = glm::rotate(model, glm::radians((GLfloat) GunRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            //        glm::mat4 view = glm::mat4(1.0f);
            shader->setMat4("model", model);
            //        shader.setMat4("view", view);
            shader->setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
            glDrawArrays(GL_LINE_STRIP, 0, 4);
//            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            
        
            //            printf("checkPhotons gunna.photons.size() = %lu\n", gunna.photons.size());
            // Now check if photons have hit surface boundaries, if so stop drawing them
            for (Spaceship &shipa : * ship_p) {
                    for ( int j=0; j < maxPhotons; j++) {
                        if (shipa.photons[j][3] > 0.0f) {
                            GLfloat x1 = shipa.photons[j][4];
                            GLfloat y1 = shipa.photons[j][5];
                            GLfloat x2 = shipa.photons[j][6];
                            GLfloat y2 = shipa.photons[j][7];
                            
                            for (int i=0; i < GunnerVertexCoords.size() - 2; i+=2) {
                                // first get the start and end points for each surface segment
                                GLfloat x3 = GunnerVertexCoords[i    ];
                                GLfloat y3 = GunnerVertexCoords[i + 1];
                                GLfloat x4 = GunnerVertexCoords[i + 2];
                                GLfloat y4 = GunnerVertexCoords[i + 3];
                                //                            printf("x3 = %3.4f  y3 = %3.4f  x4 = %3.4f  y4 = %3.4f\n", x3, y3, x4, y4);
                                
                                GLfloat denominator = (x4 - x3)*(y1 - y2) - (x1 - x2)*(y4 - y3);
                                
                                if ( denominator != 0 ) {
                                    GLfloat ta=((y3 - y4)*(x1 - x3) + (x4 - x3)*(y1 - y3))/denominator;
                                    GLfloat tb=((y1 - y2)*(x1 - x3) + (x2 - x1)*(y1 - y3))/denominator;
                                    //                        printf("ta = %3.2f  tb = %3.2f\n", ta, tb);
                                    if (ta >= 0 and ta <= 1 and tb >= 0 and tb <= 1) {
                                        //   p1 +ta*(p2 - p1)
                                        GLfloat xIntersect = x1 + ta * (x2 - x1);
                                        GLfloat yIntersect = y1 + ta * (y2 - y1);
                                        
                                        GLfloat photonSegmentLength = sqrt(pow(xIntersect - x1, 2) + pow(yIntersect - y1, 2));
                                        if (ShortestPhotonSegment[j] > photonSegmentLength)
                                            ShortestPhotonSegment[j] = photonSegmentLength;
                                        
                                    }
                                } else ShortestPhotonSegment[j] = photonMaxDistance;
                            }
                        } else ShortestPhotonSegment[j] = photonMaxDistance;
                    }
//                printf("Spaceship shipa.photons.size() = %lu\n", shipa.photons.size());
//                printf("Spaceship Draw = %d\n", Draw);
                for ( int j=0; j < maxPhotons; j++) {
                    if (shipa.photons[j][3] > 0.0f) {
                        GLfloat xstart = shipa.photons[j][4]; // start xpos
                        GLfloat ystart = shipa.photons[j][5]; // start ypos
                        GLfloat xpos = shipa.photons[j][0]; // current xpos
                        GLfloat ypos = shipa.photons[j][1]; // current ypos
                        GLfloat currentLength = sqrt(pow(xpos - xstart, 2) + pow(ypos - ystart, 2));
                        printf("currentLength = %4.5f\n", currentLength);
                        printf("ShortestPhotonSegment[%d] = %4.3f\n", j, ShortestPhotonSegment[j]);
                        
                        
                        if (currentLength >= ShortestPhotonSegment[j]) {
                            //                    printf("Collision!\n");
                            shipa.photons[j][3] = 0.0f;
                            Draw = false;
                            //                            printf("Hit!\n");
                            //                            score += attackerPoints; // each attacker destroyed is worth 10 points
                            //                            scoreLevel += attackerPoints;
                            playBell = true;
                            explodeShip = true;
                            this->ShortestPhotonSegment[j] = photonMaxDistance;
                            //                            snd->stop();
                            //                            engine->stop("/Users/dirk/games/media/sirenwawa.wav");
                        }
                    } else shipa.photons[j][3] = 0.0f;
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
    
//    void updateGunner(Vbos * vbos_p, Shader * shader, Shader * pShader, std::vector<Spaceship> * ship_p, GLfloat deltaTime)
//    {
//        //        std::cout << "init: " << Axpos << "\n";
//        if (Draw == true) {
//            if (playSiren == true) {
//                //                ISound* snd = engine->play2D("/Users/dirk/games/media/sirenwawa.wav", true, false, true);
//                for (int i=0; i < totalExplodePieces; i++) explodePiecesDir[i] = GLfloat(rand() % 360);   // direction of each piece
//                playSiren = false;
//            }
//
//            updatePhotons(true, false, vbos_p, pShader, 0.1f, GunRotation, deltaTime, true);
//
//            shader->use();
//            glBindVertexArray(vbos_p->gunnerVAO);
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(xpos, ypos, 0.0f));
//            model = glm::scale(model, glm::vec3(gunnerScaleVec, 0.0f));
//            model = glm::rotate(model, glm::radians((GLfloat) GunRotation), glm::vec3(0.0f, 0.0f, 1.0f));
//            //        glm::mat4 view = glm::mat4(1.0f);
//            shader->setMat4("model", model);
//            //        shader.setMat4("view", view);
//            shader->setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//            glBindVertexArray(vbos_p->cannonVAO);
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//
//            for (Spaceship &shipa : *ship_p) {
//                for ( int j=0; j < shipa.photons.size(); j++) {
//                    if (shipa.photons[j][3] > 0)  {
//                        if (shipa.photons[j][0] > xpos - Shape.x/2 and shipa.photons[j][0] < xpos + Shape.x/2 and shipa.photons[j][1] > ypos - Shape.y/2 and shipa.photons[j][1] < ypos + Shape.y/2) {
//                            Draw = false;
//                            shipa.photons[j][3] = -0.01f;  // photon destroyed in the collision
//                            //                        score += attackerPoints; // each attacker destroyed is worth 10 points
//                            //                        scoreLevel += attackerPoints;
//                            playBell = true;
//                            explodeShip = true;
//                            //                        snd->stop();
//                            //                        engine->stop("/Users/dirk/games/media/sirenwawa.wav");
//                        }
//                    }
//                }
//            }
//        } else if (explodeShip == true) {
//            if (playBell == true) {
//                //                engine->play2D("/Users/dirk/games/media/Explosion+11.wav");
//                for (int i=0; i < totalExplodePieces; i++) explodePiecesDir[i] = GLfloat(rand() % 360);   // direction of each piece
//                playBell = false;
//                //                std::cout << "\nI'm in!\n";
//            }
//            // Clear attacker photons queue or they can still cause ship to explode even though they are visable.
//            for (int i=0; i < photons.size(); i++) photons[i][3] = -1.0;
//            // give attacker time to explode, although it's not needed...
//            explodeTime += deltaTime;
//            if (explodeTime > explodeDuration) {
//                explodeShip = false;
//            }
//        }
//
//    }
    
    // ship:   updatePhotons(firePhoton, true, vbos_p, pShader, false, 0.25f, DirAngle, deltaTime, false);
    // gunner: updatePhotons(true, false, vbos_p, pShader, true, 0.1f, photonAngle, deltaTime, true);
    
    void updatePhotons(GLboolean shootPhoton, GLboolean enablePhotonSound, Vbos * vbos_p, Shader * pShader, GLfloat photonRate, GLfloat photonAngle, GLfloat deltaTime, GLboolean isGunner)
    {
        // Photon queue is only full when all Durations are greater than zero.
        GLfloat pQueueFull = 1.0f;
        for (int i=0; i < maxPhotons; i++) {
            pQueueFull *= photons[i][3];
        }
        // store existing photons
//        printf("photons.size() 1 = %lu\n", photons.size());
        if (shootPhoton == true and photonTimer >= photonRate and pQueueFull == 0) {
            // incrementor is used to randomize each gunner photon fire rate
            GLfloat xposShifted;
            GLfloat yposShifted;
            if ( incrementer < randomNumber and isGunner == true) {
                photonTimer = 0;
                incrementer++;
            } else {
//                printf("in updatePhotons\n");
                if (isGunner == true) {
                    photonAngle = glm::radians(photonAngle + 90);
                    
                    xposShifted = gunnerHeight * cos(photonAngle) + xpos;
                    yposShifted = gunnerHeight * sin(photonAngle) + ypos;
//                    photonAngle = glm::radians(GLfloat(rand() % 90 + 45) + photonAngle);
                    photonAngle = glm::radians(GLfloat(rand() % 90 - 45)) + photonAngle;
                } else {
                    xposShifted = xpos;
                    yposShifted = ypos;
                }
                GLfloat xPhotonEnd_rot = 2 * cos(photonAngle); //2 ia m distance in x or y
                GLfloat yPhotonEnd_rot = 2 * sin(photonAngle); //2 ia m distance in x or y
                photons[pQueue][0] = xposShifted;
                photons[pQueue][1] = yposShifted;
                photons[pQueue][2] = photonAngle;
                photons[pQueue][3] = photonDuration;
                photons[pQueue][4] = xposShifted;  //  start xpos, not updated as photon moves
                photons[pQueue][5] = yposShifted;  //  start ypos, not updated as photon moves
                photons[pQueue][6] = xposShifted + xPhotonEnd_rot; // endpoint of photon virtual line segment
                photons[pQueue][7] = yposShifted + yPhotonEnd_rot; // endpoint of photon virtual line segment
                photons[pQueue][8] = 1.0; // 1.0 indicates a new photon for the collision calculations
                photons[pQueue][9] = photonMaxDistance;  // for surfaces

//                printf("photons.size() 2 = %lu\n", photons.size());

                pQueue = ++pQueue % maxPhotons;
                photonTimer = 0;
                if (isGunner == true) {
                    incrementer = 0;
                    randomNumber = rand() % 10 + 1;
                }
                if (enablePhotonSound == true) {
                    //                engine->play2D("/Users/dirk/games/media/Gun+Silencer.wav");
                    
                }
            }
        }
        // loop timer for phonton fire rate
        photonTimer += deltaTime;
//        std::cout << "photonTimer = " << photonTimer << std::endl;
        
        
        // calculate new postion of each existing photon and draw it
        for (int i=0; i < maxPhotons; i++) {
//            printf("photona[3] = %3.4f\n", photona[3]);
//            printf("photon[][9] = %4.5f\n", photona[9]);
//            printf("photon[][10] = %4.5f\n", photona[10]);
            if (photons[i][3] > 0) {
//                printf("in photon draw\n");
                
//                GLfloat lastxpostemp = photona[0];
//                GLfloat lastypostemp = photona[1];
                photons[i][0] += photonSpeed * deltaTime * cos(photons[i][2]);
                photons[i][1] += photonSpeed * deltaTime * sin(photons[i][2]);
//                if (isGunner == false) {
//                    GLfloat drawCyclePhotonDisplacement = sqrt(pow(photona[0] - lastxpostemp, 2) + pow(photona[1] - lastypostemp, 2));
//                    printf("dist = %4.4f, dT = %4.4f, xpos = %4.4f, ypos = %4.7f\n", drawCyclePhotonDisplacement, deltaTime, photona[0], photona[1]);
//                }
                pShader->use();
                glBindVertexArray(vbos_p->pVAO);
                glEnable(GL_PROGRAM_POINT_SIZE);
                glm::mat4 model = glm::mat4(1.0f);
                //        model = glm::translate(model, glm::vec3(photons[pcounter][0], photons[pcounter][0], 0.0f));
                model = glm::translate(model, glm::vec3(photons[i][0], photons[i][1], 0.0f));
                
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

//    void checkGunners()
//    {
//        // For each line segment of the "surface" check if ship has impacted it
//        // http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
//        // equation derivation for intersection of two line segments.
//        for (Spaceship &shipa : *Ship_pntr)
//            if (shipa.Draw == true) {
//                for (int j=0; j < ShipPolarCoords.size(); j+=2){
//                    // Calculate ship vertices rotated positions and scale
//                    GLfloat theta1 = ShipPolarCoords[(j + 1) % ShipPolarCoords.size()] + shipa.Direction;
//                    GLfloat theta2 = ShipPolarCoords[(j + 3) % ShipPolarCoords.size()] + shipa.Direction;
//
//                    GLfloat x1_rot = ShipPolarCoords[(j + 0) % ShipPolarCoords.size()] * cos(theta1) * shipa.Shape.x;
//                    GLfloat x2_rot = ShipPolarCoords[(j + 2) % ShipPolarCoords.size()] * cos(theta2) * shipa.Shape.x;
//                    GLfloat y1_rot = ShipPolarCoords[(j + 0) % ShipPolarCoords.size()] * sin(theta1) * shipa.Shape.y;
//                    GLfloat y2_rot = ShipPolarCoords[(j + 2) % ShipPolarCoords.size()] * sin(theta2) * shipa.Shape.y;
//
//                    // Transpose to actual location
//                    GLfloat x1 = x1_rot + shipa.xpos;
//                    GLfloat y1 = y1_rot + shipa.ypos;
//                    GLfloat x2 = x2_rot + shipa.xpos;
//                    GLfloat y2 = y2_rot + shipa.ypos;
//                    //                printf("x1 = %3.3f  y1 = %3.3f  x2 = %3.3f  y2 = %3.3f\n", x1, y1, x2, y2);
//                    for (int i=0; i < NumOfVertices - 1; i++) {
//                        // first get the start and end points for each surface segment
//                        GLfloat x3 = SurfaceVerts[(i * 2)];
//                        GLfloat y3 = SurfaceVerts[(i * 2) + 1];
//                        GLfloat x4 = SurfaceVerts[(i * 2) + 2];
//                        GLfloat y4 = SurfaceVerts[(i * 2) + 3];
//                        //                    printf("x3 = %3.2f  y3 = %3.2f  x4 = %3.2f  y4 = %3.2f\n", x3, y3, x4, y4);
//
//                        GLfloat denominator = (x4 - x3)*(y1 - y2) - (x1 - x2)*(y4 - y3);
//
//                        if ( denominator != 0 ) {
//                            GLfloat ta=((y3 - y4)*(x1 - x3) + (x4 - x3)*(y1 - y3))/denominator;
//                            GLfloat tb=((y1 - y2)*(x1 - x3) + (x2 - x1)*(y1 - y3))/denominator;
//                            //                        printf("ta = %3.2f  tb = %3.2f\n", ta, tb);
//                            if (ta >= 0 and ta <= 1 and tb >= 0 and tb <= 1) {
//                                //                            printf("I'm in!\n");
//                                shipa.explodeShip = true;
//                                shipa.Draw = false;
//                                shipa.playBell = true;
//                            }
//                        }
//                    }
//                }
//            }
//    }
    
    
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
