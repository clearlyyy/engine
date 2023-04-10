#pragma once
#include <iostream>
#include "cube.h"



class Chunk {
public: 
	Chunk(glm::vec3 chunkPos) {
        ChunkPos = chunkPos;
        cu = Cube("textures/dirt.jpg", "textures/dirt.jpg");
        
        //initChunk();
        UpdateChunk();

        genChunkWorld();

        genMesh();
	}

    glm::vec3 getChunkSize() {
        return chunkSize;
    }

    void addBlock(int x, int y, int z, int type) {
        world[x][y][z] = type;
    }

    //world generation
    void genChunkWorld() {

        for (int x = 0; x < chunkSizeX; x++)
            for (int y = 0; y < chunkSizeY; y++)
                for (int z = 0; z < chunkSizeZ; z++)
                    world[x][y][z] = 0;

        for (int x = 0; x < chunkSizeX; x++)
            for (int y = 0; y < chunkSizeY-40; y++)
                for (int z = 0; z < chunkSizeZ; z++)
                    world[x][y][z] = 1;

        //world[1][90][1] = 1;

        for (int x = 0; x < chunkSizeX; x++)
            for (int y = 0; y < chunkSizeY; y++)
                for (int z = 0; z < chunkSizeZ; z++)
                    rWorld[x][y][z] = 0;

    }

    //chunk mesh generation!
    void genMesh() {

        
        for (int x = 0; x < chunkSizeX; x++) {
            for (int y = 0; y < chunkSizeY; y++) {
                for (int z = 0; z < chunkSizeZ; z++) {
                    if (world[x][y][z] == 1)
                        if (x == 0)
                            rWorld[x][y][z] = 1;
                        else if (y == 0)
                            rWorld[x][y][z] = 1;
                        else if (z == 0)
                            rWorld[x][y][z] = 1;
                        else if (z == chunkSizeZ - 1)
                            rWorld[x][y][z] = 1;
                        else if (x == chunkSizeX - 1)
                            rWorld[x][y][z] = 1;
                        else if (y == chunkSizeY - 1)
                            rWorld[x][y][z] = 1;
                        else
                            if (world[x + 1][y][z] == 1 && world[x - 1][y][z] == 1
                                && world[x][y + 1][z] == 1 && world[x][y - 1][z] == 1
                                && world[x][y][z + 1] == 1 && world[x][y][z - 1] == 1) {
                                rWorld[x][y][z] = 0;
                            }
                            else
                                rWorld[x][y][z] = 1;
                }
            }
        }


        for (float x = 0; x < chunkSize.x; x++) {
            for (float y = 0; y < chunkSize.y; y++) {
                for (float z = 0; z < chunkSize.z; z++) {
                    if (rWorld[(int)x][(int)y][(int)z] == 1) {
                        std::vector<float> verts = cu.getCube(x, y, z);
                        chunkVertices.insert(chunkVertices.end(),
                            verts.begin(), verts.end()
                        );
                    }
                }
            }
        }
    }

    void DrawChunk(glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f)) {

        UpdateShader(false, cubePos);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapC);
        // bind specular map
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, specularMapC);
        glBindVertexArray(chunkVAO);
        glDrawArrays(GL_TRIANGLES, 0, (chunkVertices.size() / 8));

    }

    void initChunk() {
        for (int i = 0; i < chunkSizeX; i++) {
            for (int j = 0; j < chunkSizeY; j++) {
                for (int x = 0; x < chunkSizeZ; x++) {
                    world[i][j][x] = 1;
                }
            }
        }
        //set fake world to real world;
        for (int i = 0; i < chunkSizeX; i++) {
            for (int j = 0; j < chunkSizeY; j++) {
                for (int x = 0; x < chunkSizeZ; x++) {
                    rWorld[i][j][x] = world[i][j][x];
                }
            }
        }
    }

    void initChunk2() {

        glDeleteVertexArrays(1, &chunkVAO);
        glDeleteBuffers(1, &VBO);

        InitShader();
        std::cout << "Started Vertex Buffers" << std::endl;

        glGenVertexArrays(1, &chunkVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, chunkVertices.size() * sizeof(float), &chunkVertices.front(), GL_STATIC_DRAW);

        glBindVertexArray(chunkVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        std::cout << "Finished Vertex Buffers" << std::endl;

        diffuseMapC = loadTexture(cu.getDiffuse());
        specularMapC = loadTexture(cu.getSpecular());
    }

	void UpdateChunk() {

        for (int x = 0; x < chunkSizeX; x++) {
            for (int y = 0; y < chunkSizeY; y++) {
                for (int z = 0; z < chunkSizeZ; z++) {
                    if (world[x][y][z] == 1)
                        if (x == 0)
                            rWorld[x][y][z] = 1;
                        else if (y == 0)
                            rWorld[x][y][z] = 1;
                        else if (z == 0)
                            rWorld[x][y][z] = 1;
                        else if (z == chunkSizeZ - 1)
                            rWorld[x][y][z] = 1;
                        else if (x == chunkSizeX - 1)
                            rWorld[x][y][z] = 1;
                        else if (y == chunkSizeY - 1)
                            rWorld[x][y][z] = 1;
                        else
                            if (world[x + 1][y][z] == 1 && world[x - 1][y][z] == 1
                                && world[x][y + 1][z] == 1 && world[x][y - 1][z] == 1
                                && world[x][y][z + 1] == 1 && world[x][y][z - 1] == 1) {
                                rWorld[x][y][z] = 0;
                            }
                            else
                                rWorld[x][y][z] = 1;
                }
            }
        }
	}

    void setSize(glm::vec3 chunksize) {
        chunkSize = chunksize;
    }

    void updateSize() {
        std::cout << chunkSize.x << std::endl;
        chunkVertices.clear();
        //genChunkWorld();

        genMesh();
        initChunk2();
    }


private:

    Shader SHADER;

    static const int chunkSizeX = 32;
    static const int chunkSizeY = 128;
    static const int chunkSizeZ = 32;

    glm::vec3 chunkSize = glm::vec3(32.0f, 128.0f, 32.0f);

    unsigned int diffuseMapC, specularMapC;
    unsigned int chunkVAO, VBO;

    uint8_t world[chunkSizeX][chunkSizeY][chunkSizeZ] = {};
    uint8_t rWorld[chunkSizeX][chunkSizeY][chunkSizeZ] = {};

    std::vector<float> chunkVertices = {
        
    };

    Cube cu;
    glm::vec3 ChunkPos = glm::vec3(0.0f, 0.0f, 0.0f);

    void InitShader() {
        SHADER = Shader("shaders/cube.glsl", "shaders/light.glsl");
        SHADER.use();
        //SHADER.setInt("material.diffuse", 0);
        //SHADER.setInt("material.specular", 1);


    }

    void UpdateShader(bool updateUniforms, glm::vec3 ChunkPos) {

        //SHADER.setVec3("dirLight.ambient", 0.9f, 0.9f, 0.9f);
        SHADER.use();

        SHADER.setMat4("projection", projection);
        SHADER.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::translate(model, ChunkPos);
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


};
