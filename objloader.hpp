#ifndef objloader_hpp
#define objloader_hpp
#include <glm/glm.hpp>
#include <cstdio>
#include <string>
#include <vector>


bool loadeObj(const char *path, float **out_vertices, float **out_vnormals, float **out_textures,
                         float **out_normals, float **out_colors, int *verticesCount) ;

#endif
