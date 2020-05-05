#pragma once
#include <Windows.h>

typedef struct Paint_Object
{
  int instrument;
  RGBTRIPLE figureColor, penColor;
  int penWidth,figureWidth,eraserWidth;
}PAINTOBJ;

PAINTOBJ GetInstrumentSettings();

HWND createMyWindow(LPCWSTR windowName, LPCWSTR szClassName, WNDPROC WndProcParam, HWND hWnd,RECT rect);