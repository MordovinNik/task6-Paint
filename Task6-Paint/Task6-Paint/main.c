#include <windows.h>
#include "FileSystem.h"
#include "Paint.h"
#pragma warning(disable:4996)

#define NEW_FILE              1001
#define OPEN_FILE             1002
#define SAVE_FILE             1003

int isKeyUp = 0;

struct Cursor_Points
{
  POINT btnDownPoint, firstMouseMovePoint, secondMouseMovePoint;//координаты на экране
  POINT newBtnDownPoint, newFirstMouseMovePoint, newSecondMouseMovePoint;//координаты на картинке
}CP;

struct ImagePaintInformation
{
  HBITMAP image;
  double zoom;
  POINT  offset;
  PAINTOBJ PO;
}IPI;

//////////////////////прототипы

LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);

void CreateMainMenu(HWND hWnd);

//////////////////////////////////

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
  CP.btnDownPoint = (POINT) { -1, -1 };
  CP.firstMouseMovePoint = (POINT) { -1, -1 };
  CP.secondMouseMovePoint = (POINT) { -1, -1 };

  IPI.image = NULL;
  IPI.offset = (POINT) { 0, 0 };
  IPI.PO = (PAINTOBJ) { 0, { 0, 0, 0 }, { 0, 0, 0 }, 0, 0 };
  IPI.zoom = 1;


  HWND hWnd = createMyWindow(L"\"Paint\"", L"Window1", WndProc, NULL, (RECT) { 100, 100, 1720, 880 });
  CreateMainMenu(hWnd);

  if (!hWnd)
    return 0;

  MSG msg;
  while (GetMessage(&msg, 0, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

  switch (msg)
  {
    case WM_COMMAND:
    
      CP.btnDownPoint.x = -1;
      CP.btnDownPoint.y = -1;
      switch (LOWORD(wParam))
      {
        case OPEN_FILE:

          OpenImage(&IPI.image);
          InvalidateRect(hWnd, NULL, TRUE);
          break;

        case SAVE_FILE:

          SaveImage(IPI.image);
          break;

        case INSTRUMENT_SETTINGS:

          IPI.PO = GetInstrumentSettings();
          break;
      }

      if (LOWORD(wParam) >= 1004 && LOWORD(wParam) <= 1009)
        IPI.PO.instrument = LOWORD(wParam);

      break;
    
    case WM_KEYDOWN:
      IPI.zoom += 1;
      InvalidateRect(hWnd, NULL, TRUE);
      break;

    case WM_LBUTTONDOWN:

      isKeyUp = 0;
      GetCursorPos(&CP.btnDownPoint);
      ScreenToClient(hWnd, &CP.btnDownPoint);
      CP.firstMouseMovePoint = CP.btnDownPoint;
      CP.secondMouseMovePoint = CP.btnDownPoint;
      InvalidateRect(hWnd, NULL, FALSE);
      break;

    case WM_MOUSEMOVE:

      if (CP.btnDownPoint.x != -1 && CP.btnDownPoint.y != -1)
        GetCursorPos(&CP.secondMouseMovePoint);

      ScreenToClient(hWnd, &CP.secondMouseMovePoint);
      InvalidateRect(hWnd, NULL, FALSE);

      break;

    case WM_LBUTTONUP:

      isKeyUp = 1;
      InvalidateRect(hWnd, NULL, FALSE);
      break;

    case WM_MOUSEWHEEL:

      if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        IPI.zoom *= 1.5;
      else
        IPI.zoom /= 1.5;
      InvalidateRect(hWnd, NULL, TRUE);
      break;


    case WM_DESTROY:
    
      PostQuitMessage(0);
      break;
    

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps), hCdc;
      BITMAP Bitmap;
      HBITMAP hOldBitmap;

      if (IPI.image)
      {
        GetObject(IPI.image, sizeof(BITMAP), &Bitmap);
        Bitmap.bmBitsPixel;
        hCdc = CreateCompatibleDC(hdc);
        hOldBitmap = SelectObject(hCdc, IPI.image);

        //переводим из экранных координат к координатам картинки

        CP.newSecondMouseMovePoint.x = CP.secondMouseMovePoint.x * IPI.zoom - IPI.offset.x;
        CP.newSecondMouseMovePoint.y = CP.secondMouseMovePoint.y * IPI.zoom - IPI.offset.y;
        CP.newFirstMouseMovePoint.x = CP.firstMouseMovePoint.x * IPI.zoom - IPI.offset.x;
        CP.newFirstMouseMovePoint.y = CP.firstMouseMovePoint.y * IPI.zoom - IPI.offset.y;

        if (!isKeyUp)
        {
          if (IPI.PO.instrument != INSTRUMENT_FILLER || CP.btnDownPoint.x != -1 && CP.btnDownPoint.y != -1)
          {
            CP.newBtnDownPoint.x = CP.btnDownPoint.x * IPI.zoom - IPI.offset.x;
            CP.newBtnDownPoint.y = CP.btnDownPoint.y * IPI.zoom - IPI.offset.y;
          }
          else
            CP.newBtnDownPoint = CP.btnDownPoint;

          PaintInstrumet(hCdc, IPI.PO, CP.newBtnDownPoint, CP.newFirstMouseMovePoint, CP.newSecondMouseMovePoint);

          CP.firstMouseMovePoint = CP.secondMouseMovePoint;

          if (IPI.PO.instrument == INSTRUMENT_FILLER)
          {
            CP.btnDownPoint.x = -1;
            CP.btnDownPoint.y = -1;
          }
        }
        else if (isKeyUp != 2 && CP.btnDownPoint.x != -1 && CP.btnDownPoint.y != -1)
        {
          CP.newBtnDownPoint.x = CP.btnDownPoint.x  * IPI.zoom - IPI.offset.x;
          CP.newBtnDownPoint.y = CP.btnDownPoint.y * IPI.zoom - IPI.offset.y;

          PaintFigure(hCdc, IPI.PO, CP.newBtnDownPoint, CP.newSecondMouseMovePoint);

          CP.firstMouseMovePoint = CP.secondMouseMovePoint;
          isKeyUp = 2;
        }

        StretchBlt(hdc, (int)(IPI.offset.x / IPI.zoom), (int)(IPI.offset.y / IPI.zoom), (int)(Bitmap.bmWidth / IPI.zoom), (int)(Bitmap.bmHeight / IPI.zoom), hCdc, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
        SelectObject(hCdc, hOldBitmap);
        DeleteDC(hCdc);
      }
      EndPaint(hWnd, &ps);
    }
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CreateMainMenu(HWND hWnd)
{
  HMENU hMenu = CreateMenu();
  HMENU hPopSaveMenu = CreatePopupMenu();
  HMENU hPopInstrumentMenu = CreatePopupMenu();
  HMENU hPopGeometryMenu = CreatePopupMenu();

  AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopSaveMenu, L"Ôàéë");

  AppendMenu(hPopSaveMenu, MF_STRING , NEW_FILE, L"Ñîçäàòü");
  AppendMenu(hPopSaveMenu, MF_STRING, OPEN_FILE, L"Îòêðûòü");
  AppendMenu(hPopSaveMenu, MF_STRING, SAVE_FILE, L"Ñîõðàíèòü");

  AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopInstrumentMenu, L"Èíñòðóìåíòû");

  AppendMenu(hPopInstrumentMenu, MF_STRING, INSTRUMENT_PEN, L"Ðó÷êà");
  AppendMenu(hPopInstrumentMenu, MF_STRING, INSTRUMENT_FILLER, L"Çàëèâêà");
  AppendMenu(hPopInstrumentMenu, MF_STRING, INSTRUMENT_ERASER, L"Ëàñòèê");
  AppendMenu(hPopInstrumentMenu, MF_STRING, INSTRUMENT_SETTINGS, L"Íàñòðîèòü èíñòðóìåíòû");

  AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopGeometryMenu, L"Ãåîìåòðè÷åñêèå ôèãóðû");

  AppendMenu(hPopGeometryMenu, MF_STRING, FIGURE_CIRCLE, L"Îêðóæíîñòü");
  AppendMenu(hPopGeometryMenu, MF_STRING, FIGURE_RECTANGLE, L"Ïðÿìîóãîëüíèê");
  AppendMenu(hPopGeometryMenu, MF_STRING, FIGURE_LINE, L"Ëèíèÿ");

  SetMenu(hWnd, hMenu);

}
