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
[x] set upper/lower limits for RGB values, 
[x]        left|right cyle between RGB color value selector bars
[x]         use keyTapped for above
[x]        up|down increase/decrease value of current RGB selector bar
    
Step 4:
  implement brush shape feature:
[x]      on pressing Right Trigger enter shape selection mode 
[x]      on pressing Left Trigger enter color selection mode  
[x]      set gui background properly
[x]      up or down increases/decreases brush size.  default is a square.
[x] implement min/max sizes for default square brush.
[x] implement circle brush
[x] work out numbers for accurate placement of circle brush.
[x] implement circle sizing

    ROADBLOCK: SOLVED
        circle cache not behaving properly
    SOLUTION:
        work out math/numbers for square vs circle drawing 
        ensure cache save/restore operations succeed
    IMPLEMENTATION:
        for drawing circle, the  x y coordinates are the center of the circle,
        size is passed as the radius
        offsetting circle coordinates puts the brush 
        in the same pixel region as the SQUARE brush shape, 
        now gets along nicely with the cache.
    
Step 5:
  implement symmetry mode:
[] drawing in any quadrant while holding RT adds symmetry to all others.
Step 6:
    implement gradient drawing mode:
[]  implement single gradiant variation in color selection
[]  implement single gradient pattern in shape selection
[]  implement multiple gradient variations
[]  implement multiple gradient patterns per brush shape.

Step 7: 
    optimization time
[] implement fillCircle function
[] assembly implementations, memcopy/memset for cache?
[] utilize VRAM_CACHE
[] optimize cache save/restore operations, assembly?

