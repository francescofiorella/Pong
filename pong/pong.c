/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           pong.c
** Last modified Date:  18/01/2022
** Last Version:        V1.00
** Descriptions:        functions to play pong game
** Correlated files:    pong.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "pong.h"
#include <stdio.h>

// game's variables
Status game_status;
unsigned int score, best_score;
unsigned int is_sound_on;

void init_pong(void) {	
	LCD_Clear(Black);
	
	drawBorders();
	
	// paddle drawn by adc
	game_status = INITIAL;
	best_score = 100; // if set earlier, "New Best Score!" is not written
	updateBestScore(best_score);
	updateScore(0);
	initPaddle();
	resetBall();
	
	// draw the info
	GUI_Text(65, 119, (uint8_t *) "Press KEY1 to", White, Black);
	GUI_Text(60, 139, (uint8_t *) "start the game  ", White, Black);
	GUI_Text(40, 179, (uint8_t *) "KEY2 to Pause/Resume", White, Black);
	GUI_Text(60, 199, (uint8_t *) "INT0 to Reset", White, Black);
}

void drawBorders() {
	unsigned int i; // counter
	
	for (i = 0; i <= 4; i++){
		LCD_DrawLine(0, i, 239, i, Red);	// top border
		LCD_DrawLine(i, 0, i, 319, Red);	// left border
		LCD_DrawLine(239-i, 0, 239-i, 319, Red);	// right border
	}
}

void updateScore(unsigned int new_score) { // update the score
	// cast int to string
	char score_string[30];
	sprintf(score_string, "%d   ", new_score); // + blank space to delethe the last score
	
	GUI_Text(5, 153, (uint8_t *) score_string, White, Black); // it's drawn in [156-164(px)], so 156 px from the top and 156 from the bottom
	score = new_score;
}

void increaseScore() {
	if (score < 100) {
		updateScore(score + 5);
	} else {
		updateScore(score + 10);
	}
}

void updateBestScore(unsigned int new_score) {
	char score_string[30];

	if (new_score > best_score) {
		best_score = new_score;
		GUI_Text(60, 139, (uint8_t *) "New Best Score!", Yellow, Black);
	}
	
	// cast int to string
	sprintf(score_string, "Best score: %d ", best_score);
	
	GUI_Text(80, 5, (uint8_t *) score_string, White, Black);
}

void deleteBestScore() {
	unsigned int i;
	for (i = 80; i <= 230; i++) {
		LCD_DrawLine(i, 5, i, 20, Black);
	}
}

// freqs:
/* 
131Hz		k=4240		C3
147Hz		k=3779	D3
165Hz		k=3367	E3
175Hz		k=3175	F3
196Hz		k=2834	G3
220Hz		k=2525	A3
247Hz		k=2249	B3
262Hz		k=2120		C4
294Hz		k=1890	D4
330Hz		k=1684	E4
349Hz		k=1592	F4
392Hz		k=1417	G4
440Hz		k=1263	A4
494Hz		k=1125	B4
523Hz		k=1062		C5
*/

void disableSound() {
	disable_timer(1);
	reset_timer(1);
	is_sound_on = 0;
}

void enableSound(int timer_num) {
	init_timer(1, timer_num);
	enable_timer(1);
	is_sound_on = 1;
}

void newPong() { // if you lost, set again the parameters to the default values
	if (game_status == ENDED) {
		game_status = INITIAL;
		GUI_Text(85, 159, (uint8_t *) "You Lose", Black, Black); // delete the string
		updateScore(0);
		GUI_Text(65, 119, (uint8_t *) "Press KEY1 to", White, Black);
		GUI_Text(60, 139, (uint8_t *) "start the game  ", White, Black);
		GUI_Text(40, 179, (uint8_t *) "KEY2 to Pause/Resume", White, Black);
		GUI_Text(60, 199, (uint8_t *) "INT0 to Reset", White, Black);
		resetBall();
	}
}

void setStart() { // start the game
	game_status = RUNNING;
	enable_timer(0);
	deleteBestScore();
	// delete the info
	GUI_Text(65, 119, (uint8_t *) "Press KEY1 to", Black, Black);
	GUI_Text(60, 139, (uint8_t *) "start the game  ", Black, Black);
	GUI_Text(40, 179, (uint8_t *) "KEY2 to Pause/Resume", Black, Black);
	GUI_Text(60, 199, (uint8_t *) "INT0 to Reset", Black, Black);
}

void setLost() { // you lost
	game_status = ENDED;
	disable_timer(0);
	reset_timer(0);
	GUI_Text(85, 159, (uint8_t *) "You Lose", White, Black);
	updateScore(score); // re-draw score
	updateBestScore(score);
}

void setPause() { // pause the game
	game_status = PAUSED;
	disable_timer(0);
	reset_timer(0);
	
	// disable sound (just to be sure)
	disableSound();
	
	GUI_Text(88, 153, (uint8_t *) "Paused", White, Black);
	updateBestScore(best_score); // draw the best score
}

void setResume() { // resume the game
	game_status = RUNNING;
	deleteBestScore();
	GUI_Text(88, 153, (uint8_t *) "Paused", Black, Black);
	enable_timer(0);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
