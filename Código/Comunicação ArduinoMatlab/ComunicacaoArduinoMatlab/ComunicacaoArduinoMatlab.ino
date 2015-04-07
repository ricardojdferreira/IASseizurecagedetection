#include <EEPROMex.h> //bibliotecas extra necessárias
#include <EEPROMVar.h>
#include <Event.h>
#include <Timer.h>

Timer t; // variavel de tempo
   
float a0A0; // par de variaveis que guardará o declive e ordenada na origem para construção da recta de calibração do inputpin0
float a1A0;

float a0A1; // par de variaveis que guardará o declive e ordenada na origem para construção da recta de calibração do inputpin1
float a1A1;

float a0A2; // par de variaveis que guardará o declive e ordenada na origem para construção da recta de calibração do inputpin2
float a1A2;

float a0A3; // par de variaveis que guardará o declive e ordenada na origem para construção da recta de calibração do inputpin3
float a1A3;

int address = 511; // variavel que guarda o número do address na memória
   
   void setup() {
   Serial.begin(128000);  // velocidade de transmissão de dados
     a0A0 = EEPROM.readFloat(address-4); // leitura dos valores de declive e ordenada na origem a partir da memória do arduino para o inputpin0
     a1A0 = EEPROM.readFloat(address-20);
     
     a0A1 = EEPROM.readFloat(address-8); // leitura dos valores de declive e ordenada na origem a partir da memória do arduino para o inputpin1
     a1A1 = EEPROM.readFloat(address-24);
     
     a0A2 = EEPROM.readFloat(address-12); // leitura dos valores de declive e ordenada na origem a partir da memória do arduino para o inputpin2
     a1A2 = EEPROM.readFloat(address-28);
     
     a0A3 = EEPROM.readFloat(address-16); // leitura dos valores de declive e ordenada na origem a partir da memória do arduino para o inputpin3
     a1A3 = EEPROM.readFloat(address-32); 
   establishContact();  // estabele o contacto com o MatLab
   t.every(20,takereadingAC); // a cada 20ms le-se os valores AC e DC de cada sensor
 }

 void loop() {
  t.update(); // actualiza o tempo
 }

 void establishContact() { //rotina para estabelecer o primeiro contacto com o Matlab. O Arduino envia um caracter 'A'
    // imprime 'A'
   while (Serial.available() <= 0) { 
     Serial.println('A'); 
     delay(200);
     }
 }
 
 void takereadingAC()
 {
   Serial.println(analogRead(A0)); // Leitura do valor AC no inputpin0
   Serial.println(analogRead(A1)); // Leitura do valor AC no inputpin1
   Serial.println(analogRead(A2)); // Leitura do valor AC no inputpin2
   Serial.println(analogRead(A3)); // Leitura do valor AC no inputpin3
   Serial.println((analogRead(A4)-a0A0)/a1A0); // Leitura do valor DC do inputpin0 e respectiva calibração
   Serial.println((analogRead(A5)-a0A1)/a1A1); // Leitura do valor DC do inputpin1 e respectiva calibração
   Serial.println((analogRead(A6)-a0A2)/a1A2); // Leitura do valor DC do inputpin2 e respectiva calibração
   Serial.println((analogRead(A7)-a0A3)/a1A3); // Leitura do valor DC do inputpin3 e respectiva calibração
 }
