//
//  vbos.h
//  astro
//
//  Created by Dirk Reese on 2/17/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef vbos_h
#define vbos_h

#include <vector>
#include <cmath>

class Vbos
{
    
public:
    GLuint sVBO, sVAO; // spaceship
    GLuint fVBO, fVAO; // spaceship rocket burn
    GLuint pVBO, pVAO; // photon
    GLuint gunnerVBO, gunnerVAO; // Gunner body
    GLuint cannonVBO, cannonVAO; // Gunner cannon
    GLuint blueholeVBO, blueholeVAO;
    GLuint tVAO, tVBO; // text
    GLuint exitVBO, exitVAO; // exit marker
    // Ship vertices
    GLfloat spaceship[6] = {
        // positions
        -0.5f, -0.4f,
        0.5f,  0.0f,
        -0.5f,  0.4f
    };
    std::vector<GLfloat> ShipPolarCoords;
    
    GLfloat gunner[12] = {
        0.3f, 0.3f,
        -0.5f, 0.0f,
        0.5f, 0.0f,
        
        0.3f, 0.3f,
        -0.3f, 0.3f,
        -0.5f, 0.0f,
    };
    std::vector<GLfloat> GunnerPolarCoords;
    
    GLfloat cannon[12] = {
        -0.1f, 0.3,
        0.1f, 0.45f,
        -0.1f, 0.45f,
        
        0.1f, 0.45f,
        -0.1f, 0.3f,
        0.1f, 0.3f
    };
    
    
    
    
    
    
    
//    static const GLuint skyPoints = 24 * 3;
//    GLfloat skyIsland[skyPoints * 2] = {
//
//        -0.60f, -0.40f,
//        -0.55f, -0.30f,
//        -0.55f, -0.30f,
//        0.70f, -0.30f,
//        0.70f, -0.30f,
//        0.70f, -0.40f,
//        0.70f, -0.40f,
//        0.60f, -0.50f,
//        0.60f, -0.50f,
//
//        0.45f, -0.50f,
//        0.45f, -0.50f,
//        0.40f, -0.40f,
//        0.40f, -0.40f,
//        -0.10f, -0.40f,
//        -0.10f, -0.40f,
//        -0.25f, -0.50f,
//        -0.25f, -0.50f,
//        -0.30f, -0.40f,
//        -0.30f, -0.40f,
//
//        -0.50f, -0.40f,
//        -0.50f, -0.40f,
//        -0.55f, -0.50f,
//        -0.55f, -0.50f,
//        -0.60f, -0.40f
//    };
   
    
    
    Vbos ()
    {}
    
    ~Vbos ()
    {
    }
    
    std::vector<GLfloat> getShipPolarCoords() {
        ShipPolarCoords.push_back(sqrt(pow(spaceship[0],2) + pow(spaceship[1],2)));   // ship bottom R
        ShipPolarCoords.push_back(atan2(spaceship[1], spaceship[0]));               // ship bottom angle theta
        ShipPolarCoords.push_back(sqrt(pow(spaceship[2],2) + pow(spaceship[3],2))); // ship front R
        ShipPolarCoords.push_back(atan2(spaceship[3], spaceship[2]));             // ship front angle 0.
        ShipPolarCoords.push_back(sqrt(pow(spaceship[4],2) + pow(spaceship[5],2)));   //ship top R
        ShipPolarCoords.push_back(atan2(spaceship[5], spaceship[4]));               //ship top angle theta

        return ShipPolarCoords;
    }
    std::vector<GLfloat> getGunnerPolarCoords() {
        GunnerPolarCoords.push_back(sqrt(pow(gunner[4],2) + pow(gunner[5],2)));
        GunnerPolarCoords.push_back(atan2(gunner[5], gunner[4]));
        GunnerPolarCoords.push_back(sqrt(pow(gunner[6],2) + pow(gunner[7],2)));
        GunnerPolarCoords.push_back(atan2(gunner[7], gunner[6]));
        GunnerPolarCoords.push_back(sqrt(pow(gunner[8],2) + pow(gunner[9],2)));
        GunnerPolarCoords.push_back(atan2(gunner[9], gunner[8]));
        GunnerPolarCoords.push_back(sqrt(pow(gunner[10],2) + pow(gunner[11],2)));
        GunnerPolarCoords.push_back(atan2(gunner[11], gunner[10]));
        return GunnerPolarCoords;
    }
    
