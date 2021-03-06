//
//  surface.h
//  scratch
//
//  Created by Dirk Reese on 2/24/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef surface_h
#define surface_h

// Header file
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "vbos.h"

#include <math.h>


//!!!!!!!!!!!REMOVE LATER!!!!!!!!!!!!!!!!!!!!!
//const GLfloat xPan = 0.0f;

class Surface
{
    GLfloat slope;
    GLfloat yIntercept;
    glm::vec3 surfaceScaleFactor = glm::vec3(1.0f, 1.0f, 1.0f);
    
public:
//    glm::vec2 Position;
    GLuint NumOfVertices;
    GLfloat * Vbos_vertices_p;
    Spaceship * Ship_pntr;
    Spaceship * Gunner1_p;
    std::vector<Spaceship> * Guns_P;
    Shader * sShader;
    GLuint Vao;
    GLuint DrawArraySpecifier;
    
    
    
    
//    Spaceship * Gunner2_p;
    
    Surface (const GLuint numOfVertices, GLfloat * vbos_vertices_p, Spaceship * ship_pntr, std::vector<Spaceship> * Guns_p, Shader * shader, GLuint vao, GLuint drawArraySpecifier)
    {
        NumOfVertices = numOfVertices;
        Vbos_vertices_p = vbos_vertices_p;
        Ship_pntr = ship_pntr;
        Guns_P = Guns_p;
        Vao = vao;
        sShader = shader;
        DrawArraySpecifier = drawArraySpecifier;
//        Gunner2_p = gunner2_p;
    }
    
    // Continuous surfaces use GL_LINE_STRIP so vertexIncrement = 1
    // Island surfaces use GL_LINES so vertexIncrement = 2
    void checkBoundary(GLuint vertexIncrement)
    {
        // For each line segment of the "surface" check if ship has impacted it
        // All surface segments are drawn from left to right or in a clockwise order
        for (int i=0; i < NumOfVertices - 1; i += vertexIncrement) {
            // first get the start and end points for the segment
            GLfloat x1 = Vbos_vertices_p[(i * 2)];
            GLfloat y1 = Vbos_vertices_p[(i * 2) + 1];
            GLfloat x2 = Vbos_vertices_p[(i * 2) + 2];
            GLfloat y2 = Vbos_vertices_p[(i * 2) + 3];
            GLfloat dx = x2 - x1;
            GLfloat dy = y2 - y1;
            if ( dx != 0.0f) {
                slope = dy/dx;
            } else if (x2 > x1) {
                slope = 1000000.0f;
            } else slope = -1000000.0f;
            yIntercept = y2 - x2 * slope;
            // If x2 > x1, then we collide from above
            if (Ship_pntr->Draw == true) {
                if ( x2 - x1 > 0.00001) {
                    if (Ship_pntr->xpos >= x1 and Ship_pntr->xpos < x2 and Ship_pntr->ypos > std::min(y2, y1)) {
                        if ((Ship_pntr->ypos - slope * Ship_pntr->xpos) < (yIntercept + Ship_pntr->Shape.y/2)) {
                            Ship_pntr->explodeShip = true;
                            Ship_pntr->Draw = false;
                            Ship_pntr->playBell = true;
                        }
                    }
                } // if x2 < x1, then we collide from below
                else if ( x2 - x1 < -0.00001) {
                    if (Ship_pntr->xpos <= x1 and Ship_pntr->xpos > x2 and Ship_pntr->ypos < std::max(y2, y1)) {
                        if ((Ship_pntr->ypos - slope * Ship_pntr->xpos) > (yIntercept - Ship_pntr->Shape.y/2)) {
                            Ship_pntr->explodeShip = true;
                            Ship_pntr->Draw = false;
                            Ship_pntr->playBell = true;
                        }
                    }
                } // if x2 = x1, vertical surface.  If y2 > y1, we collide from the left
                else if ( y2 - y1 > 0.00001) {
                    if ( Ship_pntr->ypos >= y1 and Ship_pntr->ypos < y2 and Ship_pntr->xpos < x1) {
                        if (Ship_pntr->xpos >= x1 - Ship_pntr->Shape.x/2) {
                            Ship_pntr->explodeShip = true;
                            Ship_pntr->Draw = false;
                            Ship_pntr->playBell = true;
                        }
                    }
                } // if x2 = x1, vertical surface.  If y2 < y1, we collide from the right
                else if ( y2 - y1 < -0.00001) {
                    if ( Ship_pntr->ypos <= y1 and Ship_pntr->ypos > y2 and Ship_pntr->xpos > x1) {
                        if (Ship_pntr->xpos <= x1 + Ship_pntr->Shape.x/2) {
                            Ship_pntr->explodeShip = true;
                            Ship_pntr->Draw = false;
                            Ship_pntr->playBell = true;
                        }
                    }
                }
            }
            checkPhotons(x1, y1, x2, y2, slope, yIntercept, Ship_pntr, false);
            for (Spaceship &gunna : *Guns_P) {
                checkPhotons(x1, y1, x2, y2, slope, yIntercept, &gunna, true);
            }
//            checkPhotons(x1, y1, x2, y2, slope, yIntercept, Gunner2_p, true);
        }
    }
    
//    void draw(Shader shader, GLuint VAO, GLuint drawArraySpecifier)
//    void draw(glm::vec2 gateTrans = glm::vec2 (0.0f, 0.0f), GLuint gateRot = 0)
    void draw(glm::vec3 gateColor = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat gateThickness = 0.003f)
    {
        sShader->use();
        glBindVertexArray(Vao);
        glm::mat4 model = glm::mat4(1.0f);
        //        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::scale(model, surfaceScaleFactor);
        
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
//        view = glm::translate(view, glm::vec3(xPan, 0.0f, 0.0f));
//        model = glm::translate(model, glm::vec3(gateTrans, 0.0f));
//        model = glm::rotate(model, glm::radians((float)gateRot), glm::vec3(0.0f, 0.0f, 1.0f));
//        model = glm::rotate(model, glm::radians((float)glfwGetTime()*20), glm::vec3(0.0f, 0.0f, 1.0f));
        
        sShader->setMat4("model", model);
        sShader->setMat4("view", view);
        sShader->setVec3("aColor", gateColor);
        sShader->setFloat("scaling", gateThickness);
        glDrawArrays(DrawArraySpecifier, 0, NumOfVertices);
//        glDrawArrays(GL_LINE_STRIP, 0, NumOfVertices);
    }
    
//    void panUpdate()
//    {
//        // Pan Right
//        if (Ship_pntr->xpos > 0.5) {
//            xPan -= Ship_pntr->xpos - 0.5;
//            Ship_pntr->xpos = 0.5f;
//        }
//        
//        // Pan Left
//        if (Ship_pntr->xpos < -0.5) {
//            xPan -= Ship_pntr->xpos + 0.5;
//            Ship_pntr->xpos = -0.5f;
//        }
//        
//        if (xPan <= -2.0) {
//            xPan += 2.0;
//            Ship_pntr->xpos = 0.5f;
//        }
//        if (xPan >= 2.0) {
//            xPan -= 2.0;
//            Ship_pntr->xpos = -0.5f;
//        }
//    }
    
