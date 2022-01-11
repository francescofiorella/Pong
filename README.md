# Pong
The software implements the game called “Pong”, the classic table tennis-themed arcade game, originally released in 1972 by Atari. The game is developed for the "LandTiger", a development board based on a second-generation ARM Cortex-M3 microcontroller.

This first project implements a single-player version of the game where the player must not let the ball fall below the paddle. The paddle can only move horizontally, and the player operates it through the potentiometer available in the LandTiger board.

The game field is implemented vertically, i.e., the paddle is in the bottom portion of the LCD display with red walls (5px thick) on the left, upper and right portion of the display for the ball to bounce on.
When the ball hits a wall, it bounces with a reflection angle equal to the incident one. For instance, if a ball going downwards hits the left wall with an incident angle of 30°, then it bounces downwards with the same 30° angle.
When the ball hits the paddle, the reflection angle is not always equal to the incident one, and it changes depending on what part of the paddle is hit by the ball.
The paddle is 32px high from the bottom part of the screen and 10px thick, and the ball is a 5x5px square.
 
Whenever the ball hits a wall or the paddle, the buzzer (loudspeaker) emits a lower pitched note (when bouncing on the wall) and a higher pitched note (when bouncing on the paddle).

Every time the ball hits the paddle, the score is incremented by 5. If the score is greater than 100, whenever the paddle touches the ball, it is incremented by 10 points. The score is printed on the left side of the screen, at mid high (160px from both the bottom and top part of the LCD). The score is saved in between games, and the new record (the maximum score since the board was last reset) is displayed in the top right corner. The initial record to beat is 100 points. Whenever the board is powered off or the RESET button is pressed, the score is reset as well. If the ball falls below the paddle, a message “You lose” is displayed.

The user starts the game by pressing the button KEY1. When the game starts, the ball touches the right wall at mid high (160px from both the bottom and top part of the screen) and go downwards with an angle of 45°. To pause the game and later resume it, the user can press the button KEY2. In case of game over, to prepare a new game the player can press the INT0 button, followed by KEY1 to start it.

_Homepage preview:_

<img width="599" alt="pong_1_preview_1" src="https://user-images.githubusercontent.com/22116863/149002357-a1da94b4-7e4b-4a63-90ba-d259216f001a.png">
