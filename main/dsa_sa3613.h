
/*
 * https://docs.arduino.cc/tutorials/communication/DigitalPotControl
 * 
 * 
 * PD_1 (A14) - MOSI(2) used only since unidirectional. TIVA to SA3613
 * 
 *  TIVA              |   SA3613
 *  PD_1 (MOSI(2))    ->     SI
 *  PP_2              ->     LE
 *  PD_3 (CLK(2))     ->    CLK
 *  
 */

#include <SPI.h>

const int SLAVE_SELECT_PIN = PP_2;
int value=0;
int address = 0;
float attenuation = 0;
void initSA3613()
{
  pinMode(SLAVE_SELECT_PIN, OUTPUT); 
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.transfer(address);
  SPI.transfer(value);
}

void controlSA3613()
{
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();  
      if(incomingByte=='+'){
        address = address + 2;
        attenuation = attenuation + 0.5;
        if(attenuation >= 31.5)
        {
          attenuation = 31.5;
        }
        digitalWrite(SLAVE_SELECT_PIN, LOW);
        delay(100);
        
        //  send in the address and value via SPI:
        SPI.transfer(address);
        SPI.transfer(value);
        delay(100);
//        Serial.print("Attenuation: ");
//        Serial.print(attenuation);
//        Serial.println(" dB");
        
        // take the SS pin high to de-select the chip:
        digitalWrite(SLAVE_SELECT_PIN, HIGH);
      }
      
      if(incomingByte=='-'){
        address = address - 2;
        attenuation = attenuation - 0.5;
        if(address < 0)
        {
          address = 0;
          attenuation = 0;
        }
        digitalWrite(SLAVE_SELECT_PIN, LOW);
        delay(100);
        //  send in the address and value via SPI:
        SPI.transfer(address);
        SPI.transfer(value);
        delay(100);      
        //Serial.print("Attenuation: ");
      //  Serial.print(attenuation);
       // Serial.println(" dB");
        // take the SS pin high to de-select the chip:
        digitalWrite(SLAVE_SELECT_PIN, HIGH);
      } 
  }
}
