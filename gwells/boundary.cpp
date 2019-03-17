//
//  boundary.cpp
//  scratch
//
//  Created by Dirk Reese on 3/11/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#include "boundary.h"
#include <iostream>

Boundary::Boundary() {}

void Boundary::Load(std::vector<GLfloat> Verts)
{
    GLfloat vertices[Verts.size()];
    for (int i=0; i < Verts.size(); i++) {
        vertices[i] = Verts[i];
//        std::cout << vertices[i] << std::endl;
    }

    //  SkyIsland
    //---------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // set and enable vertices pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}

void Boundary::DeleteVBO(GLuint vao, GLuint vbo)
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

//GLuint Boundary::getVAO()
//{
//    return VAO;
//}
