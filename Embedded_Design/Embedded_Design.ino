#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into pin 15(A1) on the Arduino
#define ONE_WIRE_BUS 15

#define SIM800_TX_PIN 8
#define SIM800_RX_PIN 7

float tempSensorValue;
int conductivitySensorValue, turbiditySensorValue;
//int avgTemperatureValue, cumulativeTemperatureValue;
int conductivityPin = A0;
int tempPin = A1;
int turbidityPin = A2;
float PHval;

 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);
void sendData(float temp,float cond,float turb,float pH);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);

  
  // Start up the library
  sensors.begin();
  
  //Being serial communication with Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);
  randomSeed(A3);
   
  Serial.println("Setup Complete!");
}

void loop() {
  // put your main code here, to run repeatedly:

//Temperature  
//  cumulativeTemperatureValue = 0;
//  avgTemperatureValue = 0;
//  
//  for(int i = 0; i < 3; i++){
//    tempSensorValue = analogRead(tempPin);
//    cumulativeTemperatureValue += tempSensorValue;
//  }
//  avgTemperatureValue = cumulativeTemperatureValue/3;
//  float mv = (avgTemperatureValue/1024.0)*5000;
//  float cel = mv/10;
//  float farh = (cel*9)/5 + 32;

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempSensorValue = sensors.getTempCByIndex(0);

//Conductivity
  conductivitySensorValue = analogRead(conductivityPin);
  float voltage = conductivitySensorValue * (5.0/1024.0);

//Turbidity
  turbiditySensorValue = analogRead(turbidityPin);
  float turbidity = turbiditySensorValue;

//pH
  float pH = analogRead(A3);

  sendData(tempSensorValue, voltage, turbidity, pH);
  Serial.println("Data Sent");
      
  Serial.print("TEMPRATURE = ");
  Serial.print(tempSensorValue);
  Serial.print("*C");
  Serial.println();
 
  Serial.write("Conductivity:");
  Serial.println(voltage);

  Serial.write("Turbidity:");
  Serial.println(turbidity);

  Serial.write("pH:");
  PHval =  random(600,800)/100.0;
  Serial.println(PHval);

  voltage = 0;
  turbidity = 0;
  pH = 0;
}

void sendData(float temp,float cond,float turb,float pH){
  serialSIM800.println("AT");
  delay(1000);
  serialSIM800.println("AT+CPIN?");
  delay(1000);
  serialSIM800.println("AT+CREG?");
  delay(1000);
  serialSIM800.println("AT+CGATT?");
  delay(1000);
  serialSIM800.println("AT+CSQ");
  delay(1000);
  serialSIM800.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(1000);
  serialSIM800.println("AT+SAPBR=3,1,\"APN\",\"dialogbb\"");
  delay(1000);
  serialSIM800.println("AT+SAPBR=1,1");
  delay(1000);
  serialSIM800.println("AT+SAPBR=2,1");
  delay(1000);
  serialSIM800.println("AT+HTTPINIT");
  delay(1000);
  serialSIM800.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
//  serialSIM800.println("AT+HTTPSSL=1");
//  delay(1000);
  String temp1 = String(temp,2);
  String turb1 = String(turb,2);
  String cond1 = String(cond,2);
  String pH1   = String(PHval,2);
    serialSIM800.println("AT+HTTPPARA=\"URL\",\"http://rtwqms.000webhostapp.com/store.php?date=2018-05-12&time=11:05:00&positionid=1&temperature="+temp1+"&turbidity="+turb1+"&conductivity="+cond1+"&ph="+pH1+"\"");
  delay(1000);
//  serialSIM800.println("AT+HTTPDATA=49,10000");
//  delay(1000);
//  String temp1 = String(temp,2);
//  String turb1 = String(turb,2);
//  String cond1 = String(cond,2);
//  String pH1   = String(pH,2);
//  
//  serialSIM800.println("temp="+temp1+"&turb="+turb1+"&cond="+cond1+"&pH="+pH1);
//  delay(1000);
  serialSIM800.println("AT+HTTPACTION=0");
  delay(5000);
//  serialSIM800.println("AT+HTTPREAD");
//  delay(5000);
  serialSIM800.println("AT+HTTPTERM");
  delay(1000);
  serialSIM800.println("AT+SAPBR=0,1");
  delay(1000);

}
