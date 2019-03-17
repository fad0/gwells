//
//  game.cpp
//  gravdb
//
//  Created by Dirk Reese on 3/10/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//


#include <iostream>
#include "game.h"

//std::vector<Surface2> Game::surfaces;
//std::vector<GameLevel> Game::Levels;
static GLuint currentLevel = 0;

Game::Game(GLuint width, GLuint height)
//:Width(width), Height(height)
: State(GAME_ACTIVE), Width(width), Height(height)
{
    std::cout << "Width = " << Width << "  Height = " << Height << std::endl;
}

void Game::Init()
{
    std::cout << "In Init: Width = " << Width << "  Height = " << Height << std::endl;
    // Load shaders
    // Compile all shaders
    // Used to draw ships and asteriods
    std::string name = "shader";
    static Shader shader("/Users/dirk/games/gwells/shaders/spaceship.vs", "/Users/dirk/games/gwells/shaders/spaceship.fs");
//    Shaders[name] = shader;
//    std::cout << "game shader ID = " << shader.ID << std::endl;
    
    Shader_p = &shader;
    // Used to draw all photons
    static Shader pShader("/Users/dirk/games/gwells/shaders/photon.vs", "/Users/dirk/games/gwells/shaders/spaceship.fs");
    PShader_p = &pShader;
    // Used to draw all lines
    static Shader lShader("/Users/dirk/games/gwells/shaders/lineshader.vs", "/Users/dirk/games/gwells/shaders/spaceship.fs", "/Users/dirk/games/gwells/shaders/lineshader.gs");
    LShader_p = &lShader;
    
//    /// Text shader
//    Shader tshader("/Users/dirk/games/gwells/shaders/text.vs", "/Users/dirk/games/gwells/shaders/text.fs");
//
//    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
//    tshader.use();
//    glUniformMatrix4fv(glGetUniformLocation(tshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    static Vbos vbos;

    Vbos_p = &vbos;
    vbos.init();
    
    // Load all Levels
    GameLevel level1("/Users/dirk/games/gwells/levels/level1.txt"); level1.Load();
    GameLevel level2("/Users/dirk/games/gwells/levels/level2.txt"); level2.Load();
    GameLevel level3("/Users/dirk/games/gwells/levels/level3.txt"); level3.Load();
    GameLevel level4("/Users/dirk/games/gwells/levels/level4.txt"); level4.Load();
    GameLevel level5("/Users/dirk/games/gwells/levels/level5.txt"); level5.Load();
    
    Levels.push_back(level1);
    Levels.push_back(level2);
    Levels.push_back(level3);
    Levels.push_back(level4);
    Levels.push_back(level5);
//    std::cout << "Level 1 skyvertices size: " << Levels[0].Level[Levels[0].vboName[0]].size() << std::endl;
//    std::cout << "Level 2 skyvertices size: " << Levels[1].Level[Levels[1].vboName[0]].size() << std::endl;
//    std::cout << "Level 3 skyvertices size: " << Levels[2].Level[Levels[2].vboName[0]].size() << std::endl;
    currentLevel = 0;
    
    std::vector<std::string> vertNames;
    std::vector<std::string> gateIndexes;
    for (int i=0; i < Levels.size(); i++) {
        for (int j=0; j < Levels[i].vboName.size(); j++) {
            
            if (std::strncmp (Levels[i].vboName[j].c_str(),"VERT_surf",9) == 0) {
                vertNames.push_back(Levels[i].vboName[j]);
                //                std::cout << Levels[i].vboName[j] << std::endl;
            } else if (std::strncmp (Levels[i].vboName[j].c_str(),"VERT_gate",8) == 0) {
                gateIndexes.push_back(Levels[i].vboName[j]);
            }
        }
        levelVertNames.push_back(vertNames);
        levelGateName.push_back(gateIndexes);
        vertNames.clear();
        gateIndexes.clear();
    }

    
    exitPoints.clear();
    startPoints.clear();
    for (int i=0; i < Levels.size(); i++) {
        if ( Levels[i].Level["VERT_exit"].size() <= 0) {
            exitPoints.push_back(glm::vec2(-10.0, -10.0));
            std::cout << " VERT_exit does not exit in Level[" << i << "]\n";
        } else
            exitPoints.push_back(glm::vec2(Levels[i].Level["VERT_exit"][0], Levels[i].Level["VERT_exit"][1]));
        
        if ( Levels[i].Level["VERT_ship"].size() > 0)
            startPoints.push_back(glm::vec2(Levels[i].Level["VERT_ship"][0], Levels[i].Level["VERT_ship"][1]));
//        std::cout << "Level = " << i << std::endl;
//        std::cout << "exit.x = " << Levels[i].Level["VERT_exit"][0] << std::endl;
//        std::cout << "exit.y = " << Levels[i].Level["VERT_exit"][1] << std::endl;
    }

    // Ship Object and Pointer for passing to asteriod and attacker objects
    static Spaceship ship;
    Ship_p = &ship;
    // Initialize ship
//    ship.init(0.0, 0.00, 0.0);
    ship.init(startPoints[currentLevel][0], startPoints[currentLevel][1], 0.0);
    
    // Initialize first Level
    this->InitGuns();
    
    // Initialize first Level
    this->InitSurfaces();
    
//    this->InitSurfaces();
    this->InitGravity();
    
    // Set Level exit location and next level flag
    exitPoint = exitPoints[currentLevel];

    drawGate = true;
    fadeGate = false;
    fadeOutTime = gateFadeDelay;
    fadeDimmer = 0.0f;
    
}

