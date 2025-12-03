#pragma once

#include <glm/glm.hpp>

namespace Nilos {

/**
 * @brief Camera helper functions
 * 
 * Additional camera utilities beyond the CameraComponent.
 * This is a namespace for camera-related calculations.
 */
namespace Camera {

    /**
     * @brief Calculate view matrix from position and direction
     */
    inline glm::mat4 CalculateViewMatrix(const glm::vec3& position, 
                                          const glm::vec3& front, 
                                          const glm::vec3& up) {
        return glm::lookAt(position, position + front, up);
    }

    /**
     * @brief Calculate perspective projection matrix
     */
    inline glm::mat4 CalculatePerspectiveMatrix(float fov, 
                                                  float aspectRatio, 
                                                  float nearPlane, 
                                                  float farPlane) {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    /**
     * @brief Calculate orthographic projection matrix
     */
    inline glm::mat4 CalculateOrthographicMatrix(float left, float right, 
                                                   float bottom, float top, 
                                                   float nearPlane, float farPlane) {
        return glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    }

} // namespace Camera

} // namespace Nilos

