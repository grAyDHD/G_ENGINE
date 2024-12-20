# GBA UI Framework Interactive Demo

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
-[ ] 4 direction sprite sheets for 9 characters
-[ ] Menu UI elements
  -[ ] Bordered frame
  -[ ] Borderless frame
  -[ ] Grid UI
  -[ ] Slider bar
  -[ ] Checkbox
  -[ ] Highlight graphics?
  -[ ] New Font implementation?

--- 

## Implementation Phases

### Phase 1: 
    Base Game Environment
[ ] Decide on assets to use
[ ] Implement player sprite
[ ] 4 directional movement/animation
[ ] Static NPC
[ ] Basic collision detection
[ ] Implement interaction detection zone
[ ] Set up state machine- SPEAKING, IDLE, MOVEMENT, MENU
[ ] Switch to SPEAKING on interaction with NPC
[ ] Switch to MENU on pressing START

### Phase 2: 
    Add new font
[ ] Create new font sprite sheet
[ ] Write program to scan bitmap and generate font data table

### Phase 2:
    NPC interactions
[ ] Add graphics for text box
[ ] Show sprite of speaking character
[ ] First time talking w/NPC, have NPC ask for name
[ ] Implement character input box for player to enter name 
[ ] Subsequent NPC interactions will remember your name

### Phase 3:
    Non interactive iteration of of PAUSE menu
[ ] on pressing start, darken screen
[ ] Draw menu list with options
[ ] Add highlighting to current selection, change on UP/DOWN

### Phase 4: 
    Implement character selection    
[ ] Change to grid system when pressing A on Select Character
[ ] Ensure having 9 sets of animation sprite sheets
[ ] Current character will have background highlighted
[ ] Draw highlighted border to select new character
[ ] Press A to set player sprite to selected character
[ ] Press B to return to menu, START to return to gameplay

### Phase 5:
    Implement sliders
[ ] Draw slider for movement speed
[ ] Press A to highlight slider
[ ] When slider highlighted, L/R dpad to adjust value
[ ] Update movement speed

### Phase 6:
    Add final menu parameter
[ ] Current text speed brighter color than others
[ ] Press A to highlight current speed, allowing selection
[ ] L/R dpad to change speed, A or B to return to menu item
[ ] Pressing A on Invert green will toggle checkbox

