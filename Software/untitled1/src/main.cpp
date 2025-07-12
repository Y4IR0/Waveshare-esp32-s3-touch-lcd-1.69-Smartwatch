#include <../lib/Adafruit_GFX_Library/Adafruit_GFX.h>    // Core graphics library
#include <../lib/Adafruit_ST7789/Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <time.h>
//#include <SPI.h>

#define TFT_CS         5
#define TFT_RST        8
#define TFT_DC         4
#define TFT_MOSI       7
#define TFT_MISO      -1
#define TFT_SCLK       6
#define TFT_BL         15

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void display1()
{
  // Fill Background
  tft.fillScreen(ST77XX_BLACK);

  // Show Time
  tft.setCursor(17, 80);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(7);

  struct tm timeinfo;

  if (getLocalTime(&timeinfo))
  {
    char timeStr[9];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    tft.print(timeStr);
  }
}


void setTimeFromTM(struct tm timeinfo) {
  struct timeval tv = {0};

  tv.tv_sec = mktime(&timeinfo);
  tv.tv_usec = 0;

  if (settimeofday(&tv, NULL) == 0) {
    // Successful!
  } else {
    // Unsuccessful :(
  }
}


void setup(void)
{
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  #define freq 1000
  #define resolution 8
  #define dutyCycle 50
  ledcSetup(0, freq, resolution);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, dutyCycle);
  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.init(240, 280);
  tft.setRotation(2);
  tft.enableTearing(false);


  // Time
  setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
  tzset();

  struct tm timeinfo = {0, 51, 17, 29, 3-1, 2025-1900};
  setTimeFromTM(timeinfo);
}

void loop()
{
  display1();
  delay(1000/5);
}