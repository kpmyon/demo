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

#include "polyscope/polyscope.h"

#include "polyscope/surface_mesh.h"

#include "readfile.h"


int main() {
  // Initialize Polyscope
  polyscope::init();
  std::vector<glm::vec3> vertexPositions;
  std::vector<std::vector<size_t>> faceIndices;

  meshGenerate(true, vertexPositions, faceIndices);

  auto* psMesh = polyscope::registerSurfaceMesh("simple triangle",
                                                vertexPositions, faceIndices);
  // Show the gui
  polyscope::show();
}