#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

// REQUIREMENT: Ray/triangle intersections with normal interpolation (smooth shading)
class triangle : public hittable {
public:
    triangle(const point3& v0, const point3& v1, const point3& v2, 
             shared_ptr<material> mat)
        : v0(v0), v1(v1), v2(v2), mat(mat), smooth_shading(false) {
        edge1 = v1 - v0;
        edge2 = v2 - v0;
        auto n = cross(edge1, edge2);
        normal = unit_vector(n);
        
        set_bounding_box();
    }


    // REQUIREMENT: Normal interpolation for smooth shading
    triangle(const point3& v0, const point3& v1, const point3& v2,
             const vec3& n0, const vec3& n1, const vec3& n2,
             shared_ptr<material> mat)
        : v0(v0), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2), 
          mat(mat), smooth_shading(true) {
        edge1 = v1 - v0;
        edge2 = v2 - v0;
        auto n = cross(edge1, edge2);
        normal = unit_vector(n);
        
        set_bounding_box();
    }



    // Compute bounding box
    void set_bounding_box() {
        auto min_point = point3(
            std::fmin(std::fmin(v0.x(), v1.x()), v2.x()),
            std::fmin(std::fmin(v0.y(), v1.y()), v2.y()),
            std::fmin(std::fmin(v0.z(), v1.z()), v2.z())
        );
        auto max_point = point3(
            std::fmax(std::fmax(v0.x(), v1.x()), v2.x()),
            std::fmax(std::fmax(v0.y(), v1.y()), v2.y()),
            std::fmax(std::fmax(v0.z(), v1.z()), v2.z())
        );
        bbox = aabb(min_point, max_point).pad();
    }

    aabb bounding_box() const override { return bbox; }




    // Ray-triangle test
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 h = cross(r.direction(), edge2);
        double a = dot(edge1, h);

        if (a > -1e-8 && a < 1e-8)
            return false;

        double f = 1.0 / a;
        vec3 s = r.origin() - v0;
        double u = f * dot(s, h); // first barycentric corrdinate

        if (u < 0.0 || u > 1.0)
            return false;

        vec3 q = cross(s, edge1);
        double v = f * dot(r.direction(), q); // second barycentric coordinate

        if (v < 0.0 || u + v > 1.0)
            return false;

        double t = f * dot(edge2, q);

        if (!ray_t.surrounds(t))
            return false;

        rec.t = t;
        rec.p = r.at(t);
        

        // REQUIREMENT: Normal interpolation for smooth shading
        if (smooth_shading) {
            double w = 1.0 - u - v;  // thrid barycentric coordinate
            vec3 interpolated_normal = w * n0 + u * n1 + v * n2;
            rec.set_face_normal(r, unit_vector(interpolated_normal));
        } else {
            rec.set_face_normal(r, normal);
        }
        
        rec.u = u; // Stores barycentric coordinates as texture coordinates
        rec.v = v;
        rec.mat = mat;

        return true;
    }




private:
    point3 v0, v1, v2;
    vec3 n0, n1, n2;
    vec3 edge1, edge2;
    vec3 normal;
    shared_ptr<material> mat;
    aabb bbox;
    bool smooth_shading;
};

#endif
