This is a painting program. The player can cycle between paintbrush shapes, as well as increase and decrease size of the brush.
A minimum and maximum size will be set dependant on smallest recognizable drawing of each shape.
These are the shapes:
  square/rectangle
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
[x]     hold RT + A, draw with symmetry

    ROADBLOCK: SOLVED
        unsure how to implement symmetrical drawing.
    SOLUTION:
        determine necessary constraints, then work out formulas
    IMPLEMENTATION:
        check which, if any, quadrant resides entirely within
        for now, NO portion of brush can over hang or it wont draw at all
        start with square for simplicity, then add in circle
        
        
    QUADRANTS:
        1- top left
        2- top right
        3- bottom left
        4- bottom right
    SQUARE:
        in first quadrant:
            x min = 0, x max = 120 - brush size 
            y min = 0, y max = 80 - brush size

            paint at brush coordinate
            2nd quadrant: x = 240 - brush.x - brush.size, y = original
            3rd quadrant: x = original, y = 160 - brush.y - brush.size
            4th: x = 240 - brush.x - brush.size, y = 160 - brush.y - brush.size
        in second quadrant:
            x min = 120, x max 240 - brush size
            y min = 0, y max = 80 - brush size

            paint at brush coordinate
            1st: x = 240 - brush.x - brush size, y = original
            3rd: both flip
            4th: x = original, y = 160 - brush.y - brush.size

        in third quadrant:
            x min = 0, x max = 120 - brush size
            y min = 80, y max = 160 - brush size
        in fourth quadrant:
            x min = 120, x max = 240 - brush size
            y min = 80, y max = 160 - brush size

        CONCLUSION:
            to flip along X-axis, screen width - brush.x - brush.size
            to flip along Y-axis, screen height - brush.y - brush.size

Step 6:
    implement gradient drawing mode:
[x]  in COLORS or SHAPES modes, LT + RT goes to GRADIENTS
[x]  In GRADIENTS, LT goes to COLORS, RT goes to SHAPES
[x]  implement single gradiant variation in color selection
[x]     hold LT + A, draw with gradient
[x]  implement multiple gradient variations/patterns?
[x]  implement parameters in GRADIENTS mode:
    (first color is brush color, each successive color scaled for gradient)
    [x] red scaling 
    [x] blue scaling
    [x] green scaling
[x] implement preview of current gradient/shape/pattern

    ROADBLOCK: SOLVED
        handling gradient scaling
    SOLUTION:
        making decisions through trial and error once GUI is implemented
    IMPLEMENTATION:
        determine min/max scaler values (trial and error)
        decide if I want to loop back around when surpassing 31 color value, or keep at 31 for further colors
        implement inputs in GRADIENTS mode to select scaler and increment/decrement scaler value
        three stacks of vertical squares, one per scaler value per color scaler
        Left/Right to select scaler, A/B to cycle gradient patterns
    

Step 7: 
    optimizations
[x] half max brush size
[x] half cache size
[x] implement fillCircle function
[x] implement circle symmetry
[x] implement circle gradients
[x] refactor all state management into single function that is called once per switch case in main, handles inputs
[x] refactor headers/function definitions into multiple files

    ROADBLOCK: SOLVED
        Flickering cursor-brush not displaying clearly only near top of screen
    SOLUTION:
        assess all points of performance impairment one by once
        find catching point in handleBrushInput
        seems things aren't aligned with VBLANK
    IMPLEMENTATION:
        shortened loops in cache operations based on brush size.
        issue: this bug will likely return at larger brush size, asm will resolve

Step 8: 
    finalize basic features/refine GUI
DRAWING:

COLORS:
[x] spread color bars out evenly
[x]  align B/A between RGB, preview to right
[x] change background color (may refine further)

SHAPES:
[x] change background color
[x] when changing to circle, erase square and draw circle
[x] erase circle to background color and redraw on resize
[x] erase circle when changing to square
[x] erase square to background color on redraw
[] have shape check on input L or R

GRADIENTS:
[x] ensure proper number of bars representing scaling
[x] align third bar over blue scaler square
[x] fill unselected green/blue scaler squares with green/blue

    [] create fillSquare (or rect) function and replace wherever needed
    [] symmetry draw - ensure other squares fill in
    [] fill in COLORS demo squares
    [] all other places to use fillSquare

    may further refine colors in GUI

BASE APP FINISHED
----------------------------------------------------
    Additional Features

    FEATURE IDEA:
        Gradient toggle button, for two forms of gradient:
        Smooth (current circle gradient scaling)
Harsh (current square gradient scaling)
        4th selector, is two way toggle, up or down

    FEATURE IDEA:
        Gradient symmetry on holding LT && RT
        Will require major optimizations for circles 

    OPTIMIZATION IDEA:
        Current brushCache is actually the canvas cache (will rename) 
        Create brushCache in addition to canvas cache for gradients
        This will ensure fewer operations per draw, set cache in GUI

[] Gradient toggle (toggles how gradient scales)
[] diagonal gradients (new GRADIENT case)
[] reverse gradient directions (toggleable option in GRADIENTS)
[] Gradient symmetry


[] utilize VRAM_CACHE
[] assembly implementations for cache: memcopy32
[] asm circle drawing?  fillCircle and drawCircle
[] optimize square drawing

[] holding LT: preview gradient - this should be straightforward
[] holding RT: preview symmetry - this will require 3 additional canvas caches.

[] with improved cache restoriation, utilize larger cache for larger max brush size?
    [] calculate how much available space in VRAM mode 3

[] hexagon implementation

BUGLOG:
after refactoring handleBrush entirely to one function, may be an improper order of operations that only happens occasionally, sometimes big rectangular block of pixels gets shifted left by one pixel.
seems to happen when directions are held and A is tapped?


some square sizes have single empty pixel, implement better squareFill function (due to odd dimensions not being divisible by 2)

when reducing square size and unpausing, previous square size drawn on unpausing


[x] on clearing screen, the current brush position gets drawn
[x] no condition to unpause from COLORS mode
