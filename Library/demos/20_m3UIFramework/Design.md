 GBA UI Framework Interactive Demo

## Project Overview
This program is an interactive demo of a dynamic menu system.
It starts with a player sprite and npc sprite on screen.
Facing the NPC and pressing A will initiate dialogue.
First time talking to NPC, player will be asked for a name.
This will initiate an character selection and input screen.
Subsequent interactions NPC will reference you by name.

On pressing START, dim background and open a menu
Menu should be centered in screen with list of options
UP and DOWN d-pad to select option
Highlight selected option

Menu options:
    Character (select new sprite)- A opens dedicated interface
    Movement speed slider
    Text Rendering Speed- slow, medium, fast
    invert green checkbox

Example Menu
 ------------------------------------
| Select Character                   |
| Movement speed    [--   ]          |
| Text Speed        slow medium fast |
| Invert green      [x]              |
|------------------------------------|

Pressing A on Select Character will open a grid system of animated sprites
Currently selected sprite will have highlighted grid cell border
Press B to return to Menu, or START to return to gameplay
Pressing A on option with slider will highlight slider to adjust
Pressing A or B will deselect slider
Pressing A on option with checkbox will toggle
Text speed will have brighter color for current speed
Pressing A when text speed selected will highlight selected speed
    Allowing selection of new speed


### Required Assets:
    Graphics
-[x] 4 direction sprite sheets for multiple characters (use robo sheet as 4 characters with color selection, smiley for one, and chocobo)
-[ ] Menu UI elements
  -[ ] Bordered frame (1 image each corner, vertical frame, horizontal frame, and internal frame)
  -[ ] Borderless frame (start only with one frame type, just swap assets)
  -[ ] Card UI (like smash bros character selection cards)
  -[ ] Slider bar ()
  -[ ] Checkbox
  -[ ] Highlight graphics?
  -[ ] New Font implementation?

--- 

## Implementation Phases

### Phase 1: 
    Base Game Environment
[x] Decide on assets to use (player, npc, background)
[x] Implement player sprite
[x] 4 directional movement/animation
[x] Player movement states: IDLE, MOVING
[x] Set up game state machine- PLAYING, PAUSED
[x] Switch to MENU on pressing START

### Phase 2: 
    Add new font
[x] Create new font sprite sheet
[x] Write program to scan bitmap and generate font data table

### Phase 3:
    Non interactive iteration of of PAUSE menu
[x] Draw menu list with options
[x] Optimize rendering pipeline with dirty flags
[x] Complete basic rendering pipeline with clear sprite frame
[x] Add highlighting to current selection, change on UP/DOWN
[ ] Set up sub menu states: one for each adjustable parameter

### Phase 4: 
    Implement Checkbox
[ ] Press A to highlight current speed, allowing selection
[ ] L/R dpad to change speed, A or B to return to menu item
[ ] Pressing A on Invert green will toggle checkbox

### Phase 5:
    Implement sliders
[ ] Draw slider for movement speed
[ ] Press A to highlight slider
[ ] When slider highlighted, L/R dpad to adjust value
[ ] Update movement speed

### Phase 6:
    Implement character selection    
[ ] Change to grid system when pressing A on Select Character
[ ] Ensure having 9 sets of animation sprite sheets
[ ] Current character will have background highlighted
[ ] Draw highlighted border to select new character
[ ] Press A to set player sprite to selected character
[ ] Press B to return to menu, START to return to gameplay

