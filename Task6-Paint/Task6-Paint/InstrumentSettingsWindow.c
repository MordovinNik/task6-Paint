#include "InstrumentSettingsWindow.h"

#define SAVE_SETTINGS 1011

struct Text_Boxes
{
  HWND penColor[3], figureColor[3];
  HWND penLineWidth, figureLineWidth, eraserWidth;
}TB;

PAINTOBJ PO;

LRESULT CALLBACK WndProc2(HWND, UINT, UINT, LONG);

PAINTOBJ GetInstrumentSettings(HWND hParentWnd)
{
  HWND hWnd = createMyWindow(L"Settings", L"window2", WndProc2, NULL, (RECT) {200,200,400,260});

  TB.penColor[0] = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 170, 10, 60, 20, hWnd, NULL, NULL, NULL);
  TB.penColor[1] = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 240, 10, 60, 20, hWnd, NULL, NULL, NULL);
  TB.penColor[2] = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 310, 10, 60 ,20, hWnd, NULL, NULL, NULL);
  
  TB.penLineWidth = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 170, 40, 60, 20, hWnd, NULL, NULL, NULL);
  
  TB.eraserWidth = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 170, 130, 60, 20, hWnd, NULL, NULL, NULL);

  TB.figureColor[0] = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 170, 70, 60, 20, hWnd, NULL, NULL, NULL);
  TB.figureColor[1] = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 240, 70, 60, 20, hWnd, NULL, NULL, NULL);
  TB.figureColor[2] = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 310, 70, 60, 20, hWnd, NULL, NULL, NULL);
  
  TB.figureLineWidth = CreateWindow(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 170, 100, 60, 20, hWnd, NULL, NULL, NULL);

  HWND button1 = CreateWindow(L"BUTTON", L"—охранить", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 130, 170, 80, 20, hWnd, SAVE_SETTINGS, NULL, NULL);

  MSG msg;
  while (GetMessage(&msg, 0, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return PO;
}

ATOM registerMyClass(HINSTANCE hInstance, LPCWSTR szClassName, WNDPROC WndProcParam)
{
  WNDCLASS wc;
  wc.cbClsExtra = wc.cbWndExtra = 0;
  wc.style = 0;
  wc.lpfnWndProc = WndProcParam;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = szClassName;

  return RegisterClass(&wc);
}

HWND createMyWindow(LPCWSTR windowName, LPCWSTR szClassName, WNDPROC WndProcParam, HWND hParentWindow,RECT rect)
{
  registerMyClass(NULL, szClassName, WndProcParam);

  HWND hWnd = CreateWindow(szClassName, windowName, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, hParentWindow, NULL, NULL, NULL);

  if (!hWnd) { return 0; }
  ShowWindow(hWnd, 1);
  UpdateWindow(hWnd);
  return hWnd;
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_DESTROY:
      PostQuitMessage(0);
    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
        case SAVE_SETTINGS:
        {
          WCHAR line[20];
          int number = 0;

          GetWindowText(TB.penColor[0], line,4);
          PO.penColor.rgbtRed = _wtoi(line);
          GetWindowText(TB.penColor[1], line, 4);
          PO.penColor.rgbtGreen = _wtoi(line);
          GetWindowText(TB.penColor[2], line, 4);
          PO.penColor.rgbtBlue = _wtoi(line);

          GetWindowText(TB.figureColor[0], line, 4);
          PO.figureColor.rgbtRed = _wtoi(line);
          GetWindowText(TB.figureColor[1], line, 4);
          PO.figureColor.rgbtGreen = _wtoi(line);
          GetWindowText(TB.figureColor[2], line, 4);
          PO.figureColor.rgbtBlue = _wtoi(line);

          GetWindowText(TB.penLineWidth, line, 20);
          PO.penWidth = _wtoi(line);
          GetWindowText(TB.figureLineWidth, line, 20);
          PO.figureWidth = _wtoi(line);
          GetWindowText(TB.eraserWidth, line, 20);
          PO.eraserWidth = _wtoi(line);

          DestroyWindow(hWnd);
        }
      }
    }

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);

      TextOut(hdc, 10, 10, L"÷вет инструмента:", 17);
      TextOut(hdc, 10, 40, L"Ўирина линии ручки:", 20);
      TextOut(hdc, 10, 70, L"÷вет фигуры:", 13);
      TextOut(hdc, 10, 100, L"Ўирина линии фигуры:", 21);
      TextOut(hdc, 10, 130, L"Ўирина ластика:", 16);

      EndPaint(hWnd, &ps);
    }
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}
