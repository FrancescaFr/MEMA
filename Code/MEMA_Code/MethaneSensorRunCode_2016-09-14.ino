//METHANE SENSOR RUN CODE --For MEGA

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>

int sensorValue;
int pressureValue;
float pressureVoltage;
int voltageValue;
int RTCpin = 2;
int RelayPower = 3;
int solenoid1 = 4;
int fan1 = 5;
int solenoid2fan2 = 6;
int solenoid3 = 8;

String dataString; //start data string for SD storage

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
  pinMode(8, OUTPUT); //Relay4Pin (Gas Vent Solenoid) 
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
digitalWrite (solenoid1,LOW);
digitalWrite (solenoid3,HIGH); // open exhaust port for submersion
 //delay(900000); // wait 'till submersed
}

void loop() {
digitalWrite (RTCpin, HIGH);
digitalWrite (solenoid1,LOW);
digitalWrite (solenoid2fan2,LOW);
digitalWrite (fan1, LOW);
digitalWrite (solenoid2fan2,LOW);
digitalWrite (solenoid3, LOW);

 dataString = getdata();

  delay(1000);
  //  open the file. note that only one file can be open at a time,
 //  so you have to close this one before opening another.
  File dataFile = SD.open("CH4Log.txt", FILE_WRITE);
 analogWrite (A2,255);
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

if (pressureVoltage > 1.04) //presure indicates collection of ~10ml of gas
{
  delay(500);
   pressureValue = analogRead (A1); //read pressure value
  pressureVoltage = ((pressureValue/1023.0)*5.0); //convert pressure reading to a voltage value
  if (pressureVoltage > 1.04)
  {
  delay (500);
pressureValue = analogRead (A1); //read pressure value
  pressureVoltage = ((pressureValue/1023.0)*5.0); //convert pressure reading to a voltage value
 if (pressureVoltage > 1.04)
  {
  float baseVoltage1 = (((analogRead(A0))/1023.0)*5.0); //read base sensor value
  float initialpressure = (((analogRead(A1))/1023.0)*5.0); //measure initial pressure again
  digitalWrite (solenoid1, HIGH); //inject part of gas into chamber
  delay (500);
  digitalWrite (solenoid1, LOW);
  delay (2000);
  float finalpressure = (((analogRead(A1))/1023.0)*5.0); //re-measure gas
  digitalWrite (solenoid3, HIGH); //vent residual gas
  delay (10000);
  digitalWrite (solenoid3, LOW);
  float residualpressure = (((analogRead(A1))/1023.0)*5.0); // measure true residual gas
  String datareadout = ("INJECTION! ");
  datareadout += (" Inital Pressure: ");
  datareadout += String(initialpressure);
  datareadout += (", ");
  datareadout += (" Final Pressure: ");
  datareadout += String(finalpressure);
  datareadout += (", ");
  datareadout += (" Residual Pressure: ");
  datareadout += String(residualpressure);
  datareadout += (", ");
  digitalWrite(fan1, HIGH); // turn on circulation fan
  delay (1800); //wait for sensor to reach maximum conductance
  float peakVoltage = (((analogRead(A0))/1023.0)*5.0); //read peak sensor value
  digitalWrite (solenoid2fan2, HIGH);
  delay (600000);
  float baseVoltage2 = (((analogRead(A0))/1023.0)*5.0); //read base sensor value after venting
  if (baseVoltage2 > baseVoltage1) //vent longer if needed
  {
  delay (240000);
  }
  digitalWrite (solenoid2fan2, LOW); //stop venting
  
  digitalWrite(fan1, LOW); //turn off circulation fan
  baseVoltage2 = (((analogRead(A0))/1023.0)*5.0); //read base sensor value after venting
  
  datareadout += (" Peak Voltage: ");
  datareadout += String(peakVoltage);
   datareadout += (", ");
  datareadout += (" Base Voltage 1: ");
  datareadout += String(baseVoltage1);
   datareadout += (", ");
  datareadout += (" Base Voltage 2: ");
  datareadout += String(baseVoltage2);

  Serial.println(datareadout); // Print measurements
  
 File dataFile = SD.open("CH4Log.txt", FILE_WRITE); // save measurements to file
  if (dataFile) {
   analogWrite (A2, 0); //turn off SD Error light
   dataFile.println(datareadout);
    dataFile.close();
        }

      }
    }
  }
}
String getdata()
{

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

  /*unsigned long time = (millis()/1000);
  
  dataString += ("Time: ");
  dataString += String(time); */
        
  if (RTC.read(tm))  //add date and time to data string
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
    } 
    else {
      analogWrite (A3,255); //Turn on RTC Error Light
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
 }
 return dataString;
}

