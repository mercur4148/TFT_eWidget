#include "Arduino.h"
#include "Meter.h"

// #########################################################################
// Meter constructor
// #########################################################################
MeterWidget::MeterWidget(TFT_eSPI *tft)
{
  ltx = 0;              // Saved x coord of bottom of needle
  osx = 120, osy = 120; // Saved x & y coords
  old_analog = -999;    // Value last displayed
  old_digital = -999;   // Value last displayed

  mx = 0;
  my = 0;

  factor = 1.0;
  scaleStart = 0.0;

  mlabel[8] = '\0';

  // Defaults
  strncpy(ms0, "\0", 4);
  strncpy(ms1, "\0", 4);
  strncpy(ms2, "\0", 4);
  strncpy(ms3, "\0", 4);
  strncpy(ms4, "\0", 4);

  redStart = 0;
  redEnd = 0;
  orangeStart = 0;
  orangeEnd = 0;
  yellowStart = 0;
  yellowEnd = 0;
  greenStart = 0;
  greenEnd = 0;

  _text_color = TFT_BLACK;
  _background_color = TFT_WHITE;
  _needle_color = TFT_RED;
  _outline_color = TFT_GREY;
  _bezel_color = TFT_BLACK;

  _draw_outline = 1;
  _draw_digital = 1;
  _digital_precision = 1;
  _draw_small_label = 1;
  _draw_arc = 1;
  _draw_bezel = 1;
  _draw_thicker_needle = 0;

  _long_tick_length = 15;
  _short_tick_length = 8;

  slabel[8] = '\0';

  ntft = tft;
}

// #########################################################################
// Draw meter meter at x, y and define full scale range & the scale labels
// #########################################################################
// 3 points meter
void MeterWidget::analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s2, const char *s4)
{
  analogMeter(x, y, 0.0, fullScale, units, s0, s2, s4);
}
void MeterWidget::analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s2, const char *s4)
{
  _num_points = 3;
  strncpy(ms0, s0, 4);
  strncpy(ms2, s2, 4);
  strncpy(ms4, s4, 4);
  drawMeterFace(x, y, startScale, endScale, units);
}
// 5 points meter
void MeterWidget::analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4)
{
  analogMeter(x, y, 0.0, fullScale, units, s0, s1, s2, s3, s4);
}
void MeterWidget::analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4)
{
  _num_points = 5;
  strncpy(ms0, s0, 4);
  strncpy(ms1, s1, 4);
  strncpy(ms2, s2, 4);
  strncpy(ms3, s3, 4);
  strncpy(ms4, s4, 4);
  drawMeterFace(x, y, startScale, endScale, units);
}
// 6 points meter
void MeterWidget::analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4, const char *s5)
{
  analogMeter(x, y, 0.0, fullScale, units, s0, s1, s2, s3, s4, s5);
}
void MeterWidget::analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4, const char *s5)
{
  _num_points = 6;
  strncpy(ms0, s0, 4);
  strncpy(ms1, s1, 4);
  strncpy(ms2, s2, 4);
  strncpy(ms3, s3, 4);
  strncpy(ms4, s4, 4);
  strncpy(ms5, s5, 4);
  drawMeterFace(x, y, startScale, endScale, units);
}

