#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "vec3.h"
#include <cmath>
#include <cstdlib>

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3(random_double()*2-1, random_double()*2-1, random_double()*2-1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

struct hit_record;

class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
    
    virtual color emitted() const {
        return color(0,0,0);
    }
    
    virtual ~material() = default;
};

// Diffuse Material
class lambertian : public material {
public:
    color albedo;
    
    lambertian(const color& a) : albedo(a) {}
    
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override;
};

// Metal Material
class metal : public material {
public:
    color albedo;
    double fuzz;
    
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override;
};

// Dielectric Material (Glass)
class dielectric : public material {
public:
    double ir; 
    color tint;
    
    dielectric(double index_of_refraction, const color& t = color(1,1,1)) 
        : ir(index_of_refraction), tint(t) {}
    
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override;

private:
    static double reflectance(double cosine, double ref_idx) {
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

// Emissive Material (Light)
class emissive : public material {
public:
    color emit_color;
    double intensity;
    
    emissive(const color& c, double i = 1.0) : emit_color(c), intensity(i) {}
    
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        return false; 
    }
    
    virtual color emitted() const override {
        return intensity * emit_color;
    }
};

#endif