void Game::Update(GLfloat deltaTime)
{
    if (exitPoints[currentLevel][0] != -10.0)
        GroundSurface_p->drawExit(Shader_p, Vbos_p, exitPoint);
    Ship_p->update(Vbos_p, Shader_p, PShader_p, Guns_p, deltaTime);
    
    for (glm::vec2 &gwells : Ship_p->centerOfGravity)
        GroundSurface_p->drawBluehole(Shader_p, Vbos_p, gwells);
    
    for (Spaceship &gunna : Guns)
        gunna.updateGunner(Vbos_p, Shader_p, PShader_p, Ship_p, deltaTime);
    
    for (Surface2 &surfa : Surfaces) {
        surfa.draw();
        surfa.checkBoundary(1);
    }
    
//    for (GLuint &indexa : levelSurfacesIndexes[currentLevel]) {
//        Surfaces[indexa].draw();
//        Surfaces[indexa].checkBoundary(1);
//    }
}

// Check if all gunners have been destroyed
// if no, draw the gate
// if yes, fade the gate away
void Game::UpdateGate(GLfloat deltaTime)
{
    glm::vec3 gateColor = glm::vec3(0.6f, 0.6f, 0.f);
    if (drawGate == true) {
        GLboolean allGunsDestroyed = false;
        for (Spaceship &gunna : *Guns_p) allGunsDestroyed += gunna.Draw;
        if (allGunsDestroyed == false) {
            fadeGate = true;
        }
        if (fadeGate == false) {
            GLfloat gateFlux = 0.2 * cos(glm::radians((float)glfwGetTime()*500));
            gateColor = glm::vec3(0.6f, 0.6f, 0.f) + gateFlux;
        } else {
            fadeOutTime -= deltaTime;
            fadeDimmer = fadeOutTime/gateFadeDelay;
            gateColor = glm::vec3(0.6f, 0.6f, 0.f) * fadeDimmer;
            if (fadeDimmer <= 0) drawGate = false;
        }
        GLfloat gateThickness = 0.003f;

        Gate.draw(gateColor, gateThickness);
        Gate.checkBoundary(1);
    }
}

// keep track of extra spaceships
//--------------------------
void Game::CheckStatus()
{
    if (Ship_p->explodeShip == true) {
        if (Ship_p->shipsLeft == 0) {
            Gameover = true;
        } else Ship_p->latchExplodeShip = true;
    }
    
    // wait until ship finishes exploding before instantiating new one
    if (Ship_p->explodeShip == false and Ship_p->latchExplodeShip == true) {
        Ship_p->shipsLeft--;
        Ship_p->init(startPoints[currentLevel][0], startPoints[currentLevel][1], 0.0);
        Ship_p->latchExplodeShip = false;
    }
    
    if (drawGate == false) {
        if ( abs(Ship_p->xpos - exitPoint.x) < 0.07f and abs(Ship_p->ypos - exitPoint.y) < 0.07f) {
            UpdateLevel();
        }
    }
    
}

void Game::UpdateLevel()
{
    if (currentLevel < Levels.size() - 1) {
        currentLevel++;
//        std::cout << "currentLevel = " << currentLevel << std::endl;
//        std::cout << "Levels.size() = " << Levels.size() << std::endl;
//        Ship_p->init(0.5,0.5,0.0);
        Ship_p->init(startPoints[currentLevel][0], startPoints[currentLevel][1], 0);

        this->InitGuns();
        
        Boundary::DeleteVBO(Levels[currentLevel - 1].VAOS[0], Levels[currentLevel - 1].VBOS[0]);
        Boundary::DeleteVBO(Levels[currentLevel - 1].VAOS[1], Levels[currentLevel - 1].VBOS[1]);
        Boundary::DeleteVBO(Levels[currentLevel - 1].VAOS[2], Levels[currentLevel - 1].VBOS[2]);
        
        this->InitSurfaces();
        this->InitGravity();
        
        drawGate = true;
        fadeGate = false;
        fadeDimmer = 0.0f;
        exitPoint = exitPoints[currentLevel];
    }
}

