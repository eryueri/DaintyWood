#pragma once

#include <glm/glm.hpp>

namespace DWE {
    struct CameraProperties {
        union Vertex {
            float array[3];
            float x,y,z;
        };
        Vertex position;
        Vertex look_at;
        Vertex up_direction;
        float fov, aspect;
        float z_far, z_near;
    };
    class Camera {
    public:
        Camera() = delete;
        Camera(CameraProperties info);

        glm::mat4 getViewProjectionMatrix() const;
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        void setProperties(CameraProperties info);

        void setPostion(glm::vec3 position);
        void setLookPosition(glm::vec3 look_at);
        void setUpDirection(glm::vec3 up_direction);
        void setFov(float fov);
        void setAspect(float aspect);
        void setZPanel(float z_far, float z_near);
        
    private:
        glm::vec3 _position;
        glm::vec3 _look_at;
        glm::vec3 _up_direction;
        float _fov;
        float _aspect;
        float _z_far;
        float _z_near;
        glm::mat4 _view;
        glm::mat4 _projection;
        glm::mat4 _view_projection;
    };
}
