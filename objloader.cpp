

#include <glm/glm.hpp>
#include <cstdio>
#include <string>
#include <vector>
#include "objloader.hpp"
#include <fstream>
#include <cstring>

bool loadeObj(const char *path, float **out_vertices, float **out_vnormals, float **out_textures,
                         float **out_normals, float **out_colors, int *verticesCount) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_textures;
    std::vector<glm::vec3> temp_normals;
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fclose(file);
        printf("Imposible to open the file!\n");
        return false;
    }
    bool end = false;
    while (!end) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) {
            end = true;
        } else {
            if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            } else if (strcmp(lineHeader, "vt") == 0) {
                glm::vec2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y);
                temp_textures.push_back(uv);
            } else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            } else if (strcmp(lineHeader, "f") == 0) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                     &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                     &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9) {
                    fclose(file);
                    printf("File can't be read by our simple parser");
                    return false;
                }
                for (int i = 0; i < 3; i++) {
                    vertexIndices.push_back(vertexIndex[i]);
                    uvIndices.push_back(uvIndex[i]);
                    normalIndices.push_back(normalIndex[i]);
                }
            }
        }
    }
    *out_vertices = new float[vertexIndices.size() * 4];
    *out_vnormals = new float[normalIndices.size() * 4];
    *out_textures = new float[uvIndices.size() * 2];
    *out_normals = new float[vertexIndices.size() * 4];
    *verticesCount = vertexIndices.size();
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        (*out_vertices)[4 * i] = vertex.x;
        (*out_vertices)[4 * i + 1] = vertex.y;
        (*out_vertices)[4 * i + 2] = vertex.z;
        (*out_vertices)[4 * i + 3] = 1.0f;
    }
    for (int i = 0; i < uvIndices.size(); i++) {
        unsigned int uv_index = uvIndices[i];
        glm::vec2 tex = temp_textures[uv_index - 1];
        (*out_textures)[2 * i] = tex.x;
        (*out_textures)[2 * i + 1] = (1.0f - tex.y);
    }
    for (int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[normalIndex - 1];
        (*out_vnormals)[4 * i] = normal.x;
        (*out_vnormals)[4 * i + 1] = normal.y;
        (*out_vnormals)[4 * i + 2] = normal.z;
        (*out_vnormals)[4 * i + 3] = 0.0f;
    }
    printf("Read ok v = %d n = %d\n", vertexIndices.size(), normalIndices.size());
    /* for(int i = 0; i < vertexIndices.size(); i ++) {
         printf("%ff, %ff, %ff, %ff,\n", out_normals[4 * i], out_normals[4 * i + 1], out_normals[4 * i + 2],
                out_normals[4 * i + 3]);
     }
    for (int i = 0; i < vertexIndices.size(); i++) {
        printf("%ff, %ff, %ff, %ff,\n", (*out_vertices)[4 * i], (*out_vertices)[4 * i + 1], (*out_vertices)[4 * i + 2],
               (*out_vertices)[4 * i + 3]);
    }
    for (int i = 0; i < vertexIndices.size(); i++) {
        printf("%ff, %ff,\n", (*out_textures)[2 * i], (*out_textures)[2 * i + 1]);
    }*/
    *out_colors = new float[vertexIndices.size() * 4];
    for (int i = 0; i < (int) vertexIndices.size(); i++) {
        (*out_normals)[4 * i] = 0.7f;
        (*out_normals)[4 * i + 1] = 0.5f;
        (*out_normals)[4 * i + 2] = 0.8f;
        (*out_normals)[4 * i + 3] = 0.9f;
        (*out_colors)[4 * i] = 1.0f;
        (*out_colors)[4 * i + 1] = 1.0f;
        (*out_colors)[4 * i + 2] = 1.0f;
        (*out_colors)[4 * i + 3] = 1.0f;
    }
    fclose(file);
    return true;
}
/*
void assignVBOtoAttribute(ShaderProgram *shaderProgram, const char *attributeName, GLuint bufVBO, int vertexSize) {
    GLuint location = shaderProgram->getAttribLocation(attributeName); //Pobierz numer slotu dla atrybutu
    glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Uaktywnij uchwyt VBO
    glEnableVertexAttribArray(location); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
    glVertexAttribPointer(location, vertexSize, GL_FLOAT, GL_FALSE, 0,
                          NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
}

GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
    GLuint handle;

    glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
    glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO
    glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicê do VBO

    return handle;
}
*/
