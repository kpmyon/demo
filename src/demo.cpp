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
#include <map>
#include <string>
#include <utility>
#include <vector>

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

std::vector<glm::vec3> vertexPositions;
std::vector<std::vector<size_t>> faceIndices;
std::string path = "default";
static char inputPath[InputLength] = "default";
// static char inputScale[InputLength] = "";
float inputScale = 1;
std::pmr::map<polyscope::SurfaceMesh*, float> initialScale;
std::pmr::map<polyscope::SurfaceMesh*, float> currScale;

void myCallback();
void newMesh();
void removeMesh();
void rotate(glm::vec3 axis, float angle);
void scale(const float scale);
void transform(const float angle, const glm::vec3 axis, const glm::vec3 move,
               const float scale, std::string name);

int main() {
  // Initialize Polyscope
  polyscope::init();
  polyscope::state::userCallback = myCallback;

  // meshGenerate(false, vertexPositions, faceIndices, path);

  auto* psMesh = polyscope::registerSurfaceMesh(getName(path), vertexPositions,
                                                faceIndices);
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

  ImGui::PushItemWidth(100);  // Make ui elements 100 pixels wide,
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
  if (ImGui::Button("removeMesh")) {
    // executes when button is pressed
    removeMesh();
  }

  ImGui::LabelText("rotate label", "rotate:");
  ImGui::SameLine();
  if (ImGui::Button("X")) {
    // executes when button is pressed
    rotate(X, 90.0);
  }
  ImGui::SameLine();
  if (ImGui::Button("Y")) {
    // executes when button is pressed
    rotate(Y, 90.0);
  }
  ImGui::SameLine();
  if (ImGui::Button("Z")) {
    // executes when button is pressed
    rotate(Z, 90.0);
  }
  if (ImGui::Button("scale")) {
    // executes when button is pressed
    scale(inputScale);
  }
  ImGui::SameLine();

  ImGui::SliderFloat(
      "scale value", &inputScale, 0.1f, 10.0f, "%.3f",
      ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat);

  ImGui::PopItemWidth();
}

/**
 * @brief 读取模型文件生成网格并渲染
 *
 */
void newMesh() {
  path = readFile();
  std::strcpy(inputPath, getName(path).c_str());
  if (path.empty()) {
    return;
  }

  vertexPositions.clear();
  faceIndices.clear();
  meshGenerate(true, vertexPositions, faceIndices, path);
  auto* psMesh = polyscope::registerSurfaceMesh(getName(path), vertexPositions,
                                             faceIndices);

  transform(0, X, -std::get<0>(psMesh->boundingBox()), 1, psMesh->getName());
  // psMesh->rescaleToUnit();
  polyscope::view::lookAt(glm::vec3(1.5, 0, 1), glm::vec3(0));
  initialScale.insert(std::pair<polyscope::SurfaceMesh*, float>(psMesh, 1.0));
  currScale.insert(std::pair<polyscope::SurfaceMesh*, float>(psMesh, 1.0));
}
/**
 * @brief 删除inputPath对应的网格
 *
 */
void removeMesh() { polyscope::removeSurfaceMesh(getName(inputPath)); }

void move(const glm::vec3 move) { transform(0, X, move, 1, getName(path)); }

void rotate(const glm::vec3 axis, const float angle) {
  transform(angle, axis, glm::vec3(0.0, 0.0, 0.0), 1, getName(path));
}
void scale(const float scale) {
  transform(0, X, glm::vec3(0.0, 0.0, 0.0), scale, getName(path));
}

/**
 * @brief 变换inputPath对应的网格的顶点坐标
 *
 * @param angle 旋转角度，使用角度制（大概）
 * @param axis 选择轴
 * @param move 平移向量
 * @param scale 相对于当前大小的放大系数
 * @param name 待变换模型名
 */
void transform(const float angle, const glm::vec3 axis, const glm::vec3 move,
               const float scale, const std::string name) {
  std::string inputPathStr(inputPath);
  if (!polyscope::hasSurfaceMesh(inputPathStr)) {
    return;
  }
  auto* psMesh = polyscope::getSurfaceMesh(inputPathStr);
  std::vector<glm::vec3> oldVertexPosition = psMesh->vertexPositions.data;
  std::vector<glm::vec3> newVertexPosition;

  std::for_each(vertexPositions.rbegin(), oldVertexPosition.rend(),
                [](glm::vec3& point) {
                  // TODO 遍历旧顶点，生成变换后的顶点,并更新currScale
                });

  psMesh->updateVertexPositions(newVertexPosition);
}
