#ifndef ASTERIOD_H
#define ASTERIOD_H

// Header file
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <shader.h>
#include <vector>
#include "constants.h"
#include "globals.h"
#include "vbos.h"
#include "spaceship.h"

class Asteriod
{
//    GLfloat Direction;
    const GLfloat asteriodRate = 0.3f;
public:
    GLfloat Direction;
    glm::vec2 Position;
    glm::vec2 Shape;
    GLboolean Draw;
    GLboolean Small;
//    photonArray = new glm::vec2[maxPhotons];
    

    Asteriod (glm::vec2 position = glm::vec2(0.0f, 0.0f), GLboolean draw = true, GLboolean small = false)
//    : Right(glm::vec2(1.0f,0.0f)), Up(glm::vec2(0.0f, 1.0f))
    {
        Position = position;
        Draw = draw;
        Small = small;
    }
    
    void init()
    {
        Position.x = float(rand() %  WIDTH) * 2/float(WIDTH) - 1; // asteriod's starting xcoord
        Position.y = float(rand() %  HEIGHT) * 2/float(HEIGHT) - 1; // asteriod's starting ycoord
        Direction = float(rand() % 360);   // asteriod's direction
        if (Small == false) {
            Shape.x = 0.1 + (float(rand() %  100) * (2/float(100)) - 1)/20.0f; // asteriod's xscale
            Shape.y = 0.1 + (float(rand() %  100) * (2/float(100)) - 1)/20.0f; // asteriod's yscale
        } else {
            Shape.x = smallShape;
            Shape.y = smallShape;
        }
    }
    
    void update(glm::vec4* photons, Asteriod * asteriod, int index, GLuint bigAsteriods, Vbos * vbos_p, Shader shader, Spaceship * ship, ISoundEngine* engine   )
    {
        
        if (Draw == true) {
            // Check if any existing photons have impacted the currect asteriod, set draw to false if impact.
            for ( int j=0; j < maxPhotons; j++) {
                if (photons[j].w > 0)  {
                    if (photons[j].x > Position.x - Shape.x/2 and photons[j].x < Position.x + Shape.x/2 and photons[j].y > Position.y - Shape.y/2 and photons[j].y < Position.y + Shape.y/2) {
                        Draw = false;
                        photons[j].w = -0.01f;  // photon destroyed in the collision
                        score++; // each asteriod destroyed is worth 1 point
                        scoreLevel++;
                        
                        if (score == firstBonusShipScore or score == secondBonusShipScore or score == thirdBonusShipScore) {
                            ship->shipsLeft++;
                            engine->play2D("/Users/dirk/games/media/bubblegurggle.wav");
                        }
                        if (score == winScore) {
                            win = true;
                            engine->play2D("/Users/dirk/games/media/bubblegurggle.wav");
                        }
                        
                        if (Small == false) {
                            for (int k=0; k < smallPerBigAsteriods; k++) {
                                asteriod[(smallPerBigAsteriods * index)  + bigAsteriods + k].Draw = true;
                                asteriod[(smallPerBigAsteriods * index)  + bigAsteriods + k].Position.x = Position.x;
                                asteriod[(smallPerBigAsteriods * index)  + bigAsteriods + k].Position.y = Position.y;
                            }
                        }
                    }
                }
                // also check if current asteriod has hit the ship
                if (ship->Position.x > Position.x - Shape.x/2 and ship->Position.x < Position.x + Shape.x/2 and ship->Position.y > Position.y - Shape.y/2 and ship->Position.y < Position.y + Shape.y/2 and ship->Draw == true) {
                    Draw = false;
                    ship->Draw = false;
                    ship->explodeShip = true;
                    ship->playBell = true;
                    score++;
                    scoreLevel++;
                    if (score == firstBonusShipScore or score == secondBonusShipScore or score == thirdBonusShipScore) {
                        ship->shipsLeft++;
                        engine->play2D("/Users/dirk/games/media/bubblegurggle.wav");
                    }
                    if (score == winScore) {
                        win = true;
                        engine->play2D("/Users/dirk/games/media/bubblegurggle.wav");
                    }
                    
                    if (Small == false) {
                        for (int k=0; k < smallPerBigAsteriods; k++) {
                            asteriod[(smallPerBigAsteriods * index)  + bigAsteriods + k].Draw = true;
                            asteriod[(smallPerBigAsteriods * index)  + bigAsteriods + k].Position.x = Position.x;
                            asteriod[(smallPerBigAsteriods * index)  + bigAsteriods + k].Position.y = Position.y;
                        }
                    }
                }
            }
            // Draw the asteriod if it hasn't been hit by a photon.
            // calculate new postion
            Position.x += asteriodRate * deltaTime * cos(glm::radians((float)Direction));
            Position.y += asteriodRate * deltaTime * sin(glm::radians((float)Direction));
            
            // if it moves off screen, have it continue from opposite side
            if (Position.x < -x_modulo) {
                Position.x += x_modulo * 2;
            } else if (Position.x > x_modulo) {
                Position.x -= x_modulo * 2;
            }
            if (Position.y < -y_modulo) {
                Position.y += y_modulo * 2;
            } else if (Position.y > y_modulo) {
                Position.y -= y_modulo * 2;
            }
                shader.use();
                glBindVertexArray(vbos_p->aVAO);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(Position, 0.0f));
                model = glm::scale(model, glm::vec3(Shape, 0.0f));
                //        model = glm::rotate(model, glm::radians((float)dirAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                shader.setMat4("model", model);
                shader.setVec3("aColor", glm::vec3(0.5f, 0.7f, 0.25f));
                glDrawArrays(GL_TRIANGLES, 0, 18);
        }
    }
};
#endif
