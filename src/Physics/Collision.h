#pragma once

#include <glm/glm.hpp>

namespace Nilos {

/**
 * @brief Axis-Aligned Bounding Box (AABB)
 * 
 * Simple, fast collision detection.
 * Perfect for basic gameplay and NPC navigation.
 */
struct AABB {
    glm::vec3 Min;
    glm::vec3 Max;

    AABB() : Min(0.0f), Max(0.0f) {}
    AABB(const glm::vec3& min, const glm::vec3& max) : Min(min), Max(max) {}

    /**
     * @brief Create AABB from center and size
     */
    static AABB FromCenterSize(const glm::vec3& center, const glm::vec3& size) {
        glm::vec3 halfSize = size * 0.5f;
        return AABB(center - halfSize, center + halfSize);
    }

    /**
     * @brief Check if this AABB intersects another
     */
    bool Intersects(const AABB& other) const {
        return (Min.x <= other.Max.x && Max.x >= other.Min.x) &&
               (Min.y <= other.Max.y && Max.y >= other.Min.y) &&
               (Min.z <= other.Max.z && Max.z >= other.Min.z);
    }

    /**
     * @brief Check if a point is inside this AABB
     */
    bool Contains(const glm::vec3& point) const {
        return point.x >= Min.x && point.x <= Max.x &&
               point.y >= Min.y && point.y <= Max.y &&
               point.z >= Min.z && point.z <= Max.z;
    }

    /**
     * @brief Get center of AABB
     */
    glm::vec3 GetCenter() const {
        return (Min + Max) * 0.5f;
    }

    /**
     * @brief Get size of AABB
     */
    glm::vec3 GetSize() const {
        return Max - Min;
    }

    /**
     * @brief Expand AABB to include a point
     */
    void Expand(const glm::vec3& point) {
        Min = glm::min(Min, point);
        Max = glm::max(Max, point);
    }
};

/**
 * @brief Simple ray for raycasting
 */
struct Ray {
    glm::vec3 Origin;
    glm::vec3 Direction;

    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : Origin(origin), Direction(glm::normalize(direction)) {}

    /**
     * @brief Get point along ray at distance t
     */
    glm::vec3 GetPoint(float t) const {
        return Origin + Direction * t;
    }

    /**
     * @brief Ray-AABB intersection test
     * @param box AABB to test against
     * @param tMin Output: distance to near intersection
     * @param tMax Output: distance to far intersection
     * @return True if ray intersects box
     */
    bool Intersects(const AABB& box, float& tMin, float& tMax) const {
        tMin = 0.0f;
        tMax = 1000000.0f; // Far plane

        for (int i = 0; i < 3; ++i) {
            float invD = 1.0f / Direction[i];
            float t0 = (box.Min[i] - Origin[i]) * invD;
            float t1 = (box.Max[i] - Origin[i]) * invD;

            if (invD < 0.0f) {
                std::swap(t0, t1);
            }

            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;

            if (tMax < tMin) {
                return false;
            }
        }

        return true;
    }
};

} // namespace Nilos

