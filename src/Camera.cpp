#include "Camera.h"
#include <iostream>
Camera::Camera(int height, int width) {
    this->ViewMatrix = glm::mat4(1.f);
    this->up = glm::vec3(0.f, 1.f, 0.f);
    this->position = glm::vec3(0.f, 0.f, 2.f);
    this->right = glm::vec3(1.f, 0.f, 0.f);
    this->direction = glm::vec3(0.f, 0.f, -1.f);
    this->maxPitch = 90;
    this->worldUp = glm::vec3(0.f, 1.f, 0.f);
    this->pitch = 0.f;
    this->yaw = 0.f;
    this->roll = 0.f;
    this->FOV = 70.f;
    this->nearPlane = 0.1f;
    this->farPlane = 1000.f;
    this->frameBufferHeight = height;
    this->frameBufferWidth = width;
    rotate(0, 0, 0);
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::~Camera() {
}

void Camera::move(float right, float up, float forward){
    this->position = this->position + forward * this->direction + right * this->right + up * this->up;
    updateViewMatrix();
}

void Camera::rotate(float dpitch, float dyaw, float droll){
    this->pitch += dpitch;
    if (this->pitch > this->maxPitch)
        this->pitch = this->maxPitch;
    else if (this->pitch < -this->maxPitch)
        this->pitch = -this->maxPitch;
    
    this->yaw += dyaw;
    if ((this->yaw > 360) || (this->yaw < -360))
        this->yaw = 0;
    
    this->roll += droll;
    if ((this->roll > 360) || (this->roll < -360))
        this->roll = 0;
    
    glm::mat4 RotationMatrix(1.f);
    RotationMatrix = glm::translate(RotationMatrix, glm::vec3(0.f, 0.f, 0.f));
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(this->pitch), glm::vec3(1.f, 0.f, 0.f));
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(this->yaw), glm::vec3(0.f, 1.f, 0.f));
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(this->roll), glm::vec3(0.f, 0.f, 1.f));
    RotationMatrix = glm::scale(RotationMatrix, glm::vec3(1.f));
    this->up = glm::vec4(this->worldUp, 1.f) * RotationMatrix;
    this->direction = glm::normalize(glm::vec3(glm::vec4(0.f, 0.f, -1.f, 1.f) * RotationMatrix));
    this->right = glm::normalize(glm::cross(this->direction, this->up));
    
    updateViewMatrix();
}

void Camera::setFOV(float FOV){
    this->FOV = FOV;
    updateProjectionMatrix();
}

void Camera::setNearPlane(float nearPlane){
    this->nearPlane = nearPlane;
    updateProjectionMatrix();
}

void Camera::setFarPlane(float farPlane){
    this->farPlane = farPlane;
    updateProjectionMatrix();
}

void Camera::updateFrameBuffer(int height, int width){
    this->frameBufferHeight = height;
    this->frameBufferWidth = width;
    updateProjectionMatrix();
}

void Camera::updateViewMatrix(){
    this->ViewMatrix = glm::lookAt(this->position, this->position + this->direction, this->up);
}

void Camera::updateProjectionMatrix(){
    this->ProjectionMatrix = glm::mat4(1.f);
    this->ProjectionMatrix = glm::perspective(glm::radians(this->FOV),
        static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
        this->nearPlane, this->farPlane);
}
