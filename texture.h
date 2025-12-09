#ifndef TEXTURE_H
#define TEXTURE_H

#include "wicked.h"
#include "perlin.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include <iostream>

// Base class for all texture types
class texture {
public:
    virtual ~texture() = default;
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(const color& albedo) : albedo(albedo) {}
    solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {}

    color value(double u, double v, const point3& p) const override {
        return albedo;
    }

private:
    color albedo;
};



// REQUIREMENT: Texture loading from files
class image_texture : public texture {
public:
    image_texture(const char* filename) : data(nullptr), width(0), height(0) {
        auto components_per_pixel = 3;
        
        // Load the image
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            std::cerr << "Error: " << stbi_failure_reason() << "\n";
            width = height = 0;
            return;
        }

        bytes_per_scanline = width * 3;
        std::cerr << "SUCCESS: Loaded texture '" << filename << "' (" 
                  << width << "x" << height << ")\n";
    }

    ~image_texture() {
        if (data) {
            stbi_image_free(data);
        }
    }



    // Sample color from image at texture cooridantes (u,v)
    color value(double u, double v, const point3& p) const override {
        // If no texture data, return solid cyan (debug color)
        if (data == nullptr || height <= 0) 
            return color(0, 1, 1);

        u = interval(0, 1).clamp(u);
        v = 1.0 - interval(0, 1).clamp(v);  // Flip V to image coordinates

        auto i = int(u * width);
        auto j = int(v * height);
        
        // Integer mapping
        if (i >= width)  i = width - 1;
        if (j >= height) j = height - 1;

        auto pixel = data + j * bytes_per_scanline + i * 3;

        auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }


private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};



// 3D floor checker pattern alternating between two textures
class checker_texture : public texture {
public:
    checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
        : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_texture(double scale, const color& c1, const color& c2)
        : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

    color value(double u, double v, const point3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x()));
        auto yInteger = int(std::floor(inv_scale * p.y()));
        auto zInteger = int(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }


private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};




// REQUIREMENT: Perlin noise texture
class noise_texture : public texture {
public:
    noise_texture(double scale) : scale(scale) {}

    // Marble pattern
    color value(double u, double v, const point3& p) const override {
        return color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
    }

private:
    perlin noise;
    double scale;
};

#endif
