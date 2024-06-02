#include <glm/glm.hpp>

// Define a Ray structure representing a ray in 3D space
struct Ray {
    glm::vec3 origin;     // Origin point of the ray
    glm::vec3 direction;  // Direction vector of the ray (normalized)

    // Constructor to initialize the ray with origin and direction
    Ray(const glm::vec3& _origin, const glm::vec3& _direction) : origin(_origin), direction(glm::normalize(_direction)) {}
};
