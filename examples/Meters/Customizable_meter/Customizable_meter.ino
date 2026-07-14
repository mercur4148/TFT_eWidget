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

#include <TFT_eSPI.h>    // Hardware-specific library
#include <TFT_eWidget.h> // Widget library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

MeterWidget meter = MeterWidget(&tft);

#define LOOP_PERIOD 35 // Display updates every 35 ms

void setup(void)
{
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  meter.setColors(TFT_BLACK, TFT_DARKGREEN, 0xFBC0); // FACE-MARKS-NEEDLE

  meter.drawArc(1);
  meter.drawOutline(0);
  meter.drawDigitalValue(true, 1);
  meter.setLongTick(15);
  meter.setShortTick(8);
  meter.drawThickerNeedle(1);

  /*
  meter.setSmallLabel(true, "DIESEL");
  meter.setZones(0, 15, 0, 0, 0, 0, 0, 0); // RED, ORANGE, YELLOW, GREEN
  meter.analogMeter(0, 0, 100, "FUEL", "0", "1/2", "F"); // 3-points meter (new)
  */

 /*
 meter.setSmallLabel(true, "READY");
 meter.setZones(0, 25, 0, 0, 25, 45, 45, 100); // RED, ORANGE, YELLOW, GREEN
 meter.analogMeter(0, 0, 100, "%", "0", "25", "50", "75", "100"); // 5-points meter (legacy)
 */

/*
meter.setSmallLabel(true, "CHARGE");
meter.setZones(80, 100, 0, 0, 0, 0, 0, 0); // RED, ORANGE, YELLOW, GREEN
meter.analogMeter(0, 0, 100, "A", "0", "0.2", "0.4", "0.6", "0.8", "1.0"); // 6-points meter (new)
*/

/*
*/
meter.setSmallLabel(true, "BREAK");
// meter.setZones(0, 0, 0, 0, 0, 7, 0, 0); // RED, ORANGE, YELLOW, GREEN
meter.analogMeter(0, 0, 100, "minutes", "0", "5", "10", "15"); // 4-points meter (new)
}

void loop()
{
  static int d = 0;
  static uint32_t updateTime = 0;

  if (millis() - updateTime >= LOOP_PERIOD)
  {
    updateTime = millis();

    d += 4;
    if (d > 360)
      d = 0;

    // Create a Sine wave for testing, value is in range 0 - 100
    float value = 50.0 + 50.0 * sin((d + 0) * 0.0174532925);

    float current;
    current = mapValue(value, (float)0.0, (float)100.0, (float)-10, (float)110);

    meter.updateNeedle(current, 0);
  }
}

float mapValue(float ip, float ipmin, float ipmax, float tomin, float tomax)
{
  return tomin + (((tomax - tomin) * (ip - ipmin)) / (ipmax - ipmin));
}
