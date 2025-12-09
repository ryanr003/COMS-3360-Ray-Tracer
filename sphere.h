#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

// REQUIREMENT: Ray/sphere intersections with UV mapping for textured spheres
class sphere : public hittable {
public:
    sphere(const point3& center, double radius, shared_ptr<material> mat)
        : center1(center), radius(std::fmax(0,radius)), mat(mat), is_moving(false) {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }


    // REQUIREMENT: Motion blur, "moving" sphere
    sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat)
        : center1(center1), radius(std::fmax(0,radius)), mat(mat), is_moving(true) {
        auto rvec = vec3(radius, radius, radius);
        aabb box1(center1 - rvec, center1 + rvec);
        aabb box2(center2 - rvec, center2 + rvec);
        bbox = aabb(box1, box2);
        center_vec = center2 - center1;
    }


    // Test ray-sphere intersection with quadratic fromula
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        point3 center = is_moving ? sphere_center(r.time()) : center1;
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;
        auto discriminant = h*h - a*c;

        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);
        auto root = (h - sqrtd) / a;

        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }



private:
    point3 center1;
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec;
    aabb bbox;

    point3 sphere_center(double time) const {
        return center1 + time*center_vec;
    }

    // REQUIREMENT: UV mapping for textured spheres
    static void get_sphere_uv(const point3& p, double& u, double& v) {
        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;
        u = phi / (2*pi);
        v = theta / pi;
    }
};

#endif
