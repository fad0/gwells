//
//  game.cpp
//  scratch
//
//  Created by Dirk Reese on 3/9/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//
/*******************************************************************
 ** This code is adapted from Breakout.
 **
 ** Breakout is free software: you can redistribute it and/or modify
 ** it under the terms of the CC BY 4.0 license as published by
 ** Creative Commons, either version 4 of the License, or (at your
 ** option) any later version.
 ******************************************************************/

//#include <iostream>

#include <vector>
#include <GL/glew.h>
#include <glfw3.h>

#include "game.h"

//SpriteRenderer  *Renderer;
//GameObject      *Player;
//BallObject      *Ball;

//Game::Game(GLuint width, GLuint height)
//:Width(width), Height(height)
////: State(GAME_ACTIVE), Width(width), Height(height)
//{
//
//}

//Game::~Game()
//{
////    delete Renderer;
////    delete Player;
////    delete Ball;
//}



//void Game::Init()
//{
//    // start the sound engine with default parameters
//    engine = createIrrKlangDevice();
//    
//    if (!engine)
//    {
//        printf("Could not startup engine\n");
////        return 0; // error starting up the engine
//    }
//    // Load shaders
//    // Compile all shaders
//    // Used to draw ships and asteriods
//    Shader shader("/Users/dirk/games/gravitar/shaders/spaceship.vs", "/Users/dirk/games/gravitar/shaders/spaceship.fs");
//    Shader_p = &shader;
//    // Used to draw all photons
//    Shader pShader("/Users/dirk/games/gravitar/shaders/photon.vs", "/Users/dirk/games/gravitar/shaders/spaceship.fs");
//    PShader_p = &pShader;
//    // Used to draw all lines
//    Shader lShader("/Users/dirk/games/gravitar/shaders/lineshader.vs", "/Users/dirk/games/gravitar/shaders/spaceship.fs", "/Users/dirk/games/gravitar/shaders/lineshader.gs");
//    
//    LShader_p = &lShader;
//    
//    // Text shader
////    Shader tshader("/Users/dirk/games/gravitar/shaders/text.vs", "/Users/dirk/games/gravitar/shaders/text.fs");
////    
////    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
////    tshader.use();
////    glUniformMatrix4fv(glGetUniformLocation(tshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
////    
////    Text text;
////    text.init();
//    
//    // Added DR040319 - Changed GetShader to pass a reference to fix this.
////    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
//    // Load textures
//    //    ResourceManager::LoadTexture("/Users/dirk/games/breakout/textures/awesomeface.png", GL_TRUE, "face");
//    // Create vbos object and initialize VBOs
//    Vbos vbos;
//    
//    Vbos_p = &vbos;
//    vbos.init();
//    
//    // Create pointer to the ground vertex array
//    GLfloat * vbos_line_p;
//    vbos_line_p = &vbos.line[0];
//    
//    // Create pointer to the sky island vertex array
//    GLfloat * vbos_skyIsland_p;
//    vbos_skyIsland_p = &vbos.crossIsland[0];
//    
//    // Create pointer to the sky island Gate vertex array
//    GLfloat * vbos_gate_p;
//    vbos_gate_p = &vbos.gate[0];
//    
//    // Ship Object and Pointer for passing to asteriod and attacker objects
//    Spaceship ship;
//    Spaceship * ship_pntr;
//    ship_pntr = &ship;
//    Ship_p = ship_pntr;
//    // Initialize ship
//    ship.init(0.5f, 0.5f, 0);
//    
//    // Attacker Object and Pointer for passing to ship object
//    //    Spaceship gunner1(-0.15f, -0.9f);
//    //    Spaceship * gunner1_p;
//    //    gunner1_p = &gunner1;
//    //    gunner1.init(-0.15f, -0.3f);
//    
//    Guns_p = &Guns;
//    Spaceship gun1; gun1.init(-0.15f, -0.9f, 0);
//    Spaceship gun2; gun2.init( 0.7f, -0.9f, 0);
//    Spaceship gun3; gun3.init( 0.0f, -0.4f, 180);
//    Guns.push_back(gun1);
//    Guns.push_back(gun2);
//    Guns.push_back(gun3);
//    Gun = 0;
//    GLboolean drawGate = true;
//    GLboolean fadeGate = false;
//    GLfloat fadeOutTime = gateFadeDelay;
//    GLfloat fadeDimmer = 0.0f;
//    
//    //    Spaceship gunner2(0.5f, -0.9f);
//    //    Spaceship * gunner2_p;
//    //    gunner2_p = &gunner2;
//    //    gunner2.init(0.5f, -0.9f);
//    
//    // Create surface object for the ground and initialize
//    Surface groundSurface(floorPoints, vbos_line_p, ship_pntr, Guns_p, LShader_p, vbos.lVAO, GL_LINE_STRIP);
//    //    Surface groundSurface(floorPoints, vbos_line_p, ship_pntr, gunner1_p, gunner2_p);
//    
//    // Create surface object for the sky island and initialize
//    Surface skyIslandSurface(crossPoints, vbos_skyIsland_p, ship_pntr, Guns_p, LShader_p, vbos.siVAO, GL_LINE_STRIP);
//    SkyIslandSurface_p = &skyIslandSurface;
//    GroundSurface_p = &groundSurface;
//    
//    //    Surface skyIslandSurface(skyPoints, vbos_skyIsland_p, ship_pntr, gunner1_p, gunner2_p);
//    Surface gateSurface(gatePoints, vbos_gate_p, ship_pntr, Guns_p, LShader_p, vbos.gateVAO, GL_LINE_STRIP);
//    
//    GLboolean gameOver = false;
//    GLuint gameOverCounter = 0;
//    GLfloat lastFrame = 0.0f; // Time of last frame
//    GLboolean scoreLatch = false;
//    GLuint launchAttackCounter = 0;
//    
//    // Seed random number generator for directions of ship explode pieces
//    // and attacker photons
//    srand(int(glfwGetTime() * 10));
//    
//    // Start background music
//    //    engine->play2D("/Users/dirk/games/media/house_lo.wav", true);
//    
//    // Initialize sound pointer for stopping attacker ship siren
//    ISound* snd;
//}
  
