//
//  SphereGeometry.h
//  Sim3D
//
//  Created by Arthur Sun on 5/11/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef SphereGeometry_h
#define SphereGeometry_h

#include <vector>
#include <iostream>
#include "setup.hpp"

struct SphereGeometry
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    
    // http://www.songho.ca/opengl/gl_sphere.html
    void createVertices(int stackCount, int sectorCount)  {
        vertices.clear();
        
        float x, y, z, xy;

        float sectorStep = 2.0f * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;
        
        for(int i = 0; i <= stackCount; ++i) {
            stackAngle = M_PI * 0.5f - i * stackStep;
            xy = cosf(stackAngle);
            z = sinf(stackAngle);
            
            for(int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;
                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);
                vertices.push_back(glm::vec3(x, y, z));
                texCoords.push_back(glm::vec2(j/(float)sectorCount, i/(float)stackCount));
            }
        }
    }
    
    void createVertices2(int stacks, int sectors) {
        vertices.clear();
        vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        
        float stackStep = M_PI/(float)stacks;
        float sectorStep = 2.0f * (M_PI/(float)sectors);
        for(int i = 0; i < stacks - 1; ++i) {
            float stackAngle = M_PI * 0.5f - (i + 1) * stackStep;
            float y = sinf(stackAngle);
            float r = cosf(stackAngle);
            for(int j = 0; j < sectors; ++j) {
                float sectorAngle = j * sectorStep;
                vertices.push_back(glm::vec3(r * cosf(sectorAngle), y, r * sinf(sectorAngle)));
            }
        }
        
        vertices.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    }
    
    void createIndices2(int stacks, int sectors) {
        indices.clear();
        
        /// top
        
        for(int i = 0; i < sectors - 1; ++i) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }
        
        indices.push_back(0);
        indices.push_back(sectors);
        indices.push_back(1);

        /// middle
        
        for(int i = 0; i < stacks - 2; ++i) {
            int start = i * sectors + 1;
            for(int j = 0; j < sectors - 1; ++j) {
                indices.push_back(start + j);
                indices.push_back(start + j + sectors);
                indices.push_back(start + j + sectors + 1);
                
                indices.push_back(start + j);
                indices.push_back(start + j + 1);
                indices.push_back(start + j + sectors + 1);
            }
            
            int j = sectors - 1;
            
            indices.push_back(start + j);
            indices.push_back(start + j + sectors);
            indices.push_back(start + j + 1);
            
            indices.push_back(start);
            indices.push_back(start + j);
            indices.push_back(start + sectors);
        }
        
        /// bottom
        
        int lastIndex = (int)(vertices.size()) - 1;
        
        for(int i = 0; i < sectors - 1; ++i) {
            indices.push_back(lastIndex);
            indices.push_back(lastIndex - sectors + i);
            indices.push_back(lastIndex - sectors + i + 1);
        }
        
        indices.push_back(lastIndex);
        indices.push_back(lastIndex - 1);
        indices.push_back(lastIndex - sectors);
    }
    
    void createIndices(int stackCount, int sectorCount) {
        indices.clear();
        int k1, k2;
        for(int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;
            
            for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if(i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                
                if(i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
    
    /// create the sphere
    inline void create(int stacks, int sectors) {
        if(stacks <= 2 || sectors <= 2)
            throw "SphereGeometry.create: Invalid Arguments";
        
        createVertices(stacks, sectors);
        createIndices(stacks, sectors);
    }
};

#endif /* SphereGeometry_h */
