#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <cmath>
#include <vecmath.h>

class Plane : public Object3D {
  public:
    Plane() {}

    Plane(const Vector3f &normal, float d, Material *m)
        : Object3D(m), normal(normal) {
        point = normal * d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float t = Vector3f::dot(point - r.getOrigin(), normal) /
                  Vector3f::dot(r.getDirection(), normal);
        if (t >= 0 && t > tmin && t < h.getT()) {
            h.set(t, material, normal);
            return true;
        }
        return false;
    }

  protected:
    Vector3f normal;
    Vector3f point;
};

#endif // PLANE_H