// Centralized face drawing logic
void MeterWidget::drawMeterFace(uint16_t x, uint16_t y, float startScale, float endScale, const char *units)
{
  mx = x;
  my = y;
  factor = 100.0 / (endScale - startScale);
  scaleStart = startScale;

  strncpy(mlabel, units, 8);

  // Meter outline
  if (_draw_outline)
    ntft->fillRect(x, y, 239, 126, _outline_color);
  ntft->fillRect(x + 5, y + 3, 230, 119, _background_color);
  ntft->setTextColor(_text_color);

  constexpr uint8_t _full_swing_angle = 100;

  // Draw ticks every 5 degrees from -50 to +50 degrees (for 100 deg. FSD swing)
  for (int8_t i = -(_full_swing_angle / 2); i <= (_full_swing_angle / 2) + 1; i += 5)
  {
    // Long scale tick length
    int8_t tick_length = _long_tick_length;

    // Coordinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = x + sx * (100 + tick_length) + 120;
    uint16_t y0 = y + sy * (100 + tick_length) + 140;
    uint16_t x1 = x + sx * 100 + 120;
    uint16_t y1 = y + sy * 100 + 140;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = x + sx2 * (100 + tick_length) + 120;
    int y2 = y + sy2 * (100 + tick_length) + 140;
    int x3 = x + sx2 * 100 + 120;
    int y3 = y + sy2 * 100 + 140;

    // Red zone limits
    if (redEnd > redStart)
    {
      if (i >= redStart && i < redEnd)
      {
        ntft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_RED);
        ntft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_RED);
      }
    }

    // Orange zone limits
    if (orangeEnd > orangeStart)
    {
      if (i >= orangeStart && i < orangeEnd)
      {
        ntft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_ORANGE);
        ntft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_ORANGE);
      }
    }

    // Yellow zone limits
    if (yellowEnd > yellowStart)
    {
      if (i >= yellowStart && i < yellowEnd)
      {
        ntft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
        ntft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
      }
    }

    // Green zone limits
    if (greenEnd > greenStart)
    {
      if (i >= greenStart && i < greenEnd)
      {
        ntft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN);
        ntft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);
      }
    }

    // Short scale tick length
    uint8_t long_ticks_interval = 0;
    uint8_t offset = 0;
    uint8_t divisor = 1;

    if (_num_points == 5)
      long_ticks_interval = (_full_swing_angle / 4);
    else if (_num_points == 3)
    {
      long_ticks_interval = (_full_swing_angle / 2);
      divisor = 2;
    }
    else if (_num_points == 6)
    {
      long_ticks_interval = (_full_swing_angle / 5);
      offset = 10;
    }

    if (abs(i) % long_ticks_interval - offset != 0)
      tick_length = _short_tick_length;

    // Recalculate coords in case tick length changed
    if (tick_length > 0) // draw short ticks only if length is more than 0
    {
      x0 = x + sx * (100 + tick_length) + 120;
      y0 = y + sy * (100 + tick_length) + 140;
      x1 = x + sx * 100 + 120;
      y1 = y + sy * 100 + 140;

      // Draw tick
      ntft->drawLine(x0, y0, x1, y1, _text_color);
    }

    // Check if labels should be drawn, with position tweaks
    if (_num_points != 6)
    {
      if (i % (long_ticks_interval / divisor) == 0)
      {
        // Calculate label positions
        x0 = x + sx * (100 + tick_length + 10) + 120;
        y0 = y + sy * (100 + tick_length + 10) + 140;
        switch (i)
        {
        case -50:
          ntft->drawCentreString(ms0, x0, y0 - 12, 2);
          break;
        case -25:
          ntft->drawCentreString(ms1, x0, y0 - 9, 2);
          break;
        case 0:
          ntft->drawCentreString(ms2, x0, y0 - 6, 2);
          break;
        case 25:
          ntft->drawCentreString(ms3, x0, y0 - 9, 2);
          break;
        case 50:
          ntft->drawCentreString(ms4, x0, y0 - 12, 2);
          break;
        }
      }
    }
    else
    {
      if (abs(i) % long_ticks_interval - offset == 0)
      {
        // Calculate label positions
        x0 = x + sx * (100 + tick_length + 10) + 120;
        y0 = y + sy * (100 + tick_length + 10) + 140;
        switch (i)
        {
        case -50:
          ntft->drawCentreString(ms0, x0, y0 - 12, 2);
          break;
        case -30:
          ntft->drawCentreString(ms1, x0, y0 - 9, 2);
          break;
        case -10:
          ntft->drawCentreString(ms2, x0, y0 - 6, 2);
          break;
        case 10:
          ntft->drawCentreString(ms3, x0, y0 - 6, 2);
          break;
        case 30:
          ntft->drawCentreString(ms4, x0, y0 - 9, 2);
          break;
        case 50:
          ntft->drawCentreString(ms5, x0, y0 - 12, 2);
          break;
        }
      }
    }

    // Now draw the arc of the scale
    if (_draw_arc)
    {
      sx = cos((i + 5 - 90) * 0.0174532925);
      sy = sin((i + 5 - 90) * 0.0174532925);
      x0 = x + sx * 100 + 120;
      y0 = y + sy * 100 + 140;
      // Draw scale arc, don't draw the last part
      if (i < (_full_swing_angle / 2))
      {
        ntft->drawLine(x0, y0, x1, y1, _text_color);
      }
    }
  }

  if (_draw_small_label)
    ntft->drawRightString(slabel, x + 230, y + 102, 2); // Small label for additional info

  ntft->drawCentreString(mlabel, x + 120, y + 70, 4); // Comment out to avoid font 4

  if (_draw_bezel)
    ntft->drawRect(x + 5, y + 3, 230, 119, _bezel_color); // Draw bezel line

  updateNeedle(0, 0);
}
// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void MeterWidget::updateNeedle(float val, uint32_t ms_delay)
{
  int value = (val - scaleStart) * factor;

  if (_draw_digital)
  {
    ntft->setTextColor(_text_color, _background_color);
    char buf[8];
    dtostrf(val, 5, _digital_precision, buf);

    ntft->drawCentreString(buf, mx + 30, my + 102, 2);
  }

  if (value < -5)
    value = -5; // Limit value to emulate needle end stops
  if (value > 105)
    value = 105;

  // Move the needle until new value reached
  while (value != old_analog)
  {
    if (old_analog < value)
      old_analog++;
    else
      old_analog--;

    if (ms_delay == 0)
      old_analog = value; // Update immediately id delay is 0

    float sdeg = map(old_analog, -10, 110, -150, -30); // 100 degrees swing +/- additional 10 degrees
    float rad = sdeg * 0.0174532925;                   // Current needle angle in radians

    // Calculate tip of needle coords
    float sx = cos(rad);
    float sy = sin(rad);
    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    for (int i = -2; i <= 2; i++)
    {
      ntft->drawLine(mx + 120 + 20 * ltx + i, my + 140 - 20, mx + osx + i, my + osy, _background_color);
    }

    // Re-plot text under needle
    ntft->setTextColor(_text_color);
    ntft->drawCentreString(mlabel, mx + 120, my + 70, 4); // // Comment out to avoid font 4

    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 98 + 120;
    osy = sy * 98 + 140;

    // Draw the needle in the new position
    // draws 3 or 5 lines lines to thicken the needle
    int8_t thickness = _draw_thicker_needle ? 2 : 1;
    for (int8_t i = -thickness; i <= thickness; i++)
    {
      ntft->drawLine(mx + 120 + 20 * ltx + i, my + 140 - 20, mx + osx + i, my + osy, _needle_color);
    }

    // Slow needle down slightly as it approaches new position
    if (abs(old_analog - value) < 10)
      ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
}

// #########################################################################
// Set red, orange, yellow and green start+end zones as a % of full scale
// #########################################################################
void MeterWidget::setZones(uint8_t rs, uint8_t re, uint8_t os, uint8_t oe, uint8_t ys, uint8_t ye, uint8_t gs, uint8_t ge)
{
  // Meter scale is -50 to +50
  redStart = rs - 50;
  redEnd = re - 50;
  orangeStart = os - 50;
  orangeEnd = oe - 50;
  yellowStart = ys - 50;
  yellowEnd = ye - 50;
  greenStart = gs - 50;
  greenEnd = ge - 50;
}

void MeterWidget::setColors(uint32_t bg_col, uint32_t text_col, uint32_t needle_col)
{
  // Colors
  _background_color = bg_col;
  _text_color = text_col;
  _needle_color = needle_col;
}
void MeterWidget::drawOutline(bool draw_outline)
{
  _draw_outline = draw_outline;
  _outline_color = TFT_GREY;
  _bezel_color = TFT_BLACK;
}
void MeterWidget::drawOutline(bool draw_outline, uint32_t outline_col, uint32_t bezel_col)
{
  _draw_outline = draw_outline;
  _outline_color = outline_col;
  _bezel_color = bezel_col;
}
void MeterWidget::drawDigitalValue(bool draw_digital, uint8_t precision)
{
  _draw_digital = draw_digital;
  _digital_precision = precision;
}
void MeterWidget::drawArc(bool draw_arc)
{
  _draw_arc = draw_arc;
}
void MeterWidget::setLongTick(uint8_t long_tick_length)
{
  _long_tick_length = long_tick_length;
}
void MeterWidget::setShortTick(uint8_t short_tick_length)
{
  _short_tick_length = short_tick_length;
}
void MeterWidget::setSmallLabel(bool draw_small_label, const char *label)
{
  strncpy(slabel, label, 8);
  _draw_small_label = draw_small_label;
}
void MeterWidget::drawThickerNeedle(bool draw_thicker)
{
  _draw_thicker_needle = draw_thicker;
}