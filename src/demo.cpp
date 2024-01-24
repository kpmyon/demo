#include <Shlobj.h>
#include <minwindef.h>
#include <stringapiset.h>
#include <windows.h>
#include <winerror.h>
#include <winnls.h>
#include <winnt.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
#include "meshGen.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/view.h"
#include "readfile.h"

#define InputLength 128
#define X glm::vec3(1.0, 0.0, 0.0)
#define Y glm::vec3(0.0, 1.0, 0.0)
#define Z glm::vec3(0.0, 0.0, 1.0)

std::vector<glm::vec3>           vertexPositions;
std::vector<std::vector<size_t>> faceIndices;
std::string                      path                     = "default";
static char                      inputPath[ InputLength ] = "default";
// static char inputScale[InputLength] = "";
float                             inputScale = 1;
float                             inputAngle = 90;
std::pmr::map<std::string, float> initialScale;
std::pmr::map<std::string, float> currScale;
static float                      moveX  = 0;
static float                      moveY  = 0;
static float                      moveZ  = 0;
static float                      moveX0 = 0;
static float                      moveY0 = 0;
static float                      moveZ0 = 0;

void                             myCallback();
void                             newMesh();
void                             removeMesh();
void                             move(const glm::vec3 move);
void                             rotate(glm::vec3 axis, float angle);
void                             scale(const float scale);
std::vector<glm::vec3>           transform(const float            angle,
                                           const glm::vec3        axis,
                                           const glm::vec3        move,
                                           const float            scale,
                                           const std::string     &name,
                                           std::vector<glm::vec3> oldVertexPosition);
std::vector<glm::vec3>           transform(const float     angle,
                                           const glm::vec3 axis,
                                           const glm::vec3 move,
                                           const float     scale,
                                           std::string     name);
std::tuple<glm::vec3, glm::vec3> getBoundingBox(std::vector<glm::vec3> vPositions);

int main() {
    // Initialize Polyscope
    polyscope::init();
    polyscope::state::userCallback = myCallback;

    // meshGenerate(false, vertexPositions, faceIndices, path);

    auto *psMesh = polyscope::registerSurfaceMesh(getName(path), vertexPositions, faceIndices);
    // Show the gui
    polyscope::show();
}

/**
 * @brief 注册UI组件的回调函数
 *
 *
 */
