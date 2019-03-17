//
//  game_level.cpp
//  scratch
//
//  Created by Dirk Reese on 3/11/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#include "game_level.h"


#include <fstream>
#include <sstream>
#include <iostream>


// Instantiate static variables
//
//std::map<std::string, std::vector<GLfloat>>  GameLevel::Level;
//std::vector<std::string> GameLevel::vboName;

GameLevel::GameLevel(const GLchar *file): File(file) {}

void GameLevel::Load()
{
    GLfloat subPoint;
    GLboolean vboTagFound = false;
    std::string line;
    std::string vboNameTest = {"VERT"};
//    static std::vector<std::string> vboName;
    std::string previousVboName;
    std::ifstream fstream(File);
    
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file
        {
            if (std::strncmp (line.substr(0,2).c_str(),"//",2) != 0) {
                //            std::cout << line.substr(0,2) << std::endl;
                //            if (fstream.eof()) std::cout << "Got to EOF\n";
                if (std::strncmp (line.c_str(),vboNameTest.c_str(),4) == 0)
                {
                    if (vboTagFound == true) {
                        vboName.push_back(previousVboName);
                        Level[previousVboName] = verts;
                        verts.clear();
                    }
                    previousVboName = line;
                    
                    vboTagFound = true;
                }
                
                
                std::istringstream sstream(line);
                while (sstream >> subPoint) {// Read each word seperated by spaces
                    verts.push_back(subPoint);
                }
                
            }
        }
        
        if (fstream.eof()) {
            vboName.push_back(previousVboName);
            Level[previousVboName] = verts;
//            std::cout << "Got to EOF vboName[0] = " << vboName[0] << std::endl;
        }
        
    }
    


    for (int i=0; i < vboName.size(); i++) {
        if (std::strncmp (vboName[i].c_str(), "VERT_surface",10) == 0)
        {
            Boundary obj;
            obj.Load(Level[vboName[i]]);
            VAOS.push_back(obj.VAO);
            VBOS.push_back(obj.VBO);
//            std::cout << "game_level: VAO index : " << i << std::endl;
        }
        if (std::strncmp (vboName[i].c_str(), "VERT_gate",9) == 0)
        {
            Boundary gate;
            gate.Load(Level[vboName[i]]);
            gateVAO = gate.VAO;
            gateVBO = gate.VBO;
        }
        

    }
}

