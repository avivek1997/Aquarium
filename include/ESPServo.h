// #include <ESP32Servo.h>

#include<Servo.h>
Servo myServo;
// Servo myservo;  // create servo object to control a servo

// int pos = 0;    // variable to store the servo position
// // Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// #define servoPin D1


void InitializeServo()
{
	myServo.attach(D1); // attaches the servo on pin 18 to the servo object
}