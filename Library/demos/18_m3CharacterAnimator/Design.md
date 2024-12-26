Character animation demo, focused on player input/movement, npc mevement behavior, and player/npc collision handling.
Dialouge on A when facing NPC
//layout of major steps, with checklists of substeps
Step 1: 
    Start with background and npc for simplicity
[x] draw background
[x] adapt sprite frame function to handle 32 bit sprites
[x] draw NPC
[x] implement function to refill sprite background
[x] move NPC right
[x] update NPC sprite direction on direction change
[x] move NPC left

Step 2:
[x] set up Character structs that handle sprites

Step 3:
 Implement player character with movement
[x] draw player Character
[x] update position on d pad input
[x] update sprite direction to match
[x] until idle animation is created, default to frame 0

    ROADBLOCK:
        when sprite regions overlap, background layer of last sprite drawn shows over sprite
    Solution:
        collision check with bounding box for edges of two sprite regions.
        if overlap, callunique function checking transparency pixels in overlapped region
        manage outer region (transparency pixels) and inner region (character)

Step 4: refactor sprite drawing funciton to handle restoreFrameBackground

Step 5: 
 implement interactions
[x] player npc collision handling
[] refine collision bounds
[] press A to talk to NPC (NPC saves current direction to turn to player, turn back after dialogue)

Step 6:
    Move to Entity Component System
[ ] create entity struct, component list struct, and relevant component structs
[ ] update assembly function
[ ] update sprite sheets with all states
[ ] final update for assembly function


    OPTIMIZATIONS:  
        Merge SpriteFrame3Bit with restoreFrameBackground
