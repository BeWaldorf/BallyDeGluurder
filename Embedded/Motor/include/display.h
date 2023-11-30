#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Initialisatie van het display
void initDisplay();

// Tekent een verticale slider op het display
void drawVerticalSlider(int x, int value, const char *label);

// Tekent een batterij-indicator op het display
void drawBatteryIndicator(int x, int level, const char *label);

void clearDisplay();
void showDisplay();

#endif // DISPLAY_H
