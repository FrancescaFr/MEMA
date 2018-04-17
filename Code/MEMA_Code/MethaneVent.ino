#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>

int sensorValue;
int voltageValue;

const int chipSelect = 4; // set chiptype to SD card FAT32

void setup(){
  pinMode (A0, INPUT); //MQ4 reading
  pinMode(A1, OUTPUT); //SD card Failure light
  pinMode(2, OUTPUT); //heater pin
  pinMode(3,OUTPUT); //RelayPowerPin
  pinMode(4,OUTPUT); //Relay1Pin (Gas intake Solenoid)
  pinMode(5, OUTPUT); // Relay2Pin (Fans)
  pinMode(6, OUTPUT); //Relay3Pin
  pinMode(7, OUTPUT); //Relay4Pin
  
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("Initializing SD card..."); // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    digitalWrite(A1, HIGH); //fail to initialize indicator light
    return;
  }
  digitalWrite(A1,LOW); //turn off failure to initialize light
  Serial.println("card initialized.");
}

void loop() {
int RTCPin = 2;
int solenoid1 = 4;
int fan1 = 5;
int solenoid2 = 6;
int fan2 = 7;
digitalWrite (solenoid1,LOW);
digitalWrite (solenoid2,HIGH);
digitalWrite (RTCPin, HIGH);
digitalWrite (fan1, HIGH);
digitalWrite (fan2, HIGH);
  tmElements_t tm; 
  String dataString = ""; //start data string for SD storage
  sensorValue = analogRead(A0); // read analog input pin 0 (CH4 sensor)
  float voltageValue = ((sensorValue/1023.0)*5.0); //convert sensor reading to voltage value
  dataString += "V: "; 
  dataString +=String(voltageValue); //add voltage to data string 
  //dataString +=(", ");
  
 // Serial.print(voltageValue,4);  // prints the value read
 // Serial.print (" ");

  if (RTC.read(tm)) {
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
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("CH4Log.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  
}

