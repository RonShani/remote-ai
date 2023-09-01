#ifndef MATRIX_PLOTTER
#define MATRIX_PLOTTER

#define NUM_ROWS 5
#define NUM_COLUMNS 5
#define NUM_LEDS (NUM_ROWS * NUM_COLUMNS)
#define LED_PIN 13
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#include <FastLED.h>

class MatrixPlotter {
public:
    MatrixPlotter();
    void ShowString(String sMessage,CRGB myRGBcolor);
    void plotMatrixChar(CRGB (*matrix)[5], CRGB myRGBcolor, int x, char character, int width, int height);
    int led(int a_x, int a_y);
    void draw_smile();
    void show_for_ms(int a_ms_delay);
    void show_untill(int a_ms_delay);
    void clear();
    CRGB &green();
    CRGB &blue();

private:
    int get_led_index(int a_x, int a_y);
    static void parrallel_show(MatrixPlotter *a_plotter, int a_ms_delay);


private:
    CRGBArray<NUM_LEDS> m_leds;
    uint8_t m_max_bright;
    CRGB m_blue;
    CRGB m_green;
};

#endif // MATRIX_PLOTTER