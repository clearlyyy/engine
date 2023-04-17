#pragma once
#include <iostream>
#include "cube.h"
#include "PerlinNoise.hpp"

#include <noise/noise.h>



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
    void genChunkWorld(int wseed = 1, float frequency = 0.03) {

        for (int x = 0; x < chunkSizeX; x++)
            for (int y = 0; y < chunkSizeY; y++)
                for (int z = 0; z < chunkSizeZ; z++)
                    world[x][y][z] = 1;

        const siv::PerlinNoise::seed_type seed = 12345;

        const siv::PerlinNoise perlin{ seed };

        noise::module::Perlin perlinNoise;


        //for (int x = 0; x < chunkSizeX; x++)
        //    for (int y = 0; y < chunkSizeY-40; y++)
        //        for (int z = 0; z < chunkSizeZ; z++) {
        //            world[x][y][z] = 1;
        //            //std::cout << world[x][y][z] << '\t';
        //        }
        perlinNoise.SetOctaveCount(6);
        perlinNoise.SetFrequency(frequency);
        perlinNoise.SetSeed(wseed);

        //stone
        for (int x = 0; x < chunkSizeX; x++) {
            for (int y = 0; y < 80; y++) {
                for (int z = 0; z < chunkSizeZ; z++) {
                    if (perlinNoise.GetValue(x * .9f, y * .9f, z * .9f) >= -.1f)
                        world[x][y][z] = 2;
                }
            }
        }

        //dirt
        for (int x = 0; x < chunkSizeX; x++) {
            for (int y = 80; y < chunkSizeY - 40; y++) {
                for (int z = 0; z < chunkSizeZ; z++) {
                    if (perlinNoise.GetValue(x * .9f, y * .9f, z * .9f) >= -.1f)
                        world[x][y][z] = 3;
                }
            }
        }

        //bedrock
        for (int x = 0; x < chunkSizeX; x++) {
            for (int y = 0; y < 1; y++) {
                for (int z = 0; z < chunkSizeZ; z++) {
                    world[x][y][z] = 4;
                }
            }
        }

        //for (int x = 0; x < chunkSizeX; x++)
        //    for (int y = 80; y < chunkSizeY - 40; y++)
        //        for (int z = 0; z < chunkSizeZ; z++) {
        //            //int rNoise;
        //            //const double noise = perlin.octave2D((x * 0.01), (y * 0.01), 4);
        //            //rNoise = (int)(noise * 70);
        //            //std::cout << rNoise << std::endl;
        //            //if (rNoise < 48 && rNoise > 1)
        //                world[x][y][z] = 1;
        //            //std::cout << world[x][y][z] << '\t';
        //        }



        //world[1][90][1] = 1;
        world[1][90][1] = 2;

        for (int x = 0; x < chunkSizeX; x++)
            for (int y = 0; y < chunkSizeY; y++)
                for (int z = 0; z < chunkSizeZ; z++)
                    rWorld[x][y][z] = world[x][y][z];

    }

    //chunk mesh generation!
    void genMesh() {

        chunkVertices.clear();
        stoneVertices.clear();

        //Textures


        for (int x = 0; x < chunkSizeX; x++)
            for (int y = 0; y < chunkSizeY; y++)
                for (int z = 0; z < chunkSizeZ; z++)
                    rWorld[x][y][z] = world[x][y][z];

        //for (int x = 0; x < chunkSizeX; x++) {
        //    for (int y = 0; y < chunkSizeY; y++) {
        //        for (int z = 0; z < chunkSizeZ; z++) {
        //            if (world[x][y][z] != 0)
        //                if (x == 0)
        //                    rWorld[x][y][z] = world[x][y][z];
        //                else if (y == 0)
        //                    rWorld[x][y][z] = world[x][y][z];
        //                else if (z == 0)
        //                    rWorld[x][y][z] = world[x][y][z];
        //                else if (z == chunkSizeZ - 1)
        //                    rWorld[x][y][z] = world[x][y][z];
        //                else if (x == chunkSizeX - 1)
        //                    rWorld[x][y][z] = world[x][y][z];
        //                else if (y == chunkSizeY - 1)
        //                    rWorld[x][y][z] = world[x][y][z];
        //                else
        //                    if (world[x + 1][y][z] != 1 && world[x - 1][y][z] != 1
        //                        && world[x][y + 1][z] != 1 && world[x][y - 1][z] != 1
        //                        && world[x][y][z + 1] != 1 && world[x][y][z - 1] != 1) {
        //                        rWorld[x][y][z] = 1;
        //                    }
        //                    else
        //                        rWorld[x][y][z] = world[x][y][z];
        //        }
        //    }
        //}


        // -z = north
        // +z = south
        // -x = west
        // +x = east

        for (float x = 0; x < chunkSize.x; x++) {
            for (float y = 0; y < chunkSize.y; y++) {
                for (float z = 0; z < chunkSize.z; z++) {
                    
                    //if ((int)x == 0 && rWorld[(int)x][(int)y][(int)z] != 1)
                    //if ((int)z == 0)
                    //std::cout << world[0][0][-1] << std::endl;
                    
                    //stone
                    OptimizeChunk(x, y, z, world, 1.5f, BLOCK_STONE);
                    //dirt
                    OptimizeChunk(x, y, z, world, 0.0f, BLOCK_DIRT);
                    //bedrock
                    OptimizeChunk(x, y, z, world, 2.5f, BLOCK_BEDROCK);
        
        
                    //}
                    //if (rWorld[(int)x][(int)y][(int)z] == 3) {
                    //    std::vector<float> verts = cu.getCube(x, y, z, 0.0f);
                    //    chunkVertices.insert(chunkVertices.end(),
                    //        verts.begin(), verts.end()
                    //    );
                    //}
                    //if (rWorld[(int)x][(int)y][(int)z] == 4) {
                    //    std::vector<float> verts = cu.getCube(x, y, z, 2.5f);
                    //    chunkVertices.insert(chunkVertices.end(),
                    //        verts.begin(), verts.end()
                    //    );
                    //}
                }
        
            }
        }
    }


    void DrawChunk(glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f)) {

        UpdateShader(false, cubePos);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dirtTex);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, stoneTex);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bedrockTex);

        glBindVertexArray(chunkVAO);

        //dirt
        glDrawArrays(GL_TRIANGLES, 0, chunkVertices.size()/9);
        //glDrawArrays()
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
        //position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //normals attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        //block type attribute
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        std::cout << "Finished Vertex Buffers" << std::endl;

        dirtTex = loadTexture(cu.getDiffuse());
        stoneTex = loadTexture("textures/stone.png");
        bedrockTex = loadTexture("textures/bedrock.png");

        int samplers[3] = { 0, 1, 2 };
        SHADER.use();
        SHADER.setSampler2D("u_Textures", samplers);

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
        
        //genChunkWorld();

        genMesh();
        initChunk2();
    }


