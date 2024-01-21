#include "readfile.h"

/**
 * @brief 将宽类型字符串转化为string
 *
 * @param lpcwszStr 待转化字符串
 * @return std::string 转化后的字符串
 */
std::string WCharToMByte(LPCWSTR lpcwszStr) {
  std::string str;
  DWORD dwMinSize = 0;
  LPSTR lpszStr = nullptr;
  dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, nullptr, 0,
                                  nullptr, FALSE);
  if (0 == dwMinSize) {
    return FALSE;
  }
  lpszStr = new char[dwMinSize];
  WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, lpszStr, dwMinSize,
                      nullptr, FALSE);
  str = lpszStr;
  delete[] lpszStr;
  return str;
}

/**
 * @brief 弹出选择模型文件的窗体，并返回选择的文件的路径
 *
 * @param pFilePath
 * @return std::string 返回的文件的路径
 */
std::string readFile() {
  std::string path;
  // TODO 未完成
  CoInitialize(nullptr);
  IFileDialog* pfd = nullptr;
  HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr,
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
          // MessageBoxW(nullptr, pszFilePath, L"File Path", MB_OK);
          path = WCharToMByte(pszFilePath);
          CoTaskMemFree(pszFilePath);
        }
      }
      pShellItem->Release();
    }
    pfd->Release();
  }

  return path;
}