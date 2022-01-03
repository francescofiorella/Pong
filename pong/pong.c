/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           pong.c
** Last modified Date:  03/01/2022
** Last Version:        V1.00
** Descriptions:        functions to play pong game
** Correlated files:    pong.h, GLCD.h, timer.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "pong.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// paddle y position
#define PADDLE_Y_START 278
#define PADDLE_Y_END 287

// ball default movements
#define BALL_X_MOVEMENT_DEFAULT -2
#define BALL_Y_MOVEMENT_DEFAULT 2

typedef struct {
	unsigned int x_start, y_start, x_end, y_end;
	int x_movement, y_movement;
} Ball;

typedef struct {
	unsigned int x_start, x_end;
} Paddle;

// game's variables
Status game_status;
unsigned int score, best_score;
Ball ball;
Paddle paddle;
bool is_sound_on;

void init_pong(void) {	
	LCD_Clear(Black);
	
	drawBorders();
	
	// paddle drawn by adc
	game_status = STOPPED;
	best_score = 100; // setting it earlier, "New Best Score!" is not written
	updateBestScore(best_score);
	updateScore(0);
	ball.x_movement = BALL_X_MOVEMENT_DEFAULT;
	ball.y_movement = BALL_Y_MOVEMENT_DEFAULT;
	initBall(230, 157); // ball in the middle
}

void drawBorders() {
	unsigned int i; // counter
	
	for (i = 0; i <= 4; i++){
		LCD_DrawLine(0, i, 239, i, Red);	// top border
		LCD_DrawLine(i, 0, i, 277, Red);	// left border
		LCD_DrawLine(239-i, 0, 239-i, 277, Red);	// right border
	}
}

void drawPaddle(unsigned int start) { // draw a 31x10px paddle starting from "start" and 32px high from the bottom
	unsigned int i, i_max; // counter
	int i1; // counter (adjusted)
	unsigned int end = start + 30;
	int offset; // negative if the paddle is moving to left, positive otherwise
	unsigned int x_delete, x_draw; // LCD_DrawLine() arguments
	
	if (start >= 5 && start <= 204) { // if does not overlap the borders
		offset = start - paddle.x_start;		
		if (offset > 0) { // right movement
			x_delete = paddle.x_start;
			if (paddle.x_start == 0) { // draw the paddle for the first time
				x_draw = start;
				i_max = 30;
			} else {
				x_draw = paddle.x_end;
				i_max = offset;
			}
		} else { // left movement
			x_delete = paddle.x_end;
			x_draw = paddle.x_start;
			i_max = -offset;
		}
		
		for (i = 0; i <= i_max; i++) {
			if (offset > 0) {
				i1 = i;
			} else {
				i1 = -i;
			}
			// delete the previous paddle
			if (paddle.x_start >= 5 && paddle.x_start <= 204) // only if the paddle is already drawn on screen
				LCD_DrawLine(x_delete+i1, PADDLE_Y_START, x_delete+i1, PADDLE_Y_END, Black);
			
			// draw the new paddle
			LCD_DrawLine(x_draw+i1, PADDLE_Y_START, x_draw+i1, PADDLE_Y_END, Green);
		}
		
		paddle.x_start = start;
		paddle.x_end = end;
	}
}

void initBall(unsigned int x, unsigned int y) { // draw a 5x5 ball starting from "x" and "y"
	unsigned int i_old, i_new; // counter
	unsigned int x1 = x + 4;
	unsigned int y1 = y + 4;
	
	// delete the previous ball
	for (i_old = ball.y_start; i_old <= ball.y_end; i_old++){
		LCD_DrawLine(ball.x_start, i_old, ball.x_end, i_old, Black);
	}
	
	// draw the new ball
	for (i_new = y; i_new <= y1; i_new++){
		LCD_DrawLine(x, i_new, x1, i_new, White);
	}
	
	// update the variables
	ball.x_start = x;
	ball.y_start = y;
	ball.x_end = x1;
	ball.y_end = y1;
}