private:

    enum Blocks {
        BLOCK_AIR = 1,
        BLOCK_STONE = 2,
        BLOCK_DIRT = 3,
        BLOCK_BEDROCK = 4
    };

    Shader SHADER;

    int stoneStart;
    int totalDirtV;

    static const int chunkSizeX = 48;
    static const int chunkSizeY = 128;
    static const int chunkSizeZ = 48;

    glm::vec3 chunkSize = glm::vec3(48.0f, 128.0f, 48.0f);

    unsigned int dirtTex, stoneTex, bedrockTex;
    unsigned int chunkVAO, VBO;

    uint8_t world[chunkSizeX][chunkSizeY][chunkSizeZ] = {};
    uint8_t rWorld[chunkSizeX+5][chunkSizeY+5][chunkSizeZ+5] = {};

    std::vector<float> chunkVertices = {
        
    };
    std::vector<float> stoneVertices = {

    };

    Cube cu;
    glm::vec3 ChunkPos = glm::vec3(0.0f, 0.0f, 0.0f);

    void OptimizeChunk(float x, float y, float z, uint8_t world[chunkSizeX][chunkSizeY][chunkSizeZ], float blockTex, int blockID) {
        if ((int)x == 0 && world[(int)x][(int)y][(int)z] == blockID)
        {
            std::vector<float> verts = cu.getWest(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if ((int)z == 0 && world[(int)x][(int)y][(int)z] == blockID)
        {
            std::vector<float> verts = cu.getNorth(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if ((int)x == chunkSize.x - 1 && world[(int)x][(int)y][(int)z] == blockID)
        {
            std::vector<float> verts = cu.getEast(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if ((int)z == chunkSize.z - 1 && world[(int)x][(int)y][(int)z] == blockID)
        {
            std::vector<float> verts = cu.getSouth(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }

        if (world[(int)x][(int)y][(int)z - 1] == 1 && world[(int)x][(int)y][(int)z] == blockID) {
            std::vector<float> verts = cu.getNorth(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if (world[(int)x][(int)y][(int)z + 1] == 1 && world[(int)x][(int)y][(int)z] == blockID) {
            std::vector<float> verts = cu.getSouth(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if (world[(int)x - 1][(int)y][(int)z] == 1 && world[(int)x][(int)y][(int)z] == blockID) {
            std::vector<float> verts = cu.getWest(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if (world[(int)x + 1][(int)y][(int)z] == 1 && world[(int)x][(int)y][(int)z] == blockID) {
            std::vector<float> verts = cu.getEast(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if (world[(int)x][(int)y + 1][(int)z] == 1 && world[(int)x][(int)y][(int)z] == blockID) {
            std::vector<float> verts = cu.getTop(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
        if (world[(int)x][(int)y - 1][(int)z] == 1 && world[(int)x][(int)y][(int)z] == blockID) {
            std::vector<float> verts = cu.getBottom(x, y, z, blockTex);
            chunkVertices.insert(chunkVertices.end(),
                verts.begin(), verts.end()
            );
        }
    }

    void InitShader() {
        SHADER = Shader("shaders/cube.glsl", "shaders/light.glsl");
        SHADER.use();
        SHADER.setInt("material.dirt", 0);
        SHADER.setInt("material.stone", 1);


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
