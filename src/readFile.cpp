#include "readfile.h"
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
  HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL,
                                CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
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
        hr = pShellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING,
                                        &pszFilePath);
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