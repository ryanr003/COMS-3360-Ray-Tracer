#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "aabb.h"
#include "vec3.h"
#include <algorithm>

class triangle : public hittable {
public:
    point3 v0, v1, v2;
    std::shared_ptr<material> mat_ptr;

    triangle() {}
    triangle(point3 vertex0, point3 vertex1, point3 vertex2, std::shared_ptr<material> m)
        : v0(vertex0), v1(vertex1), v2(vertex2), mat_ptr(m) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    const double EPSILON = 0.0000001;
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    vec3 h = cross(r.direction(), edge2);
    double a = dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false; // Ray is parallel to triangle

    double f = 1.0 / a;
    vec3 s = r.origin() - v0;
    double u = f * dot(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    vec3 q = cross(s, edge1);
    double v = f * dot(r.direction(), q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = f * dot(edge2, q);

    if (t > t_min && t < t_max) {
        rec.t = t;
        rec.p = r.at(t);
        vec3 outward_normal = unit_vector(cross(edge1, edge2));
        rec.set_face_normal(r, outward_normal);
        rec.u = u;
        rec.v = v;
        rec.mat_ptr = mat_ptr;
        return true;
    }

    return false;
}

bool triangle::bounding_box(double t0, double t1, aabb& output_box) const {
    point3 min_point(
        std::min(std::min(v0.x(), v1.x()), v2.x()),
        std::min(std::min(v0.y(), v1.y()), v2.y()),
        std::min(std::min(v0.z(), v1.z()), v2.z())
    );
    point3 max_point(
        std::max(std::max(v0.x(), v1.x()), v2.x()),
        std::max(std::max(v0.y(), v1.y()), v2.y()),
        std::max(std::max(v0.z(), v1.z()), v2.z())
    );
    
    // Adding small padding
    const double padding = 0.0001;
    output_box = aabb(min_point - vec3(padding, padding, padding), 
                      max_point + vec3(padding, padding, padding));
    return true;
}

#endif