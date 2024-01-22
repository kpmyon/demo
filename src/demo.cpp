
#include <Shlobj.h>
#include <minwindef.h>
#include <stringapiset.h>
#include <windows.h>
#include <winerror.h>
#include <winnls.h>
#include <winnt.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#include "imgui.h"
#include "meshGen.h"
#include "polyscope/camera_view.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "readfile.h"

#define InputLength 128
#define X glm::vec3(1.0, 0.0, 0.0)
#define Y glm::vec3(0.0, 1.0, 0.0)
#define Z glm::vec3(0.0, 0.0, 1.0)

std::vector<glm::vec3> vertexPositions;
std::vector<std::vector<size_t>> faceIndices;
std::string path = "default";
static char inputPath[InputLength] = "";
// static char inputScale[InputLength] = "";
float inputScale = 1;
std::pmr::map<polyscope::SurfaceMesh*, float> initialScale;
std::pmr::map<polyscope::SurfaceMesh*, float> currScale;

void myCallback();
void newMesh();
void rotate(glm::vec3 axis, float angle);
void scale(const float scale);
void transform(const float angle, const glm::vec3 axis, const glm::vec3 move,
               const float scale);

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

// Your callback functions
void myCallback() {
  // Since options::openImGuiWindowForUserCallback == true by default,
  // we can immediately start using ImGui commands to build a UI

  ImGui::PushItemWidth(100);  // Make ui elements 100 pixels wide,
                              // instead of full width. Must have
                              // matching PopItemWidth() below.

  ImGui::InputText("mesh", inputPath, InputLength);

  // ImGui::InputText("scale", inputScale, InputLength);
  /*
  ImGui::SliderFloat(
      "scale", &inputScale, 0.1f, 10.0f, "%.5f",
      ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat);
      */
  if (ImGui::Button("reloadMesh")) {
    // executes when button is pressed
    polyscope::removeSurfaceMesh(getName(path));
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
  /*
  if (ImGui::Button("reset scale")) {
    // executes when button is pressed
    std::string inputPathStr(inputPath);
    auto* psMesh = polyscope::getSurfaceMesh(inputPathStr);
    psMesh->rescaleToUnit();
    polyscope::view::lookAt(glm::vec3(1.5, 1.5, 3), glm::vec3(0.0, 0.0, 0.0));
  }
  */
  /*
  ImGui::SameLine();
  if (ImGui::InputFloat("scale value", &inputScale)) {
  }*/

  ImGui::SliderFloat(
      "scale value", &inputScale, 0.1f, 10.0f, "%.3f",
      ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat);

  ImGui::PopItemWidth();
}
void newMesh() {
  path = readFile();
  std::strcpy(inputPath, getName(path).c_str());
  if (path.empty()) {
    return;
  }
  vertexPositions.clear();
  faceIndices.clear();
  meshGenerate(true, vertexPositions, faceIndices, path);
  polyscope::registerSurfaceMesh(getName(path), vertexPositions, faceIndices);
  auto* psMesh = polyscope::getSurfaceMesh(getName(path));
  psMesh->centerBoundingBox();
  psMesh->rescaleToUnit();
  polyscope::view::flyToHomeView();
  initialScale.insert(std::pair<polyscope::SurfaceMesh*, float>(psMesh, 1.0));
  currScale.insert(std::pair<polyscope::SurfaceMesh*, float>(psMesh, 1.0));
}
void move(const glm::vec3 move) {
  transform(0,X,move,1);
}
void rotate(const glm::vec3 axis, const float angle) {
  transform(angle, axis, glm::vec3(0.0, 0.0, 0.0),1);
}
void scale(const float scale) {
  transform(0,X,glm::vec3(0.0,0.0,0.0),scale);
}
/**
 * @brief 变换inputPath对应的网格的顶点坐标
 * 
 * @param angle 旋转角度，使用角度制（大概）
 * @param axis 选择轴
 * @param move 平移向量
 * @param scale 缩放值
 */
void transform(const float angle, const glm::vec3 axis, const glm::vec3 move,
               const float scale) {
  std::string inputPathStr(inputPath);
  if (!polyscope::hasSurfaceMesh(inputPathStr)) {
    return;
  }
  auto* psMesh = polyscope::getSurfaceMesh(inputPathStr);
  std::vector<glm::vec3> oldVertexPosition = psMesh->vertexPositions.data;
  std::vector<glm::vec3> newVertexPosition;

  std::for_each(vertexPositions.rbegin(), oldVertexPosition.rend(),
                [](glm::vec3& point) {
                  // TODO 遍历旧顶点，生成变换后的顶点
                });

  psMesh->updateVertexPositions(newVertexPosition);
}
