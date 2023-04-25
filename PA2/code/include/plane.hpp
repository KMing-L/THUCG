#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <cmath>
#include <vecmath.h>

// TODO (PA2): Copy from PA1

class Plane : public Object3D {
  public:
    Plane() {}

    Plane(const Vector3f &normal, float d, Material *m)
        : Object3D(m), norm(normal), d(d) {
        point = normal * d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float t = Vector3f::dot(point - r.getOrigin(), norm) /
                  Vector3f::dot(r.getDirection(), norm);
        if (t >= 0 && t > tmin && t < h.getT()) {
            h.set(t, material, norm);
            return true;
        }
        return false;
    }

    void drawGL() override {
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(norm, xAxis);
        xAxis = Vector3f::cross(yAxis, norm);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * norm + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

  protected:
    Vector3f norm;
    Vector3f point;
    float d;
};

#endif // PLANE_H
