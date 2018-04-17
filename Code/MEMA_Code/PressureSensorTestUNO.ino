#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>

int PressureValue1;
int PressureValue2;

void setup(){
  delay(4000);
  pinMode (A2, INPUT); // Pressure Reading +
  pinMode (A3, INPUT); //Pressure Reading -
   

  
//   Open serial communications and wait for port to open:
  Serial.begin(9600);
}

void loop() {
 // analogWrite(A3,120);
  String dataString = ""; //start data string for SD storage
 PressureValue1 = analogRead(A2); // read analog input pin 2 (PressureSensor)
  float voltageValue1 = ((PressureValue1/1023.0)*5.0); //convert sensor reading to voltage value
  PressureValue2 = analogRead(A3); // read analog input pin 2 (PressureSensor)
 float voltageValue2 = ((PressureValue2/1023.0)*5.0); //convert sensor reading to voltage valuedataString += "V: "; 
 dataString +=String(voltageValue1); //add voltage to data string 
  dataString +=(", ");
  Serial.print(voltageValue1,4);  // prints the value read
 Serial.print(" , ");
  Serial.println(voltageValue2,4);
delay(500);
 }
 
 
  


