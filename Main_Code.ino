#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;
//#include <DHT.h>
#include <BlynkSimpleEsp32.h> 
#include <WiFiUdp.h>  // For OTA
#include <ArduinoOTA.h>  // For OTA

#define DEBUG_SW 1



/*
char ssid[] = "DCSE FYP Lab";
char pass[] = "dcsefyplab";
 */

void without_internet();

int relay1State = LOW;
int touchswitch1State = HIGH;

int relay2State = LOW;
int touchswitch2State = HIGH;

int relay3State = LOW;
int touchswitch3State = HIGH;

int relay4State = LOW;
int touchswitch4State = HIGH;

int relay5State = LOW;
int touchswitch5State = HIGH;

#define AUTH "VIbzvWHe6EHfChH5j3jX0QBScWLGpyh_"
#define WIFI_SSID "Orbailix"                  
#define WIFI_PASS "howhunterhunts@"                  //Enter wifi Password

#define SERVER "blynk-cloud.com "             // Comment-out if use Blynk hosted cloud service
#define PORT 8442

#define RELAY_PIN_1       15   //D0
#define RELAY_PIN_2       4  //D1
#define RELAY_PIN_3       18   //D2
#define RELAY_PIN_4       19  //D3
#define RELAY_PIN_5       21   //D5 

#define TOUCH_SWITCH_1     13 //D6
#define TOUCH_SWITCH_2     12  //D7
#define TOUCH_SWITCH_3     14   //D8
#define TOUCH_SWITCH_4     27   //RX
#define TOUCH_SWITCH_5     33   //TX

int buzzer = 23; //buzzer
int sensor = 32; //sensor
int led_green = 25; //no leakage indication
int led_red = 26; // leakage indication
 
// Define threshold value. You might need to change it.
int sensor_limit = 600;

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3
#define VPIN_BUTTON_4    V4  
#define VPIN_BUTTON_5    V5

#define OTA_HOSTNAME "Home_Automation"
// By default the mode is with_internet
int MODE = 0;

BLYNK_CONNECTED() {

  // Request the latest state from the server

  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
   Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Blynk.syncVirtual(VPIN_BUTTON_5);

  // Alternatively, you could override server state using:
 // Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
 // Blynk.virtualWrite(VPIN_BUTTON_2, relay2State);
 // Blynk.virtualWrite(VPIN_BUTTON_3, relay3State);
 // Blynk.virtualWrite(VPIN_BUTTON_4, relay4State);

}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  relay2State = param.asInt();
  digitalWrite(RELAY_PIN_2, relay2State);
}
BLYNK_WRITE(VPIN_BUTTON_3) {
  relay3State = param.asInt();
  digitalWrite(RELAY_PIN_3, relay3State);
}
BLYNK_WRITE(VPIN_BUTTON_4) {
  relay4State = param.asInt();
  digitalWrite(RELAY_PIN_4, relay4State);
}
BLYNK_WRITE(VPIN_BUTTON_5) {
  relay5State = param.asInt();
  digitalWrite(RELAY_PIN_5, relay5State);
}

