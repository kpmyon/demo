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
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"


void meshGenerate(bool readFromFile, std::vector<glm::vec3>& vertexPositions,
                  std::vector<std::vector<size_t>>& faceIndices);
