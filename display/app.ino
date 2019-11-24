#include <TFT.h>

/* 
#define SCLK 13
#define MOSI 4
#define CS 10
#define DC 8
#define RESET 7

TFT TFTscreen = TFT(CS, DC, RESET); //, SCLK, RESET);
 */

TFT TFTscreen = TFT(8, 10, 9);

void setup() {
  // put your setup code here, to run once:
  
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();
  
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  
  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Test", 0, 0);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