void myCallback() {
    // Since options::openImGuiWindowForUserCallback == true by default,
    // we can immediately start using ImGui commands to build a UI

    ImGui::PushItemWidth(100); // Make ui elements 100 pixels wide,
                               // instead of full width. Must have
                               // matching PopItemWidth() below.

    ImGui::InputText("mesh", inputPath, InputLength);

    if (ImGui::Button("reloadMesh")) {
        // executes when button is pressed
        removeMesh();
        newMesh();
    }
    ImGui::SameLine();
    if (ImGui::Button("newMesh")) {
        // executes when button is pressed
        if (path == "default") {
            polyscope::removeSurfaceMesh(getName("default"));
        }
        newMesh();
    }
    ImGui::SameLine();
    if (ImGui::Button("removeMesh")) {
        // executes when button is pressed
        removeMesh();
    }

    ImGui::Text("*rotate:");
    ImGui::SameLine();
    if (ImGui::Button("X")) {
        // executes when button is pressed
        rotate(X, inputAngle);
    }
    ImGui::SameLine();
    if (ImGui::Button("Y")) {
        // executes when button is pressed
        rotate(Y, inputAngle);
    }
    ImGui::SameLine();
    if (ImGui::Button("Z")) {
        // executes when button is pressed
        rotate(Z, inputAngle);
    }
    ImGui::SameLine();
    ImGui::InputFloat("rotate angle", &inputAngle);

    if (ImGui::SliderFloat("scale value",
                           &inputScale,
                           0.1,
                           10.0,
                           "%.3f",
                           ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
        // executes when button is pressed
        scale(inputScale);
    }

    if (ImGui::SliderFloat("moveX",
                           &moveX,
                           -10.0,
                           10.0,
                           "%.3f",
                           ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
        float deltaX = moveX - moveX0;
        moveX0       = moveX;
        move(glm::vec3(deltaX, 0, 0));
    }

    if (ImGui::SliderFloat("moveY",
                           &moveY,
                           -10.0,
                           10.0,
                           "%.3f",
                           ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
        float deltaY = moveY - moveY0;
        moveY0       = moveY;
        move(glm::vec3(0, deltaY, 0));
    }

    if (ImGui::SliderFloat("moveZ",
                           &moveZ,
                           -10.0,
                           10.0,
                           "%.3f",
                           ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
        float deltaZ = moveZ - moveZ0;
        moveZ0       = moveZ;
        move(glm::vec3(0, 0, deltaZ));
    }

    ImGui::PopItemWidth();
}

/**
 * @brief 读取模型文件生成网格并渲染
 *
 */
void newMesh() {
    path             = readFile();
    std::string name = getName(path);
    std::strcpy(inputPath, name.c_str());
    if (path.empty()) {
        return;
    }

    vertexPositions.clear();
    faceIndices.clear();
    meshGenerate(true, vertexPositions, faceIndices, path);

    initialScale.insert(std::pair<std::string, float>(name, 1.0));
    currScale.insert(std::pair<std::string, float>(name, 1.0));
    auto  boundingBox = getBoundingBox(vertexPositions);
    float lengthX     = std::get<1>(boundingBox).x - std::get<0>(boundingBox).x;
    float lengthY     = std::get<1>(boundingBox).y - std::get<0>(boundingBox).y;
    float lengthZ     = std::get<1>(boundingBox).z - std::get<0>(boundingBox).z;
    float length      = std::max(lengthX, lengthY);
    length            = std::max(length, lengthZ);
    vertexPositions   = transform(0, X, glm::vec3(0), 1.0 / length, name, vertexPositions);
    currScale[ name ] = 1.0; /*
       vertexPositions = transform(0, X, glm::vec3(0) - std::get<0>(boundingBox),
                                   1.0, name, vertexPositions);*/
    boundingBox = getBoundingBox(vertexPositions);

    // psMesh->rescaleToUnit();
    auto *psMesh = polyscope::registerSurfaceMesh(name, vertexPositions, faceIndices);
    polyscope::view::lookAt(glm::vec3(2.5, 2, 0), glm::vec3(0));
}
/**
 * @brief 删除inputPath对应的网格
 *
 */
void removeMesh() {
    polyscope::removeSurfaceMesh(getName(inputPath));
    initialScale.erase(getName(inputPath));
    currScale.erase(getName(inputPath));
}

void move(const glm::vec3 move) {
    polyscope::getSurfaceMesh(getName(inputPath))
        ->updateVertexPositions(transform(0, X, move, 1, getName(inputPath)));
}

void rotate(const glm::vec3 axis, const float angle) {
    polyscope::getSurfaceMesh(getName(inputPath))
        ->updateVertexPositions(
            transform(angle, axis, glm::vec3(0.0, 0.0, 0.0), 1, getName(inputPath)));
}
void scale(const float scale) {
    polyscope::getSurfaceMesh(getName(inputPath))
        ->updateVertexPositions(transform(0, X, glm::vec3(0.0, 0.0, 0.0), scale, getName(path)));
}

/**
 * @brief 变换网格的顶点坐标
 *
 * @param angle 旋转角度，使用角度制（大概）
 * @param axis 选择轴
 * @param move 平移向量
 * @param scale 相对于当前大小的放大系数
 * @param name 待变换模型名
 * @param oldVertexPosition 变换的模型的顶点
 */
std::vector<glm::vec3> transform(const float            angle,
                                 const glm::vec3        axis,
                                 const glm::vec3        move,
                                 const float            scale,
                                 const std::string     &name,
                                 std::vector<glm::vec3> oldVertexPosition) {
    // std::string inputPathStr(inputPath);
    //  std::string name = getName(inputPathStr);
    // auto* psMesh = polyscope::getSurfaceMesh(name);
    // std::vector<glm::vec3> oldVertexPosition = psMesh->vertexPositions.data;
    std::vector<glm::vec3> newVertexPosition;

    glm::mat4x4 transform = glm::mat4x4(1.0);
    transform             = glm::translate(transform, move);
    transform             = glm::rotate(transform, angle, axis);
    float newScale =
        scale * static_cast<float>(initialScale[ name ]) / static_cast<float>(currScale[ name ]);
    transform         = glm::scale(transform, glm::vec3(newScale));
    currScale[ name ] = newScale * currScale[ name ];
    std::for_each(oldVertexPosition.begin(),
                  oldVertexPosition.end(),
                  [ &newVertexPosition, &transform ](glm::vec3 point) {
                      glm::vec4 newVec = glm::vec4(point, 1.0);
                      newVertexPosition.emplace_back(transform * newVec);
                  });

    // psMesh->updateVertexPositions(newVertexPosition);
    return newVertexPosition;
}
std::vector<glm::vec3> transform(const float     angle,
                                 const glm::vec3 axis,
                                 const glm::vec3 move,
                                 const float     scale,
                                 std::string     name) {
    auto psMesh = polyscope::getSurfaceMesh(name);
    return transform(angle, axis, move, scale, name, psMesh->vertexPositions.data);
}
std::tuple<glm::vec3, glm::vec3> getBoundingBox(std::vector<glm::vec3> vPositions) {
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = -FLT_MAX;
    float maxY = -FLT_MAX;
    float maxZ = -FLT_MAX;

    std::for_each(vPositions.begin(), vPositions.end(), [ & ](glm::vec3 point) {
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
