#include "Paint.h"

int PaintInstrumet(HDC hCdc,PAINTOBJ PO,POINT start,POINT firstHoldPoint,POINT secondHoldPoint)
{
  HPEN pen, hOldPen;
  HBRUSH brush, hOldBrush;

  if (PO.instrument == INSTRUMENT_PEN)
  {
    pen = CreatePen(PS_SOLID, PO.penWidth, RGB(PO.penColor.rgbtRed, PO.penColor.rgbtGreen, PO.penColor.rgbtBlue));
    hOldPen = SelectObject(hCdc, pen);

    MoveToEx(hCdc, firstHoldPoint.x, firstHoldPoint.y, NULL);
    LineTo(hCdc, secondHoldPoint.x, secondHoldPoint.y);
    SetPixel(hCdc, firstHoldPoint.x, firstHoldPoint.y, RGB(0, 0, 0));

    SelectObject(hCdc, hOldPen);
    DeleteObject(pen);
    
  }

  if (PO.instrument == INSTRUMENT_ERASER)
  {
    pen = CreatePen(PS_SOLID, PO.eraserWidth, RGB(255, 255, 255));
    hOldPen = SelectObject(hCdc, pen);

    MoveToEx(hCdc, firstHoldPoint.x, firstHoldPoint.y, NULL);
    LineTo(hCdc, secondHoldPoint.x, secondHoldPoint.y);
    SetPixel(hCdc, firstHoldPoint.x, firstHoldPoint.y, RGB(255, 255, 255));

    SelectObject(hCdc, hOldPen);
    DeleteObject(pen);
  }
  if (PO.instrument == INSTRUMENT_FILLER && start.x != -1 && start.y != -1)
  {
    brush = CreateSolidBrush(RGB(PO.penColor.rgbtRed, PO.penColor.rgbtGreen, PO.penColor.rgbtBlue));
    hOldBrush = SelectObject(hCdc, brush);

    ExtFloodFill(hCdc, start.x, start.y, GetPixel(hCdc, start.x, start.y), FLOODFILLSURFACE);

    SelectObject(hCdc, hOldBrush);
    DeleteObject(brush);
  }
}


int PaintFigure(HDC hCdc, PAINTOBJ PO, POINT start, POINT end)
{
  HPEN pen, hOldPen;

  pen = CreatePen(PS_SOLID, PO.figureWidth, RGB(PO.figureColor.rgbtRed, PO.figureColor.rgbtGreen, PO.figureColor.rgbtBlue));
  hOldPen = SelectObject(hCdc, pen);

  if (PO.instrument == FIGURE_CIRCLE)
    Ellipse(hCdc, min(start.x, end.x), min(start.y, end.y), max(start.x, end.x), max(start.y, end.y));

  if (PO.instrument == FIGURE_RECTANGLE)
    Rectangle(hCdc, min(start.x, end.x), min(start.y, end.y), max(start.x, end.x), max(start.y, end.y));

  if (PO.instrument == FIGURE_LINE)
  {
    MoveToEx(hCdc, start.x, start.y, NULL);
    LineTo(hCdc, end.x, end.y);
  }

  SelectObject(hCdc, hOldPen);
  DeleteObject(pen);
}