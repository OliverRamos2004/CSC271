//
// Created by Oliver Ramos on 9/25/2025.
//

#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "glad/glad.h"
#include "cstddef"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct VertexAttribute {
    GLuint pos;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    size_t offset;
};

struct TextureSpec {
    std::string name; // HERE
    unsigned int TextureObj;
    GLuint shaderProgramID;
    GLint layout;
    std::string path;
    bool flipVertical = false;
};

struct Transformation {
    std::string name;
    GLuint shaderProgramID;
    GLint layout;
    glm::mat4 m;


};
class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<VertexAttribute>& attributes,
        const std::vector<TextureSpec>& textures,
        // const Transformation & trans,
        const Transformation & M,
        const Transformation & V,
        const Transformation & P);

    ~Mesh();

    void draw(glm::mat4 M, glm::mat4 V, glm::mat4 P ) const;
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
    Transformation _M, _V, _P;

};



#endif //MESH_H
