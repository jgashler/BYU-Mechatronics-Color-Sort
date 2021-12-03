#include <Wire.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"

Servo mainServo, RServo, GServo, BServo, YServo;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X /*1X, 4X, 16X, 60X*/);

#define PRINT_SERIAL

enum Color {RED, GREEN, BLUE, YELLOW};

#define RUR_BOUND 1
#define RLR_BOUND 1
#define RUG_BOUND 1
#define RLG_BOUND 1
#define RUB_BOUND 1
#define RLB_BOUND 1

#define GUR_BOUND 1
#define GLR_BOUND 1
#define GUG_BOUND 1
#define GLG_BOUND 1
#define GUB_BOUND 1
#define GLB_BOUND 1

#define BUR_BOUND 1
#define BLR_BOUND 1
#define BUG_BOUND 1
#define BLG_BOUND 1
#define BUB_BOUND 1
#define BLB_BOUND 1

#define YUR_BOUND 1
#define YLR_BOUND 1
#define YUG_BOUND 1
#define YLG_BOUND 1
#define YUB_BOUND 1
#define YLB_BOUND 1
 
void setup(void) {
  Serial.begin(9600); 
  if (!tcs.begin()) {
#ifdef PRINT_SERIAL
      Serial.println("TCS34725 not found, check connections");
      Serial.println("SDA -> A4, SCL -> A5, GND -> GND, VIN -> 5V");
      Serial.println("Also make sure the connections on the TCS34725 are strong...");
#endif
      while (!tcs.begin());
    }
  Serial.println("Found sensor");


//#ifndef PRINT_COLOR_DATA
  mainServo.attach(6);
  mainServo.write(0); 
  
  RServo.attach(9);
  RServo.write(90);
  
  GServo.attach(10);
  GServo.write(0);
  
  BServo.attach(11);
  BServo.write(0);
  
//  YServo.attach(5);
//  YServo.write(0);
//#endif

}
 
void loop(void) {
  uint16_t r, g, b, c;
  PING:
  //PING dispenser
  delay(5000);
  tcs.getRawData(&r, &g, &b, &c);
  Color color = colorMath(r,g,b,tcs,0);
  
  //when a color is sensed, dropBall(color)
//  if (color == RED) {
//    dropBall(RServo);
//    goto PING;
//  }
//  else if (color == GREEN) {
//    dropBall(GServo);
//    goto PING;
//  }
//  else if (color == BLUE) {
//    dropBall(BServo);
//    goto PING;
//  }
//  else if (color == YELLOW) {
//    dropBall(YServo);
//    goto PING;
//  }

  dropBall(color);
}

void dropBall(Color color) {
  if (color == RED) RServo.write(45);
  else if (color == GREEN) GServo.write(45);
  else if (color == BLUE) BServo.write(45);
   
  delay(300);
  mainServo.write(60);
  delay(300);

  if (color == RED) RServo.write(90);
  else if (color == GREEN) GServo.write(0);
  else if (color == BLUE) BServo.write(0);

  delay(50);
  mainServo.write(0);
}

Color colorMath(uint16_t red, uint16_t green, uint16_t blue, Adafruit_TCS34725 tcs, int call) {
  ++call;
  if (call > 5) {
    Serial.println("ERROR in colorMath");
    return RED;
  }
  Color color;
  double tot = sqrt((double)(red*red + green*green + blue*blue));
  double r = red/tot;
  double g = green/tot;
  double b = blue/tot;

//  //check to see if the current rbg value falls within the defined bounds of a color
//  if (r > RLR_BOUND && r < RUR_BOUND && g > RLG_BOUND && g < RUG_BOUND && b > RLB_BOUND && b < RUB_BOUND) color = RED;
//  else if (r > GLR_BOUND && r < GUR_BOUND && g > GLG_BOUND && g < GUG_BOUND && b > GLB_BOUND && b < GUB_BOUND) color = GREEN;
//  else if (r > BLR_BOUND && r < BUR_BOUND && g > BLG_BOUND && g < BUG_BOUND && b > BLB_BOUND && b < BUB_BOUND) color = BLUE;
//  else if (r > YLR_BOUND && r < YUR_BOUND && g > YLG_BOUND && g < YUG_BOUND && b > YLB_BOUND && b < YUB_BOUND) color = YELLOW;
  
//  //if it did not scan within a bound, recursive call and retry
//  else {
//    uint16_t nr, ng, nb, nc;
//    tcs.getRawData(&nr, &ng, &nb, &nc);
//    color = colorMath(nr, ng, nb, tcs, call);
//  }

if (b > r && b > g) color = BLUE;
else if (r / b > 2 && g / b > 2) color = YELLOW;
else if (r > b && r > g) color = RED;
else if (g > r && g > b) color = GREEN;
  else {
    uint16_t nr, ng, nb, nc;
    tcs.getRawData(&nr, &ng, &nb, &nc);
    color = colorMath(nr, ng, nb, tcs, call);
  }

#ifdef PRINT_SERIAL
  //print data to serial
  Serial.print("R: ");
  Serial.println(r);
  Serial.print("G: ");
  Serial.println(g);
  Serial.print("B: ");
  Serial.println(b);
  Serial.print("Color: ");
  switch(color) {
    case RED :
      Serial.println("RED");
      break;
    case GREEN :
      Serial.println("GREEN");
      break;
    case BLUE :
      Serial.println("BLUE");
      break;
    case YELLOW :
      Serial.println("YELLOW");
      break;
  }
#endif
  return color;
}
