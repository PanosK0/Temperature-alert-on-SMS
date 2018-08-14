/*Temp_To_SMS_v.1.05
Temperature alert on SMS.
With this project you can receive SMS on your phone when a temperature is high.
For example you want to get alerted when the temperature of your freezer is higher than  -10°C or the temperature of the computer room is higher than  40° C.
• The system reads the temperature from two diferent DHT22 sensors and send SMS if a constant temperature is reached. 
• Then sends an SMS when the temperature is lowered to normal levels. 
• Sends SMS on a missed call
Reqirements: Arduino nano, A6 gprs gsm module, 2 DHT22 sensors and a led with a resistor.
By PanosK0

*/

#include "DHT.h"

#define DHT01PIN 2     // what digital pin we're connected to DHT 22
#define DHT02PIN 3     // what digital pin we're connected to DHT 11
const int ledPin = 7;
const float setTemp = 31.8; //Here yoy set the desired temperature in this case is set to 31.8
float h;
float t;
float hic;
float h2;
float t2;
float hic2;
float tHold;
float tHight;  // this variable is for future use 

String phone = "+1234567891011";  //Here you set your phome number 
String incoming;



#define DHTTYPE2 DHT22  
#define DHTTYPE1 DHT22  


// Initialize DHT sensor.
DHT dht1(DHT01PIN, DHTTYPE1);
DHT dht2(DHT02PIN, DHTTYPE2);

void setup() {
 Serial.begin(115200); // to A6 module

 pinMode(ledPin, OUTPUT);
  dht1.begin();
  dht2.begin();
  tHold = setTemp -0.5; // Here you should adjust your normal temperature lever
  tHight = setTemp +2 ;  // this variable is for future use
  delay(16000);

}

void send_SMS() {
    // turn LED on:
    digitalWrite(ledPin, HIGH);

  //send SMS
    Serial.println("AT+CMGF=1");    
    delay(200);
    Serial.println("AT+CMGS=\"" + phone + "\"\r");
    delay(200);
    if (isnan(t)) {
       Serial.println("Failed to read from sensor 1");
       } 
       else {
          Serial.print("Sensor 1: Temperature: ");
          Serial.print(t);
          Serial.print(" *C ");
          Serial.print("Humidity: ");
          Serial.print(h);
          Serial.print(" % ");
          Serial.print("Heat index: ");
          Serial.print(hic);         
          Serial.println(" *C ");
          }
    if (isnan(t2)) {
       Serial.println("Failed to read from sensor 2");
       } 
       else {
          Serial.print("Sensor 2: Temperature: ");
          Serial.print(t2);
          Serial.print(" *C ");
          Serial.print("Humidity: ");
          Serial.print(h2);
          Serial.print(" % ");
          Serial.print("Heat index: ");
          Serial.print(hic2);
          Serial.println(" *C ");
          }
  
    Serial.println (char(26)); // ctrl-z end of message

    delay(800);
  // turn LED off:
    digitalWrite(ledPin, LOW);
    delay(15000);
  }

  
void read_Sensors(){

  
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading sensor 1 temperature or humidity takes about 250 milliseconds!
  h = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht1.readTemperature();
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht1.computeHeatIndex(t, h, false);
  // Reading sensor 2
  h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  t2 = dht2.readTemperature();
  // Compute heat index in Celsius (isFahreheit = false)
  hic2 = dht2.computeHeatIndex(t, h, false);
  }

  
void send_SMS_normal(){
  
  // Sends SMS "Back to normal" when the temperature drops to normal level

        // turn LED on:
          digitalWrite(ledPin, HIGH);
        //send SMS
          Serial.println("AT+CMGF=1");    
          delay(200);
          Serial.println("AT+CMGS=\"" + phone + "\"\r");
          delay(200);
          Serial.println("Back to normal.");
          if (isnan(t)) {
             Serial.println("Failed to read from sensor 1");
             } 
             else {
                Serial.print("Sensor 1:  Temperature: ");
                Serial.print(t);
                Serial.println(" *C ");
                }
          if (isnan(t2)) {
             Serial.println("Failed to read from sensor 2");
             } 
             else {
                Serial.print("Sensor 2: Temperature: ");
                Serial.print(t2);
                Serial.print(" *C ");         
                }
          Serial.println (char(26)); // ctrl-z end of message
          delay(800);
         // turn LED off:
          digitalWrite(ledPin, LOW);
  }


  
void loop() {

 read_Sensors();
  
 if (t >= setTemp || t2 >= setTemp) {

    send_SMS();
  
    while (t >= tHold || t2 >= tHold) {
        read_Sensors();

        //send sms if a misscall
        if (Serial.available() > 0) {
        incoming = Serial.readString();
        }
           if (incoming.indexOf("RING")>=0){
           Serial.println("ATH");
           delay(5000);
           read_Sensors();
           send_SMS();
           incoming ="";
        }


        
    }//ends while

    send_SMS_normal();
}
  else {
  }
        
//send sms if a misscall
        if (Serial.available() > 0) {
        incoming = Serial.readString();
        }
           if (incoming.indexOf("RING")>=0){
           Serial.println("ATH");
           delay(5000);
           read_Sensors();
           send_SMS();
           incoming ="";
        }

  
}
