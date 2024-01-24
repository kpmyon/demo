#include "simpleMesh.hpp"
#include <string>
/*
std::vector<glm::vec3> const &SimpleMesh::GetVertexPositions() {
    return this->vertexPositions;
}
std::string const &SimpleMesh::GetName() {
    return this->name;
}
std::vector<glm::vec2> const &SimpleMesh::GetTexturePositions() {
    return this->texturePositions;
}
std::vector<glm::vec3> const &SimpleMesh::GetNormalPositions() {
    return this->normalPositions;
}
std::vector<std::vector<size_t>> const &SimpleMesh::GetFaceIndices() {
    return this->faceIndices;
}
float const &SimpleMesh::GetInitialScale() {
    return this->initialScale;
}
float const &SimpleMesh::GetCurrScale() {
    return this->currScale;
}
void SimpleMesh::SetName(const std::string &r_name) {
    this->name = r_name;
}
void SimpleMesh::SetVertexPositions(std::vector<glm::vec3> const &r_vertexPositions) {
    this->vertexPositions = r_vertexPositions;
}
void SimpleMesh::SetfaceIndices(std::vector<std::vector<size_t>> const &r_faceIndices) {
    this->faceIndices = r_faceIndices;
}
void SimpleMesh::SetTexturePositions(std::vector<glm::vec2> const &r_texturePositions) {
    this->texturePositions = r_texturePositions;
}
void SimpleMesh::SetNormalPositions(std::vector<glm::vec3> const &r_normalPositions) {
    this->normalPositions = r_normalPositions;
}
void SimpleMesh::SetInitialScale(float const &r_initialScale) {
    this->initialScale = r_initialScale;
}
void SimpleMesh::SetCurrScale(float const &r_currScale) {
    this->currScale = r_currScale;
}
*/
std::tuple<glm::vec3, glm::vec3> SimpleMesh::getBoundingBox() {
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = -FLT_MAX;
    float maxY = -FLT_MAX;
    float maxZ = -FLT_MAX;

    std::for_each(
        this->vertexPositions.begin(), this->vertexPositions.end(), [ & ](glm::vec3 point) {
            if (minX > point.x) {
                minX = point.x;
            }
            if (minY > point.y) {
                minY = point.y;
            }
            if (minZ > point.z) {
                minZ = point.z;
            }
            if (maxX < point.x) {
                maxX = point.x;
            }
            if (maxY < point.y) {
                maxY = point.y;
            }
            if (maxZ < point.z) {
                maxZ = point.z;
            }
        });

    return std::make_tuple<glm::vec3, glm::vec3>({minX, minY, minZ}, {maxX, maxY, maxZ});
}