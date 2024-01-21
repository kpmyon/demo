#include <Shlobj.h>
#include <minwindef.h>
#include <stringapiset.h>
#include <windows.h>
#include <winerror.h>
#include <winnls.h>
#include <winnt.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "meshGen.h"
#include "polyscope/camera_view.h"
#include "polyscope/options.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "readfile.h"

std::vector<glm::vec3> vertexPositions;
std::vector<std::vector<size_t>> faceIndices;
std::string path = readFile();

void myCallback();

int main() {
  // Initialize Polyscope
  polyscope::init();
  polyscope::state::userCallback = myCallback;
  // polyscope::options::autocenterStructures = true;
  // polyscope::options::autoscaleStructures = true;

  meshGenerate(true, vertexPositions, faceIndices, path);

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

  if (ImGui::Button("reloadMesh")) {
    // executes when button is pressed
    polyscope::removeSurfaceMesh(getName(path));
    path = readFile();
    vertexPositions.clear();
    faceIndices.clear();
    meshGenerate(true, vertexPositions, faceIndices, path);
    polyscope::registerSurfaceMesh(getName(path), vertexPositions, faceIndices);
    polyscope::view::flyToHomeView();
  }

  ImGui::PopItemWidth();
}