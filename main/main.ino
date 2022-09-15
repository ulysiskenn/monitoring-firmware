#include "vswr_ad8364.h"
#include "dsa_sa3613.h"
#include "temperature_mcp9700.h"
#include "VersionLog.h"

#define LED_PULSE PN_1    //D1
#define LED_NORMAL PF_4   //D3
#define LED_WARNING PN_0  //D2
#define LED_DANGER PF_0   //D4

#define SERIAL_TIMING 1000
#define NORMAL_TIMING 500
#define WARNING_TIMING 300
#define DANGER_TIMING 150

int vswr_status = 0;

long serial_millis = millis();
long led_millis = millis();

int normal_indicator = 0;
int warning_indicator = 0;
int danger_indicator = 0;

void setup() {
  Serial.begin(9600); 
  initAD8364();
  initSA3613();
  initMCP7000();
  Serial.println(VERSION);
  pinMode(LED_PULSE, OUTPUT); 
  pinMode(LED_NORMAL, OUTPUT); 
  pinMode(LED_WARNING, OUTPUT); 
  pinMode(LED_DANGER, OUTPUT); 

  digitalWrite(LED_WARNING, LOW);
}

void loop() {
  analogReadResolution(12);       //setting the ADC to 12-bit resolution
  readPowerAD8364(); 
  controlSA3613();
  readMCP97000();
  readTemperatureAD8364();

  get_serial_data();
  led_indicator();
}


void set_vswr_status(){
  //Unconnected
  if (vswr >= 10.01 | vswr <= 0){
      vswr_status = 0;
      digitalWrite(LED_NORMAL, LOW);
      digitalWrite(LED_DANGER, LOW);
      digitalWrite(LED_WARNING, LOW);
      normal_indicator =0;
      warning_indicator = 0;
      danger_indicator = 0;
    }
  //Normal
  if (vswr >= 1.00 && vswr < 1.30){
      vswr_status = 1;
      digitalWrite(LED_WARNING, LOW);
      digitalWrite(LED_DANGER, LOW);
      normal_indicator =1;
      warning_indicator = 0;
      danger_indicator = 0;
    }
  //Low Warning
  if (vswr >= 1.31 && vswr < 1.80){
      vswr_status = 2;
      digitalWrite(LED_WARNING, HIGH);
      digitalWrite(LED_DANGER, LOW);
      normal_indicator = 0;
      warning_indicator = 1;
      danger_indicator = 0;
    }
   //High Warning
   if (vswr >= 1.81 && vswr < 3.00){
      vswr_status = 3;
      digitalWrite(LED_WARNING, HIGH);
      digitalWrite(LED_DANGER, LOW);
      normal_indicator = 0;
      warning_indicator = 1;
      danger_indicator = 0;
    }  
  //Danger
  if (vswr >= 3.01 && vswr < 10.00){
      vswr_status = 4;
      digitalWrite(LED_NORMAL, LOW);
      digitalWrite(LED_WARNING, HIGH);
      normal_indicator = 0;
      warning_indicator = 0;
      danger_indicator = 1;
    }  
  }

void get_serial_data() {
  if ((millis() - serial_millis) > SERIAL_TIMING) {
    serial_millis = millis();
    
    Serial.print(power_forward_dbm);
    Serial.print(",");
    Serial.print(power_reverse_dbm);
    Serial.print(",");
    Serial.print(vswr);
    Serial.print(",");

    set_vswr_status();
    
    Serial.print(attenuation);
    Serial.print(",");
    Serial.print(temperature_MCP9700);
    Serial.print(",");
    Serial.print(temperature_AD8364);
    Serial.print(",");
    Serial.println(vswr_status);

    digitalWrite(LED_PULSE, !digitalRead(LED_PULSE));
    }
  }

void led_indicator(){
     if ((millis() - led_millis) > NORMAL_TIMING && normal_indicator ) {
       led_millis = millis();
       digitalWrite(LED_NORMAL, !digitalRead(LED_NORMAL));
      }
    
    if ((millis() - led_millis) > WARNING_TIMING && warning_indicator ) {
       led_millis = millis();
       digitalWrite(LED_NORMAL, !digitalRead(LED_NORMAL));
      }
      
    if ((millis() - led_millis) > DANGER_TIMING && danger_indicator ) {
       led_millis = millis();
       digitalWrite(LED_DANGER, !digitalRead(LED_DANGER));
      }
  }
