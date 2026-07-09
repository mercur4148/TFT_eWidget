/*
  Example animated analogue meters

  Needs Font 2 (also Font 4 if using large scale label)

  Make sure all the display driver and pin connections are correct by
  editing the User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################

  Requires widget library here:
  https://github.com/Bodmer/TFT_eWidget
*/

#include <TFT_eSPI.h>     // Hardware-specific library
#include <TFT_eWidget.h>  // Widget library

TFT_eSPI tft  = TFT_eSPI();      // Invoke custom library

MeterWidget   meter  = MeterWidget(&tft);

#define LOOP_PERIOD 35 // Display updates every 35 ms

void setup(void)
{
  tft.init();
  tft.setRotation(1);

  /*
    tft.fillScreen(TFT_BLACK);
    meter.setColors(TFT_BLACK, TFT_SILVER, 0xFBC0);
  */
  
  tft.fillScreen(0xFBC0);
  meter.setColors(0xFBC0, TFT_BLACK, TFT_BLACK);

  meter.drawArc(false);
  meter.drawOutline(1);
  meter.drawDigitalValue(true, 1);
  meter.setLongTick(15);
  meter.setShortTick(5);
  meter.drawThickerNeedle(1);

  // meter.setZones(80, 100, 0, 0, 0, 0, 20, 80);   // RED, ORANGE, YELLOW, GREEN

  meter.setSmallLabel(true, "100 deg");
  // meter.analogMeter(0, 0, 100, "deg", "0", "25", "50", "75", "100");

  meter.analogMeter(0, 0, 100, "deg", "0", "50", "100");
  // meter.analogMeter(0, 60, 0, 25, "min", "0", "5", "10", "15", "20", "25");
}


void loop()
{
  static int d = 0;
  static uint32_t updateTime = 0;

  if (millis() - updateTime >= LOOP_PERIOD)
  {
    updateTime = millis();

    d += 4; if (d > 360) d = 0;

    // Create a Sine wave for testing, value is in range 0 - 100
    float value = 50.0 + 50.0 * sin((d + 0) * 0.0174532925);

    float current;
    current = mapValue(value, (float)0.0, (float)100.0, (float) -10, (float)110);

    meter.updateNeedle(current, 0);
  }
}

float mapValue(float ip, float ipmin, float ipmax, float tomin, float tomax)
{
  return tomin + (((tomax - tomin) * (ip - ipmin)) / (ipmax - ipmin));
}
