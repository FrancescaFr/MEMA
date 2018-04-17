//METHANE SENSOR TEST CODE --For MEGA

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>

int sensorValue;
int pressureValue;
float pressureVoltage;
int voltageValue;

const uint8_t chipSelect = 53; // set chiptype to SD card FAT32
const uint8_t cardDetect = 49;

void setup(){
  delay(4000);
  pinMode (A0, INPUT); //MQ4 reading
  pinMode(A1, INPUT); // Pressure Sensor
  pinMode(A2, OUTPUT); //SD card failure light (YELLOW)
  pinMode (49, INPUT); //SD card not present light (YELLOW)
  pinMode(A3, OUTPUT); //RTC failure light (RED)
  pinMode(2, OUTPUT); //RTC Power pin
  pinMode(3,OUTPUT); //RelayPowerPin
  pinMode(4,OUTPUT); //Relay1Pin (Gas intake Solenoid)
  pinMode(5, OUTPUT); // Relay2Pin (Circulation Fan)
  pinMode(6, OUTPUT); //Relay3Pin (Air Vent Solenoid/Vent Fan)
  pinMode(7, OUTPUT); //Relay4Pin (Gas Vent Solenoid) 
  digitalWrite (2, HIGH); //RTC Power
  
//   Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("Initializing SD card..."); // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
   Serial.println("Card failed, or not present");
    digitalWrite(A2, HIGH); //fail to initialize indicator light(yellow)
    return;
 }
  digitalWrite(A2,LOW); //turn off failure to initialize light
  Serial.println("card initialized.");
}

void loop() {
int RTCpin = 2;
int RelayPower = 3;
int solenoid1 = 4;
int fan1 = 5;
int solenoid2fan2 = 6;
int solenoid3 = 7;

digitalWrite (RTCpin, HIGH);
digitalWrite (solenoid1,LOW);
digitalWrite (solenoid2fan2,HIGH);
digitalWrite (fan1, HIGH);
digitalWrite (solenoid2fan2,HIGH);
digitalWrite (solenoid3, LOW);

  
  tmElements_t tm; 
  sensorValue = analogRead(A0); // read CH4 sensor value
  pressureValue = analogRead (A1); //read pressure value
  pressureVoltage = ((pressureValue/1023.0)*5.0); //convert pressure reading to a voltage value
  
  String dataString = ""; //start data string for SD storage
  float voltageValue = ((sensorValue/1023.0)*5.0); //convert sensor reading to voltage value
  dataString += ("V: "); 
  dataString += String(voltageValue); //add voltage to data string 
  dataString +=(", ");
  dataString +=("P: ");
  dataString += String(pressureVoltage); //add pressure to data string
  dataString +=(", ");
  
 /* if (RTC.read(tm))  //add date and time to data string
  {
    dataString += (" Time: ");
    dataString += String(tm.Hour);
    dataString +=(':');
    dataString += String(tm.Minute);
    dataString +=(':');
    dataString += String(tm.Second);
    dataString +=(" Date: ");
    dataString += String(tm.Day); 
    dataString +=('/');
    dataString += String(tm.Month);
    dataString +=('/');
    dataString += String(tmYearToCalendar(tm.Year));
    analogWrite(A3,0); //Turn off RTC Error light
  } 
  else {
    if (RTC.chipPresent()) {
      analogWrite (A3,255); //Turn on RTC Error Light (RED)
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      analogWrite (A3,255); //Turn on RTC Error Light
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }*/
  delay(1000);
  //  open the file. note that only one file can be open at a time,
 //  so you have to close this one before opening another.
  File dataFile = SD.open("CH4Log.txt", FILE_WRITE);


 //  if the file is available, write to it:
  if (dataFile) {
   analogWrite (A2, 0); //turn off SD Error light
   dataFile.println(dataString);
    dataFile.close();
    
//print to the serial port too:
    Serial.println(dataString);
 }
 // if the file isn't open, pop up an error:
 else {
  analogWrite (A2, 255); //turn on SD Error light (YELLOW)
    Serial.println("error opening datalog.txt");
 }
 //check if SD card is inserted
int SDcard = digitalRead(cardDetect);  
if (SDcard == 0) { 
   analogWrite(A2, 255); //turn on SD Error light if not present
 }
  
}

