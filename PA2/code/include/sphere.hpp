#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <cmath>
#include <glut.h>
#include <vecmath.h>

// TODO (PA2): Copy from PA1

class Sphere : public Object3D {
  public:
    Sphere() {
        // unit ball at the center
        center = Vector3f::ZERO;
        radius = 1.0f;
    }

    Sphere(const Vector3f &center, float radius, Material *material)
        : Object3D(material), center(center), radius(radius) {}

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float a = Vector3f::dot(r.getDirection(), r.getDirection());
        float b = 2 * Vector3f::dot(r.getDirection(), r.getOrigin() - center);
        float c =
            Vector3f::dot(r.getOrigin() - center, r.getOrigin() - center) -
            radius * radius;
        float delta = b * b - 4 * a * c;
        if (delta < 0) {
            return false;
        }
        float t1 = (-b - sqrt(delta)) / (2 * a);
        if (t1 > tmin && t1 < h.getT()) {
            h.set(t1, material, (r.pointAtParameter(t1) - center).normalized());
            return true;
        }
        float t2 = (-b + sqrt(delta)) / (2 * a);
        if (t2 > tmin && t2 < h.getT()) {
            h.set(t2, material, (r.pointAtParameter(t2) - center).normalized());
            return true;
        }
        return false;
    }

    void drawGL() override {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(center.x(), center.y(), center.z());
        glutSolidSphere(radius, 80, 80);
        glPopMatrix();
    }

  protected:
    Vector3f center;
    float radius;
};

#endif
