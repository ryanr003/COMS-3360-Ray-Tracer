#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "mesh.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
        return color(0,0,0);

    // Check for intersection with any object
    if (world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) {
        ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted();
        
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emitted + attenuation * ray_color(scattered, world, depth-1);
        }
        return emitted;
    }

    // Background gradient
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned>(time(0)));

    // Image settings
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;  // Anti-aliasing samples
    const int max_depth = 50;

    // World
    std::vector<std::shared_ptr<hittable>> objects;

    // Materials
    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto bubble_material = std::make_shared<dielectric>(1.5, color(1.0, 0.7, 0.9)); // Pink tinted glass
    auto sparkle_material = std::make_shared<metal>(color(1.0, 0.9, 1.0), 0.1);
    auto light_material = std::make_shared<emissive>(color(1.0, 1.0, 1.0), 2.0);

    // Ground
    objects.push_back(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // Main bubble (Glinda's bubble)
    objects.push_back(std::make_shared<sphere>(point3(0, 1, 0), 1.0, bubble_material));
    
    // Inner glow sphere
    auto inner_glow = std::make_shared<emissive>(color(1.0, 0.8, 0.95), 0.3);
    objects.push_back(std::make_shared<sphere>(point3(0, 1, 0), 0.95, inner_glow));

    // Some sparkles around the bubble cause it's magical
    for (int i = 0; i < 20; i++) {
        double theta = 2.0 * M_PI * random_double();
        double phi = M_PI * random_double();
        double r = 1.2 + random_double() * 0.3;
        
        double x = r * sin(phi) * cos(theta);
        double y = 1.0 + r * sin(phi) * sin(theta);
        double z = r * cos(phi);
        
        objects.push_back(std::make_shared<sphere>(
            point3(x, y, z), 
            0.03 + random_double() * 0.02, 
            sparkle_material
        ));
    }

    // Lights
    objects.push_back(std::make_shared<sphere>(point3(3, 4, -2), 0.5, light_material));
    objects.push_back(std::make_shared<sphere>(point3(-3, 3, 1), 0.4, light_material));

    // Triangles to demonstrate triangle rendering
    auto triangle_mat = std::make_shared<metal>(color(0.9, 0.8, 1.0), 0.3);
    objects.push_back(std::make_shared<triangle>(
        point3(-2, 0, -2),
        point3(-1.5, 0.5, -2),
        point3(-2, 0.5, -1.5),
        triangle_mat
    ));

    // Build BVH acceleration structure
    std::cout << "Building BVH with " << objects.size() << " objects..." << std::endl;
    auto world = std::make_shared<bvh_node>(objects, 0, objects.size());

    // Camera
    point3 lookfrom(4, 2, 3);
    point3 lookat(0, 1, 0);
    vec3 vup(0, 1, 0);
    auto vfov = 40.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio);

    // Render
    std::cout << "Rendering " << image_width << "x" << image_height << " image..." << std::endl;
    std::ofstream outfile("output.ppm");
    outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            
            // Anti-aliasing
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, *world, max_depth);
            }
            
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }

    outfile.close();
    std::cerr << "\nDone! Image saved to output.ppm\n";
    
    return 0;
}
