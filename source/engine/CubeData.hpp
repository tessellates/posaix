#pragma once

#include "Rendering.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::rotate


enum CubeFace {
    Top,
    Bottom,
    Left,
    Right,
    Front, // Optional, as it would be the identity matrix
    Back
};

inline glm::mat4 rotateFrontFace(CubeFace face) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f); // Start with an identity matrix

    switch(face) {
        case Top:
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            break;
        case Bottom:
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(+90.0f), glm::vec3(1, 0, 0));
            break;
        case Left:
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(+90.0f), glm::vec3(0, 1, 0));
            break;
        case Right:
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
            break;
        case Back:
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
            break;
        case Front:
            // No rotation needed to align the front face to the front
            break;
        default:
            // Optional: Handle invalid input case
            break;
    }

    return rotationMatrix;
}


struct Cube {
    glm::vec3 location;
    float size;
    float angleX = 0;
    float angleY = 0;
    glm::vec3 vertices_[8];
    bool culling = true;
    glm::mat4 rotationMatrix;

    // Function to create a cube with given size and location
    static Cube create(const glm::vec3& loc, float s) {
        Cube c{ loc, s };
        c.rotate();
        return c;
    }

    // Function to rotate the cube around the specified axis
    void rotateCenter(float angle_, const glm::vec3& axis) {
        // Create rotation matrix
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle_), axis);
        // Apply rotation to cube's location
        location = glm::vec3(rotationMatrix * glm::vec4(location, 1.0f));
    }


    void rotate()
    {
        
        // Start with an identity matrix
        rotationMatrix = glm::mat4(1.0f);

        // First rotation around the Y-axis
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angleY), glm::vec3(0, 1, 0));

        // Then rotation around the X-axis
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angleX), glm::vec3(1, 0, 0));
        //rotationMatrix +=  glm::rotate(glm::mat4(1.0f), glm::radians(angleX), {1,0,0});
        glm::vec3 vertices[8] = {
            location + glm::vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f), // Front top left
            location + glm::vec3(size / 2.0f, -size / 2.0f, -size / 2.0f),  // Front top right
            location + glm::vec3(-size / 2.0f, size / 2.0f, -size / 2.0f),  // Front bot left
            location + glm::vec3(size / 2.0f, size / 2.0f, -size / 2.0f),   // Front bot right
            location + glm::vec3(-size / 2.0f, -size / 2.0f, size / 2.0f),  // Back top left
            location + glm::vec3(size / 2.0f, -size / 2.0f, size / 2.0f),   // Back top right
            location + glm::vec3(-size / 2.0f, +size / 2.0f, size / 2.0f),   // Back bot left
            location + glm::vec3(size / 2.0f, +size / 2.0f, size / 2.0f)     // Back bot right
        };

        // Rotate each vertex around the center of the cube
        for (int i = 0; i < 8; ++i) {
            vertices_[i] = glm::vec3(rotationMatrix * glm::vec4(vertices[i] - location, 1.0f)) + location;
        }
    }

    // Function to project the cube onto 2D space (XY plane)
    std::vector<glm::vec2> project2D() const {        
        // Project vertices onto 2D space (XY plane)
        std::vector<glm::vec2> projectedVertices;
        for (int i = 0; i < 8; ++i) {
            projectedVertices.push_back(glm::vec2(vertices_[i].x, vertices_[i].y));
        }

        return projectedVertices;
    }
};

inline float calculateZComponentOfCrossProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
    // Vector from a to b
    glm::vec2 ab = b - a;
    // Vector from a to c
    glm::vec2 ac = c - a;
    // Z-component of the cross product
    return ab.x * ac.y  - ab.y * ac.x;
}

inline void drawFace(SDL_Renderer* renderer, const std::vector<glm::vec2>& vertices, const std::vector<int>& indices, bool culling) {
    if (calculateZComponentOfCrossProduct(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]]) > 0 || !culling) {
        // If the z-component of the cross product is negative, the face is considered "facing towards us"
        for (int i = 0; i < indices.size(); ++i) {
            SDL_RenderDrawLine(renderer,
                               vertices[indices[i]].x, vertices[indices[i]].y,
                               vertices[indices[(i + 1) % indices.size()]].x, vertices[indices[(i + 1) % indices.size()]].y);
        }
    }
}

