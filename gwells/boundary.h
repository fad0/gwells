//
//  boundary.hpp
//  scratch
//
//  Created by Dirk Reese on 3/11/19.
//  Copyright © 2019 Dirk Reese. All rights reserved.
//

#ifndef boundary_h
#define boundary_h

#include <stdio.h>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Boundary
{
public:
    
//    std::string BoundaryName;
//    std::vector<GLfloat> Verts;
    GLuint VBO;
    GLuint VAO;
    // Constructor
    Boundary();
    // Loads level from file
    void Load(std::vector<GLfloat> Verts);
static  void DeleteVBO(GLuint vao, GLuint vbo);
};

#endif /* boundary_h */
