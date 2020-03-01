#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include "stb_image.h"
#include "Matrix.hpp"

enum BlockType {
    none, grass
};

struct Block {
    BlockType type;
    char visible=0;
};

template<int width, int height, int length>
class Chunk {
    public:
        Chunk(int x, int y, int z) {
            chunkX=x;
            chunkY=y;
            chunkZ=z;
            for (int x=0; x<width; x++) {
                for (int y=0; y<height; y++) {
                    for (int z=0; z<length; z++) {
                        // visiblity corresponds to right, left, front, back, top, bottom
                        data[x][y][z].type=grass;
                        data[x][y][z].visible+=(x==width-1);
                        data[x][y][z].visible=data[x][y][z].visible<<1;
                        data[x][y][z].visible+=(x==0);
                        data[x][y][z].visible=data[x][y][z].visible<<1;
                        data[x][y][z].visible+=(z==length-1);
                        data[x][y][z].visible=data[x][y][z].visible<<1;
                        data[x][y][z].visible+=(z==0);
                        data[x][y][z].visible=data[x][y][z].visible<<1;
                        data[x][y][z].visible+=(y==height-1);
                        data[x][y][z].visible=data[x][y][z].visible<<1;
                        data[x][y][z].visible+=(y==0);
                    }
                }
            }
        }
        int getX() { return chunkX; }
        int getY() { return chunkY; }
        int getZ() { return chunkZ; }
        Block& getBlock(int x, int y, int z) {
            return data[x][y][z];
        }
        void updateVisibility(int x, int y, int z) {
            if (x<width-1)  getBlock(x+1,y,z).visible^=1<<5;
            if (0<x)        getBlock(x-1,y,z).visible^=1<<4;
            if (z<length-1) getBlock(x,y,z+1).visible^=1<<3;
            if (0<z)        getBlock(x,y,z-1).visible^=1<<2;
            if (y<height-1) getBlock(x,y+1,z).visible^=1<<1;
            if (0<y)        getBlock(x,y-1,z).visible^=1<<0;
        }
    private:
        int chunkX,chunkY,chunkZ;
        Block data[width][height][length];
};

class ChunkMesh {
    public:
        template<int w, int h, int l>
        ChunkMesh(Chunk<w, h, l> chunk) {
            vertex_count = 0;
            int texcoord = 0;
            for (int x=0; x<w; x++) {
                for (int y=0; y<h; y++) {
                    for (int z=0; z<l; z++) {
                        Block block = chunk.getBlock(x,y,z);
                        if (block.type == grass) {
                            char visible=block.visible;
                            int face=0;
                            while (visible>0) {
                                if (visible & 0b1) {
                                    for (int i=6*face; i<(6*face+6); i++) {
                                        int v = indices[i];
                                        vertices.push_back(0.5*cube[3*v+0]+x+chunk.getX());
                                        vertices.push_back(0.5*cube[3*v+1]+y+chunk.getY());
                                        vertices.push_back(0.5*cube[3*v+2]+z+chunk.getZ());
                                        
                                        vertices.push_back(texcoords[(texcoord++)%12]);
                                        vertices.push_back(texcoords[(texcoord++)%12]);
                                        vertex_count += 5;
                                    }
                                }
                                visible=visible>>1;
                                face++;
                            }
                        }
                    }
                }
            }
        }
        void setUp() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
        void setTexture(const char *path) {
            stbi_set_flip_vertically_on_load(true);
            int width, height, nChannels;
            unsigned char *data = stbi_load(path, &width, &height, &nChannels, STBI_rgb);
            
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            stbi_image_free(data);
        }
        void render() {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        }
    private:
        int vertex_count;
        unsigned int VAO, VBO, texture;
        std::vector<float> vertices;
        float cube[8*3] = {
            -1.0f, -1.0f, -1.0f, // bottom back left
             1.0f, -1.0f, -1.0f, // bottom back right
            -1.0f, -1.0f,  1.0f, // bottom front left
             1.0f, -1.0f,  1.0f, // bottom front right
            -1.0f,  1.0f, -1.0f, // top back left
             1.0f,  1.0f, -1.0f, // top back right
            -1.0f,  1.0f,  1.0f, // top front left
             1.0f,  1.0f,  1.0f  // top front right
        };
        float texcoords[6*2] = {
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };
        int indices[6*6] = {
            0, 1, 3, 0, 2, 3, // bottom
            4, 5, 7, 4, 6, 7, // top
            0, 1, 5, 0, 4, 5, // back
            2, 3, 7, 2, 6, 7, // front
            2, 0, 4, 2, 6, 4, // left
            3, 1, 5, 3, 7, 5  // right
        };
};

template<int width, int height, int length>
class ChunkManager {
    public:
        ChunkManager(int count, int chunksXAxis, int worldX, int worldY, int worldZ, const char* path) {
            for (int i=0; i<count; i++) {
                Chunk<width,height,length> chunk(worldX+(i%chunksXAxis)*width, worldY, worldZ-floor(i/chunksXAxis)*length);
                chunks.push_back(chunk);
                meshes.push_back(ChunkMesh(chunk));
                meshes[i].setUp();
                meshes[i].setTexture(path);
            }
        }
        void render() {
            for (auto mesh=meshes.begin(); mesh<meshes.end(); mesh++) {
                (*mesh).render();
            }
        };
    private:
        std::vector<Chunk<width, height, length> > chunks;
        std::vector<ChunkMesh> meshes;
};
