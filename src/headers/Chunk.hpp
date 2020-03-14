#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include "stb_image.h"
#include "Perlin.hpp"

enum BlockType {
    none, grass
};

struct Block {
    BlockType type=none;
    char visible=0b111111;
};

template<int width, int height, int length>
class Chunk {
    public:
        Chunk(int x, int y, int z, float height_map[width][length]) {
            chunkX=x;
            chunkY=y;
            chunkZ=z;
            for (int x=0; x<width; x++) {
                for (int z=0; z<length; z++) {
                    int max_y = (int)((height_map[x][z]/2.0) * length+4);
                    for (int y=0; y<max_y; y++) {
                        // visiblity corresponds to right, left, front, back, top, bottom
                        getBlock(x,y,z).type=grass;
                    }
                }
            }
            for (int x=0; x<width; x++) {
                for (int y=0; y<height; y++) {
                    for (int z=0; z<length; z++) {
                        if (getBlock(x,y,z).type==grass) {
                            updateBlockVisibility(x,y,z);
                        }
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
        void updateBlockVisibility(int x, int y, int z) {
            Block& this_block = getBlock(x,y,z);
            
            Block block;
            if (x<width-1) {
                block = getBlock(x+1,y,z);
                if (block.type==none) this_block.visible |= (1<<5);
                else this_block.visible &= ~(1<<5); 
            }
            if (0<x) {
                block = getBlock(x-1,y,z);
                if (block.type==none) this_block.visible |= (1<<4);
                else this_block.visible &= ~(1<<4);
            }
            if (z<length-1) {
                block = getBlock(x,y,z+1);
                if (block.type==none) this_block.visible |= (1<<3);
                else this_block.visible &= ~(1<<3);
            }
            if (0<z) {
                block = getBlock(x,y,z-1);
                if (block.type==none) this_block.visible |= (1<<2);
                else this_block.visible &= ~(1<<2);
            }
            if (y<height-1) {
                block = getBlock(x,y+1,z);
                if (block.type==none) this_block.visible |= (1<<1);
                else this_block.visible &= ~(1<<1);
            }
            if (0<y) {
                block = getBlock(x,y-1,z);
                if (block.type==none) this_block.visible |= (1<<0);
                else this_block.visible &= ~(1<<0);
            }
        }
    private:
        int chunkX,chunkY,chunkZ;
        Block data[width][height][length];
};

class ChunkMesh {
    public:
        template<int w, int h, int l>
        ChunkMesh(Chunk<w, h, l> chunk) {
            xPos=chunk.getX();
            zPos=chunk.getZ();
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
                                        vertices.push_back(face==1);
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
        int getX() { return xPos; }
        int getZ() { return zPos; }
        void setUp() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
        void setTexture(const char *path, unsigned int &texture) {
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
        void setTextures(const char* path1, const char* path2) {
            setTexture(path1, texture1);
            setTexture(path2, texture2);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
        }
        void render() {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        }
    private:
        int xPos, zPos;
        int vertex_count;
        unsigned int VAO, VBO, texture1, texture2;
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
        ChunkManager(const char* path1, const char* path2) {
            text_path1 = path1;
            text_path2 = path2;

            generateChunk(0, 0);
        }
        void generateHeightMap(float (&height_map)[width][length], float g[2][2][2]) {
            prl::node_dist=width;
            for (int x=0; x<width; x++) {
                for (int z=0; z<length; z++) {
                    height_map[x][z] = abs(prl::perlin(g, x, z));
                }
            }
        }
        void generateChunk(int x, int z) {
            for (auto coords=chunk_coords.begin(); coords<chunk_coords.end(); coords++) {
                if ((*coords)[0] <= x && x <= (*coords)[1]) {
                    if ((*coords)[2] <= z && z <= (*coords)[3]) {
                        return;
                    }
                }
            }

            int ix, iz, fx, fz;
            if (x < 0) {
                ix = -1*(abs(x/width)+1)*width;
                fx = ix + width;
            } else {
                ix = (x/width)*width;
                fx = ix + width;
            }
            if (z < 0) {
                iz = -1*(abs(z/length)+1)*length;
                fz = iz + length;
            } else {
                iz = (z/length)*length;
                fz = iz + length;
            }

            float height_map[width][length];

            float g[2][2][2]={};

            float randvecs[4][2] = {
                           {-1, -1},
                           {-1,  1},
                           { 1, -1},
                           { 1,  1}};
            for (int i=0; i<2; i++) {
                for (int j=0; j<2; j++) {
                    int v = (int)(rand() % 4);
                    g[i][j][0]=randvecs[v][0];
                    g[i][j][1]=randvecs[v][1];
                }
            }

            generateHeightMap(height_map, g);

            Chunk<width,height,length> chunk(ix, 0, iz, height_map);
            
            chunks.push_back(chunk);
            meshes.push_back(ChunkMesh(chunk));
            
            meshes.back().setUp();
            meshes.back().setTextures(text_path1, text_path2);
            
            chunk_coords.push_back({ix, fx, iz, fz}); 
        }
        void render(int x, int z, int render_dist) {
            for (auto mesh=meshes.begin(); mesh<meshes.end(); mesh++) {
                if ( (x-render_dist*width) < (*mesh).getX()  &&
                     (*mesh).getX() < (x+render_dist*width)  &&
                     (z-render_dist*length) < (*mesh).getZ() &&
                     (*mesh).getZ() < (z+render_dist*length) ) {
                    (*mesh).render();
                }
            }
        };
    private:
        const char *text_path1, *text_path2;
        std::vector<std::array<int, 4> > chunk_coords;
        std::vector<Chunk<width, height, length> > chunks;
        std::vector<ChunkMesh> meshes;
};
