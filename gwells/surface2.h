//
//  surface.h
//  scratch
//
//  Created by Dirk Reese on 2/24/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef surface2_h
#define surface2_h

// Header file
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "vbos.h"

#include <math.h>
#include <vector>

//!!!!!!!!!!!REMOVE LATER!!!!!!!!!!!!!!!!!!!!!
//const GLfloat xPan = 0.0f;

class Surface2
{
    GLfloat slope;
    GLfloat yIntercept;
    glm::vec3 surfaceScaleFactor = glm::vec3(1.0f, 1.0f, 1.0f);
    
 
public:
//    glm::vec2 Position;
    GLuint NumOfVertices;
    std::vector<GLfloat> SurfaceVerts;
    GLfloat * Vbos_vertices_p;
    Spaceship * Ship_pntr;
//    Spaceship * Gunner1_p;
//    std::vector<Spaceship> * Guns_P;
    Shader * sShader;
    GLuint Vao;
    
    // Polar coords for Ship vertices
    std::vector<GLfloat> ShipPolarCoords;
    
    
//    Spaceship * Gunner2_p;
    
    Surface2 () {}
    
//    Surface2 (const GLuint numOfVertices, std::vector<GLfloat> SurfaceVerts, Spaceship * ship_pntr, std::vector<Spaceship> * Guns_p, Shader * shader, GLuint vao, GLuint drawArraySpecifier)
//    {
//        NumOfVertices = numOfVertices;
////        Vbos_vertices_p = vbos_vertices_p;
//        SurfaceVerts = SurfaceVerts;
//        Ship_pntr = ship_pntr;
//        Guns_P = Guns_p;
//        Vao = vao;
//        sShader = shader;
//        DrawArraySpecifier = drawArraySpecifier;
////        Gunner2_p = gunner2_p;
//    }
    
    void init(const GLuint numOfVertices, std::vector<GLfloat> surfaceVerts, Spaceship * ship_pntr, Shader * shader, GLuint vao, std::vector<GLfloat> shipPolarCoords)
    {
        NumOfVertices = numOfVertices;
        //        Vbos_vertices_p = vbos_vertices_p;
        SurfaceVerts = surfaceVerts;
        Ship_pntr = ship_pntr;
//        Guns_P = Guns_p;
        Vao = vao;
        sShader = shader;
        ShipPolarCoords = shipPolarCoords;
    }
    
    // Continuous surfaces use GL_LINE_STRIP so vertexIncrement = 1
    // Island surfaces use GL_LINES so vertexIncrement = 2
    void checkBoundary()
    {
        // For each line segment of the "surface" check if ship has impacted it
        // http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
        // equation derivation for intersection of two line segments.
        if (Ship_pntr->Draw == true) {
            for (int j=0; j < ShipPolarCoords.size(); j+=2){
                // Calculate ship vertices rotated positions and scale
                GLfloat theta1 = ShipPolarCoords[(j + 1) % ShipPolarCoords.size()] + Ship_pntr->Direction;
                GLfloat theta2 = ShipPolarCoords[(j + 3) % ShipPolarCoords.size()] + Ship_pntr->Direction;
                
                GLfloat x1_rot = ShipPolarCoords[(j + 0) % ShipPolarCoords.size()] * cos(theta1) * Ship_pntr->Shape.x;
                GLfloat x2_rot = ShipPolarCoords[(j + 2) % ShipPolarCoords.size()] * cos(theta2) * Ship_pntr->Shape.x;
                GLfloat y1_rot = ShipPolarCoords[(j + 0) % ShipPolarCoords.size()] * sin(theta1) * Ship_pntr->Shape.y;
                GLfloat y2_rot = ShipPolarCoords[(j + 2) % ShipPolarCoords.size()] * sin(theta2) * Ship_pntr->Shape.y;
                
                // Transpose to actual location
                GLfloat x1 = x1_rot + Ship_pntr->xpos;
                GLfloat y1 = y1_rot + Ship_pntr->ypos;
                GLfloat x2 = x2_rot + Ship_pntr->xpos;
                GLfloat y2 = y2_rot + Ship_pntr->ypos;
//                printf("x1 = %3.3f  y1 = %3.3f  x2 = %3.3f  y2 = %3.3f\n", x1, y1, x2, y2);               
                for (int i=0; i < NumOfVertices - 1; i++) {
                    // first get the start and end points for each surface segment
                        GLfloat x3 = SurfaceVerts[(i * 2)] + xPan;
                        GLfloat y3 = SurfaceVerts[(i * 2) + 1];
                        GLfloat x4 = SurfaceVerts[(i * 2) + 2] + xPan;
                        GLfloat y4 = SurfaceVerts[(i * 2) + 3];
//                    printf("x3 = %3.2f  y3 = %3.2f  x4 = %3.2f  y4 = %3.2f\n", x3, y3, x4, y4);
                    
                    GLfloat denominator = (x4 - x3)*(y1 - y2) - (x1 - x2)*(y4 - y3);
                    
                    if ( denominator != 0 ) {
                        GLfloat ta=((y3 - y4)*(x1 - x3) + (x4 - x3)*(y1 - y3))/denominator;
                        GLfloat tb=((y1 - y2)*(x1 - x3) + (x2 - x1)*(y1 - y3))/denominator;
//                        printf("ta = %3.2f  tb = %3.2f\n", ta, tb);
                        if (ta >= 0 and ta <= 1 and tb >= 0 and tb <= 1) {
//                            printf("I'm in!\n");
                            Ship_pntr->explodeShip = true;
                            Ship_pntr->Draw = false;
                            Ship_pntr->playBell = true;
                        }
                    }
                }
            }
        }
    }
    
//    void draw(Shader shader, GLuint VAO, GLuint drawArraySpecifier)
//    void draw(glm::vec2 gateTrans = glm::vec2 (0.0f, 0.0f), GLuint gateRot = 0)
    void draw(glm::vec3 gateColor = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat gateThickness = 0.001f)
    {
        sShader->use();
        glBindVertexArray(Vao);
        glm::mat4 model = glm::mat4(1.0f);
        //        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::scale(model, surfaceScaleFactor);
        
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(xPan, 0.0f, 0.0f));
//        model = glm::translate(model, glm::vec3(gateTrans, 0.0f));
//        model = glm::rotate(model, glm::radians((float)gateRot), glm::vec3(0.0f, 0.0f, 1.0f));
//        model = glm::rotate(model, glm::radians((float)glfwGetTime()*20), glm::vec3(0.0f, 0.0f, 1.0f));
        
