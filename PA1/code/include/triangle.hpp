#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <cmath>
#include <iostream>
#include <vecmath.h>
using namespace std;

class Triangle : public Object3D {

  public:
    Triangle() = delete;

    // a b c are three vertex positions of the triangle
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c,
             Material *m)
        : Object3D(m) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        normal = Vector3f::cross(b - a, c - a).normalized();
    }

    bool intersect(const Ray &ray, Hit &hit, float tmin) override {
        Vector3f e1 = vertices[1] - vertices[0];
        Vector3f e2 = vertices[2] - vertices[0];
        Vector3f s = ray.getOrigin() - vertices[0];
        Vector3f s1 = Vector3f::cross(ray.getDirection(), e2);
        Vector3f s2 = Vector3f::cross(s, e1);
        float divisor = Vector3f::dot(s1, e1);
        if (divisor == 0) {
            return false;
        }
        float t = Vector3f::dot(s2, e2) / divisor;
        float b1 = Vector3f::dot(s1, s) / divisor;
        float b2 = Vector3f::dot(s2, ray.getDirection()) / divisor;
        float b0 = 1 - b1 - b2;
        if (t > 0 && t > tmin && t < hit.getT() && b0 >= 0 && b1 >= 0 &&
            b2 >= 0) {
            hit.set(t, material, normal);
            return true;
        }
        return false;
    }
    Vector3f normal;

  protected:
    Vector3f vertices[3];
};

#endif // TRIANGLE_H
