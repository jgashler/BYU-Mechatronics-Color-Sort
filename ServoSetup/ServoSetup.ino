#include <Servo.h>

Servo s1, s2, s3, s4;
//    M   R   G   B

//#define OPEN

//MAIN = 6
//BLUE = 11
//RED = 9
//GREEN = 10


void setup() {
  // put your setup code here, to run once:
  s1.attach(6);
//  s2.attach(9);
//  s3.attach(10);
  s4.attach(11);

#ifndef OPEN
  s1.write(0);
  s2.write(90);
  s3.write(0);
  s4.write(0);
#endif

#ifdef OPEN
  s1.write(60);
  s2.write(45);
  s3.write(45);
  s4.write(45);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