    void init()
    {
        // Ship vertices
//        GLfloat spaceship[] = {
//            // positions
//            -0.5f, -0.4f,
//            0.5f,  0.0f,
//            -0.5f,  0.4f
//        };
        
        // Ship's rocket flame vertices
        GLfloat flame[] = {
            -0.5f, -0.25f,
            -0.5f,  0.25f,
            -0.8f,  0.0f
        };
        
        // Photon vertex
        GLfloat photonv[] = {
            0.0f, 0.0f
        };
        
        
//        GLfloat gunner[] = {
//            -0.5f, 0.0f,
//            0.3f, 0.3f,
//            -0.3f, 0.3f,
//
//             0.3f, 0.3f,
//            -0.5f, 0.0f,
//             0.5f, 0.0f,
//        };
//
//        GLfloat cannon[] = {
//            -0.1f, 0.3,
//            0.1f, 0.45f,
//            -0.1f, 0.45f,
//
//             0.1f, 0.45f,
//            -0.1f, 0.3f,
//             0.1f, 0.3f
//        };
        
        GLfloat exit[] = {
            0.0f, 0.5,
            -0.2f, 0.0f,
            0.2f, 0.0f,
            
            0.2f, 0.0f,
            -0.2f, 0.0f,
            0.0f, -0.5f
        };
        
        GLfloat bluehole[] = {
            0.5000f, 0.1340f,
            0.3660f, 0.3660f,
            0.0000f, 0.0000f,
            0.3660f, 0.3660f,
            0.1340f, 0.5000f,
            0.0000f, 0.0000f,
            0.1340f, 0.5000f,
            -0.1340f, 0.5000f,
            0.0000f, 0.0000f,
            -0.1340f, 0.5000f,
            -0.3660f, 0.3660f,
            0.0000f, 0.0000f,
            -0.3660f, 0.3660f,
            -0.5000f, 0.1340f,
            0.0000f, 0.0000f,
            -0.5000f, 0.1340f,
            -0.5000f, -0.1340f,
            0.0000f, 0.0000f,
            -0.5000f, -0.1340f,
            -0.3660f, -0.3660f,
            0.0000f, 0.0000f,
            -0.3660f, -0.3660f,
            -0.1340f, -0.5000f,
            0.0000f, 0.0000f,
            -0.1340f, -0.5000f,
            0.1340f, -0.5000f,
            0.0000f, 0.0000f,
            0.1340f, -0.5000f,
            0.3660f, -0.3660f,
            0.0000f, 0.0000f,
            0.3660f, -0.3660f,
            0.5000f, -0.1340f,
            0.0000f, 0.0000f,
            0.5000f, -0.1340f,
            0.5000f, 0.1340f,
            0.0000f, 0.0000f,   
        };

        // VAO's at every level
        //  Spaceship
        //---------------------
        glGenVertexArrays(1, &sVAO);
        glGenBuffers(1, &sVBO);
        glBindVertexArray(sVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spaceship), spaceship, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
        //  Spaceship flame
        //---------------------
        glGenVertexArrays(1, &fVAO);
        glGenBuffers(1, &fVBO);
        glBindVertexArray(fVAO);
        glBindBuffer(GL_ARRAY_BUFFER, fVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(flame), flame, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
        //  Photon
        //---------------------
        glGenVertexArrays(1, &pVAO);
        glGenBuffers(1, &pVBO);
        glBindVertexArray(pVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(photonv), photonv, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
        //  Gunner body
        //---------------------
        glGenVertexArrays(1, &gunnerVAO);
        glGenBuffers(1, &gunnerVBO);
        glBindVertexArray(gunnerVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gunnerVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gunner), gunner, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
        //  Gunner cannon
        //---------------------
        glGenVertexArrays(1, &cannonVAO);
        glGenBuffers(1, &cannonVBO);
        glBindVertexArray(cannonVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cannonVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cannon), cannon, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
        //  Bluehole
        //---------------------
        glGenVertexArrays(1, &blueholeVAO);
        glGenBuffers(1, &blueholeVBO);
        glBindVertexArray(blueholeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, blueholeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(bluehole), bluehole, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Configure VAO/VBO for texture quads
        glGenVertexArrays(1, &tVAO);
        glGenBuffers(1, &tVBO);
        glBindVertexArray(tVAO);
        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
 
        //  Exit Marker
        //---------------------
        glGenVertexArrays(1, &exitVAO);
        glGenBuffers(1, &exitVBO);
        glBindVertexArray(exitVAO);
        glBindBuffer(GL_ARRAY_BUFFER, exitVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(exit), exit, GL_STATIC_DRAW);
        // set and enable vertices pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
    }
    
    void delete_vbos()
    {
        glDeleteVertexArrays(1, &sVAO);
        glDeleteVertexArrays(1, &fVAO);
        glDeleteVertexArrays(1, &pVAO);
        glDeleteVertexArrays(1, &gunnerVAO);
        glDeleteVertexArrays(1, &cannonVAO);
        glDeleteVertexArrays(1, &tVAO);
        glDeleteVertexArrays(1, &exitVAO);
        glDeleteVertexArrays(1, &blueholeVAO);
//        glDeleteVertexArrays(1, &lVAO);
//        glDeleteVertexArrays(1, &siVAO);
//        glDeleteVertexArrays(1, &gateVAO);
        glDeleteBuffers(1, &sVBO);
        glDeleteBuffers(1, &fVBO);
        glDeleteBuffers(1, &pVBO);
        glDeleteBuffers(1, &gunnerVBO);
        glDeleteBuffers(1, &cannonVBO);
        glDeleteBuffers(1, &tVBO);
        glDeleteBuffers(1, &exitVBO);
        glDeleteBuffers(1, &blueholeVBO);
//        glDeleteBuffers(1, &lVBO);
//        glDeleteBuffers(1, &siVBO);
//        glDeleteBuffers(1, &gateVBO);
    }
};


#endif /* vbos_h */
