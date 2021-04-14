#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

class vector {
  public:
    float X;
    float Y;
    vector(float x, float y);
};
vector::vector(float x, float y) {
  X = x;
  Y = y;
}

//values
int width = 45;
int height = 45;
int x_offset = 0;
int y_offset = 0;
float val = 0;
float rotate_speed = 0.5f; 
float inclination = 0.3f;
vector main_point(SCREEN_WIDTH/2, 0);

float Lerp(float firstFloat, float secondFloat, float by)
{
  return firstFloat * (1 - by) + secondFloat * by;
}

vector GetPoint(float val, int x_offset, int y_offset, int width = 200, int height = 50) {
  float y = sin(val) * (float)height;
  float x = cos(val) * (float)width;
  vector result(x + x_offset, y + y_offset);
  return result;
}

vector getPointOnShape(float x, float y, float val){
  int radius = Lerp(width, 0, y);
  vector p1 = GetPoint(val, SCREEN_WIDTH/2, Lerp(height, 0, y), radius, radius*inclination);
  vector p2 = GetPoint(val + 1.5f, SCREEN_WIDTH/2, Lerp(height, 0, y), radius, radius*inclination);
  vector result(Lerp(p1.X, p2.X, x), Lerp(p1.Y, p2.Y, x));
  Serial.print(Lerp(p1.X, p2.X, x));
  Serial.print("|");
  Serial.println(Lerp(p1.Y, p2.Y, y));
  return result;
}

void DrawLine(vector a, vector b) {
  display.drawLine(a.X, a.Y, b.X, b.Y, SSD1306_WHITE);
}


void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.display();
  delay(1000);
}

void loop() {
  val += rotate_speed;

  vector p1 = GetPoint(val, SCREEN_WIDTH/2, height, width, inclination*width);
  vector p2 = GetPoint(val + 1.5f, SCREEN_WIDTH/2, height, width, inclination*width);
  vector p3 = GetPoint(val + 3, SCREEN_WIDTH/2, height, width, inclination*width);
  vector p4 = GetPoint(val + 4.5f, SCREEN_WIDTH/2, height, width, inclination*width);

  display.clearDisplay();

  DrawLine(main_point, p1);
  DrawLine(main_point, p2);
  DrawLine(main_point, p3);
  DrawLine(main_point, p4);

  DrawLine(p4, p1);
  DrawLine(p1, p2);
  DrawLine(p2, p3);
  DrawLine(p3, p4);

  DrawLine(getPointOnShape(0, 0.5f, val), getPointOnShape(1, 0.5f, val));
  DrawLine(getPointOnShape(0, 0.5f, val + 1.5f), getPointOnShape(0.5f, 0.5f, val + 1.5f));
  DrawLine(getPointOnShape(1, 0.5f, val - 1.5f), getPointOnShape(0.5f, 0.5f, val - 1.5f));

  DrawLine(getPointOnShape(0.1f, 0.5f, val), getPointOnShape(0.15f, 0.45f, val));
  DrawLine(getPointOnShape(0.15f, 0.45f, val), getPointOnShape(0.30f, 0.45f, val));
  DrawLine(getPointOnShape(0.30f, 0.45f, val), getPointOnShape(0.35f, 0.5f, val));
  
  DrawLine(getPointOnShape(1 - 0.1f, 0.5f, val), getPointOnShape(1 - 0.15f, 0.45f, val));
  DrawLine(getPointOnShape(1 - 0.15f, 0.45f, val), getPointOnShape(1 - 0.30f, 0.45f, val));
  DrawLine(getPointOnShape(1 - 0.30f, 0.45f, val), getPointOnShape(1 - 0.35f, 0.5f, val));
  
  DrawLine(getPointOnShape(0.45f, 0.45f, val), getPointOnShape(0.4f, 0.4f, val));
  DrawLine(getPointOnShape(1 - 0.45f, 0.45f, val), getPointOnShape(1 - 0.4f, 0.4f, val));
  DrawLine(getPointOnShape(0.4f, 0.4f, val), getPointOnShape(0.6f, 0.4f, val));
    
  DrawLine(getPointOnShape(0.2f, 0.3f, val), getPointOnShape(0.8f, 0.3f, val));
  DrawLine(getPointOnShape(0.16f, 0.35f, val), getPointOnShape(0.10f, 0.25f, val));
  DrawLine(getPointOnShape(1 - 0.16f, 0.35f, val), getPointOnShape(1 - 0.10f, 0.25f, val));
  
  display.display();
  delay(50);
}
