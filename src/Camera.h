#ifndef CAMERA_H
#define CAMERA_H
#include "../Dependencies/GLM/include/glm/gtc/matrix_transform.hpp"
#include "../Dependencies/GLM/include/glm/glm.hpp"
class Camera {
public:
    Camera(int, int);
    ~Camera();
    void move(float, float, float);
    void rotate(float, float, float);
    void setFOV(float);
    void setNearPlane(float);
    void setFarPlane(float);
    void updateFrameBuffer(int, int);
    inline glm::mat4 getViewMatrix() { return ViewMatrix; } 
    inline glm::mat4 getProjectionMatrix() { return ProjectionMatrix;}
    inline glm::vec3 getPosition() { return position; }
    
private:
    float maxPitch;
    float FOV;
    float nearPlane;
    float farPlane;
    int frameBufferHeight;
    int frameBufferWidth; 
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 worldUp;
    glm::vec3 up;
    glm::vec3 right;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    float pitch;
    float yaw;
    float roll;
    void updateViewMatrix();
    void updateProjectionMatrix();
};

#endif /* CAMERA_H */