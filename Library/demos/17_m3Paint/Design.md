This is a painting program. The player can cycle between paintbrush shapes, as well as increase and decrease size of the brush.
A minimum and maximum size will be set dependant on smallest recognizable drawing of each shape.
These are the shapes:
  square/rectangle
  triangle 
  circle
  hexagon
I'll use asesprite to test draw various sizes for each of these.
The player can choose between a solid and a gradient, as well as select color with RGB sliders ranging 0-31 each 
In color selection mode, there is given a selection of gradients based upon the chosen RGB value
In shape selection mode, there are multiple gradient patterns per shape.
Meaning, higher speed, that color cycles through it's gradient range faster.


Step 1:
  set up state handling for following tasks:
    while drawing:
[x]      hold A + directions, draw
[x]      hold B + directions, erase
[]     hold RT + A, draw with gradient?
[x]     hold LT + RT + A + B, clear screen

Step 2:
[x]  Set starting parameters for drawing and implement basic drawing and erasing with a 16x16 square (use a pattern?)

    ROADBLOCK: RESOLVED!
        when not drawing OR erasing, cursor draws over old value.
    SOLUTION:
        preserve a cache for values within the cursor.
    IMPLEMENTATION:
        create CursorCache struct
            Coordinate, size, shape, colors
        before drawing cursor, save pixels to cache
        draw cursor to screen
    IF CURSOR POSITION CHANGE IS DETECTED:
        copy from cursor cache to screen BEFORE cursor updated
        update cursor
        draw brush value to the screen.
    MEMORY USE:
        Mode 3 screen takes ~70kb vram, leaving plenty of space for a cache

Step 3:
  implement color slider selectors.

[x] on START change to COLOR mode;


[x] save screen to cache, size of color select gui
[x] draw region on screen for color select gui
[x] draw 3 bars, outline selected in white, else black
[x] underneath 3 bars show 3 colors, B to set eraser and A to set brush. 3rd is RGB value.
    while paused:
      in RGB color selection mode:
[] set upper/lower limits for RGB values, 
[x]        left|right cyle between RGB color value selector bars
[]         use keyTapped for above
[x]        up|down increase/decrease value of current RGB selector bar
    
Step 4:
  implement brush shape feature:
[]      on pressing Right Trigger enter shape selection mode 
[]      on pressing Left Trigger enter color selection mode  
[]      up or down increases/decreases brush size.  default is a square.
[] implement min/max sizes for default square brush.
[] implement circle brush, set min/max sizes.
Step 5:
  implement 3-5 gradiant variations in color selection
  implement 3-5 gradient patterns in shape selection
Step 6:
Step 7:
  implement symmetry mode:
[] drawing in any quadrant adds symmetry to all others.
