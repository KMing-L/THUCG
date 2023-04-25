#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "camera.hpp"
#include "group.hpp"
#include "image.hpp"
#include "light.hpp"
#include "scene_parser.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum]
                  << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2]; // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    SceneParser sceneParser(inputFile.c_str());
    Camera *camera = sceneParser.getCamera();
    Image image(camera->getWidth(), camera->getHeight());
    for (int x = 0; x < camera->getWidth(); ++x) {
        for (int y = 0; y < camera->getHeight(); ++y) {
            Ray ray = camera->generateRay(Vector2f(x, y));
            Hit hit;
            bool isIntersect = sceneParser.getGroup()->intersect(ray, hit, 0);
            if (isIntersect) {
                Vector3f color = Vector3f::ZERO;
                for (int i = 0; i < sceneParser.getNumLights(); ++i) {
                    Light *light = sceneParser.getLight(i);
                    Vector3f L, lightColor;
                    light->getIllumination(ray.pointAtParameter(hit.getT()), L,
                                           lightColor);
                    color += hit.getMaterial()->Shade(ray, hit, L, lightColor);
                }
                image.SetPixel(x, y, color);
            } else {
                image.SetPixel(x, y, sceneParser.getBackgroundColor());
            }
        }
    }
    image.SaveBMP(outputFile.c_str());

    cout << "Hello! Computer Graphics!" << endl;
    return 0;
}
