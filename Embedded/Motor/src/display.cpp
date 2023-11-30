// display.c
#include "display.h"

extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

void drawVerticalSlider(int x, int value, const char *label)
{
    int sliderHeight = map(abs(value), 0, 100, 0, (SCREEN_HEIGHT - 16) / 2);
    int y = (SCREEN_HEIGHT - 16) / 2; // Start in het midden van het scherm
    if (value > 0)
    {
        display.fillRect(x, y - sliderHeight, 20, sliderHeight, WHITE);
    }
    else
    {
        display.fillRect(x, y, 20, sliderHeight, WHITE);
    }
    display.drawRect(x, 0, 20, SCREEN_HEIGHT - 16, WHITE);
    display.setCursor(x, SCREEN_HEIGHT - 12);
    display.print(label);
}

void drawBatteryIndicator(int x, int level, const char *label)
{
    int batteryHeight = (SCREEN_HEIGHT - 16); // Maak de batterij even hoog als de sliders
    int batteryY = 0;                         // Startpunt van de batterij

    // Teken de omtrek van de batterij
    display.drawRect(x, batteryY, 20, batteryHeight, WHITE);

    // Teken de batterijvakjes
    int segmentHeight = batteryHeight / 5;
    for (int i = 0; i < 5; i++)
    {
        if (i < level)
        {
            // Vul de segmenten van onder naar boven
            display.fillRect(x + 2, batteryY + batteryHeight - (i + 1) * segmentHeight, 16, segmentHeight - 2, WHITE);
        }
        else
        {
            // Teken de lege segmenten van onder naar boven
            display.drawRect(x + 2, batteryY + batteryHeight - (i + 1) * segmentHeight, 16, segmentHeight - 2, WHITE);
        }
    }

    // Teken de tekst "Batterij" onder de indicator
    display.setCursor(x, SCREEN_HEIGHT - 12);
    display.print(label);
}

void clearDisplay()
{
    display.clearDisplay();
}

void showDisplay()
{
    display.display();
}