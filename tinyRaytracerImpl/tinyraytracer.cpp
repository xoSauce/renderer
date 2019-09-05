#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"


class Sphere {
    private:
        Sphere(Vec3f c, float r) :center(c), radius(r)  {}
        Vec3f center;
        float radius;

    public:
        bool rayIntersect(const Vec3f& orig, const Vec3f& dir, float& t0) const {
            Vec3f distanceToRayOrigin = this.center - orig;

            // Small trick to save computation time.
            // The idea is of making a projection from center onto the ray. This is just a shortcut for that
            float projectionOntoRayLength = distanceToRayOrigin * dir;
            
            float distance = (distanceToRayOrigin * distanceToRayOrigin) - (projectionIntersectionPoint * projectionOntoRay)
            if (distance > radius * radius) {
                return false;
            }
            projection.norm()



        }
}

void render() {
    const int width    = 1024;
    const int height   = 768;
    std::vector<Vec3f> framebuffer(width*height);

    for (size_t j = 0; j<height; j++) {
        for (size_t i = 0; i<width; i++) {
            framebuffer[i+j*width] = Vec3f(j/float(height),i/float(width), 0);
        }
    }

    std::ofstream ofs; // save the framebuffer to file
    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height*width; ++i) {
        for (size_t j = 0; j<3; j++) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}

int main() {
    render();
    return 0;
}