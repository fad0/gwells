//
//  game.hpp
//  scratch
//
//  Created by Dirk Reese on 3/9/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//
#ifndef GAME_H
#define GAME_H
//
//
//#include <irrKlang.h>
//// include console I/O methods (conio.h for windows, our wrapper in linux)
//#include <conio.h>
//// Also, we tell the compiler to use the namespaces 'irrklang'.
//// All classes and functions of irrKlang can be found in the namespace 'irrklang'.
//// If you want to use a class of the engine,
//// you'll have to type an irrklang:: before the name of the class.
//// For example, to use the ISoundEngine, write: irrklang::ISoundEngine. To avoid having
//// to put irrklang:: before of the name of every class, we tell the compiler that
//// we use that namespaces here.
//using namespace irrklang;
//// To be able to use the irrKlang.dll file, we need to link with the irrKlang.lib.
//// We could set this option in the project settings, but to make it easy we use
//// a pragma comment:
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#include <vector>
#include <GL/glew.h>
#include <glfw3.h>

//#include "shader.h"
#include "globals.h"
#include "constants.h"
//#include "vbos.h"
//#include "spaceship.h"
//#include "surface.h"

//#include "game_level.h"

// Represents the current state of the game
//enum GameState {
//    GAME_ACTIVE,
//    GAME_MENU,
//    GAME_WIN
//};
//
//// Represents the four possible (collision) directions
//enum Direction {
//    UP,
//    RIGHT,
//    DOWN,
//    LEFT
//};

//// Defines a Collision typedef that represents collision data
//typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>
//
//// Initial size of the player paddle
//const glm::vec2 PLAYER_SIZE(100, 20);
//// Initial velocity of the player paddle
//const GLfloat PLAYER_VELOCITY(500.0f);
//// Initial velocity of the Ball
//const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
//// Radius of the ball object
//const GLfloat BALL_RADIUS = 12.5f;


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
//    GameState              State;
    GLuint                 Width, Height;
//    Shader * Shader_p;
//    Shader * PShader_p;
//    Shader * LShader_p;
//    Vbos * Vbos_p;
//    Spaceship * Ship_p;
//    Surface * GroundSurface_p;
//    Surface * SkyIslandSurface_p;
//    std::vector<Spaceship> Guns;
//    std::vector<Spaceship> * Guns_p;
//    ISoundEngine * engine;
//    
//    GLuint Gun;
//    std::vector<GameLevel> Levels;
//    GLuint                 Level;
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
//    ~Game();
    // Initialize game state (load all shaders/textures/levels)
//    void Init();
//    // GameLoop
//    void ProcessInput(GLFWwindow* window, GLfloat deltaTime);
//    void CheckBoundary();
//    void Draw();
//    void UpdateGunners();
//    void Update();
//    void DeAllocate();
//    void Update(GLfloat dt);
//    void Render();
//    void DoCollisions();
//    // Reset
//    void ResetLevel();
//    void ResetPlayer();
};

#endif
