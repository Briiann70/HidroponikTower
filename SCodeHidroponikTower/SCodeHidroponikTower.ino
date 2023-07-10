/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6VuGgfCYo"
#define BLYNK_TEMPLATE_NAME "MonitoringHidroponikTower"
#define BLYNK_AUTH_TOKEN "2W2Fgky4dI4VXw3NZDQ7kW48ol-V4FtY"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "briiann";
char pass[] = "NiellL12050";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

//Library TDS
#include <GravityTDS.h>
#include <EEPROM.h>
#define TdsSensorPin A0
GravityTDS gravityTds;

//Relay
#define relayw 5
#define relayp 4

//Servo
Servo servoku;
bool servoMoving = false;

//Use to switch on/off relay 1
BLYNK_WRITE(V0){
 int SW_relayW = 1; 
 SW_relayW = param.asInt();
  if(SW_relayW == 1){
    Serial.println("RelayP terbuka");
    digitalWrite(relayw, LOW);
  }
  else{
    Serial.println("RelayP tertutup");
    digitalWrite(relayw, HIGH);
  }
}

//use to control servo with swipe
BLYNK_WRITE(V2){
  
  servoMoving = true;
  servoku.write(param.asInt());
  servoMoving = false;
}

//Use to switch on/off relay 2
BLYNK_WRITE(V3){
 int SW_relayP = 0; 
 SW_relayP = param.asInt();
  if(SW_relayP == 1){
    Serial.println("RelayP terbuka");
    digitalWrite(relayp, LOW);
  }
  else{
    Serial.println("RelayP tertutup");
//    Blynk.virtualWrite(V3, LOW);
    digitalWrite(relayp, HIGH);
  }
}

//use control schedule servo to 180 degrees
BLYNK_WRITE(V4) 
{
  servoMoving = true; // indicate that servo is moving
  servoku.write(180);
  servoMoving = false; // indicate that servo has stopped moving
}

//use control schedule relay1
BLYNK_WRITE(V5)
{
  digitalWrite(relayp, LOW); 
}

//use control schedule relay2
BLYNK_WRITE(V6)
{
  digitalWrite(relayp, HIGH);
}

//use control schedule servo back to 0 degrees
BLYNK_WRITE(V7)
{
  servoMoving = true; // indicate that servo is moving
  servoku.write(0);
  servoMoving = false; // indicate that servo has stopped moving


void setup()
{
  // Debug console
  Serial.begin(9600);

  //Inisialisai pin
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  servo.attach(8);
  pinMode(relayw, OUTPUT);
  pinMode(relayp, OUTPUT);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  float temperature = 25 ,tdsValue = 0; 
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate 
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue,0);
  //use to displays nutrient solution data
  Blynk.virtualWrite(V1, tdsValue);
  Blynk.run();

}


