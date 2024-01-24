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

#include "polyscope/simple_triangle_mesh.h"
#include "simpleMesh.hpp"
#include <vector>
void meshGenerate(bool readFromFile, SimpleMesh &mesh, std::string path);
