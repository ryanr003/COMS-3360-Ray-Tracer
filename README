# I have been Traced for Good: A very Wicked Ray Tracer!
# By: Ryan Riebesehl
# COMS 3360

Fully renderable ray tracer in C++ inspired by Glindas pink bubble from the musical and movie: *Wicked*.

---

## Requirements Met

All requirements implemented with `// REQUIREMENT:` comments throughout the code:

**Core Features (Top List from Piazza):**
- Camera with configurable position, orientation, FOV (`camera.h`)
- Anti-aliasing (`camera.h`)
- Ray/sphere intersections (`sphere.h`)
- Ray/triangle intersections (`triangle.h`)
- Texture loading from files (`texture.h`)
- Textured spheres and triangles (`sphere.h`, `triangle.h`)
- BVH spatial subdivision acceleration structure (`bvh.h`)
- Diffuse, specular, dielectric materials (`material.h`)
- Emissive materials/lights (`material.h`)

**Addational Features (85 extra pts):**
- High dynamic range (10 pts) - `color.h`
- Volume rendering (10 pts) - `bvh.h` 
- Quads (10 pts) - `quad.h`
- Motion blur (10 pts) - `ray.h`, `sphere.h`
- Defocus blur/DOF (10 pts) - `camera.h` 
- Object instancing (10 pts) - `main.cpp`
- Perlin noise (10 pts) - `perlin.h`
- Parallelization (10 pts) - `camera.h` 
- Normal interpolation (5 pts) - `triangle.h` 

## File Structure

```
RayTracer/
├── README                # For explaining all detials of ray tracer
├── CHANGELOG             # Full log of changes made throughout the semester
├── main.cpp              # Scene setup
├── makefile              # Build configuration
├── wicked.h              # Utilities, constants
├── vec3.h                # 3D vector math
├── ray.h                 # Ray with time component
├── color.h               # HDR gamma correction
├── interval.h            # Intervals
├── aabb.h                # Bounding boxes
├── hittable.h            # Intersection interface
├── hittable_list.h       # Scene container
├── sphere.h              # Sphere primitives
├── triangle.h            # Triangles with smooth shading
├── quad.h                # Quad 
├── bvh.h                 # BVH + volume rendering
├── camera.h              # Camera + parallelization
├── material.h            # All material types
├── texture.h             # Textures
├── perlin.h              # Perlin noise
├── external/
│   ├── stb_image.h       # Image loading file (3rd party library)
│   └── inspo.webp        # Insperation image 
└── textures/
    ├── pink_gradient.jpg # Used on main sphere in the center
    └── sparkle.png       # Used on far right sphere
```

## Building and Running

### On Pyrite (Iowa State server)
```bash
ssh <netid>@pyrite.cs.iastate.edu
tar -xvzf RayTracer.tar.gz
cd path/to/RayTracer
make
make render      # Will generate output.ppm
```

### Local Build
```bash
make              # Compile code
make render       # Render
make clean        # Clean files
```

## Scene Description

The scene includes:
- Main, dual-layer bubble (glass outer, textured inner) 
- Motion-blurred glowing sparkles (lights)
- 5-pointed star from textured triangles
- Metal sphere (small one in front), sparkle-textured sphere (far right), Perlin noise sphere (far left)
- Volumetric mist in main bubble, emerald checker ground
- Dark gold platform platform quad

## Configuration

If you want, adjust in `main.cpp`:
- `cam.image_width` - Resolution (default: 800)
- `cam.samples_per_pixel` - Quality (default: 200)
- `cam.max_depth` - Ray bounces (default: 50)
- `cam.vfov` - Field of view (default: 40 degrees)
- `cam.defocus_angle` - DOF strength

**Expected render time:** It takes 30 sec. - 1 min. on Pyrite for me

## Credits

**Implementation References:**
1. Peter Shirley, Trevor D. Black, and Steve Hollasch, Ray Tracing in One Weekend: The Book Series, https://raytracing.github.io/
2. Matt Pharr, Wenzel Jakob, and Greg Humphreys, Physically Based Rendering: From Theory to Implementation, 4th edition, https://pbr-book.org/

**Inspiration:** *Wicked & Wicked: For Good* (movie and broadway musical)

**3rd Party Libraries:** stb_image.h by Sean Barrett (in external)
