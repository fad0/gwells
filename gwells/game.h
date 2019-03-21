//
//  game.hpp
//  gravdb
//
//  Created by Dirk Reese on 3/10/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef game_h
#define game_h

#include <GL/glew.h>
#include <glfw3.h>

#include <stdio.h>
#include <map>
#include <string>

//#include <fstream>
//#include <sstream>
//#include <iostream>

//#include "constants.h"
//#include "globals.h"

#include "shader.h"
#include "vbos.h"
#include "spaceship.h"
#include "surface.h"
#include "surface2.h"
#include "game_level.h"
#include "boundary.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Game
{

    
public:
    // Game state
    //    GameState              State;
    GLuint                 Width, Height;
    GameState              State;
    GLboolean              Gameover = false;
    
    Game(GLuint width, GLuint height);
    Shader * Shader_p;
    Shader * PShader_p;
    Shader * LShader_p;
    Vbos * Vbos_p;
    Vbos * vbos_line_p;
    Vbos * vbos_skyIsland_p;
    Vbos * vbos_gate_p;
//    Spaceship * Ship_p;
    std::vector<Spaceship> Ship;
    std::vector<Spaceship> * Ship_p;
    std::vector<Spaceship> Guns;
    std::vector<Spaceship> * Guns_p;
    
    Surface2 * GroundSurface_p;
    Surface2 * SkyIslandSurface_p;
    Surface2 * SkyIslandSurface_p1;
    Surface2 * GateSurface_p;
    
    std::vector<GameLevel> Levels;
    std::vector<std::vector<std::string>>  levelVertNames;
    std::vector<std::vector<std::string>>  levelGateName;
    
    std::vector<Surface2> Surfaces;
    Surface2 Gate;
    
    GLboolean drawGate = true;
    GLboolean fadeGate = false;
    const GLfloat gateFadeDelay = 2.0;
    GLfloat fadeOutTime = gateFadeDelay;
    GLfloat fadeDimmer = 0.0f;
    
    //Level dependent variables
    glm::vec2 exitPoint;
    std::vector<glm::vec2> exitPoints;
    std::vector<glm::vec2> startPoints;
    std::vector<std::vector<glm::vec2>> centerOfGravitys;
    std::vector<std::vector<GLfloat>> gravityStrengths;
    
    std::vector<GLfloat> ShipPolarCoords;
    
    void Init();
    void InitShip();
    void InitGuns();
    void InitSurfaces();
    void Update(GLfloat dt);
    void ProcessInput(GLFWwindow *window, GLfloat deltaTime);
    void UpdateGate(GLfloat deltaTime);
    void CheckStatus();
    void UpdateLevel();
    void InitGravity();
    
};


#endif /* game_h */