    // Stop all photons from passing through the surfaces
    void checkPhotons(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat slope, GLfloat yIntercept,Spaceship * anyShip_pntr, GLboolean panEnable)
    {
        // Now check if photons have hit surface boundaries, if so stop drawing them
        for ( int j=0; j < anyShip_pntr->maxPhotons; j++) {
            if (anyShip_pntr->photons[j][3] > 0)  {
                if ( x2 - x1 > 0.00001) {
                    //                            std::cout << "I'm in 1\n";
                    if (anyShip_pntr->photons[j][0] >= x1 and anyShip_pntr->photons[j][0] < x2 and anyShip_pntr->photons[j][1] > std::min(y2, y1) - 0.02) {
                        //                                std::cout << "I'm in 1\n";
                        //                                std::cout << (anyShip_pntr->photons[j][1] - slope * anyShip_pntr->photons[j][0]) << std::endl;
                        //                                std::cout << "yInt: " << yIntercept << std::endl;
                        if ((anyShip_pntr->photons[j][1] - slope * (anyShip_pntr->photons[j][0])) < yIntercept) {
                            anyShip_pntr->photons[j][3] = -0.01f;  // photon destroyed in the collision
                        }
                    }
                } // if x2 < x1, then we collide from below
                else if ( x2 - x1 < -0.00001) {
                    if (anyShip_pntr->photons[j][0] <= x1 and anyShip_pntr->photons[j][0] > x2 and anyShip_pntr->photons[j][1] < std::max(y2, y1) + 0.02) {
                        if ((anyShip_pntr->photons[j][1] - slope * (anyShip_pntr->photons[j][0])) > yIntercept) {
                            anyShip_pntr->photons[j][3] = -0.01f;  // photon destroyed in the collision
                        }
                    }
                } // if x2 = x1, vertical surface.  If y2 > y1, we collide from the left
                else if ( y2 - y1 > 0.00001) {
                    if ( anyShip_pntr->photons[j][1] >= y1 and anyShip_pntr->photons[j][1] < y2 and anyShip_pntr->photons[j][0] < x1 + 0.02) {
                        if (anyShip_pntr->photons[j][0] >= x1) {
                            anyShip_pntr->photons[j][3] = -0.01f;  // photon destroyed in the collision
                        }
                    }
                } // if x2 = x1, vertical surface.  If y2 < y1, we collide from the right
                else if ( y2 - y1 < -0.00001) {
                    if ( anyShip_pntr->photons[j][1] <= y1 and anyShip_pntr->photons[j][1] > y2 and anyShip_pntr->photons[j][0] > x1 - 0.02) {
                        if (anyShip_pntr->photons[j][0] <= x1) {
                            anyShip_pntr->photons[j][3] = -0.01f;  // photon destroyed in the collision
                        }
                    }
                }
            }
        }
        
    }
    
    static void drawExit(Shader * shader, Vbos * vao, glm::vec2 position)
    {
        glm::vec3 gateColor = glm::vec3(1.0f, 1.0f, 0.0f);
        shader->use();
        glBindVertexArray(vao->exitVAO);
        glm::mat4 model = glm::mat4(1.0f);
        //        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.0f));
        
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        model = glm::translate(model, glm::vec3(position, 0.0f));
        //        model = glm::rotate(model, glm::radians((float)gateRot), glm::vec3(0.0f, 0.0f, 1.0f));
        //        model = glm::rotate(model, glm::radians((float)glfwGetTime()*20), glm::vec3(0.0f, 0.0f, 1.0f));
        
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setVec3("aColor", gateColor);
//        shader->setFloat("scaling", gateThickness);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //        glDrawArrays(GL_LINE_STRIP, 0, NumOfVertices);
    }
    
};
#endif /* surface_h */
