#ifndef CLASS_TEMPLTE_HPP
#define CLASS_TEMPLTE_HPP


#include <GLFW/glfw3.h>
#include "class_template.hpp"

class Template
{   //definiujemy tablice na wierzcho³ki, normalne, textury itp
    protected:
        const char * path;
        int vertexCount;
        float *vertices;
        float *normals;
        float *vertexNormals;
        float *texCoords;
        float *colors;

    public:
        Template(char * path);
        void drawSolid();

};
#endif

