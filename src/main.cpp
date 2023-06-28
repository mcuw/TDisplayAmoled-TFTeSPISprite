#include "main.h"
#include <rm67162.h>
#include <TFT_eSPI.h>
#include <true_color.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define WIDTH 536
#define HEIGHT 240
unsigned long targetTime = 0;
byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
unsigned int colour = red << 11;

void setup()
{
  // Use TFT_eSPI Sprite made by framebuffer , unnecessary calling during use tft.xxxx function
  Serial.begin(115200);
  rm67162_init();
  lcd_setRotation(1);
  sprite.createSprite(WIDTH, HEIGHT);
  sprite.setSwapBytes(1);
}

void loop()
{
  sprite.pushImage(0, 0, WIDTH, HEIGHT, (uint16_t *)gImage_true_color);
  lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)sprite.getPointer());
  delay(2000);

  sprite.fillSprite(TFT_BLACK);

  sprite.fillRect(0, 0, 67, 120, TFT_RED);
  sprite.fillRect(67 * 1, 0, 67, 120, TFT_GREEN);
  sprite.fillRect(67 * 2, 0, 67, 120, TFT_BLUE);
  sprite.fillRect(67 * 3, 0, 67, 120, TFT_RED);
  sprite.fillRect(67 * 4, 0, 67, 120, TFT_GREEN);
  sprite.fillRect(67 * 5, 0, 67, 120, TFT_BLUE);
  sprite.fillRect(67 * 6, 0, 67, 120, TFT_RED);
  sprite.fillRect(67 * 7, 0, 67, 120, TFT_GREEN);

  sprite.fillRect(0, 120, 67, 120, TFT_BLUE);
  sprite.fillRect(67 * 1, 120, 67, 120, TFT_RED);
  sprite.fillRect(67 * 2, 120, 67, 120, TFT_GREEN);
  sprite.fillRect(67 * 3, 120, 67, 120, TFT_BLUE);
  sprite.fillRect(67 * 4, 120, 67, 120, TFT_RED);
  sprite.fillRect(67 * 5, 120, 67, 120, TFT_GREEN);
  sprite.fillRect(67 * 6, 120, 67, 120, TFT_BLUE);
  sprite.fillRect(67 * 7, 120, 67, 120, TFT_RED);

  lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)sprite.getPointer());
  delay(2000);

  uint16_t colors[6] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA};
  for (int i = 0; i < 6; ++i)
  {
    sprite.fillSprite(TFT_BLACK);
    sprite.setTextColor(colors[i], TFT_BLACK);
    sprite.drawString("LilyGo.cc", WIDTH / 2 - 30, 85, 4);
    sprite.drawString("T-Display AMOLED", WIDTH / 2 - 70, 110, 4);
    lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)sprite.getPointer());
    delay(200);
  }
  delay(2000);

  for (int pos = WIDTH; pos > 0; pos--)
  {
    int h = HEIGHT;
    while (h--)
      sprite.drawFastHLine(0, h, WIDTH, rainbow(h * 4));
    sprite.setTextSize(1);
    sprite.setTextFont(4);
    sprite.setTextColor(TFT_WHITE);
    sprite.setTextWrap(false);
    sprite.setCursor(pos, 100);
    sprite.print("LilyGo AMOLED");
    lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)sprite.getPointer());
  }
  delay(2000);

  targetTime = millis() + 1000;
  uint32_t runTime = millis() + 6000;
  sprite.fillSprite(TFT_BLACK);
  while (runTime > millis())
  {
    drawRainbow();
  }
  delay(2000);
}

void drawRainbow()
{
  if (targetTime < millis())
  {
    targetTime = millis() + 500;
    Serial.println(targetTime);
    // Colour changing state machine
    for (int i = 0; i < WIDTH; i++)
    {
      sprite.drawFastVLine(i, 0, sprite.height(), colour);
      switch (state)
      {
      case 0:
        green += 2;
        if (green == 64)
        {
          green = 63;
          state = 1;
        }
        break;
      case 1:
        red--;
        if (red == 255)
        {
          red = 0;
          state = 2;
        }
        break;
      case 2:
        blue++;
        if (blue == 32)
        {
          blue = 31;
          state = 3;
        }
        break;
      case 3:
        green -= 2;
        if (green == 255)
        {
          green = 0;
          state = 4;
        }
        break;
      case 4:
        red++;
        if (red == 32)
        {
          red = 31;
          state = 5;
        }
        break;
      case 5:
        blue--;
        if (blue == 255)
        {
          blue = 0;
          state = 0;
        }
        break;
      }
      colour = red << 11 | green << 5 | blue;
    }

    // The standard ADAFruit font still works as before
    sprite.setTextColor(TFT_BLACK);
    sprite.setCursor(12, 5);
    sprite.print("Original ADAfruit font!");

    // The new larger fonts do not use the .setCursor call, coords are embedded
    sprite.setTextColor(TFT_BLACK, TFT_BLACK); // Do not plot the background colour

    // Overlay the black text on top of the rainbow plot (the advantage of not drawing the background colour!)
    sprite.drawCentreString("Font size 2", 80, 14, 2); // Draw text centre at position 80, 12 using font 2

    // sprite.drawCentreString("Font size 2",81,12,2); // Draw text centre at position 80, 12 using font 2

    sprite.drawCentreString("Font size 4", 80, 30, 4); // Draw text centre at position 80, 24 using font 4

    sprite.drawCentreString("12.34", 80, 54, 6); // Draw text centre at position 80, 24 using font 6

    sprite.drawCentreString("12.34 is in font size 6", 80, 92, 2); // Draw text centre at position 80, 90 using font 2

    // Note the x position is the top left of the font!

    // draw a floating point number
    float pi = 3.14159;                                        // Value to print
    int precision = 3;                                         // Number of digits after decimal point
    int xpos = 50;                                             // x position
    int ypos = 110;                                            // y position
    int font = 2;                                              // font number only 2,4,6,7 valid. Font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : a p m
    xpos += sprite.drawFloat(pi, precision, xpos, ypos, font); // Draw rounded number and return new xpos delta for next print position
    sprite.drawString(" is pi", xpos, ypos, font);             // Continue printing from new x position
    lcd_PushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)sprite.getPointer());
  }
}

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(uint8_t value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = red through to 127 = blue

  uint8_t red = 0;   // Red is the top 5 bits of a 16 bit colour value
  uint8_t green = 0; // Green is the middle 6 bits
  uint8_t blue = 0;  // Blue is the bottom 5 bits

  uint8_t sector = value >> 5;
  uint8_t amplit = value & 0x1F;

  switch (sector)
  {
  case 0:
    red = 0x1F;
    green = amplit;
    blue = 0;
    break;
  case 1:
    red = 0x1F - amplit;
    green = 0x1F;
    blue = 0;
    break;
  case 2:
    red = 0;
    green = 0x1F;
    blue = amplit;
    break;
  case 3:
    red = 0;
    green = 0x1F - amplit;
    blue = 0x1F;
    break;
  }

  return red << 11 | green << 6 | blue;
}
