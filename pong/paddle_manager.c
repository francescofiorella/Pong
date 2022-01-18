/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "pong.h"
#include <stdio.h>

// paddle y position
#define PADDLE_Y_START 278
#define PADDLE_Y_END 287

Paddle paddle;

void initPaddle() {
	paddle.y_start = PADDLE_Y_START;
	paddle.y_end = PADDLE_Y_END;
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
				LCD_DrawLine(x_delete+i1, paddle.y_start, x_delete+i1, paddle.y_end, Black);
			
			// draw the new paddle
			LCD_DrawLine(x_draw+i1, paddle.y_start, x_draw+i1, paddle.y_end, Green);
		}
		
		paddle.x_start = start;
		paddle.x_end = end;
	}
}

int hasHitThePaddle(Ball *ball) {
	return (ball->y_movement > 0 // verso il basso [user_paddle]
		&& ball->y_end+ball->y_movement >= paddle.y_start // paddle height reached
		&& ball->y_end < paddle.y_start // paddle height not passed
		&& ball->x_end >= paddle.x_start // end_ball compared to start_paddle
		&& ball->x_start <= paddle.x_end); // start_ball compared to end_paddle
}

int calculateNewBallMovement(Ball *ball) {
	int pixel_hit;
	
	// set ball y-movement checking the central pixel of the ball
	/*
		Paddle bits:
		[....-00-01-02-03-04-05-06-07-08-09-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-....]
		x-movement: [DC = Don't Change]
		[ -6 |   -5   |   -4   |   -3   |   -2   |   -1   |DC|   01   |   02   |   03   |   04   |   05   | 06 ]
		Considering y-movement = 2;
		Corner (degrees) :
		[18.4|  21.8  |  26.6  |  33.7  |   45   |  63.4  |DC|  63.4  |   45   |  33.7  |  26.6  |  21.8  |18.4]
	*/
	
	pixel_hit = ball->x_start + 2 - paddle.x_start;
	// pixel_hit can be < 0 or > 30 because the central pixel can fall outside the paddle
	if (pixel_hit < 0) {
		return -6;
	} else if (pixel_hit <= 2) {
		return -5;
	} else if (pixel_hit <= 5) {
		return -4;
	} else if (pixel_hit <= 8) {
		return -3;
	} else if (pixel_hit <= 11) {
		return -2;
	} else if (pixel_hit <= 14) {
		return -1;
	} else if (pixel_hit == 15) {
		// do nothing
		return ball->x_movement;
	} else if (pixel_hit <= 18) {
		return 1;
	} else if (pixel_hit <= 21) {
		return 2;
	} else if (pixel_hit <= 24) {
		return 3;
	} else if (pixel_hit <= 27) {
		return 4;
	} else if (pixel_hit <= 30) {
		return 5;
	} else {
		return 6;
	}
}
