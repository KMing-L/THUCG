#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <cmath>
#include <float.h>
#include <vecmath.h>

class Camera {
  public:
    Camera(const Vector3f &center, const Vector3f &direction,
           const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

  protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

class PerspectiveCamera : public Camera {
  public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
                      const Vector3f &up, int imgW, int imgH, float angle)
        : Camera(center, direction, up, imgW, imgH) {
        fx = imgW / (2 * tan(angle / 2));
        fy = imgH / (2 * tan(angle / 2));
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f dir = Vector3f((point.x() - width / 2) / fx,
                                (point.y() - height / 2) / fy, 1)
                           .normalized();
        return Ray(center,
                   dir.x() * horizontal + dir.y() * up + dir.z() * direction);
    }

  protected:
    float fx, fy;
};

#endif // CAMERA_H
