#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <minwindef.h>
#include <sstream>
#include <stringapiset.h>
#include <vector>

#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"

#include <Shlobj.h>
#include <cstring>
#include <iostream>
#include <string>
#include <windows.h>
#include <winerror.h>
#include <winnls.h>
#include <winnt.h>


void meshGenerate(bool readFromFile, std::vector<glm::vec3>& vertexPositions,
                  std::vector<std::vector<size_t>>& faceIndices);
void defaultMeshGenerate(std::vector<glm::vec3>& vertexPositions, std::vector<std::vector<size_t>>& faceIndices);
void string2faceInfo(std::string string, int& v, int& vt, int& vn);
std::string readFile(LPWSTR& pFilePath);


int main() {
  // Initialize Polyscope
  polyscope::init();
  std::vector<glm::vec3> vertexPositions;
  std::vector<std::vector<size_t>> faceIndices;

  meshGenerate(true, vertexPositions, faceIndices);

  auto* psMesh = polyscope::registerSurfaceMesh("simple triangle", vertexPositions, faceIndices);
  // Show the gui
  polyscope::show();
}
/**
 * @brief 读取模型，生成坐标和面的信息
 *
 * @param readFromFile 为true时会从指定路径读取模型
 * @param vertexPositions 读取出来的顶点坐标信息
 * @param faceIndices 读取出来的面信息
 */
void meshGenerate(bool readFromFile, std::vector<glm::vec3>& vertexPositions,
                  std::vector<std::vector<size_t>>& faceIndices) {

  if (!readFromFile) {
    defaultMeshGenerate(vertexPositions, faceIndices);
    return;
  }

  // TODO 修改path的获取方式
  std::string path = "E:/source/instrinsic-simplification/intrinsic-simplification/meshes/resource/shan.obj";

  if (path.empty()) {
    std::cout << "path is empty"
              << "\n";
    return;
  }

  std::ifstream fin;
  fin.open(path, std::ios::in);
  if (!fin.is_open()) {
    std::cout << "cannot open the file"
              << "\n";
    return;
  }

  std::string buf;
  std::vector<std::string> slice;
  std::istringstream istringstream;
  int line = 1;
  // 循环读取文件的每一行到buf中，并进行处理直至文件末尾
  while (std::getline(fin, buf)) {
    if (buf.empty()) {
      continue;
    }
    line++;
    slice.clear();
    istringstream.clear();
    istringstream.str(buf);
    std::string t;
    std::string s0;

    // 对于读取到的每一行，进行拆分
    while (std::getline(istringstream, t, ' ')) {
      if (t.empty()) {
        continue;
      }
      slice.push_back(t);
    }

    // 具体处理每一行
    if (slice.front() == "v") {
      glm::vec3 vertex;
      istringstream.clear();
      istringstream.str(buf.substr(1));
      istringstream >> vertex.x >> vertex.y >> vertex.z;
      vertexPositions.emplace_back(vertex);
    } else if (slice.front() == "f") {
      std::vector<size_t> triangle;
      int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
      string2faceInfo(slice.at(1), v1, vt1, vn1);
      string2faceInfo(slice.at(2), v2, vt2, vn2);
      string2faceInfo(slice.at(3), v3, vt3, vn3);
      // std::cout << v1 << v2 << v3 << "\n";
      //  TODO 处理vt和vn的索引
      triangle.clear();
      triangle.emplace_back(v1 - 1);
      triangle.emplace_back(v2 - 1);
      triangle.emplace_back(v3 - 1);
      faceIndices.emplace_back(triangle);
    } else if (slice.front() == "vt") {
      // TODO
    } else if (slice.front() == "vn") {
      // TODO
    } else if (slice.front().at(0) == '#') {
      continue;
    }
  }
  fin.close();
}
void defaultMeshGenerate(std::vector<glm::vec3>& vertexPositions, std::vector<std::vector<size_t>>& faceIndices) {
  glm::vec3 p;

  p[0] = 0.0;
  p[1] = 0.0;
  p[2] = 0.0;
  vertexPositions.emplace_back(p);

  p[0] = 0.0;
  p[1] = 1.0;
  p[2] = 0.0;
  vertexPositions.emplace_back(p);

  p[0] = 2.0;
  p[1] = 0.0;
  p[2] = 0.0;
  vertexPositions.emplace_back(p);

  std::vector<size_t> tri;

  tri.clear();
  tri.emplace_back(0);
  tri.emplace_back(1);
  tri.emplace_back(2);

  faceIndices.emplace_back(tri);
}
/**
 * @brief 辅助解析obj文件中关于面的行
 *
 * @param string 待解析的行
 * @param v 解析出来的顶点坐标的索引
 * @param vt 解析出来的顶点纹理坐标的索引
 * @param vn 解析出来的顶点法线坐标的索引
 */
void string2faceInfo(std::string string, int& v, int& vt, int& vn) {
  std::istringstream istringstream;
  std::string::size_type pos = 0;

  if (string.find("//") == std::string::npos) {
    if (string.find("/") == std::string::npos) {
      // 解析f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3这种形式，此时传入的string相当于v/vt/vn
      while ((pos = string.find("/")) != std::string::npos) {
        string.replace(pos, 1, " ");
      }
      istringstream.clear();
      istringstream.str(string);
      istringstream >> v >> vt >> vn;
    } else {
      // 解析f v1 v2 v3这种形式，此时传入的string相当于v
      istringstream.clear();
      istringstream.str(string);
      istringstream >> v;
      vt = NULL;
      vn = NULL;
    }
  } else {
    // 解析f v1//vn1 v2//vn2 v3//vn3这种形式，此时传入的string相当于v//vn
    string.replace(0, 2, " ");
    istringstream.clear();
    istringstream.str(string);
    istringstream >> v >> vn;
    vt = NULL;
  }
}
/**
 * @brief 弹出选择模型文件的窗体，并返回选择的文件的路径
 *
 * @param pFilePath
 * @return std::string 返回的文件的路径
 */
std::string readFile(LPWSTR& pFilePath) {
  std::string path;
  // TODO 未完成
  CoInitialize(nullptr);
  IFileDialog* pfd = nullptr;
  HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
  if (SUCCEEDED(hr)) {
    DWORD dwFlags = 0;
    hr = pfd->GetOptions(&dwFlags);
    hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | FOS_ALLOWMULTISELECT);
    std::array<COMDLG_FILTERSPEC, 1> fileType = {
        // TODO add more types
        {L"Obj files", L"*.obj*"},
    };
    hr = pfd->SetFileTypes(ARRAYSIZE(fileType), fileType.data());
    hr = pfd->SetFileTypeIndex(1);

    hr = pfd->Show(nullptr);

    if (SUCCEEDED(hr)) {
      IShellItem* pShellItem = nullptr;
      hr = pfd->GetResult(&pShellItem);
      if (SUCCEEDED(hr)) {
        LPWSTR pszFilePath = nullptr;
        hr = pShellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath);
        if (SUCCEEDED(hr)) {
          // 读取到的路径存在pszFilePath中
          MessageBoxW(nullptr, pszFilePath, L"File Path", MB_OK);
          CoTaskMemFree(pszFilePath);
        }
      }
      pShellItem->Release();
    }
    pfd->Release();
  }

  return path;
}