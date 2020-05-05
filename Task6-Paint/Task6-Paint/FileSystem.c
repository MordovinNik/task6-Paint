#include "FileSystem.h"

BOOLEAN OpenImage(HBITMAP *hBm)
{
  OPENFILENAME ofn;

  WCHAR fileName[260];

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = fileName;
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(fileName);
  ofn.lpstrFilter = L"BMP\0*.BMP\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  GetOpenFileName(&ofn);

  *hBm = LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

//getfolderpath
//getsavefilename

void SaveImage(HBITMAP image)
{
  OPENFILENAME ofn;
  WCHAR fileName[260];
  
  BITMAP BM;
  BITMAPFILEHEADER BFH;
  LPBITMAPINFO BIP;
  HDC DC;
  LPBYTE Buf;
  DWORD ColorSize, DataSize;
  WORD BitCount;
  HANDLE FP;
  DWORD dwTemp;

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = fileName;
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(fileName);
  ofn.lpstrFilter = L"BMP\0*.BMP\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST;
  GetSaveFileName(&ofn);

  GetObject(image, sizeof(BITMAP), (LPSTR)&BM);

  BitCount = (WORD)BM.bmPlanes * BM.bmBitsPixel;

  if (BitCount > 8)
    ColorSize = 0;
  else
    ColorSize = sizeof(RGBQUAD) * (1 << BitCount);

  DataSize = (BM.bmWidth * BitCount + 15) / 8 * BM.bmHeight;

  BIP = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + ColorSize);

  BIP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  BIP->bmiHeader.biWidth = BM.bmWidth;
  BIP->bmiHeader.biHeight = BM.bmHeight;
  BIP->bmiHeader.biPlanes = 1;
  BIP->bmiHeader.biBitCount = BitCount;
  BIP->bmiHeader.biCompression = 0;
  BIP->bmiHeader.biSizeImage = DataSize;
  BIP->bmiHeader.biXPelsPerMeter = 0;
  BIP->bmiHeader.biYPelsPerMeter = 0;

  if (ColorSize)
    BIP->bmiHeader.biClrUsed = (1 << BitCount);
  else
    BIP->bmiHeader.biClrUsed = 0;

  BIP->bmiHeader.biClrImportant = 0;

  BFH.bfType = 0x4d42;
  BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + ColorSize;
  BFH.bfReserved1 = 0;
  BFH.bfReserved2 = 0;
  BFH.bfSize = BFH.bfOffBits + DataSize;

  Buf = (LPBYTE)malloc(DataSize);

  DC = GetDC(0);
  GetDIBits(DC, image, 0, (WORD)BM.bmHeight, Buf, BIP, DIB_RGB_COLORS);
  ReleaseDC(0, DC);

  FP = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  WriteFile(FP, &BFH, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);
  WriteFile(FP, BIP, sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed * sizeof(RGBQUAD), &dwTemp, NULL);
  WriteFile(FP, Buf, DataSize, &dwTemp, NULL);

  CloseHandle(FP);
  free(Buf);
  free(BIP);

  return FALSE;
}
