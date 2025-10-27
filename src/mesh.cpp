//
// Created by Oliver Ramos on 9/30/2025.
//
#include "mesh.h"

#include <iostream>
#include <ostream>

#include "stb_image.h"

void Mesh::createBuffers_(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,const std::vector<VertexAttribute> &attributes) {
    indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),indices.data(),GL_STATIC_DRAW);

    for (const auto& attr: attributes) {
        glVertexAttribPointer(attr.pos, attr.size, attr.type, attr.normalized, attr.stride, reinterpret_cast<const void*> (attr.offset));
        glEnableVertexAttribArray(attr.pos);
    }
    glBindVertexArray(0);
}

void Mesh::loadTexturesFromFile_(const std::vector<TextureSpec> &t) {
    textures = t;

    for(size_t i = 0; i < textures.size(); i++) {
        if(!textures[i].path.empty()) {
            stbi_set_flip_vertically_on_load(textures[i].flipVertical ? 1:0);
            int w = 0, h = 0, ch = 0;
            unsigned char* data = stbi_load(textures[i].path.c_str(), &w, &h, &ch,0);
            if(!data) {
                std::cerr << "Failed to load texture " << textures[i].path << std::endl;
                continue;
            }
            GLenum srcFormat = (ch == 4) ? GL_RGBA : GL_RGB;
            GLint internal = (ch == 4) ? GL_RGBA : GL_RGB;

            glGenTextures(1, &textures[i].TextureObj);
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + static_cast<GLenum>(i)));
            glBindTexture(GL_TEXTURE_2D, textures[i].TextureObj);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, internal, w, h, 0, srcFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
            glUniform1i(glGetUniformLocation(textures[i].shaderProgramID, textures[i].name.c_str()), textures[i].layout);
        }
    }
}

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
            const std::vector<VertexAttribute> &attributes,
            const std::vector<TextureSpec> &textures, const Transformation &M, const Transformation &V, const Transformation &P) { // update

    _M = M; _V = V; _P = P;

    _M.layout = glGetUniformLocation(_M.shaderProgramID, _M.name.c_str());
    glUniformMatrix4fv(_M.layout, 1, GL_FALSE, glm::value_ptr(_M.m));

    // #2
    _V.layout = glGetUniformLocation(_V.shaderProgramID, _V.name.c_str());
    glUniformMatrix4fv(_V.layout, 1, GL_FALSE, glm::value_ptr(_V.m));

    // #3
    _P.layout = glGetUniformLocation(_P.shaderProgramID, _P.name.c_str());
    glUniformMatrix4fv(_P.layout, 1, GL_FALSE, glm::value_ptr(_P.m));

    createBuffers_(vertices, indices, attributes);
    loadTexturesFromFile_(textures);
}

Mesh::~Mesh() {
    cleanup();
}

void Mesh:: draw(glm::mat4 m, glm::mat4 v, glm::mat4 p) const {
    bind();
    glUniformMatrix4fv(_M.layout, 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix4fv(_V.layout, 1, GL_FALSE, glm::value_ptr(v));
    glUniformMatrix4fv(_P.layout, 1, GL_FALSE, glm::value_ptr(p));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    unbind();
}

void Mesh::bind() const {
    glBindVertexArray(VAO);
}

void Mesh::unbind() const {
    glBindVertexArray(0);
}

void Mesh::cleanup() {
    for(auto& t:textures) {
        glDeleteTextures(1, &t.TextureObj);
    }
    textures.clear();
    if(VAO){glDeleteVertexArrays(1, &VAO); VAO = 0;}
    if(VBO){glDeleteBuffers(1, &VBO); VBO = 0;}
    if(EBO){glDeleteBuffers(1, &EBO); EBO = 0;}
}