//    // Load levels
//    GameLevel one; one.Load("/Users/dirk/games/breakout/levels/one.lvl", this->Width, this->Height * 0.5);
//    GameLevel two; two.Load("/Users/dirk/games/breakout/levels/two.lvl", this->Width, this->Height * 0.5);
//    GameLevel three; three.Load("/Users/dirk/games/breakout/levels/three.lvl", this->Width, this->Height * 0.5);
//    GameLevel four; four.Load("/Users/dirk/games/breakout/levels/four.lvl", this->Width, this->Height * 0.5);
//    this->Levels.push_back(one);
//    this->Levels.push_back(two);
//    this->Levels.push_back(three);
//    this->Levels.push_back(four);
//    this->Level = 0;
//    // Configure game objects
//    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
//    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
//    // Added for ball
//    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
//    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
//                          ResourceManager::GetTexture("face"));


//void Game::ProcessInput(GLFWwindow *window, GLfloat deltaTime)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        Ship_p->ProcessKeyboard(CLOCKWISE, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        Ship_p->ProcessKeyboard(COUNTERCLOCKWISE, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        Ship_p->ProcessKeyboard(BURN, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
//        Ship_p->ProcessKeyboard(NOBURN, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
//        Ship_p->ProcessKeyboard(PHOTON, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
//        Ship_p->ProcessKeyboard(NOPHOTON, deltaTime);
//}

//void Game::CheckBoundary()
//{
//    SkyIslandSurface_p->checkBoundary(1);
//    GroundSurface_p->checkBoundary(1);
//}

//void Game::Draw()
//{
//    SkyIslandSurface_p->draw();
//    GroundSurface_p->draw();
//}

//void Game::UpdateGunners()
//{
//    for (Spaceship &gunna : Guns)
//        gunna.updateGunner(Vbos_p, Shader_p, PShader_p, Ship_p, engine);
//}

//void Game::Update()
//{
//    Ship_p->update(Vbos_p, Shader_p, PShader_p, Guns_p, engine);
//}

//void Game::DeAllocate()
//{
//    Vbos_p->delete_vbos();
//
//    engine->drop(); // delete engine
//}



//void Game::Update(GLfloat dt)
//{
//    // Update objects
//    Ball->Move(dt, this->Width);
//    // Check for collisions
//    this->DoCollisions();
//    // Check loss condition
//    if (Ball->Position.y >= this->Height) // Did ball reach bottom edge?
//    {
//        this->ResetLevel();
//        this->ResetPlayer();
//    }
//}
//
//
//void Game::ProcessInput(GLfloat dt)
//{
//    if (this->State == GAME_ACTIVE)
//    {
//        GLfloat velocity = PLAYER_VELOCITY * dt;
//        // Move playerboard
//        if (this->Keys[GLFW_KEY_A])
//        {
//            if (Player->Position.x >= 0)
//            {
//                Player->Position.x -= velocity;
//                if (Ball->Stuck)
//                    Ball->Position.x -= velocity;
//            }
//        }
//        if (this->Keys[GLFW_KEY_D])
//        {
//            if (Player->Position.x <= this->Width - Player->Size.x)
//            {
//                Player->Position.x += velocity;
//                if (Ball->Stuck)
//                    Ball->Position.x += velocity;
//            }
//        }
//        if (this->Keys[GLFW_KEY_SPACE])
//            Ball->Stuck = false;
//    }
//}
//
//void Game::Render()
//{
//    if (this->State == GAME_ACTIVE)
//    {
//        // Draw background
//        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
//        // Draw level
//        this->Levels[this->Level].Draw(*Renderer);
//        // Draw player
//        Player->Draw(*Renderer);
//        Ball->Draw(*Renderer);
//    }
//}
//
//void Game::ResetLevel()
//{
//    if (this->Level == 0) {
//        this->Levels[0].Load("/Users/dirk/games/breakout/levels/one.lvl", this->Width, this->Height * 0.5f);
//        std::cout << "Trying to reload Level 0\n";
//    }
//    else if (this->Level == 1)
//        this->Levels[1].Load("/Users/dirk/games/breakout/levels/two.lvl", this->Width, this->Height * 0.5f);
//    else if (this->Level == 2)
//        this->Levels[2].Load("/Users/dirk/games/breakout/levels/three.lvl", this->Width, this->Height * 0.5f);
//    else if (this->Level == 3)
//        this->Levels[3].Load("/Users/dirk/games/breakout/levels/four.lvl", this->Width, this->Height * 0.5f);
//}
//
//void Game::ResetPlayer()
//{
//    // Reset player/ball stats
//    Player->Size = PLAYER_SIZE;
//    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
//    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
//}
//

