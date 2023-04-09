#pragma once
#include <iostream>
#include "cube.h"



class Chunk {
public: 
	Chunk(glm::vec3 chunkPos) {
        cubePos = chunkPos;
        cu = Cube(cubePos, "textures/dirt.jpg", "textures/dirt.jpg");
        initChunk();
        UpdateChunk();
	}

    void DrawChunk(glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f)) {
        for (float i = 0; i < chunkSizeX; i++) {
            for (float j = 0; j < chunkSizeY; j++) {
                for (float x = 0; x < chunkSizeZ; x++) {
                    switch (rWorld[(int)i][(int)j][(int)x]) {
                    case 1:
                        cu.setPos((i / 2) + cubePos.x / 2, (x / 2) + cubePos.y / 2, (j / 2) + cubePos.z / 2);
                        cu.Draw(false);
                    }
                }
            }
        }
    }

    void initChunk() {
        for (int i = 0; i < chunkSizeX; i++) {
            for (int j = 0; j < chunkSizeY; j++) {
                for (int x = 0; x < chunkSizeZ; x++) {
                    world[i][j][x] = 1;
                }
            }
        }

        for (int i = 0; i < chunkSizeX; i++) {
            for (int j = 0; j < chunkSizeY; j++) {
                for (int x = 0; x < chunkSizeZ; x++) {
                    rWorld[i][j][x] = world[i][j][x];
                }
            }
        }
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


private:

    static const int chunkSizeX = 1;
    static const int chunkSizeY = 1;
    static const int chunkSizeZ = 1;

    uint8_t world[chunkSizeX][chunkSizeY][chunkSizeZ] = {};
    uint8_t rWorld[chunkSizeX][chunkSizeY][chunkSizeZ] = {};

    Cube cu;
    glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);


};