void with_internet()
{
  if (digitalRead(TOUCH_SWITCH_1) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (touchswitch1State != LOW) {

      // Toggle Relay state
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    touchswitch1State = LOW;
  } else {
    touchswitch1State = HIGH;
  }

  if (digitalRead(TOUCH_SWITCH_2) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (touchswitch2State != LOW) {

      // Toggle Relay state
      relay2State = !relay2State;
      digitalWrite(RELAY_PIN_2, relay2State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_2, relay2State);
    }
    touchswitch2State = LOW;
  } else {
    touchswitch2State = HIGH;
  }

if (digitalRead(TOUCH_SWITCH_3) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (touchswitch3State != LOW) {

      // Toggle Relay state
      relay3State = !relay1State;
      digitalWrite(RELAY_PIN_3, relay3State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_3, relay3State);
    }
    touchswitch3State = LOW;
  } else {
    touchswitch3State = HIGH;
  }

  if (digitalRead(TOUCH_SWITCH_4) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (touchswitch4State != LOW) {

      // Toggle Relay state
      relay4State = !relay4State;
      digitalWrite(RELAY_PIN_4, relay4State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_4, relay4State);
    }
    touchswitch4State = LOW;
  } else {
    touchswitch4State = HIGH;
  }
  if (digitalRead(TOUCH_SWITCH_5) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (touchswitch5State != LOW) {

      // Toggle Relay state
      relay5State = !relay5State;
      digitalWrite(RELAY_PIN_5, relay5State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_5, relay5State);
    }
    touchswitch5State = LOW;
  } else {
    touchswitch5State = HIGH;
  }
}

void setup()
{

//FOR GASS SENSOR
pinMode(buzzer, OUTPUT);
pinMode(sensor, INPUT);
pinMode(led_green, OUTPUT);
pinMode(led_red, OUTPUT);
digitalWrite(led_green, LOW);
digitalWrite(led_red, LOW);

  Serial.begin(115200);
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS,"blynk-cloud.com", 8442);
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // For OTA - Use your own device identifying name
  ArduinoOTA.begin();  // For OTA

// FOR HOMEAUTOMATION(ON/OFF SWITCH)
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(TOUCH_SWITCH_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, relay1State);


  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(TOUCH_SWITCH_2, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_2, relay2State);


  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(TOUCH_SWITCH_3, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_3, relay3State);


  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(TOUCH_SWITCH_4, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_4, relay4State);

   pinMode(RELAY_PIN_5, OUTPUT);
  pinMode(TOUCH_SWITCH_5, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_5, relay5State);

  // Setup a function to be called every 100 ms
  timer.setInterval(500L,  checkBlynk);
}

void without_internet()
{

  // FOR SWITCH
  digitalWrite(RELAY_PIN_1 , !digitalRead(TOUCH_SWITCH_1));
  digitalWrite(RELAY_PIN_2 , !digitalRead(TOUCH_SWITCH_2));
  digitalWrite(RELAY_PIN_3 , !digitalRead(TOUCH_SWITCH_3));
  digitalWrite(RELAY_PIN_4, !digitalRead(TOUCH_SWITCH_4));
  digitalWrite(RELAY_PIN_5 , !digitalRead(TOUCH_SWITCH_5));
}
//Check Blynk Connectivity every 3 seconds
void checkBlynk()
{
  bool isconnected = Blynk.connected();
  if (isconnected == false)
  {
    MODE = 1;
  }
  if (isconnected == true)
  {
    MODE = 0;
  }
}

void loop()
{
        //FOR GASS SENSOR
        
 int sensor_value = analogRead(sensor);
Serial.print("Gas Level: ");
Serial.println(sensor_value);
Blynk.virtualWrite(V6, sensor_value);
// Checks if it has reached the threshold value
if (sensor_value > sensor_limit)
{
digitalWrite(led_green, HIGH);
digitalWrite(led_red, LOW);
digitalWrite(buzzer, HIGH);
delay(500);
digitalWrite(buzzer, LOW);
Blynk.notify("Alert: Gas Leakage Detected");
}
else
{
digitalWrite(buzzer, LOW);
digitalWrite(led_green, LOW);
digitalWrite(led_red, HIGH);
}
delay(100);

        // SWITCH  (HOMEAUTOMATION)
        
    if (WiFi.status() != WL_CONNECTED)
  {
    if (DEBUG_SW) Serial.println("Not Connected");
  }
  else
  {
    if (DEBUG_SW) Serial.println(" Connected");
    Blynk.run();
  }
  Blynk.run();
  ArduinoOTA.handle();  // For OTA
    timer.run(); // Initiates SimpleTimer
  if (MODE == 0)
    with_internet();
  else
    without_internet();
  // put your main code here, to run repeatedly:
}