void drawBall(unsigned int x, unsigned int y) { // draw a 5x5 ball starting from the previous one
	unsigned int i, i_max, i_x_max, i_y_max; // counter
	unsigned int i_x = 0;
	unsigned int i_y = 0;
	// "a" and "b" allow to delete 5px (instead of 4) when the ball is moving in one direction only
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned int x1 = x + 4;
	unsigned int y1 = y + 4;
	// LCD_DrawLine() arguments
	unsigned int x1_delete1, x2_delete1, x1_delete2, x2_delete2, x1_draw1, x2_draw1, x1_draw2, x2_draw2;
	unsigned int y1_delete1, y2_delete1, y1_delete2, y2_delete2, y1_draw1, y2_draw1, y1_draw2, y2_draw2;
	
	if (x >= 5 && y >= 5 && x <= 230 && y <= 315) {
		// set the maximum number of iterations (x, y e total)		
		i_x_max = abs(ball.x_movement)-1;
		i_y_max = abs(ball.y_movement)-1;
		if (i_x_max < i_y_max) {
			i_max = i_y_max;
		} else {
			i_max = i_x_max;
		}
		
		for (i = 0; i <= i_max; i++) {
			// set LCD_DrawLine() arguments
			if (ball.x_movement > 0) { // to right
				x1_delete1 = ball.x_start + i_x;
				x2_delete1 = ball.x_start + i_x;
				
				x1_delete2 = ball.x_start + i_x + 1;
				x2_delete2 = ball.x_end + i_x + a;
				
				x1_draw1 = ball.x_end + 1 + i_x;
				x2_draw1 = ball.x_end + 1 + i_x;
				
				x1_draw2 = ball.x_start + 1 + i_x;
				x2_draw2 = ball.x_end + 1 + i_x;
			} else { // to left
				x1_delete1 = ball.x_end - i_x;
				x2_delete1 = ball.x_end - i_x;
				
				x1_delete2 = ball.x_start - i_x - a;
				x2_delete2 = ball.x_end - i_x - 1;
				
				x1_draw1 = ball.x_start - 1 - i_x;
				x2_draw1 = ball.x_start - 1 - i_x;
				
				x1_draw2 = ball.x_start - 1 - i_x;
				x2_draw2 = ball.x_end - 1 - i_x;
			}
			if (ball.y_movement > 0) { // to bottom
				y1_delete1 = ball.y_start + i_y;
				y2_delete1 = ball.y_end + i_y + b;
				
				y1_delete2 = ball.y_start + i_y;
				y2_delete2 = ball.y_start + i_y;
				
				y1_draw1 = ball.y_start + 1 + i_y;
				y2_draw1 = ball.y_end + i_y + b;
				
				y1_draw2 = ball.y_end + 1 + i_y;
				y2_draw2 = ball.y_end + 1 + i_y;
			} else { // to top
				y1_delete1 = ball.y_start - i_y - b;
				y2_delete1 = ball.y_end - i_y;
				
				y1_delete2 = ball.y_end - i_y;
				y2_delete2 = ball.y_end - i_y;
				
				y1_draw1 = ball.y_start - i_y - b;
				y2_draw1 = ball.y_end -1 - i_y;
				
				y1_draw2 = ball.y_start - 1 - i_y;
				y2_draw2 = ball.y_start - 1 - i_y;
			}
			
			// delete the previous ball
			if (i <= i_x_max)
				LCD_DrawLine(x1_delete1, y1_delete1, x2_delete1, y2_delete1, Black); // vertical
			if (i <= i_y_max)
				LCD_DrawLine(x1_delete2, y1_delete2, x2_delete2, y2_delete2, Black); // horizontal
			
			// draw the new ball
			if (i <= i_x_max)
				LCD_DrawLine(x1_draw1, y1_draw1, x2_draw1, y2_draw1, White); // vertical
			if (i <= i_y_max)
				LCD_DrawLine(x1_draw2, y1_draw2, x2_draw2, y2_draw2, White); // horizontal
			
			// increment the indexes and set the offset, if necessary
			if (i+1 <= i_x_max) {
				i_x++;
				a = 0;
			} else {
				a = 1;
			}
			if (i+1 <= i_y_max) {
				i_y++;
				b = 0;
			} else {
				b = 1;
			}
		}
		
		// update the variables
		ball.x_start = x;
		ball.y_start = y;
		ball.x_end = x1;
		ball.y_end = y1;
	}
}

void updateScore(unsigned int new_score) { // update the score
	// cast int to string
	char score_string[30];
	sprintf(score_string, "%d   ", new_score); // + blank space to delethe the last score
	
	GUI_Text(5, 153, (uint8_t *) score_string, White, Black); // it's drawn in [156-164(px)], so 156 px from the top and 156 from the bottom
	score = new_score;
}

void updateBestScore(unsigned int new_score) {
	char score_string[30];

	if (new_score > best_score) {
		best_score = new_score;
		GUI_Text(60, 159, (uint8_t *) "New Best Score!", White, Black);
	}
	
	// cast int to string
	sprintf(score_string, "%d ", best_score);
	
	GUI_Text(180, 5, (uint8_t *) score_string, Yellow, Black);
}

