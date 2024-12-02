Character animation demo, focused on player input/movement, npc mevement behavior, and player/npc collision handling.  Jump and attack animations optional on completion of base project.upon basic implementation
//layout of major steps, with checklists of substeps
Step 1:
    set up state machines for player character and NPC:
    PLAYER:
    Player animations- 4 direction (idle, move, jump, action).
[]
    NPC:
    Npc animations- 2 directions up/down (move)
[]

Step 2:
    Start with npc for simplicity due to lack of input.
    
    REQUIREMENTS:
    move up animation
    move down animation
    2 frames per step, 4 frame animations. 
    moving 2 squares in a row will cycle all 4 frames

Step 3:
 Implement player character with movement NO animation

Feature Checklist
Core Features

Player movement (D-pad).
NPC movement with basic AI (linear path).

    Collision detection between player and NPC.

Future Extensions

Add Mode-specific effects (e.g., bitmap effects in Mode 3, palette swapping in Mode 4).

    Extend to interactable environments (e.g., platforms, obstacles).

Roadblocks and Solutions
Potential Roadblock 1: Animations misaligned with movement

Solution: Ensure animations sync with movement states by using a centralized state handler.
Potential Roadblock 2: Inefficient frame rendering

Solution: Optimize sprite drawing logic using assembly for frequently updated areas.
Implementation Steps
Step 1: Movement Logic

Implement D-pad movement for the player.
Define a Character struct to manage position, velocity, and state.

    Add bounds to prevent movement off-screen.

Step 2: Animations

Add frame-based animations for idle, movement, jump, and action states.

    Create a timer to control frame switching.

Step 3: NPC Logic

Add NPC movement (up/down linear path).

    Implement collision detection with the player.

Step 4: Mode 3-Specific Effects

    Add a visual effect during the action animation (e.g., screen ripple or blur).

Step 5: Debugging and Optimizations

Use GDB to test animation timing and memory allocation.

    Optimize sprite rendering for performance.

Mode Transition Logic
State	Trigger	Description
Idle	No input	Player remains stationary.
Move	D-pad input	Player moves in the input direction, animation updates accordingly.
Jump	Press A	Player performs a jump animation.
Action	Press B	Player performs an action animation, triggers special Mode 3 effect.
Debugging Notes

    Test movement responsiveness using GDB and breakpoints in movement logic.
    Validate animation frames with on-screen debug overlay.
    Use logging to verify collision logic.

Bug Log
Bug	Cause	Status	Resolution
Animation frame stuck in idle state	Incorrect state transition logic.	Pending	Debug state_update function.
NPC overlaps player on collision	Missing collision resolution logic.	Pending	Implement pushback on collision.
