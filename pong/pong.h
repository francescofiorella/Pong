/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           pong.h
** Last modified Date:  03/01/2022
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the pong.c file
** Correlated files:    
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

// game_status options
typedef enum {INITIAL, RUNNING, PAUSED, ENDED} Status;

typedef struct {
	unsigned int x_start, y_start, x_end, y_end;
	int x_movement, y_movement;
} Ball;

typedef struct {
	unsigned int x_start, x_end, y_start, y_end;
} Paddle;

// pong functions
extern void init_pong( void );
extern void drawBorders( void );
extern void updateScore( unsigned int new_score );
extern void increaseScore( void );
extern void updateBestScore( unsigned int new_score );
extern void deleteBestScore( void );
extern void disableSound( void );
extern void enableSound( int timer_num );
extern void newPong( void );
extern void setStart( void );
extern void setLost( void );
extern void setPause( void );
extern void setResume( void );

// paddle_manager functions
extern void initPaddle( void );
extern void drawPaddle( unsigned int start );
extern int hasHitThePaddle( Ball *ball );
extern int calculateNewBallMovement( Ball *ball );

// ball_manager functions
extern void resetBall( void );
extern void initBall( unsigned int x_start, unsigned int y_start );
extern void drawBall( unsigned int x_start, unsigned int y_start );
extern void moveBall( void );

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
