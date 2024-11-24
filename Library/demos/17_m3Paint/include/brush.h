#ifndef BRUSH_H
#define BRUSH_H

#include "types.h"

void paint(Brush brush);
void erase(Brush brush);
void updateBrushPosition(Brush *brush);
Brush initiateBrush();
void handleBrushInput(Brush *brush);

void paintGradient(Brush brush);
void symmetryPaint(Brush brush);

#endif