inline void drawCube(SDL_Renderer* renderer, const std::vector<glm::vec2>& vertices, bool culling) {
    // Each face is represented by the indices of its vertices
    std::vector<std::vector<int>> faces = {
        {0, 1, 3, 2}, // Front face
        {5, 4, 6, 7}, // Back face
        {4, 5, 1, 0}, // Top face
        {2, 3, 7, 6}, // Bottom face
        {4, 0, 2, 6}, // Left face
        {1, 5, 7, 3}  // Right face
    };

    for (const auto& face : faces) {
        drawFace(renderer, vertices, face, culling);
    }
}

/*
inline void drawCube(SDL_Renderer* renderer, const std::vector<glm::vec2>& vertices) {
    // Front face
    SDL_RenderDrawLine(renderer, vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y); // Bottom edge
    SDL_RenderDrawLine(renderer, vertices[1].x, vertices[1].y, vertices[3].x, vertices[3].y); // Right edge
    SDL_RenderDrawLine(renderer, vertices[3].x, vertices[3].y, vertices[2].x, vertices[2].y); // Top edge
    SDL_RenderDrawLine(renderer, vertices[2].x, vertices[2].y, vertices[0].x, vertices[0].y); // Left edge

    // Back face
    SDL_RenderDrawLine(renderer, vertices[4].x, vertices[4].y, vertices[5].x, vertices[5].y); // Bottom edge
    SDL_RenderDrawLine(renderer, vertices[5].x, vertices[5].y, vertices[7].x, vertices[7].y); // Right edge
    SDL_RenderDrawLine(renderer, vertices[7].x, vertices[7].y, vertices[6].x, vertices[6].y); // Top edge
    SDL_RenderDrawLine(renderer, vertices[6].x, vertices[6].y, vertices[4].x, vertices[4].y); // Left edge

    // Top face
    SDL_RenderDrawLine(renderer, vertices[2].x, vertices[2].y, vertices[3].x, vertices[3].y); // Front edge
    SDL_RenderDrawLine(renderer, vertices[3].x, vertices[3].y, vertices[7].x, vertices[7].y); // Right edge
    SDL_RenderDrawLine(renderer, vertices[7].x, vertices[7].y, vertices[6].x, vertices[6].y); // Back edge
    SDL_RenderDrawLine(renderer, vertices[6].x, vertices[6].y, vertices[2].x, vertices[2].y); // Left edge

    // Bottom face
    SDL_RenderDrawLine(renderer, vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y); // Front edge
    SDL_RenderDrawLine(renderer, vertices[1].x, vertices[1].y, vertices[5].x, vertices[5].y); // Right edge
    SDL_RenderDrawLine(renderer, vertices[5].x, vertices[5].y, vertices[4].x, vertices[4].y); // Back edge
    SDL_RenderDrawLine(renderer, vertices[4].x, vertices[4].y, vertices[0].x, vertices[0].y); // Left edge

    // Left face
    SDL_RenderDrawLine(renderer, vertices[0].x, vertices[0].y, vertices[2].x, vertices[2].y); // Front edge
    SDL_RenderDrawLine(renderer, vertices[2].x, vertices[2].y, vertices[6].x, vertices[6].y); // Top edge
    SDL_RenderDrawLine(renderer, vertices[6].x, vertices[6].y, vertices[4].x, vertices[4].y); // Back edge
    SDL_RenderDrawLine(renderer, vertices[4].x, vertices[4].y, vertices[0].x, vertices[0].y); // Bottom edge

    // Right face
    SDL_RenderDrawLine(renderer, vertices[1].x, vertices[1].y, vertices[3].x, vertices[3].y); // Front edge
    SDL_RenderDrawLine(renderer, vertices[3].x, vertices[3].y, vertices[7].x, vertices[7].y); // Top edge
    SDL_RenderDrawLine(renderer, vertices[7].x, vertices[7].y, vertices[5].x, vertices[5].y); // Back edge
    SDL_RenderDrawLine(renderer, vertices[5].x, vertices[5].y, vertices[1].x, vertices[1].y); // Bottom edge
};*/

struct CubeScene
{
    SDL_Point sceneSize = {1024, 1024};
    SDL_Texture* scene;
    SDL_Texture* projection;
    SDL_Surface* surface;

    UpdatableData<Cube> cube;
    TextureRenderData* trd;

    void init(TextureRenderData* trd);
    void init();
    void update();
    void renderScene();
    void projectCube();
};
