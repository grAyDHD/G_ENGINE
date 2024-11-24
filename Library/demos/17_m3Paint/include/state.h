#ifndef STATE_H
#define STATE_H

#include "types.h"

void saveToBrushCache(Brush brush);
void saveToGUICache();
void restoreFromBrushCache(Brush brush);
void restoreFromGUICache();

enum MODE handlePause();
enum MODE changeState(enum MODE appState, Brush *brush);
void changeBrushShape(Brush *brush);
void changeBrushSize(Brush *brush);
void adjustColorValue(enum COLOR colorSelect, int *red, int *green, int *blue,
                      int increase);

enum COLOR handleColorSelection(enum COLOR colorSelect);
void manageGradientType(Brush *brush);

void handleGradientControls(enum COLOR colorSelect, Brush *brush);

void simpleWait(int delay);

#endif