void moveBall() {	
	int pixel_hit;
	
	// if the sound is on, disable it
	if (is_sound_on) {
		disable_timer(1);
		reset_timer(1);
		is_sound_on = false;
	}
	
	if (game_status == STARTED) {
		// check the horizontal movement (x)
		if ((ball.x_movement > 0 && (ball.x_end+ball.x_movement >= 235)) // right border reached
			|| (ball.x_movement < 0 && (ball.x_start+ball.x_movement <= 4))) { // left border reached
			ball.x_movement = - ball.x_movement;
			
			// emit a lower pitched note
			is_sound_on = true;
			init_timer(1, 4240);
			enable_timer(1);
		}
		
		// check the vertical movement
		// (y) - top
		if (ball.y_movement < 0 && ball.y_start+ball.y_movement <= 4) { // top reached
			ball.y_movement = - ball.y_movement;
			
			// emit a lower pitched note
			is_sound_on = true;
			init_timer(1, 4240);
			enable_timer(1);
		}
		
		// (y) - bottom (paddle reached or not)
		if (ball.y_movement > 0 // verso il basso
			&& ball.y_end+ball.y_movement >= PADDLE_Y_START // paddle height reached
			&& ball.y_end < PADDLE_Y_START // paddle height not passed
			&& ball.x_end >= paddle.x_start // end_ball compared to start_paddle
			&& ball.x_start <= paddle.x_end) { // start_ball compared to end_paddle
				ball.y_movement = - ball.y_movement;
				
			// emit a higher pitched note
			is_sound_on = true;
			init_timer(1, 2120);
			enable_timer(1);
			
			// set ball y-movement checking the central pixel of the ball
			/*
				Paddle bits:
				[....-00-01-02-03-04-05-06-07-08-09-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-....]
				x-movement:
				[ -6 |   -5   |   -4   |   -3   |   -2   |   -1   |00|   01   |   02   |   03   |   04   |   05   | 06 ]
				Considering y-movement = 2;
				Corner (degrees) :
				[18.4|  21.8  |  26.6  |  33.7  |   45   |  63.4  |90|  63.4  |   45   |  33.7  |  26.6  |  21.8  |18.4]
			*/
			
			pixel_hit = ball.x_start + 2 - paddle.x_start;
			// pixel_hit can be < 0 or > 30 because the central pixel can fall outside the paddle
			if (pixel_hit < 0) {
				ball.x_movement = -6;
			} else if (pixel_hit <= 2) {
				ball.x_movement = -5;
			} else if (pixel_hit <= 5) {
				ball.x_movement = -4;
			} else if (pixel_hit <= 8) {
				ball.x_movement = -3;
			} else if (pixel_hit <= 11) {
				ball.x_movement = -2;
			} else if (pixel_hit <= 14) {
				ball.x_movement = -1;
			} else if (pixel_hit == 15) {
				ball.x_movement = 0;
			} else if (pixel_hit <= 18) {
				ball.x_movement = 1;
			} else if (pixel_hit <= 21) {
				ball.x_movement = 2;
			} else if (pixel_hit <= 24) {
				ball.x_movement = 3;
			} else if (pixel_hit <= 27) {
				ball.x_movement = 4;
			} else if (pixel_hit <= 30) {
				ball.x_movement = 5;
			} else {
				ball.x_movement = 6;
			}
			
			// update score
			if (score < 100) {
				updateScore(score + 5);
			} else {
				updateScore(score + 10);
			}
			
			// re-draw best score (could have been modified by the ball)
			updateBestScore(best_score);
		} else if (ball.y_movement > 0 && ball.y_end+ball.y_movement >= 319) { // you lost
			setLost();
		}
		drawBall(ball.x_start + ball.x_movement, ball.y_start + ball.y_movement);
	}
}

void newPong() { // if you lost, start a new game
	if (game_status == LOST) {
		game_status = STOPPED;
		GUI_Text(85, 139, (uint8_t *) "You Lose", Black, Black); // delete the string
		GUI_Text(60, 159, (uint8_t *) "New Best Score!", Black, Black); // delete the string
		updateScore(0);
		ball.x_movement = BALL_X_MOVEMENT_DEFAULT;
		ball.y_movement = BALL_Y_MOVEMENT_DEFAULT;
		initBall(230, 157); // ball in the middle
	}
}

void setLost() { // you lost
	game_status = LOST;
	disable_timer(0);
	reset_timer(0);
	GUI_Text(85, 139, (uint8_t *) "You Lose", White, Black);
	updateScore(score); // re-draw score
	updateBestScore(score);
}

void setPause() { // pause the game
	game_status = PAUSED;
	GUI_Text(88, 153, (uint8_t *) "Paused", White, Black);
}

void setResume() { // resume the game
	game_status = STARTED;
	GUI_Text(88, 153, (uint8_t *) "Paused", Black, Black);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
