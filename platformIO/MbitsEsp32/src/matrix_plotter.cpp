#include "matrix_plotter.hpp"
#define NUM_ROWS 5
#define NUM_COLUMNS 5
#define NUM_LEDS (NUM_ROWS * NUM_COLUMNS)
#define LED_PIN 13
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#include <FastLED.h>
#include "Dots5x5font.h"

MatrixPlotter::MatrixPlotter()
: m_leds{}
, m_max_bright(10)
, m_blue(0, 0, 100)
, m_green(100, 0, 0)
{
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(m_leds, NUM_LEDS);
    FastLED.setBrightness(m_max_bright);
}


int MatrixPlotter::get_led_index(int a_x, int a_y)
{
  return a_x%5 + a_y*5;
}

int MatrixPlotter::led(int a_x, int a_y)
{
  static int mapLED[5][5];
  static bool is_mapped = false;
  if(!is_mapped){
    for(int x=0; x<5; ++x){
      for(int y=0; y<5; ++y){
        mapLED[x][y] = get_led_index(x,y);
      }
    }
    is_mapped = true;
  }
  return mapLED[a_x][a_y];
}

void MatrixPlotter::show_for_ms(int a_ms_delay)
{
  FastLED.show();
  FastLED.delay(a_ms_delay);
  FastLED.clear();
  FastLED.show();
}

CRGB &MatrixPlotter::green()
{
  return m_green;
}

CRGB &MatrixPlotter::blue()
{
  return m_blue;
}

void MatrixPlotter::draw_smile()
{
  m_leds[led(1,0)] = m_blue;
  m_leds[led(3,0)] = m_blue;
  m_leds[led(2,2)] = m_blue;
  m_leds[led(0,3)] = m_blue;
  m_leds[led(4,3)] = m_blue;
  m_leds[led(1,4)] = m_blue;
  m_leds[led(2,4)] = m_blue;
  m_leds[led(3,4)] = m_blue;

}

void MatrixPlotter::plotMatrixChar(CRGB (*matrix)[5], CRGB myRGBcolor, int x, char character, int width, int height) {
  int y = 0;
  if (width > 0 && height > 0) {
    int charIndex = (int)character - 32;
    int xBitsToProcess = width;
    for (int i = 0; i < height; i++) {
      byte fontLine = FontData[charIndex][i];
      for (int bitCount = 0; bitCount < xBitsToProcess; bitCount++) {
        CRGB pixelColour = CRGB(0, 0, 0);
        if (fontLine & 0b10000000) {
          pixelColour = myRGBcolor;
        }
        fontLine = fontLine << 1;
        int xpos = x + bitCount;
        int ypos = y + i;
        if (xpos < 0 || xpos > 10 || ypos < 0 || ypos > 5);
        else {
          matrix[xpos][ypos] = pixelColour;
        }
      }
    }
  }
}

void MatrixPlotter::ShowString(String sMessage,CRGB myRGBcolor) {
  CRGB matrixBackColor[10][5];
  int mapLED[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
  int messageLength = sMessage.length();
  for (int x = 0; x < messageLength; x++) {
    char myChar = sMessage[x];
    plotMatrixChar(matrixBackColor, myRGBcolor, 0 , myChar, 5, 5);
    for (int sft = 0; sft <= 5; sft++) {
      for (int x = 0; x < NUM_COLUMNS; x++) {
        for (int y = 0; y < 5; y++) {
          int stripIdx = mapLED[y * 5 + x];
          if (x + sft < 5) {
            m_leds[stripIdx] = matrixBackColor[x + sft][y];
          } else {
            m_leds[stripIdx] = CRGB(0, 0, 0);
          }
        }
      }
      FastLED.show();
      if (sft == 0) {
        FastLED.delay(200);
      } else {
        FastLED.delay(30);
      }
    }
  }
}