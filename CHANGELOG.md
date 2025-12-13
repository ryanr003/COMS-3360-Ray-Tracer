# Changelog

Full log timeline for my Ray Tracer (Sept 16 (Started) - Dec 14 (Ended/due date), 2025).

By: Ryan Riebesehl

---

## Week 1: (Sept. 16 - Sept. 21)
- Set up project structure and initial Makefile
- Created core utilities referencing the RTW textbook: `wicked.h`, `vec3.h`, `ray.h`, `color.h`, `interval.h`
- Created basic `hittable.h` interface and `hittable_list.h` container
- Started basic camera setup with viewport calculations `camera.h`
- Started CHANGELOG for weekly updates

---

## Week 2: (Sept. 22 - Sept. 28)
- Created `aabb.h` for axis-aligned bounding boxes
- Started `sphere.h` with ray-sphere intersections
- Started basic `material.h` with diffuse material
- Experimented with anti-aliasing
- Debugged `vec3.h` near_zero() function

---

## Week 3: (Sept. 29 - Oct. 5)
- Created `texture.h` base class and solid color texture
- Got image loading from stb_image.h library
- Added textures/pink_gradient.jpg and textures/sparkle.png found on Google
- UV mapping implementation in `sphere.h`
- Updated `color.h` with HDR gamma correction
- Debugged `texture.h` UV coordinate flipping
- Uploaded initial code to GitHub


---

## Week 4: (Oct. 6 - Oct. 12)
- Completed `material.h` with specular and dielectric materials
- Updated `ray.h` with time component for motion blur
- Added moving sphere support to `sphere.h`
- Started `camera.h` with configurable FOV
- Debugged `sphere.h` moving sphere bounding boxes

---

## Week 5: (Oct. 13 - Oct. 19)
- Completed `camera.h` with anti-aliasing and defocus blur (DOF)
- Updated `wicked.h` with random generation
- Started `bvh.h` BVH acceleration structure with recursive building
- Updated `aabb.h` with padding to prevent zero-thickness boxes
- Debugged `bvh.h` single-object leaf node handling

---

## Week 6: (Oct. 20 - Oct. 26)
- Started `quad.h` quadrilateral primitive implementation
- Updated `material.h` with emissive material
- Updated `texture.h` with image_texture class
- Added checker_texture to `texture.h`
- Debugged `texture.h` error handling for missing files

---

## Week 7: (Oct. 27 - Nov. 2)
- Started `triangle.h` with ray-triangle intersections
- Added Barycentric coordinate calculation for triangles
- Added normal interpolation for smooth shading in `triangle.h`
- Updated `hittable.h` with transformation classes/object instancing
- Debugged `triangle.h` barycentric edge cases

---

## Week 8: (Nov. 3 - Nov. 9)
- Started `perlin.h` Perlin noise implementation with turbulence/noise based on RTW textbook
- Updated `texture.h` with noise_texture class
- Got marble texture working using Perlin noise
- Tested various noise scales and frequencies
- Started README file

---

## Week 9: (Nov. 10 - Nov. 16)
- Updated `bvh.h` with constant_medium class for volume rendering
- Updated `material.h` with isotropic material for volumes
- Volumetric mist implementation around main bubble
- Debugged volume rendering boundary calculations

---

## Week 10: (Nov. 17 - Nov. 23)
- Started `main.cpp` scene construction
- Created dual-layer bubble (outer glass + inner textured)
- Added supporting spheres (metal, sparkle-textured, marble/perlin noise)
- Adjusted camera positioning and orientation

---

## Week 11: (Nov. 24 - Nov. 30)
- Thanksgiving break, ate mac and cheese :D !

---

## Week 12: (Dec. 1 - Dec. 7)
- Started triangle meshes (ended up scrapping)
- Completed `main.cpp` with full scene
- Built 5-pointed star from textured triangles (pain and suffering)
- Added motion-blurred glowing sparkles
- Added parallelization rendering loop
- Uploaded rough draft of code to GitHub  
- Started writing report

---

## Week 13: (Dec. 8 - Dec. 14)
- Adjusted sparkle emission intensities
- Updated background and floor color in `main.cpp` for better visibility 
- Added `// REQUIREMENT:` comments throughout all files
- Finalized README
- Finalized report
- Uploaded final draft of code to GitHub  

---