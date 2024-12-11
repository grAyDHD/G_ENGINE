Character animation demo, focused on player input/movement, npc mevement behavior, and player/npc collision handling.
Dialouge on A when facing NPC
//layout of major steps, with checklists of substeps
Step 1: 
    Start with background and npc for simplicity due to lack of input.

    REQUIREMENTS:
    move up animation
    move down animation
    2 frames per step, 4 frame animations. 
    moving 2 squares in a row will cycle all 4 frames

[x] draw background
[x] adapt sprite frame function to handle 32 bit sprites
[x] draw NPC
[x] implement function to refill sprite background
[] move NPC on timer- right right left left
[] update NPC sprite direction on direction change






Step 2:
 Implement player character with movement
[] draw player Character
[] update position on d pad input
[] update sprite direction to match

Step 3:
    set up state machines for player character and NPC:
    Player animations- 4 direction (idle, move, jump, action).
    Npc animations- 2 directions up/down (move)
[] set up PLAYER state machine
[] set up NPC state machine
[] set up didrection state machine

[] animate NPC, 2 frames per single square of movement
[] animate player

Step 4: 
 implement interactions
[] player npc collision handling
[] press A to talk to NPC (NPC saves current direction to turn to player, turn back after dialogue)
