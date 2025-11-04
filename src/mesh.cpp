// Created by dengq on 8/29/25.
//
#include "mesh.h"
#include "stb_image.h"
#include <iostream>
#include "tiny_obj_loader.h"
#include <unordered_set>

// Vertex layout: [px,py,pz, nx,ny,nz, u,v]

void Mesh::createBuffers_()
{

    const GLint posLoc = glGetAttribLocation(shaderProgramID, "aPos");
    const GLint uvLoc  = glGetAttribLocation(shaderProgramID, "aTexCoord");

    GLsizei stride = 5 * sizeof(float);
    std::vector<VertexAttribute> attributes = {
            VertexAttribute{ static_cast<GLuint>(posLoc), 3, GL_FLOAT, GL_FALSE, stride, 0 },
            VertexAttribute{ static_cast<GLuint>(uvLoc),  2, GL_FLOAT, GL_FALSE, stride, 3 * sizeof(float) }};

    indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
                 indices.data(),
                 GL_STATIC_DRAW);

    for (const auto& attr : attributes) {
        glVertexAttribPointer(attr.pos,
                              attr.size,
                              attr.type,
                              attr.normalized,
                              attr.stride,
                              reinterpret_cast<const void*>(attr.offset));
        glEnableVertexAttribArray(attr.pos);
    }

    glBindVertexArray(0);
}

Mesh::Mesh(std::vector<float> v, std::vector<unsigned int> idx, GLuint id){
    shaderProgramID = id;
    vertices = v;
    indices = idx;

    createBuffers_();
}

Mesh::Mesh(const std::string& path, GLuint id){
    shaderProgramID = id;
    loadOBJ_(path);
    createBuffers_();
}

Mesh::~Mesh() {
    cleanup();
}

void Mesh::draw() const{
    bind();
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
    if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
    if (VBO) { glDeleteBuffers(1, &VBO); VBO = 0; }
    if (EBO) { glDeleteBuffers(1, &EBO); EBO = 0; }
}

bool Mesh::loadOBJ_(const std::string& path)
{
    vertices.clear();
    indices.clear();

    tinyobj::ObjReaderConfig config;
    config.triangulate = true;   // let tinyobj make all faces triangles
    config.vertex_color = false;

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path, config)) {
        if (!reader.Error().empty())
            std::cerr << "tinyobj error: " << reader.Error() << "\n";
        return false;
    }
    if (!reader.Warning().empty()) {
        std::cerr << "tinyobj warning: " << reader.Warning() << "\n";
    }

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();

    auto getPos = [&](int vi) -> glm::vec3 {
        const float* v = attrib.vertices.data() + 3 * vi;
        return glm::vec3(v[0], v[1], v[2]);
    };

    auto getUV = [&](int ti) -> glm::vec2 {
        const float* t = attrib.texcoords.data() + 2 * ti;
        return glm::vec2(t[0], t[1]);
    };

    uint32_t baseIndex = 0;

    for (const auto& shape : shapes) {
        const auto& mesh = shape.mesh;

        size_t index_offset = 0;
        for (size_t f = 0; f < mesh.num_face_vertices.size(); ++f) {
            int fv = mesh.num_face_vertices[f];
            if (fv != 3) {                // should be 3 due to triangulate=true
                index_offset += fv;
                continue;
            }

            // emit 3 vertices (pos + uv only)
            for (int v = 0; v < 3; ++v) {
                const tinyobj::index_t idx = mesh.indices[index_offset + v];

                // position
                const glm::vec3 p = getPos(idx.vertex_index);
                vertices.push_back(p.x);
                vertices.push_back(p.y);
                vertices.push_back(p.z);

                // uv (default 0,0 if missing)
                float u = 0.0f, vv = 0.0f;
                if (idx.texcoord_index >= 0 &&
                    static_cast<size_t>(2 * idx.texcoord_index + 1) < attrib.texcoords.size())
                {
                    const glm::vec2 uv = getUV(idx.texcoord_index);
                    u = uv.x; vv = uv.y;
                }
                vertices.push_back(u);
                vertices.push_back(vv);

                indices.push_back(baseIndex++);
            }

            index_offset += fv;
        }
    }

    return !vertices.empty() && !indices.empty();
}
