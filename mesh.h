#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "hittable.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class mesh {
public:
    static std::vector<std::shared_ptr<hittable>> load_obj(
        const std::string& filename, 
        std::shared_ptr<material> mat
    ) {
        std::vector<std::shared_ptr<hittable>> triangles;
        std::vector<point3> vertices;
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open OBJ file: " << filename << std::endl;
            return triangles;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            
            if (prefix == "v") {
                double x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(point3(x, y, z));
            }
            else if (prefix == "f") {
                // Face (supports both "f 1 2 3" and "f 1/1/1 2/2/2 3/3/3" formats)
                std::vector<int> indices;
                std::string vertex_str;
                
                while (iss >> vertex_str) {
                    // Parse vertex index
                    size_t slash_pos = vertex_str.find('/');
                    std::string index_str = (slash_pos != std::string::npos) 
                        ? vertex_str.substr(0, slash_pos) 
                        : vertex_str;
                    int idx = std::stoi(index_str) - 1; 
                    indices.push_back(idx);
                }
                
                // Triangulate face
                for (size_t i = 1; i + 1 < indices.size(); i++) {
                    if (indices[0] >= 0 && indices[0] < vertices.size() &&
                        indices[i] >= 0 && indices[i] < vertices.size() &&
                        indices[i+1] >= 0 && indices[i+1] < vertices.size()) {
                        triangles.push_back(std::make_shared<triangle>(
                            vertices[indices[0]],
                            vertices[indices[i]],
                            vertices[indices[i+1]],
                            mat
                        ));
                    }
                }
            }
        }
        
        file.close();
        std::cout << "Loaded " << vertices.size() << " vertices and " 
                  << triangles.size() << " triangles from " << filename << std::endl;
        
        return triangles;
    }
};

#endif