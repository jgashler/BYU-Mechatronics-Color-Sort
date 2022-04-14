#define PRINT_SERIAL
#include <Wire.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"

Servo mainServo, RServo, GServo, BServo;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
enum Color {RED, GREEN, BLUE, YELLOW};
uint16_t r, g, b, c;

void setup(void) {
  Serial.begin(9600); 
  if (!tcs.begin()) {
    Serial.println("TCS34725 not found, check connections");
    Serial.println("SDA -> A4, SCL -> A5, GND -> GND, VIN -> 5V");
    Serial.println("Also make sure the connections on the TCS34725 are strong...");
    while (!tcs.begin());
  }
  Serial.println("Found sensor");
  
  //attach servos and set initial positions
  mainServo.attach(6);
  mainServo.write(0);   
  RServo.attach(9);
  RServo.write(90);
  GServo.attach(10);
  GServo.write(0);
  BServo.attach(11);
  BServo.write(0);
  pinMode(13, OUTPUT);
}
 
//main
void loop(void) {
  pingDispenser();
  tcs.getRawData(&r, &g, &b, &c);
  Color color = colorMath(r,g,b);
  dropBall(color);
}

void pingDispenser() {
  digitalWrite(13, HIGH);
  delay(50);                //door open for 50ms
  digitalWrite(13, LOW);
  delay(2000);              //change this delay to wait for the ball to enter box
}

//receive RGB data from color sensor and do some maths to determine which of the four colors we have
Color colorMath(uint16_t red, uint16_t green, uint16_t blue) {
  Color color;
  
  //normalize the color values
  double tot = sqrt((double)(red*red + green*green + blue*blue));
  double r = red/tot;
  double g = green/tot;
  double b = blue/tot;
  
  //based on weights of values, choose the most likely color (as per the data gathered on the whiteboard)
  if (b > r && b > g) color = BLUE;
  else if (r / b > 2 && g / b > 2) color = YELLOW;
  else if (r > b && r > g) color = RED;
  else if (g > r && g > b) color = GREEN;
  else color = RED; //default :(
#ifdef PRINT_SERIAL
  //print data to console for debugging
  Serial.print("R: ");
  Serial.println(r);
  Serial.print("G: ");
  Serial.println(g);
  Serial.print("B: ");
  Serial.println(b);
  Serial.print("Color: ");
  if (color == RED) Serial.println("RED");
  else if (color == GREEN) Serial.println("GREEN");
  else if (color == BLUE) Serial.println("BLUE");
  else if (color == YELLOW) Serial.println("YELLOW");
#endif
  return color;
}

//receive color of ball as argument and open the corresponding door
void dropBall(Color color) {
  //open colored door
  if (color == RED) RServo.write(45);
  else if (color == GREEN) GServo.write(45);
  else if (color == BLUE) BServo.write(45); 
  delay(300);
  
  //wait, then open the main door
  mainServo.write(60);
  delay(300);
  
  //wait, then close all doors
  if (color == RED) RServo.write(90);
  else if (color == GREEN) GServo.write(0);
  else if (color == BLUE) BServo.write(0);
  mainServo.write(0);  
}
