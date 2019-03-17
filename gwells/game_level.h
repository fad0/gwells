//
//  game_level.hpp
//  scratch
//
//  Created by Dirk Reese on 3/11/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef game_level_h
#define game_level_h

#include <stdio.h>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "boundary.h"


/// GameLevel holds all Tiles as part of a Breakout level and
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // Level state
    const GLchar *File;
    std::vector<std::string> vboName;
    std::map<std::string, std::vector<GLfloat>>  Level;
    std::vector<GLfloat> verts;
    std::vector<GLuint> VBOS;
    std::vector<GLuint> VAOS;
    GLuint gateVAO;
    GLuint gateVBO;
    // Constructor
    GameLevel(const GLchar *file);
    // Loads level from file
    void Load();
//    // Render level
//    void      Draw();
//    // Check if the level is completed (all non-solid tiles are destroyed)
//    GLboolean IsCompleted();
//private:
//    // Initialize level from tile data
//    void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif /* game_level_h */
