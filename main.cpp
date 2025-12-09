#include "wicked.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "quad.h"
#include "triangle.h"
#include "bvh.h"
#include "material.h"
#include "texture.h"



// REQUIREMENT: Visible features shown in Glinda's pink bubble scene
int main() {
    // REQUIREMENT: Object instancing: world container
    hittable_list world;




    // REQUIREMENT: Texture loading: Load image textures from files
    auto pink_gradient = make_shared<image_texture>("textures/pink_gradient.jpg");
    auto sparkle_texture = make_shared<image_texture>("textures/sparkle.png");
    




    // REQUIREMENT: Dielectric material: outter glass-like bubble
    auto bubble_material = make_shared<dielectric>(1.3);
    




    // REQUIREMENT: Diffuse material with texture: inner bubble uses pink gradient
    auto textured_pink = make_shared<lambertian>(pink_gradient);
    




    // Main bubble sphere, REQUIREMENT: Ray/sphere intersections & textured spheres
    auto bubble_center = point3(0, 1.0, 0);
    auto bubble_radius = 1.0;
    



    // Outer glass layer, REQUIREMENT: Dielectric material
    world.add(make_shared<sphere>(bubble_center, bubble_radius, bubble_material));
    



    // Inner textured layer, REQUIREMENT: Textured spheres with pink gradient
    world.add(make_shared<sphere>(bubble_center, bubble_radius - 0.02, textured_pink));
    



    // REQUIREMENT: Textured spheres with sparkle texture (the far right shpere)
    auto sparkle_material = make_shared<lambertian>(sparkle_texture);
    world.add(make_shared<sphere>(point3(2.2, 0.9, -0.8), 0.9, sparkle_material));
    



    // REQUIREMENT: Specular material, reflective metal (seen on small sphere in the front)
    auto shiny_pink = make_shared<metal>(color(1.0, 0.7, 0.9), 0.1);
    



    // REQUIREMENT: Motion blur, floating sparkles around the main sphere (cause it needs to be magical and stuff!)
    for (int i = 0; i < 30; i++) {
        double theta = random_double(0, 2*pi);
        double phi = random_double(0, pi);
        double r = bubble_radius + random_double(0.1, 0.5);
        
        auto x = r * std::sin(phi) * std::cos(theta);
        auto y = 1.0 + r * std::cos(phi);
        auto z = r * std::sin(phi) * std::sin(theta);
        
        point3 center1(x, y, z);
        point3 center2 = center1 + vec3(random_double(-0.1, 0.1), 
                                        random_double(-0.05, 0.05), 
                                        random_double(-0.1, 0.1));
        

        // REQUIREMENT: Emissive materials (lights), glowing pink sparkles around main sphere
        auto sparkle_mat = make_shared<diffuse_light>(color(1.0, 0.5, 0.95) * random_double(4, 8));
        world.add(make_shared<sphere>(center1, center2, 0.025, sparkle_mat));
    }
    




    // Ground with checker pattern, REQUIREMENT: Diffuse material
    auto checker = make_shared<checker_texture>(0.8, color(0.1, 0.3, 0.2), color(0.15, 0.4, 0.3));
    auto ground_mat = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));
    




    // REQUIREMENT: Quads, dark gold platform beneath main bubble with gradient texture
    auto platform_mat = make_shared<lambertian>(color(0.4, 0.3, 0.05));
    world.add(make_shared<quad>(point3(-2, 0, -2), vec3(4, 0, 0), vec3(0, 0, 4), platform_mat));
    




    // REQUIREMENT: Ray/triangle intersections with normal interpolation (smooth shading)
    // REQUIREMENT: Textured triangles using gradient texture, star shape with connected edges (this took forever, was playing with fire here honestly)
    auto textured_triangle_mat = make_shared<lambertian>(pink_gradient);
    



    // 5-pointed star (made of triangles) with all triangles connected
    point3 star_center(-2.8, 1.8, -0.5);
    double star_outer_radius = 1.1;
    double star_inner_radius = 0.45;
    
    


    // Generate all 10 points
    std::vector<point3> star_points;
    for (int i = 0; i < 10; i++) {
        double angle = i * pi / 5 - pi/2;  
        double radius = (i % 2 == 0) ? star_outer_radius : star_inner_radius;
        star_points.push_back(point3(
            star_center.x() + radius * std::cos(angle),
            star_center.y() + radius * std::sin(angle),
            star_center.z()
        ));
    }
    




    // Create triangles connecting center to each edge of the star
    vec3 normal = vec3(0, 0, 1);
    for (int i = 0; i < 10; i++) {
        int next = (i + 1) % 10;
        // REQUIREMENT: Textured triangles with loaded pink gradient texture
        // REQUIREMENT: Normal interpolation
        world.add(make_shared<triangle>(star_center, star_points[i], star_points[next], 
                                       normal, normal, normal, textured_triangle_mat));
    }
    




    // Additional small, lit triangles seen around the main bubble for more glow
    for (int i = 0; i < 5; i++) {
        double angle = i * 2 * pi / 5 + pi/5;
        double r = bubble_radius + 0.8;
        auto x = r * std::cos(angle);
        auto z = r * std::sin(angle);
        
        point3 v0(x, 0.9, z);
        point3 v1(x + 0.1*std::cos(angle + 0.6), 1.1, z + 0.1*std::sin(angle + 0.6));
        point3 v2(x + 0.1*std::cos(angle - 0.6), 1.1, z + 0.1*std::sin(angle - 0.6));
        
        vec3 n0 = unit_vector(v0 - bubble_center);
        vec3 n1 = unit_vector(v1 - bubble_center);
        vec3 n2 = unit_vector(v2 - bubble_center);
        
        auto light_triangle_mat = make_shared<diffuse_light>(color(1.0, 0.6, 0.95) * 2.5);
        world.add(make_shared<triangle>(v0, v1, v2, n0, n1, n2, light_triangle_mat));
    }
    



    // REQUIREMENT: Volume rendering, mist around main bubble
    auto boundary = make_shared<sphere>(bubble_center, bubble_radius + 0.3, 
                                       make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, 0.5, color(1.0, 0.8, 1.0)));
    




    // REQUIREMENT: Perlin noise, marble-textured sphere (far left sphere) 
    // I know, very out of place for the scene, but I wanted to implement it
    auto perlin_texture = make_shared<noise_texture>(3.0);
    auto perlin_mat = make_shared<lambertian>(perlin_texture);
    world.add(make_shared<sphere>(point3(-3.0, 0.7, 0.5), 0.7, perlin_mat));
    



    // Small sphere, REQUIREMENT: Specular material
    world.add(make_shared<sphere>(point3(1.5, 0.3, 1), 0.3, shiny_pink));
    



    // REQUIREMENT: Emissive materials
    auto light_mat = make_shared<diffuse_light>(color(1, 1, 1) * 4);
    world.add(make_shared<sphere>(point3(4, 6, 3), 1.5, light_mat));
    

    
    // Making lights pink and bright
    auto fill_light = make_shared<diffuse_light>(color(1.0, 0.5, 0.9) * 3.5);
    world.add(make_shared<quad>(point3(-3, 5, -3), vec3(2, 0, 0), vec3(0, 0, 2), fill_light));
    
    


    // REQUIREMENT: Spatial subdivision acceleration structure (BVH)
    world = hittable_list(make_shared<bvh_node>(world));



    // REQUIREMENT: Camera with configurable position, orientation, and field of view
    camera cam;
    


    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 200;  // REQUIREMENT: Anti-aliasing
    cam.max_depth = 50;
    cam.background = color(0.4, 0.18, 0.32);  // Dark pink sky
    

    cam.vfov = 40;  // REQUIREMENT: Field of view
    cam.lookfrom = point3(3, 2, 5);  // REQUIREMENT: Configurable position
    cam.lookat = point3(0, 1, 0);    // REQUIREMENT: Configurable orientation
    cam.vup = vec3(0, 1, 0);         // REQUIREMENT: Configurable orientation
    



    // REQUIREMENT: Defocus blur/depth of field
    cam.defocus_angle = 0.3;
    cam.focus_dist = (cam.lookfrom - cam.lookat).length();



    // REQUIREMENT: Parallelization happens inside render()
    cam.render(world);
    
    return 0;
}