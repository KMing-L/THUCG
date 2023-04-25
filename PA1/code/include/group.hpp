#ifndef GROUP_H
#define GROUP_H

#include "hit.hpp"
#include "object3d.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D {

  public:
    Group() {}

    explicit Group(int num_objects) : size(num_objects), objects(num_objects) {}

    ~Group() override {
        for (auto &object : objects) {
            delete object;
        }
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool hit = false;
        for (auto &object : objects) {
            if (object->intersect(r, h, tmin)) {
                hit = true;
            }
        }
        return hit;
    }

    void addObject(int index, Object3D *obj) {
        if (index < size) {
            objects[index] = obj;
        }
    }

    int getGroupSize() { return size; }

  private:
    std::vector<Object3D *> objects;
    int size;
};

#endif
