#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "lights.h"
#define STB_IMAGE_IMPLEMENTATION



class Cube {
public:
    Cube(glm::vec3 position, const char* diffuse, const char* specular) {
        //shader initilization

        cubePosition = position;

        InitShader();

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        diffuseMapC = loadTexture(diffuse);
        specularMapC = loadTexture(specular);
	}

    Cube(float x, float y, float z, const char* diffuse, const char* specular) {
        //shader initilization
        //
        cubePosition.x = x;
        cubePosition.y = y;
        cubePosition.z = z;

        InitShader();

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        diffuseMapC = loadTexture(diffuse);
        specularMapC = loadTexture(specular);
    }

    void Draw() {

        UpdateShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapC);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMapC);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void setPos(glm::vec3 position) {
        cubePosition = position;
    }
    void setPos(float x, float y, float z) {
        cubePosition.x = x;
        cubePosition.y = y;
        cubePosition.z = z;
    }

    glm::vec3 getPos() { return cubePosition; }
 
    float getPosX() { return cubePosition.x; }
    float getPosY() { return cubePosition.y; }
    float getPosZ() { return cubePosition.z; }

private:

    Shader SHADER;

    glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);

    unsigned int VBO, cubeVAO;
    unsigned int diffuseMapC, specularMapC;

    void InitShader() {
        SHADER = Shader("shaders/cube.glsl", "shaders/light.glsl");
        SHADER.use();
        SHADER.setInt("material.diffuse", 0);
        SHADER.setInt("material.specular", 1);
    }
    
    void UpdateShader() {
         // directional light
        SHADER.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        SHADER.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        SHADER.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        SHADER.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        SHADER.setVec3("pointLights[0].position", pointLightPositions[0]);
        SHADER.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        SHADER.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        SHADER.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        SHADER.setFloat("pointLights[0].constant", 1.0f);
        SHADER.setFloat("pointLights[0].linear", 0.09f);
        SHADER.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        SHADER.setVec3("pointLights[1].position", pointLightPositions[1]);
        SHADER.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        SHADER.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        SHADER.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        SHADER.setFloat("pointLights[1].constant", 1.0f);
        SHADER.setFloat("pointLights[1].linear", 0.09f);
        SHADER.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        SHADER.setVec3("pointLights[2].position", pointLightPositions[2]);
        SHADER.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        SHADER.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        SHADER.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        SHADER.setFloat("pointLights[2].constant", 1.0f);
        SHADER.setFloat("pointLights[2].linear", 0.09f);
        SHADER.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4

        // spotLight
        SHADER.setVec3("spotLight.position", camera.Position);
        SHADER.setVec3("spotLight.direction", camera.Front);
        SHADER.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        SHADER.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        SHADER.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        SHADER.setFloat("spotLight.constant", 1.0f);
        SHADER.setFloat("spotLight.linear", 0.09f);
        SHADER.setFloat("spotLight.quadratic", 0.032f);
        SHADER.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        SHADER.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        SHADER.setMat4("projection", projection);
        SHADER.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePosition);
        SHADER.setMat4("model", model);

    }

    unsigned int loadTexture(char const* path) {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }
        return textureID;
    }
   
    float vertices[288] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
};
