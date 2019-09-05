#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"

static const Vec3f BG_COLOR = Vec3f(0.0, 0.0, 0.0);
static const Vec3f FG_COLOR = Vec3f(0.4, 0.4, 0.3);

class Material {
    public:
    Material(const Vec3f& color) : diffuseColor(color) {}
    Material() : diffuseColor() {}
    Vec3f diffuseColor;

};

class Sphere {
    public:
        Vec3f center;
        float radius;
        Material material;

    public:
        Sphere(const Vec3f& c, const float r, const Material& m) : center(c), radius(r), material(m)  {}
        bool rayIntersect(const Vec3f& orig, 
                            const Vec3f& dir, 
                            float& closestIntersectionPoint) const {
            // Does it intersect the ray ?
            Vec3f distanceToRayOrigin = this->center - orig;
            float projectionOntoRayLength = distanceToRayOrigin * dir;
            float distanceToRaySq = (distanceToRayOrigin * distanceToRayOrigin) - (projectionOntoRayLength * projectionOntoRayLength);
            if (distanceToRaySq > radius * radius) { return false; }
            // We know it intersects. Find point
            float distanceFromPerpIntersectionToSphereIntersection = sqrtf(radius * radius - distanceToRaySq);
            closestIntersectionPoint = projectionOntoRayLength - distanceFromPerpIntersectionToSphereIntersection;
            float furthestIntersectionPoint = projectionOntoRayLength + distanceFromPerpIntersectionToSphereIntersection;
            if (closestIntersectionPoint < 0) { closestIntersectionPoint = furthestIntersectionPoint;}
            if (closestIntersectionPoint < 0) { return false; }
            return true;
        }
};

bool scene_intersect(const Vec3f &orig, 
                        const Vec3f &dir, 
                        const std::vector<Sphere> &spheres, 
                        Vec3f &hit,
                        Vec3f &N, 
                        Material &material) {
    float spheres_dist = std::numeric_limits<float>::max();
    for (size_t i=0; i < spheres.size(); i++) {
        float dist_i;
        if (spheres[i].rayIntersect(orig, dir, dist_i) && dist_i < spheres_dist) {
            spheres_dist = dist_i;
            hit = orig + dir*dist_i;
            N = (hit - spheres[i].center).normalize();
            material = spheres[i].material;
        }
    }
    return spheres_dist<1000;
}

Vec3f castRay(const Vec3f& orig, const Vec3f& dir, const std::vector<Sphere>& spheres) {
    Vec3f point, N;
    Material material;

    if (!scene_intersect(orig, dir, spheres, point, N, material)) {
        return BG_COLOR;
    }
    return material.diffuseColor;
}



void render(const std::vector<Sphere>& spheres) {
    const int width    = 1024;
    const int height   = 768;
    const int fov      = M_PI/2.;

    std::vector<Vec3f> framebuffer(width*height);

    for (size_t j = 0; j<height; j++) {
        for (size_t i = 0; i<width; i++) {
            float x =  (2*(i + 0.5)/(float)width  - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            framebuffer[i+j*width] = castRay(Vec3f(0,0,0), dir, spheres);
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

    Material      ivory(Vec3f(0.4, 0.4, 0.3));
    Material red_rubber(Vec3f(0.3, 0.1, 0.1));
    render(std::vector<Sphere>{
        Sphere(Vec3f(0, 0, -10), 2, ivory),
        Sphere(Vec3f(2, 0, -12), 2, red_rubber),
        Sphere(Vec3f(-2, 0, -12), 2, red_rubber)
    });
    return 0;
}
