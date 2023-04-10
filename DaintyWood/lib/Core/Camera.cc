#include "Core/Camera.hh"

#include <glm/gtc/type_ptr.hpp>

namespace DWE {
    Camera::Camera(CameraProperties info)
            : _position(glm::make_vec3(info.position.array)), _look_at(glm::make_vec3(info.look_at.array)),
            _up_direction(glm::make_vec3(info.up_direction.array)),
            _fov(info.fov), _aspect(info.aspect), 
            _z_far(info.z_far), _z_near(info.z_near) {
        _view = glm::lookAt(_position, _look_at, _up_direction);
        _projection = glm::perspective(_fov, _aspect, _z_near, _z_far);
        _view_projection = _projection * _view;
    }

    glm::mat4 Camera::getViewMatrix() const { return _view; }
    glm::mat4 Camera::getProjectionMatrix() const { return _projection; }
    glm::mat4 Camera::getViewProjectionMatrix() const { return _view_projection; }

    void Camera::setProperties(CameraProperties info)
    {
        _position = glm::make_vec3(info.position.array);
        _look_at = glm::make_vec3(info.look_at.array);
        _up_direction = glm::make_vec3(info.up_direction.array);
        _fov = info.fov;
        _aspect = info.aspect;
        _z_far = info.z_far;
        _z_near = info.z_near;
        _view = glm::lookAt(_position, _look_at, _up_direction);
        _projection = glm::perspective(_fov, _aspect, _z_near, _z_far);
        _view_projection = _projection * _view;
    }

    void Camera::setPostion(glm::vec3 position)
    {
        _position = position;
        _view = glm::lookAt(_position, _look_at, _up_direction);
        _view_projection = _projection * _view;
    }

    void Camera::setLookPosition(glm::vec3 look_at)
    {
        _look_at = look_at;
        _view = glm::lookAt(_position, _look_at, _up_direction);
        _view_projection = _projection * _view;
    }

    void Camera::setUpDirection(glm::vec3 up_direction)
    {
        _up_direction = up_direction;
        _view = glm::lookAt(_position, _look_at, _up_direction);
        _view_projection = _projection * _view;
    }

    void Camera::setFov(float fov)
    {
        _fov = fov;
        _projection = glm::perspective(_fov, _aspect, _z_near, _z_far);
        _view_projection = _projection * _view;
    }

    void Camera::setAspect(float aspect)
    {
        _aspect = aspect;
        _projection = glm::perspective(_fov, _aspect, _z_near, _z_far);
        _view_projection = _projection * _view;
    }

    void Camera::setZPanel(float z_far, float z_near)
    {
        _z_far = z_far;
        _z_near = z_near;
        _projection = glm::perspective(_fov, _aspect, _z_near, _z_far);
        _view_projection = _projection * _view;
    }
}