void Game::InitGuns() {
    Guns.clear();
    for(int i=0; i < Levels[currentLevel].Level["VERT_guns"].size()/3; i++) {
        Spaceship obj;
        obj.init(Levels[currentLevel].Level["VERT_guns"][(i * 3)], Levels[currentLevel].Level["VERT_guns"][(i * 3) + 1], Levels[currentLevel].Level["VERT_guns"][(i * 3) + 2]);
//        std::cout << "Gun vertex[0] " << Levels[currentLevel].Level["VERT_guns"][(i * 3)] << std::endl;
//        std::cout << "Gun vertex[1] " << Levels[currentLevel].Level["VERT_guns"][(i * 3)+1] << std::endl;
//        std::cout << "Gun vertex[2] " << Levels[currentLevel].Level["VERT_guns"][(i * 3)+2] << std::endl;
        Guns.push_back(obj);
    }
    Guns_p = &Guns;
    Gun = 0;
}

void Game::InitSurfaces() {
    Surfaces.clear();
    for(int i=0; i < levelVertNames[currentLevel].size(); i++) {
        Surface2 obj;
        obj.init((GLuint) Levels[currentLevel].Level[levelVertNames[currentLevel][i]].size()/2, Levels[currentLevel].Level[levelVertNames[currentLevel][i]], Ship_p, Guns_p, LShader_p, Levels[currentLevel].VAOS[i], GL_LINE_STRIP);
        Surfaces.push_back(obj);
        //------------------------
//        std::cout << "current level = " << currentLevel << std::endl;
//        std::cout << "levelVertNames[][i] = " << levelVertNames[currentLevel][i] << std::endl;
//        std::cout << "levelVertNames[].size() = " << levelVertNames[currentLevel].size() << std::endl;
//        
//        std::cout << "Levels[].Level[name].size = " << Levels[currentLevel].Level[levelVertNames[currentLevel][i]].size() << std::endl;
//        std::cout << "Levels[currentLevel].VAOS[i] = " << Levels[currentLevel].VAOS[i] << std::endl;
        //------------------------
        
        
    }
    Gate.init((GLuint) Levels[currentLevel].Level["VERT_gate"].size()/2, Levels[currentLevel].Level["VERT_gate"], Ship_p, Guns_p, LShader_p, Levels[currentLevel].gateVAO, GL_LINE_STRIP);
    
}

//void Game::InitSurfaces() {
//    Surfaces.clear();
//    for(int i=0; i < levelSurfacesIndexes[currentLevel].size(); i++) {
//        Surface2 obj;
//        obj.init((GLuint) Levels[currentLevel].Level[Levels[currentLevel].vboName[i]].size()/2, Levels[currentLevel].Level[Levels[currentLevel].vboName[i]], Ship_p, Guns_p, LShader_p, Levels[currentLevel].VAOS[i], GL_LINE_STRIP);
//        Surfaces.push_back(obj);
//    }
//}

void Game::InitGravity() {
    Ship_p->centerOfGravity.clear();
    Ship_p->gravityStrength.clear();
    glm::vec2 centerOfG;
    GLfloat gstrength;
    for(int i=0; i < Levels[currentLevel].Level["VERT_cofg"].size()/3; i++) {
        centerOfG = glm::vec2(Levels[currentLevel].Level["VERT_cofg"][(i * 3)], Levels[currentLevel].Level["VERT_cofg"][(i * 3) + 1]);
        gstrength = Levels[currentLevel].Level["VERT_cofg"][(i * 3) + 2];
//        std::cout << "cofg vertex[0] " << Levels[currentLevel].Level["VERT_cofg"][(i * 3)] << std::endl;
//        std::cout << "cofg vertex[1] " << Levels[currentLevel].Level["VERT_cofg"][(i * 3)+1] << std::endl;
//        std::cout << "cofg vertex[2] " << Levels[currentLevel].Level["VERT_cofg"][(i * 3)+2] << std::endl;
        Ship_p->centerOfGravity.push_back(centerOfG);
        Ship_p->gravityStrength.push_back(gstrength);
    }
}

void Game::ProcessInput(GLFWwindow *window, GLfloat deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Ship_p->ProcessKeyboard(CLOCKWISE, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Ship_p->ProcessKeyboard(COUNTERCLOCKWISE, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Ship_p->ProcessKeyboard(BURN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        Ship_p->ProcessKeyboard(NOBURN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        Ship_p->ProcessKeyboard(PHOTON, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
        Ship_p->ProcessKeyboard(NOPHOTON, deltaTime);
}
