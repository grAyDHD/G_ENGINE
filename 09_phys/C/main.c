#include "engine.h"
//objectives:
//make sure jump isn't continuos on btn hold
//bounmce whn hitting walls
//three platforms- one bouncy, one giving jump boost, one cant jump from
//
#define GRAVITY 1       // Gravity constant
#define SW 240          // Screen width
#define SH 160          // Screen height
#define BALL_SIZE 4     // Size of the ball
#define MOVE_SPEED 2    // Speed of movement when pressing left/right
#define SHOOT_VEL 5     // Initial velocity when shooting with bumpers
#define JUMP_VELOCITY -10

struct Object {
    int x;
    int y;
    int vx;
    int vy;
    int ax;
    int ay;
    int onGround;
};

int gravityDirection = 1;

void applyGravity(struct Object* obj) {
    obj->ay = GRAVITY * gravityDirection;
}

void updateMovement(struct Object* obj) {
    obj->vx += obj->ax;
    obj->vy += obj->ay;
    obj->x += obj->vx;
    obj->y += obj->vy;
}

void handleCollisions(struct Object* obj) {
    if (gravityDirection == 1) { 
        if (obj->y >= SH - BALL_SIZE) {
            obj->y = SH - BALL_SIZE;        
            obj->vy = -(obj->vy / 2);       
            obj->onGround = 1;              
        } else if (obj->y <= 0) {           
            obj->y = 0;
            obj->vy = 0;                    
            obj->onGround = 0;              
        } else {
            obj->onGround = 0;              
        }
    }

    else if (gravityDirection == -1) {        
        if (obj->y <= 0) {                    
            obj->y = 0;
            obj->vy = -(obj->vy / 2);         
            obj->onGround = 1;                
        } else if (obj->y >= SH - BALL_SIZE) {
            obj->y = SH - BALL_SIZE;
            obj->vy = 0;                  
            obj->onGround = 0;            
        } else {
            obj->onGround = 0;            
        }
    }

    if (obj->x < 0) {
        obj->x = 0;
        obj->vx = 0;                      
    }
    if (obj->x >= SW - BALL_SIZE) {
        obj->x = SW - BALL_SIZE;
        obj->vx = 0;                      
    }
}

void checkInput(struct Object* ball) { 

    if ((KEYS & A) == 0) {
        ball->vy = -SHOOT_VEL * gravityDirection; 
    }

    if ((KEYS & B) == 0) {
        gravityDirection = -1;
    } else {
        gravityDirection = 1;
    }

    if ((KEYS & SL) == 0) {
        ball->vx = 0;
        ball->vy = 0;
        ball->ax = 0;
        ball->ay = 0;
    } else {

        applyGravity(ball);
        if ((KEYS & L) == 0) {
            ball->vx = -MOVE_SPEED;
        } else if ((KEYS & R) == 0) {
            ball->vx = MOVE_SPEED;
        } else {
            ball->vx = 0;  
        }

        if ((KEYS & LS) == 0) {
            ball->vx = -SHOOT_VEL;
            ball->vy = -SHOOT_VEL;
        } else if ((KEYS & RS) == 0) {
            ball->vx = SHOOT_VEL;
            ball->vy = -SHOOT_VEL;
        }
    }
}

int main() {
    DSPC = MODE3 | BG2;
    struct Object ball = {120, 80, 0, 0, 0, 0};
    Coordinate prevCrnr = {ball.x, ball.y};  // Track the previous position
    while (1) {
        drawRect(prevCrnr, BALL_SIZE, BALL_SIZE, 0x0000, VRAM);
        checkInput(&ball);
        updateMovement(&ball);
        handleCollisions(&ball);
        Coordinate crnr = {ball.x, ball.y};
        drawRect(crnr, BALL_SIZE, BALL_SIZE, 0x03E0, VRAM);
        prevCrnr = crnr;
        waitVBLANK();
    }
    return 0;
}
