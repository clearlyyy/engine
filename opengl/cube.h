#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "lights.h"

#include <vector>

#define STB_IMAGE_IMPLEMENTATION

//cube class

class Cube {
public:

    Cube() {}

    Cube(glm::vec3 position, const char* diffuse, const char* specular) {
        //shader initilization

        cubePosition = position;
        std::cout << vertices.capacity() << std::endl;
        std::cout << vertices.max_size() << std::endl;

       float curVert1  = -0.6f;
       float curVert2 = -1.6f;
       
       std::cout << "Starting Chunk Mesh Generation" << std::endl;

       for (float i = 0; i > -333; i--) {
           for (float j = 0; j > -333; j--) {
               vertices.insert(vertices.end(),
                   {
                       i-1.0f, j  -1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                       i,      j  -1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                       i,      j       , -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                       i,      j       , -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                       i-1.0f, j       , -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                       i-1.0f, j  -1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                   });
               //curVert1 -= 1.1f;
               //curVert2 -= 1.1f;
           }
       }
       std::cout << "Finished Chunk Mesh Generation" << std::endl;

       //for (int i = 0; i < vertices.size(); i++) {
       //    std::cout << vertices[i] << " ";
       //    if (i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56 || i == 64)
       //        std::cout << std::endl;
       //    
       //}

        InitShader();

        std::cout << "Starting Vertex Buffers" << std::endl;

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        std::cout << "Finished Vertex Buffers" << std::endl;

        diffuseMapC = loadTexture(diffuse);
        specularMapC = loadTexture(specular);
	}

    Cube(float x, float y, float z, const char* diffuse, const char* specular) {
        //shader initilization
        //
        cubePosition.x = x;
        cubePosition.y = y;
        cubePosition.z = z;
        //
        //float curVert = -1.5f;
        //float curVert2 = -1.0f;
        //
        //
        //    vertices.insert(vertices.end(),
        //        {
        //            curVert2, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        //            curVert,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        //            curVert,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        //            curVert,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        //            curVert2,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        //            curVert2, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        //        });
        

        InitShader();

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

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

    void Draw(bool updateUniforms = true) {

        UpdateShader(updateUniforms);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapC);
        // bind specular map
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, specularMapC);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, (vertices.size()/8));
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
    bool flag = false;
    unsigned int VBO, cubeVAO;
    unsigned int diffuseMapC, specularMapC;

    void InitShader() {
        SHADER = Shader("shaders/cube.glsl", "shaders/light.glsl");
        SHADER.use();
        //SHADER.setInt("material.diffuse", 0);
        //SHADER.setInt("material.specular", 1);


    }
    
    void UpdateShader(bool updateUniforms) {
      
            //SHADER.setVec3("dirLight.ambient", 0.9f, 0.9f, 0.9f);
        SHADER.use();
            
        //if (updateUniforms) {
        //    std::cout << "Updating Uniforms" << std::endl;
        //    SHADER.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        //    SHADER.setVec3("dirLight.diffuse", 0.9f, 0.9f, 0.9f);
        //    SHADER.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //    // directional light
        //    // point light 1
        //    SHADER.setVec3("pointLights[0].position", pointLightPositions[0]);
        //    SHADER.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        //    SHADER.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        //    SHADER.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        //    SHADER.setFloat("pointLights[0].constant", 1.0f);
        //    SHADER.setFloat("pointLights[0].linear", 0.09f);
        //    SHADER.setFloat("pointLights[0].quadratic", 0.032f);
        //    // point light 2
        //    SHADER.setVec3("pointLights[1].position", pointLightPositions[1]);
        //    SHADER.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        //    SHADER.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        //    SHADER.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        //    SHADER.setFloat("pointLights[1].constant", 1.0f);
        //    SHADER.setFloat("pointLights[1].linear", 0.09f);
        //    SHADER.setFloat("pointLights[1].quadratic", 0.032f);
        //    // point light 3
        //    SHADER.setVec3("pointLights[2].position", pointLightPositions[2]);
        //    SHADER.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        //    SHADER.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        //    SHADER.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        //    SHADER.setFloat("pointLights[2].constant", 1.0f);
        //    SHADER.setFloat("pointLights[2].linear", 0.09f);
        //    SHADER.setFloat("pointLights[2].quadratic", 0.032f);
        //    // point light 4
        //
        //    // spotLight
        //    SHADER.setVec3("spotLight.position", camera.Position);
        //    SHADER.setVec3("spotLight.direction", camera.Front);
        //    SHADER.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        //    SHADER.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        //    SHADER.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        //    SHADER.setFloat("spotLight.constant", 1.0f);
        //    SHADER.setFloat("spotLight.linear", 0.09f);
        //    SHADER.setFloat("spotLight.quadratic", 0.032f);
        //    SHADER.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        //    SHADER.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        //}
        
        SHADER.setMat4("projection", projection);
        SHADER.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
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
   
    std::vector<float> vertices = {

    };
    float f[288] = {
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

    float frontVertices[48] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    };
    float backVertices[288] = {
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
