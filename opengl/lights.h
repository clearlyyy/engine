#pragma once

//GLOBAL VARIABLES
// contains the whole view matrix, the camera, and positions of lights in the scene
// so every object in the scene can access them.
//-----------------------------------------------------------------------------------------------------------------------------
// instead of creating a bullshit massive nested class system, these are global, because its easier, and cleaner in my opinion,
// since every object in the scene needs access to these things.

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::mat4 projection;
glm::mat4 view;

glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
};
