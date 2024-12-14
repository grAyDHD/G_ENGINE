Character animation demo, focused on player input/movement, npc mevement behavior, and player/npc collision handling.
Dialouge on A when facing NPC
//layout of major steps, with checklists of substeps
Step 1: 
    Start with background and npc for simplicity due to lack of input.


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
[] update position on d pad input
[] update sprite direction to match


[] animate NPC, 2 frames per single square of movement
[] animate player

Step 4: 
 implement interactions
[] player npc collision handling
[] press A to talk to NPC (NPC saves current direction to turn to player, turn back after dialogue)

    ROADBLOCK: 
