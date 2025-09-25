//
// Created by Oliver Ramos on 9/25/2025.
//

#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "glad/glad.h"
#include "cstddef"

struct VetexAttribute {
    GLuint pos;
    Glint size;
    Glenum type;
    Glboolean normalized;
    glsizei stride;
    size_t offset;
};

struct TextureSpec {
    std::string name;
    unisigned int TextureObj;
    GLuint shaderProgramID;
    GLint layout;
    std::string path;
    bool flipVertical = false;
};
class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<VertexAttribute>& attributes,
        const std::vector<TextureSpec>& textures);
    ~Mesh();

    void draw() const;
    void bind() const;
    void unbind() const;
    void cleanup();


private:
    void createBuffers_(
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<VertexAttribute>& attributes
        );
    void loadTexturesFromFile_(const std::vector<TextureSpec>& textures);

    GLuint VAO=0, VBO=0, EBO=0;
    GLsizei indexCount=0;
    std::vector<TextureSpec> textures;

};



#endif //MESH_H
