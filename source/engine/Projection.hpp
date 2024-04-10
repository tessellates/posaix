#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::rotate

#include "CubeData.hpp"

inline void projectFace(Uint32* src, Uint32* dst, const glm::mat4& rotationMatrix, float size, const glm::vec3& location, const SDL_Rect& face, int width)
{
    // Loop over the rectangle area in the source surface
    for (int y = 0; y < face.h; ++y) {
        for (int x = 0; x < face.w; ++x) {
            // Apply rotation to each point (x, y)
            glm::vec3 point(-size / 2.0f + x, -size / 2.0f + y, -size / 2.0f); 
            point += location;

            auto rotatedPoint = glm::vec3(rotationMatrix * glm::vec4(point - location, 1.0f)) + location;
            // Perform bounds checking
            if (rotatedPoint.x >= 0 && rotatedPoint.x < width && rotatedPoint.y >= 0 && rotatedPoint.y < width) {
                // Calculate the original and new positions in the arrays
                int srcPos = (y + face.y) * width + x + face.x;
                int dstPos = static_cast<int>(rotatedPoint.y)  * width + static_cast<int>(rotatedPoint.x);
                // Stream pixel data
                dst[dstPos] = src[srcPos];

                // write 4 times to fix dead pixels by those pesky rotations
                int dstPos2 = static_cast<int>(rotatedPoint.y +0.5f)  * width + static_cast<int>(rotatedPoint.x);
                int dstPos3 = static_cast<int>(rotatedPoint.y)  * width + static_cast<int>(rotatedPoint.x +0.5f );
                int dstPos4 = static_cast<int>(rotatedPoint.y +0.5f)  * width + static_cast<int>(rotatedPoint.x +0.5f);
                dst[dstPos2] = src[srcPos];
                dst[dstPos3] = src[srcPos];
                dst[dstPos4] = src[srcPos];
                /*
                if (int(rotatedPoint.y+rotatedPoint.x) > int(rotatedPoint.x)+int(rotatedPoint.y))
                {
                    if (true)//rotatedPoint.y - (int)(rotatedPoint.y) > rotatedPoint.x-(int)(rotatedPoint.x))
                    {
                        if (dstPos+width < width*width)
                            dst[dstPos+width] = src[srcPos];
                    }
                    if (true)
                    {
                        if (dstPos+1 < width*width)
                            dst[dstPos+1] = src[srcPos];    
                    }
                }*/
            }
        }
    }
}

inline void testP(SDL_Renderer* renderer, SDL_Texture* projection, SDL_Surface* source, const Cube& cube)
{
    // First, lock the texture for pixel access
    void* mPixels;
    int mPitch;
    SDL_LockTexture(projection, NULL, &mPixels, &mPitch);
    
    memset(mPixels, 0, mPitch * source->w); // CLEAR THE TEXTURE    

    // Assuming the texture and surface have the same pixel format
    Uint32* dstPixels = static_cast<Uint32*>(mPixels);
    Uint32* srcPixels = static_cast<Uint32*>(source->pixels);

    std::vector<std::vector<int>> faces = {
        {0, 1, 3, 2}, // Front face
        {5, 4, 6, 7}, // Back face
        {4, 5, 1, 0}, // Top face
        {2, 3, 7, 6}, // Bottom face
        {4, 0, 2, 6}, // Left face
        {1, 5, 7, 3}  // Right face
    };

    const auto vertices = cube.vertices_;
    // front face
    {
        const auto& indices = faces[0];
        if (calculateZComponentOfCrossProduct(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]]) > 0) 
        {
            glm::mat4 rotationMatrix = cube.rotationMatrix; // Assuming Cube can provide a rotation matrix
            SDL_Rect face = {0, 384, (int)cube.size, (int)cube.size};
            projectFace(srcPixels, dstPixels, rotationMatrix, cube.size, cube.location, face, source->w);
        }   
    }

    // front RIGHT
    {
        const auto& indices = faces[5];
        if (calculateZComponentOfCrossProduct(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]]) > 0) 
        {
            glm::mat4 rotationMatrix = cube.rotationMatrix * rotateFrontFace(CubeFace::Right);
            SDL_Rect face = {256, 640, (int)cube.size, (int)cube.size};
            projectFace(srcPixels, dstPixels, rotationMatrix, cube.size, cube.location, face, source->w);
        }   
    }

    // front LEFT
    {
        const auto& indices = faces[4];
        if (calculateZComponentOfCrossProduct(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]]) > 0) 
        {
            glm::mat4 rotationMatrix = cube.rotationMatrix * rotateFrontFace(CubeFace::Left);
            SDL_Rect face = {256, 128, (int)cube.size, (int)cube.size};
            projectFace(srcPixels, dstPixels, rotationMatrix, cube.size, cube.location, face, source->w);
        }   
    }

    // front Bottom
    {
        const auto& indices = faces[3];
        if (calculateZComponentOfCrossProduct(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]]) > 0) 
        {
            glm::mat4 rotationMatrix = cube.rotationMatrix * rotateFrontFace(CubeFace::Bottom);
            SDL_Rect face = {256, 384, (int)cube.size, (int)cube.size};
            projectFace(srcPixels, dstPixels, rotationMatrix, cube.size, cube.location, face, source->w);
        }   
    }

    // front Top
    {
        const auto& indices = faces[2];
        if (calculateZComponentOfCrossProduct(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]]) > 0) 
        {
            glm::mat4 rotationMatrix = cube.rotationMatrix * rotateFrontFace(CubeFace::Top);
            SDL_Rect face = {768, 384, (int)cube.size, (int)cube.size};
            projectFace(srcPixels, dstPixels, rotationMatrix, cube.size, cube.location, face, source->w);
        }   
    }


    // front Back
    {
        const auto& indices = faces[1];
        if (calculateZComponentOfCrossProduct(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]]) > 0) 
        {
            glm::mat4 rotationMatrix = cube.rotationMatrix * rotateFrontFace(CubeFace::Back);
            SDL_Rect face = {512, 384, (int)cube.size, (int)cube.size};
            projectFace(srcPixels, dstPixels, rotationMatrix, cube.size, cube.location, face, source->w);
        }   
    }

    // Unlock the texture
    SDL_UnlockTexture(projection);
}