        sShader->setMat4("model", model);
        sShader->setMat4("view", view);
        sShader->setVec3("aColor", gateColor);
        sShader->setFloat("aThickness", gateThickness);
        glDrawArrays(GL_LINE_STRIP, 0, NumOfVertices);
//        glDrawArrays(GL_LINE_STRIP, 0, NumOfVertices);
    }

    // Stop all photons from passing through the surfaces
    // Inside vector holds photon params : 0-xpos, 1-ypos, 2-direction, 3-duration,
    // 4-start xpos, 5-start ypos, 6-end xpos (virtual), 7-end ypos (virtual), 8-new photon indicator,
    // 9-photonMaxDistance = 2 * sqrt(2) -> this will be replaced by distane to nearest collision surface
    void checkPhotons(std::vector<Spaceship> * anyShip)
    {
        
        for (Spaceship &gunna : * anyShip) {
//            printf("checkPhotons gunna.photons.size() = %lu\n", gunna.photons.size());
            // Now check if photons have hit surface boundaries, if so stop drawing them
            for ( int j=0; j < gunna.photons.size(); j++) {
//                printf("made it this far 1\n");
//                if (gunna.photons[j][3] > 0)  {
                if (gunna.photons[j][8] > 0) {
                    GLfloat x1 = gunna.photons[j][4];
                    GLfloat y1 = gunna.photons[j][5];
                    GLfloat x2 = gunna.photons[j][6];
                    GLfloat y2 = gunna.photons[j][7];
                    
                    for (int i=0; i < NumOfVertices - 1; i++) {
                        // first get the start and end points for each surface segment
                        GLfloat x3 = SurfaceVerts[(i * 2)] + xPan;
                        GLfloat y3 = SurfaceVerts[(i * 2) + 1];
                        GLfloat x4 = SurfaceVerts[(i * 2) + 2] + xPan;
                        GLfloat y4 = SurfaceVerts[(i * 2) + 3];
                        //                    printf("x3 = %3.2f  y3 = %3.2f  x4 = %3.2f  y4 = %3.2f\n", x3, y3, x4, y4);
                        
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
//                                printf("gunna.photons[%d][9] = %4.3f\n", j, gunna.photons[j][9]);
//                                printf("photonSegmentLength = %4.3f\n", photonSegmentLength);
                                if (gunna.photons[j][9] > photonSegmentLength) {
                                    gunna.photons[j].pop_back();
                                    gunna.photons[j].push_back(photonSegmentLength);
                                }
                            }
                        }
                    }
                }
//                }
            }
            for ( int j=0; j < gunna.photons.size(); j++) {
                GLfloat xstart = gunna.photons[j][4]; // start xpos
                GLfloat ystart = gunna.photons[j][5]; // start ypos
                GLfloat xpos = gunna.photons[j][0]; // current xpos
                GLfloat ypos = gunna.photons[j][1]; // current ypos
                GLfloat currentLength = sqrt(pow(xpos - xstart, 2) + pow(ypos - ystart, 2));
                if (currentLength >= gunna.photons[j][9] or gunna.photons[j][3] <= 0.0f) {
//                    printf("Collision!\n");
//                    gunna.photons[j][3] = -0.01f;
                    gunna.photons.erase (gunna.photons.begin() + j);
                }
                
            }
        }
    }
    
    void checkPhotons2()
    {
        
//        for (Spaceship &gunna : * anyShip) {
//            printf("checkPhotons gunna.photons.size() = %lu\n", gunna.photons.size());
            // Now check if photons have hit surface boundaries, if so stop drawing them
            for ( int j=0; j < Ship_pntr->photons.size(); j++) {
//                printf("made it this far 1\n");
                //                if (Ship_pntr->photons[j][3] > 0)  {
                if (Ship_pntr->photons[j][8] > 0) {
                    GLfloat x1 = Ship_pntr->photons[j][4];
                    GLfloat y1 = Ship_pntr->photons[j][5];
                    GLfloat x2 = Ship_pntr->photons[j][6];
                    GLfloat y2 = Ship_pntr->photons[j][7];
                    
                    for (int i=0; i < NumOfVertices - 1; i++) {
                        // first get the start and end points for each surface segment
                        GLfloat x3 = SurfaceVerts[(i * 2)] + xPan;
                        GLfloat y3 = SurfaceVerts[(i * 2) + 1];
                        GLfloat x4 = SurfaceVerts[(i * 2) + 2] + xPan;
                        GLfloat y4 = SurfaceVerts[(i * 2) + 3];
                        //                    printf("x3 = %3.2f  y3 = %3.2f  x4 = %3.2f  y4 = %3.2f\n", x3, y3, x4, y4);
                        
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
//                                printf("Ship_pntr->photons[%d][9] = %4.3f\n", j, Ship_pntr->photons[j][9]);
//                                printf("photonSegmentLength = %4.3f\n", photonSegmentLength);
                                if (Ship_pntr->photons[j][9] > photonSegmentLength) {
                                    Ship_pntr->photons[j].pop_back();
                                    Ship_pntr->photons[j].push_back(photonSegmentLength);
                                }
                            }
                        }
                    }
//                }
                //                }
            }
            for ( int j=0; j < Ship_pntr->photons.size(); j++) {
                GLfloat xstart = Ship_pntr->photons[j][4]; // start xpos
                GLfloat ystart = Ship_pntr->photons[j][5]; // start ypos
                GLfloat xpos = Ship_pntr->photons[j][0]; // current xpos
                GLfloat ypos = Ship_pntr->photons[j][1]; // current ypos
                GLfloat currentLength = sqrt(pow(xpos - xstart, 2) + pow(ypos - ystart, 2));
                if (currentLength >= Ship_pntr->photons[j][9] or Ship_pntr->photons[j][3] <= 0.0f) {
//                    printf("Collision!\n");
                    //                    Ship_pntr->photons[j][3] = -0.01f;
                    Ship_pntr->photons.erase (Ship_pntr->photons.begin() + j);
                }
                
            }
        }
    }
    
    void drawExit(Shader * shader, Vbos * vao, glm::vec2 position, GLfloat gateThickness = 0.002f)
    {
        glm::vec3 gateColor = glm::vec3(1.0f, 1.0f, 0.0f);
        shader->use();
        glBindVertexArray(vao->exitVAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.0f));
        
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
//        view = glm::translate(view, glm::vec3(xPan, 0.0f, 0.0f));
        
        model = glm::rotate(model, glm::radians((float)glfwGetTime()*20), glm::vec3(0.0f, 0.0f, 1.0f));
        
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setVec3("aColor", gateColor);
        shader->setFloat("aThickness", gateThickness);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //        glDrawArrays(GL_LINE_STRIP, 0, NumOfVertices);
    }
    
    void drawBluehole(Shader * shader, Vbos * vao, glm::vec2 position)
    {
//        glm::vec3 gateColor = glm::vec3(0.2f, 0.0f, 0.5f);
        glm::vec3 gateColor = glm::vec3(0.4f, 0.0f, 0.4) + glm::vec3(0.1f * cos(glm::radians((float)glfwGetTime()*100)));
        shader->use();
        glBindVertexArray(vao->blueholeVAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.0f));
        
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        //        view = glm::translate(view, glm::vec3(xPan, 0.0f, 0.0f));
        
//        model = glm::rotate(model, glm::radians((float)glfwGetTime()*20), glm::vec3(0.0f, 0.0f, 1.0f));
        
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setVec3("aColor", gateColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //        glDrawArrays(GL_LINE_STRIP, 0, NumOfVertices);
    }
    
};
#endif /* surface_h */
