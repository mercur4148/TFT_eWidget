
#ifndef Meter_h
#define Meter_h

#include "Arduino.h"
#include "TFT_eSPI.h"

// Meter boarder colour
#define TFT_GREY 0x5AEB

// Meter class
class MeterWidget
{
  public:
    MeterWidget(TFT_eSPI* tft);

    // Set red, orange, yellow and green start+end zones as a percentage of full scale
    void setZones(uint8_t rs, uint8_t re, uint8_t os, uint8_t oe, uint8_t ys, uint8_t ye, uint8_t gs, uint8_t ge);
	
	// Draw 3 points meter meter at x, y and define full scale range plus the scale labels
    void analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s2, const char *s4);
    // Draw 3 points meter meter at x, y, define both start and end points plus the scale labels
    void analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s2, const char *s4);
	// Draw 4 points meter meter at x, y and define full scale range plus the scale labels
	void analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3);
	// Draw 4 points meter meter at x, y, define both start and end points plus the scale labels
	void analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3);
	// Draw 5 points meter meter at x, y and define full scale range plus the scale labels
    void analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4);
    // Draw 5 points meter meter at x, y, define both start and end points plus the scale labels
    void analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4);
	// Draw 6 points meter meter at x, y and define full scale range plus the scale labels
	void analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4, const char *s5);
	// Draw 6 points meter meter at x, y, define both start and end points plus the scale labels
	void analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4, const char *s5);
	
    // Move needle to new position
    void updateNeedle(float value, uint32_t ms_delay);
	
	// Optional settings; if you don't call them, the meter looks like the original code
	// Set meter colors: background, text labels and needle; default: setColors(TFT_WHITE, TFT_BLACK, TFT_RED);
	void setColors(uint32_t bg_col, uint32_t text_col, uint32_t needle_col);
	// Enable or disable rectangular meter outline; default: drawOutline(true);
	void drawOutline(bool draw_outline);
	// Ditto, but with outline (wide line) and bezel (1 px) colours; default: drawOutline(true, TFT_GREY, TFT_BLACK);
	void drawOutline(bool draw_outline, uint32_t outline_col, uint32_t bezel_col);
	// Enable or disable digital value in the lower left corner and adjust precision (can be set to 0 if you don't want fractions); default: drawDigitalValue(true, 1);
	void drawDigitalValue(bool draw_digital, uint8_t precision);
	// Enable or disable digital value in the lower left corner; default: drawDigitalValue(true);
	void drawDigitalValue(bool draw_digital);
	// Enable or disable arc drawing; default: drawArc(true);
	void drawArc(bool draw_arc);
	// Set "long tick" length; default: setLongTick(15);
	void setLongTick(uint8_t long_tick_length);
	// Set "short tick" length; default: setShortTick(8);
	void setShortTick(uint8_t short_tick_length);
	// Define additional label in the lower right corner (e.g. "CHARGE"); default: setSmallLabel(true, "");
	void setSmallLabel(bool draw_small_label, const char *label);
	// Draw 5-px needle instead of 3-px for better visibility; default: drawThickerNeedle(false);
	void drawThickerNeedle(bool draw_thicker);
	

  private:
    // Pointer to TFT_eSPI class functions
    TFT_eSPI* ntft;

    float ltx;         // x delta of needle start
    uint16_t osx, osy; // Saved x & y coords of needle end
    int old_analog;    // Value last displayed
    int old_digital;   // Value last displayed

    // x, y coord of top left corner of meter graphic
    uint16_t mx;
    uint16_t my;

    // Scale factor
    float factor;
    float scaleStart;

    // Scale label
    char     mlabel[9];

    // Scale values
    char     ms0[5];
    char     ms1[5];
    char     ms2[5];
    char     ms3[5];
    char     ms4[5];
	char 	 ms5[5];
    uint8_t _num_points;

    // Scale colour zone start end end values
    int8_t redStart;
    int8_t redEnd;
    int8_t orangeStart;
    int8_t orangeEnd;
    int8_t yellowStart;
    int8_t yellowEnd;
    int8_t greenStart;
    int8_t greenEnd;
	
	// User colors
	uint32_t _background_color;
	uint32_t _text_color;
	uint32_t _needle_color;
	uint32_t _outline_color;
	uint32_t _bezel_color;
	
	// Appearance
	bool _draw_outline;
	bool _draw_bezel;
	
	bool _draw_arc;
	bool _draw_thicker_needle;
	
	bool _draw_digital;
	uint8_t _digital_precision;
	
	bool _draw_small_label;
    char slabel[9];
		
	uint8_t _long_tick_length;
	uint8_t _short_tick_length;
	
	void drawMeterFace(uint16_t x, uint16_t y, float startScale, float endScale, const char *units);
};

#endif
