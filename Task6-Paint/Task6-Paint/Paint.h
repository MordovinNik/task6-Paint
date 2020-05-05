#pragma once
#include "InstrumentSettingsWindow.h"

#define INSTRUMENT_PEN        1004
#define INSTRUMENT_FILLER     1005
#define INSTRUMENT_ERASER     1006
#define FIGURE_CIRCLE         1007
#define FIGURE_RECTANGLE      1008
#define FIGURE_LINE           1009
#define INSTRUMENT_SETTINGS   1010

int PaintFigure(HDC hCdc, PAINTOBJ PO, POINT start, POINT end);

int PaintInstrumet(HDC hCdc, PAINTOBJ PO, POINT start, POINT firstHoldPoint, POINT secondHoldPoint);
