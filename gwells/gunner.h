//
//  gunners.h
//  scratch
//
//  Created by Dirk Reese on 2/27/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef gunner_h
#define gunner_h

// Header file
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "constants.h"
#include "globals.h"
#include "vbos.h"
#include "spaceship.h"

#include <math.h>


class Gunner
{
    
    // Gunner parameters
    // Location
    
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
    float Direction;
    GLint DirAngle = 90;
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
    
    Gunner (glm::vec2 position = glm::vec2(0.0f, 0.0f), GLint dirangle = 90, GLboolean draw = true, glm::vec2 shape = glm::vec2(0.05f, 0.05f))
    {
        xpos = 0.0f;
        ypos = 0.0f;
        Position = position;
        DirAngle = dirangle;
        Direction = glm::radians((float) dirangle);
        Draw = draw;
        Shape = shape;
    }
    
    Gunner (GLfloat Xpos, GLfloat Ypos)
    {
        xpos = Xpos;
        ypos = Ypos;
        Shape = gunnerScaleVec;
    }
    
    void init(GLfloat Xpos, GLfloat Ypos)
    {
        xpos = Xpos;
        ypos = Ypos;
        Position = glm::vec2(xpos, ypos);
        lastxpos = xpos;
        lastypos = ypos;
        DirAngle = 90;
        Direction = glm::radians((float) DirAngle);
        Draw = true;
        explodeShip = false;
        explodeTime = 0;
        for (int i=0; i < maxPhotons; i++) photons[i] = glm::vec4(0.0f, 0.0f, 0.0f, -1.0f);
    }
    
    void update(Vbos * vbos_p, Shader shader, Shader pShader, Spaceship * ship_p, ISoundEngine* engine)
    {
        //        std::cout << "init: " << Axpos << "\n";
        if (Draw == true) {
            if (playSiren == true) {
                //                ISound* snd = engine->play2D("/Users/dirk/games/media/sirenwawa.wav", true, false, true);
                for (int i=0; i < totalExplodePieces; i++) explodePiecesDir[i] = GLfloat(rand() % 360);   // direction of each piece
                playSiren = false;
            }
            
            DirAngle = GLfloat(rand() % 90 + 50);
            updatePhotons(true, false, vbos_p, pShader, engine, true, 0.5f);
            
            shader.use();
            glBindVertexArray(vbos_p->gunnerVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(xpos + xTranslation, ypos, 0.0f));
            model = glm::scale(model, glm::vec3(gunnerScaleVec, 0.0f));
            //        model = glm::rotate(model, glm::radians((float)glfwGetTime()*20), glm::vec3(0.0f, 0.0f, 1.0f));
            //        glm::mat4 view = glm::mat4(1.0f);
            // note that we're translating the scene in the reverse direction of where we want to move
            //        view = glm::translate(view, glm::vec3(xTranslation, 0.0f, 0.0f));
            shader.setMat4("model", model);
            //        shader.setMat4("view", view);
            shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(vbos_p->cannonVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            
            for ( int j=0; j < maxPhotons; j++) {
                if (ship_p->photons[j].w > 0)  {
                    if (ship_p->photons[j].x > xpos + xTranslation - Shape.x/2 and ship_p->photons[j].x + xTranslation < xpos + Shape.x/2 and ship_p->photons[j].y > ypos - Shape.y/2 and ship_p->photons[j].y < ypos + Shape.y/2) {
                        Draw = false;
                        ship_p->photons[j].w = -0.01f;  // photon destroyed in the collision
                        score += attackerPoints; // each attacker destroyed is worth 10 points
                        scoreLevel += attackerPoints;
                        playBell = true;
                        explodeShip = true;
                        //                        snd->stop();
                        //                        engine->stop("/Users/dirk/games/media/sirenwawa.wav");
                    }
                }
            }
        } else if (explodeShip == true) {
            if (playBell == true) {
                engine->play2D("/Users/dirk/games/media/Explosion+11.wav");
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
    
    void updatePhotons(GLboolean shootPhoton, GLboolean enablePhotonSound , Vbos * vbos_p, Shader pShader, ISoundEngine* engine, GLboolean panPhotons, GLfloat photonRate)
    {
        // store existing photons
        if (shootPhoton == true and photonTimer >= photonRate) {
            photons[pQueue][0] = xpos;
            photons[pQueue][1] = ypos;
            photons[pQueue][2] = DirAngle;
            photons[pQueue][3] = photonDuration;
            pQueue = ++pQueue % maxPhotons;
            photonTimer = 0;
            if (enablePhotonSound == true) {
                engine->play2D("/Users/dirk/games/media/Gun+Silencer.wav");
            }
        }
        // loop timer for phonton fire rate
        photonTimer += deltaTime;
        
        // calculate new postion of each existing photon and draw it
        for ( int i=0; i < maxPhotons; i++) {
            if (photons[i][3] > 0) {
                
                photons[i][0] += photonSpeed * deltaTime * cos(glm::radians((float) photons[i][2]));
                photons[i][1] += photonSpeed * deltaTime * sin(glm::radians((float) photons[i][2]));
                pShader.use();
                glBindVertexArray(vbos_p->pVAO);
                glEnable(GL_PROGRAM_POINT_SIZE);
                glm::mat4 model = glm::mat4(1.0f);
                //        model = glm::translate(model, glm::vec3(photons[pcounter][0], photons[pcounter][0], 0.0f));
                if (panPhotons == true) {
                    model = glm::translate(model, glm::vec3(photons[i][0] + xTranslation, photons[i][1], 0.0f));
                } else model = glm::translate(model, glm::vec3(photons[i][0], photons[i][1], 0.0f));
                
                
                pShader.setMat4("model", model);
                //                    shader.setFloat("aPointSize", 4.0f);
                pShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
                glDrawArrays(GL_POINTS, 0, 1);
                glDisable(GL_PROGRAM_POINT_SIZE);
                glBindVertexArray(0);
                photons[i][3] -= deltaTime;
            }
        }
    }
    
};


#endif /* gunner_h */
