#include <Wire.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"

Servo mainServo, RServo, GServo, BServo, YServo;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X /*1X, 4X, 16X, 60X*/);

//#define PRINT_COLOR_DATA
 
void setup(void) {
  Serial.begin(9600); 
  if (!tcs.begin()) {
      Serial.println("TCS34725 not found, check connections");
      Serial.println("SDA -> A4, SCL -> A5, GND -> GND, VIN -> 5V");
      Serial.println("Also make sure the connections on the TCS34725 are strong...");
      while (!tcs.begin());
    }
  Serial.println("Found sensor");

#ifndef PRINT_COLOR_DATA
  mainServo.attach(6);
  mainServo.write(0); 
  RServo.attach(9);
  RServo.write(0);
  GServo.attach(10);
  GServo.write(0);
  BServo.attach(11);
  BServo.write(0);
  YServo.attach(5);
  YServo.write(0);
#endif

}
 
void loop(void) {
  uint16_t r, g, b, c;

#ifdef PRINT_COLOR_DATA
   uint16_t colorTemp, lux;
  while(1) {
    tcs.getRawData(&r, &g, &b, &c);
    if (r + g + b + c != 0) {
      colorTemp = tcs.calculateColorTemperature(r, g, b);
      lux = tcs.calculateLux(r, g, b);
      Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
      Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
      Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
      Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
      Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
      Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
      Serial.println(" ");
    }
    else {
      Serial.println("Lost signal");
      while(1);
    }
  }
#endif


  //PING dispenser

  while(1){
    tcs.getRawData(&r, &g, &b, &c);
  
    //when a color is sensed, dropBall(color)
    if (r > 200 && r > g && r > b){
      Serial.println("RED DETECTED");
      Serial.println("open red door");
      dropBall(RServo);
      continue;
    }
    else if (g > 200 && g > r && g > b){
      Serial.println("GREEN DETECTED");
      Serial.println("open green door");
      dropBall(GServo);
      continue;
    }
    else if (b > 200 && b > r && b > g){
      Serial.println("BLUE DETECTED");
      Serial.println("open blue door");
      dropBall(BServo);
      continue;
    }
    //TODO setup a system to distinguish yellow from green, then add YServo.
    //     this will require some calibration for the TCS34725
  }

}

void dropBall(Servo color) {
  color.write(200);
  delay(1000);
  Serial.println("open main door");
  mainServo.write(200);
  delay(1500);
  Serial.println("close all doors");
  color.write(0);
  mainServo.write(0);
  //PING dispenser
}


//  tcs.getRawData(&r, &g, &b, &c);
//  colorTemp = tcs.calculateColorTemperature(r, g, b);
//  lux = tcs.calculateLux(r, g, b);
//  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
//  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
//  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
//  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
//  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
//  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
//  Serial.println(" ");
