#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "hit.hpp"
#include "ray.hpp"
#include <glut.h>
#include <iostream>

// TODO (PA2): Copy from PA1.
class Material {
  public:
    explicit Material(const Vector3f &d_color,
                      const Vector3f &s_color = Vector3f::ZERO, float s = 0)
        : diffuseColor(d_color), specularColor(s_color), shininess(s) {}

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const { return diffuseColor; }

    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight,
                   const Vector3f &lightColor) {
        Vector3f l_ambient, l_diffuse, l_specular;

        l_ambient = Vector3f::ZERO;

        l_diffuse = clamp(Vector3f::dot(dirToLight, hit.getNormal())) *
                    lightColor * diffuseColor;

        l_specular =
            pow(clamp(Vector3f::dot(
                    -ray.getDirection(),
                    2 * hit.getNormal() *
                            Vector3f::dot(dirToLight, hit.getNormal()) -
                        dirToLight)),
                shininess) *
            lightColor * specularColor;

        return l_ambient + l_diffuse + l_specular;
    }

    // For OpenGL, this is fully implemented
    void Use() {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                     Vector4f(diffuseColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                     Vector4f(specularColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,
                     Vector2f(shininess * 4.0, 1.0f));
    }

  protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;

  private:
    float clamp(float x) {
        if (x < 0)
            return 0;
        if (x > 1)
            return 1;
        return x;
    }
};

#endif // MATERIAL_H
