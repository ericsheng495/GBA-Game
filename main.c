#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "gba.h"
#include "images/won.h"
#include "images/lose.h"
#include "images/titlescreen.h"
#include "images/troll.h"

int main(void) {
  REG_DISPCTL = MODE3 | BG2_ENABLE;
  //int buttonWasDown[NBUTTONS] = {0};
  //int buttonJustReleased[NBUTTONS] = {0};
  unsigned short bgcolor = BLACK;
  int score = 0; 
  //u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  struct state currentState, previousState;

  currentState.gamestate = GS_START;

  // Run forever (or until the power runs out)
  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons





    // Keep track of the state we're about to overwrite
    previousState = currentState;
    // Update button status
    // for (int i=0; i<NBUTTONS; i++) {
    //   buttonJustReleased[i] = KEY_DOWN(i, BUTTONS) == 0 && buttonWasDown[i];
    //   buttonWasDown[i] = KEY_DOWN(i, BUTTONS) != 0;
    // }

    // Compute the graphics needed for the current state
    switch (currentState.gamestate) {
    case GS_START:
      drawFullScreenDMA(titlescreen);
      // drawImageDMA(50,50,50,50,troll);

      char buffer[51];
      sprintf(buffer, "Press START to play...");
      drawRectDMA(150, 5, 200, 10, WHITE);
      drawString(150, 5, buffer, RED);
      break;
      

    case GS_BOUNCE:
      // See if we need to do something with buttons
      if (KEY_DOWN(BUTTON_UP, currentButtons)) {
        currentState.rectangleOne.row = currentState.rectangleOne.row - currentState.rectangleOne.verticalVelocity;
      }
      if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
        currentState.rectangleOne.row = currentState.rectangleOne.row + currentState.rectangleOne.verticalVelocity;
      }
      if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
        currentState.rectangleOne.col = currentState.rectangleOne.col - currentState.rectangleOne.horizontalVelocity;
      }
      if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
        currentState.rectangleOne.col = currentState.rectangleOne.col + currentState.rectangleOne.horizontalVelocity;
      }

    
      //determine if wall bounce is necessary
      if(currentState.rectangleOne.row < 0) { //hitting the top 
        currentState.rectangleOne.row = 0;
      }
      if(currentState.rectangleOne.row >= SCREEN_HEIGHT-currentState.size-14) { //hitting bottom of screen - scoreboard height  
        currentState.rectangleOne.row = SCREEN_HEIGHT -currentState.size-14;
      }
      if(currentState.rectangleOne.col < 0) { //hitting left side 
        currentState.rectangleOne.col = 0;
        }
      if(currentState.rectangleOne.col >= SCREEN_WIDTH- currentState.size) {//hitting right side 
        currentState.rectangleOne.col = SCREEN_WIDTH-currentState.size;
      }


      //determine if it collides with another object 
      if(currentState.rectangleOne.col  < currentState.obstacle.col + currentState.size && 
        currentState.rectangleOne.col + currentState.size > currentState.obstacle.col && 
        currentState.rectangleOne.row < currentState.obstacle.row + currentState.size && 
        currentState.rectangleOne.row + currentState.size > currentState.obstacle.row
      ){
        score++;
        currentState.obstacle.row = rand() % SCREEN_HEIGHT;
        currentState.obstacle.col = rand() % SCREEN_WIDTH;
        currentState.obstacle.horizontalVelocity = 1; 
        currentState.obstacle.verticalVelocity = 1; 
        currentState.obstacle.color = BLUE;
        if(currentState.obstacle.row >= SCREEN_HEIGHT-currentState.size-14) { //make sure obstacle does not go below score board. 
          currentState.obstacle.row = SCREEN_HEIGHT -currentState.size-14;
        }
      }
      break;

    case GS_INIT:
      currentState.size = 5;
      currentState.rectangleOne.row = 0; 
      currentState.rectangleOne.col = 0; 
      currentState.rectangleOne.horizontalVelocity = 1; 
      currentState.rectangleOne.verticalVelocity = 1; 
      currentState.rectangleOne.color = RED;

      currentState.obstacle.row = rand() % SCREEN_HEIGHT;
      currentState.obstacle.col = rand() % SCREEN_WIDTH;
      currentState.obstacle.horizontalVelocity = 1; 
      currentState.obstacle.verticalVelocity = 1; 
      currentState.obstacle.color = BLUE;
      if(currentState.obstacle.row >= SCREEN_HEIGHT-currentState.size-14) { //make sure obstacle does not go below score board. 
        currentState.obstacle.row = SCREEN_HEIGHT -currentState.size-14;
      }
      score = 0; 
      break;


    case GS_WIN:
      break;

    case GS_LOSE:
      break;

    }


  
    // Wait for a Vblank to update the screen
    waitForVBlank();

    // Do the drawing and choose the next state
    switch (currentState.gamestate) {

    case GS_START:
      if (KEY_DOWN(BUTTON_START, currentButtons)){
        drawRectDMA(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bgcolor);
        currentState.gamestate = GS_INIT;
      }
      break;


    case GS_INIT:
      currentState.gamestate = GS_BOUNCE;
      for(int i = 0; i < SCREEN_WIDTH-currentState.size; i++){
        drawString(SCREEN_HEIGHT - 17, i, "-", YELLOW);
      }
      drawImageDMA(148,228,13,13,troll);
      break;

    case GS_BOUNCE:
      // Erase the old ball
      drawRectDMA(previousState.rectangleOne.row,previousState.rectangleOne.col,previousState.size,previousState.size,bgcolor);
      drawRectDMA(previousState.obstacle.row,previousState.obstacle.col,previousState.size,previousState.size,bgcolor);

      // Draw the ball
      drawRectDMA(currentState.rectangleOne.row,currentState.rectangleOne.col,currentState.size, currentState.size,currentState.rectangleOne.color);
      drawRectDMA(currentState.obstacle.row,currentState.obstacle.col,currentState.size, currentState.size,currentState.obstacle.color);

      char buffer[51];
      sprintf(buffer, "Score: %d", score);
      drawRectDMA(150, 5, 200, 10, bgcolor);
      drawString(150, 5, buffer, YELLOW);

      if (KEY_DOWN(BUTTON_SELECT, currentButtons)){
        drawRectDMA(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bgcolor);
        currentState.gamestate = GS_START;
      }
      //if z is pressed on the keybaord, game is Quiz
      if(KEY_DOWN(BUTTON_A, currentButtons)){
        if(score > 5){
          currentState.gamestate = GS_WIN;
          drawFullScreenDMA(won);
        }else{
          currentState.gamestate = GS_LOSE;
          drawFullScreenDMA(lose);
        }
      }
      break;



    case GS_WIN:
      drawString(SCREEN_HEIGHT / 4, (SCREEN_WIDTH - 9 * 6) / 2, "You Won!", GREEN);
      drawString(120, (SCREEN_WIDTH - 9 * 14) / 2, "Replay: Press START", GREEN);
      if (KEY_DOWN(BUTTON_SELECT, currentButtons)){
        drawRectDMA(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bgcolor);
        currentState.gamestate = GS_START;
      }
      if (KEY_DOWN(BUTTON_START, currentButtons)){
        drawRectDMA(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bgcolor);
        currentState.gamestate = GS_INIT;
      }
      break;



    case GS_LOSE:
      drawString(SCREEN_HEIGHT / 4, (SCREEN_WIDTH - 9 * 7) / 2, "You Lost!", RED);
      drawString(120, (SCREEN_WIDTH - 9 * 14) / 2, "Press START to replay", RED);

      if (KEY_DOWN(BUTTON_SELECT, currentButtons)){
        drawRectDMA(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bgcolor);
        currentState.gamestate = GS_START;
      }
      if (KEY_DOWN(BUTTON_START, currentButtons)){
        drawRectDMA(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bgcolor);
        currentState.gamestate = GS_INIT;
      }
      break;
    }

    //previousButtons = currentButtons;
  } // Game Loop
  return 0;
}