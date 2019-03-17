//
//  main.cpp
//  scratch
//
//  Created by Dirk Reese on 2/21/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#include <GL/glew.h>
#include <glfw3.h>

#include <iostream>
//// Header file
//#include <glm/fwd.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

//shader class and compiler
//#include <shader.h>
//
//#include "spaceship.h"
//#include "globals.h"
//#include "constants.h"
//#include "vbos.h"
//#include "spaceship.h"
//#include "surface.h"
#include "game.h"

// ******* BEGIN added for displaying text ****** //
// FreeType
//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include <map>
//#include <string>
//
//#include "text.h"

// START OF MAIN

const GLuint WIDTH = 800, HEIGHT = 800;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window, Spaceship * ship);

//#define GLFW_INCLUDE_GLCOREARB

//******************************//
Game gWell(WIDTH, HEIGHT);

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    // ***** added for text ***** //
    //    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval( 1 );
    
    // Initialize GLEW
    glewExperimental = GL_TRUE; glewInit();

//    glLoadIdentity();
  
//    // start the sound engine with default parameters
//    ISoundEngine* engine = createIrrKlangDevice();
//
//    if (!engine)
//    {
//        printf("Could not startup engine\n");
//        return 0; // error starting up the engine
//    }

    // Set OpenGL options
    // These options are needed to render text with freetype
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
//    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
//    tshader.use();
//    glUniformMatrix4fv(glGetUniformLocation(tshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//    
//    Text text;
//    text.init();
//   
//    GLboolean gameOver = false;
//    GLuint gameOverCounter = 0;


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
//    
//
    //******************************//
    gWell.Init();
    
    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        gWell.ProcessInput(window, deltaTime);
        
        // get loop time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        
        // keep track of extra spaceships
        //--------------------------
//        if (ship.explodeShip == true) {
//            if (ship.shipsLeft == 0) {
//                gameOver = true;
//            } else ship.latchExplodeShip = true;
//        }
//
//        // wait until ship finishes exploding before instantiating new one
//        if (ship.explodeShip == false and ship.latchExplodeShip == true) {
//            ship.shipsLeft--;
//            ship.init(0.5f, 0.5f, 0);
//            ship.latchExplodeShip = false;
//        }
        
        
        // draw the spaceship, gunners, surfaces, and gate
        //--------------------------
        gWell.Update(deltaTime);
        gWell.UpdateGate(deltaTime);
        
        // Check status of Spaceship
        //---------------------------
        gWell.CheckStatus();
        
        
//        // Check the score
//        //--------------------------
//        std::string scorestring = std::to_string(score);
//        std::string scoreboard = "Score: " + scorestring;
//        text.RenderText(tshader, vbos_p, scoreboard, 25.0f, 10.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
//        std::string slstring = std::to_string(ship_pntr->shipsLeft);
//        std::string slboard = "Ships: " + slstring;
//        text.RenderText(tshader, vbos_p, slboard, 200.0f, 10.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
//
//        glm::vec3 loseColor;
//        if (gameOver == true) {
//            if (gameOverCounter++ % 100  < 50) {
//                loseColor = glm::vec3(1.0, 0.0f, 0.2f);
//            } else {
//                loseColor = glm::vec3(0.1, 0.1f, 0.1f);
//            }
//            text.RenderCenteredText(tshader, vbos_p, "Game Over", (float)WIDTH/2.0f, (float)HEIGHT/2.0f, 1.5f + 0.5f * cos((float)glfwGetTime() * glm::radians(60.0f)), loseColor);
//        }
//
//        if (win == true) {
//            text.RenderCenteredText(tshader, vbos_p, "YOU WIN!!!", (float)WIDTH/2.0f, (float)HEIGHT/2.0f, 2.0f, glm::vec3(0.5, 0.8f, 0.2f));
//        }
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    //******************************//
//    gWell.DeAllocate();
    
    
//    vbos.delete_vbos();
//    
//    engine->drop(); // delete engine
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
