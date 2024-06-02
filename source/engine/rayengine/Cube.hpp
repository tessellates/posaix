#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Define a Ray structure representing a ray in 3D space
struct Ray {
    glm::vec3 origin;     // Origin point of the ray
    glm::vec3 direction;  // Direction vector of the ray (normalized)

    // Constructor to initialize the ray with origin and direction
    Ray(const glm::vec3& _origin, const glm::vec3& _direction) : origin(_origin), direction(glm::normalize(_direction)) {}
};

// Define a Cube structure representing a cube in 3D space with transformation
struct CubeFunction {
    glm::mat4 transform;  // Transformation matrix representing the cube's position, rotation, and scale

    // Constructor to initialize the cube with a transformation matrix
    CubeFunction(const glm::mat4& _transform) : transform(_transform) {}

    // Function to check if a point is inside the cube
    bool contains(const glm::vec3& point) const {
        // Transform the point by the inverse of the cube's transformation matrix
        glm::vec4 localPoint = glm::inverse(transform) * glm::vec4(point, 1.0f);
        // Check if the transformed point lies within the unit cube centered at the origin
        return glm::all(glm::greaterThanEqual(localPoint.xyz(), glm::vec3(-0.5f))) && 
               glm::all(glm::lessThanEqual(localPoint.xyz(), glm::vec3(0.5f)));
    }

    // Function to find the first intersection between a ray and the cube
    bool intersect(const Ray& ray, float& t, Ray* intersectionRay = nullptr) const {
        // Transform the ray by the inverse of the cube's transformation matrix
        glm::vec3 localOrigin = glm::vec3(glm::inverse(transform) * glm::vec4(ray.origin, 1.0f));
        glm::vec3 localDirection = glm::vec3(glm::inverse(glm::mat3(transform)) * ray.direction);

        // Initialize variables to store the minimum and maximum t values
        float tMin = -std::numeric_limits<float>::infinity();
        float tMax = std::numeric_limits<float>::infinity();

        // Initialize variables to store the normal and intersection point
        glm::vec3 faceNormal(0.0f);
        glm::vec3 intersection;

        // Iterate over each face of the cube and perform intersection tests
        for (int i = 0; i < 3; ++i) {
            if (localDirection[i] == 0.0f) {
                // Ray is parallel to the face, check if ray origin is within face bounds
                if (localOrigin[i] < -0.5f || localOrigin[i] > 0.5f) {
                    return false;  // Ray misses the cube
                }
            } else {
                // Compute t values for intersection with each face
                float t1 = (-0.5f - localOrigin[i]) / localDirection[i];
                float t2 = (0.5f - localOrigin[i]) / localDirection[i];
                // Update tMin and tMax
                tMin = glm::max(tMin, glm::min(t1, t2));
                tMax = glm::min(tMax, glm::max(t1, t2));
                // Check if tMax is smaller than tMin
                if (tMax < tMin) {
                    return false;  // Ray misses the cube
                }
            }
        }

        // Check if the intersection is in front of the ray's origin
        if (tMax < 0.0f) {
            return false;  // Ray misses the cube
        }

        // Calculate the intersection point and normal
        intersection = ray.origin + ray.direction * tMin;
        if (tMin > 0) {
            // The ray intersects with a face of the cube
            if (localDirection.x != 0.0f) {
                // The ray intersects with the yz plane
                faceNormal = glm::vec3(localDirection.x < 0 ? 1 : -1, 0, 0);
            } else if (localDirection.y != 0.0f) {
                // The ray intersects with the xz plane
                faceNormal = glm::vec3(0, localDirection.y < 0 ? 1 : -1, 0);
            } else if (localDirection.z != 0.0f) {
                // The ray intersects with the xy plane
                faceNormal = glm::vec3(0, 0, localDirection.z < 0 ? 1 : -1);
            }
        }

        // Transform the intersection point and normal back to world space
        intersectionRay->origin = glm::vec3(transform * glm::vec4(intersection, 1.0f));
        intersectionRay->direction = glm::vec3(transform * glm::vec4(intersection, 1.0f));
        intersectionPoint = glm::vec3(transform * glm::vec4(intersection, 1.0f));
        normal = glm::normalize(glm::vec3(glm::transpose(glm::inverse(transform)) * glm::vec4(faceNormal, 0.0f)));

        // Update t with the nearest intersection point
        t = tMin > 0.0f ? tMin : tMax;
        return true;
    }
};
