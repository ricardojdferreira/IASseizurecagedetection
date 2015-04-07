#include <EEPROMex.h> //Bibliotecas necessárias 
#include <EEPROMVar.h>
#include <Event.h>
#include <Timer.h>

int inputpin0 = A4; // pin definido para receber o input de um sensor A0
int inputpin1 = A5; // pin definido para receber o input de um sensor A1
int inputpin2 = A6; // pin definido para receber o input de um sensor A2
int inputpin3 = A7; // pin definido para receber o input de um sensor A3

int sensorvalue0; //variaveis que contem o registo dos valores lidos pelos quatro sensores
int sensorvalue1; //a cada medida de calibração efectuada
int sensorvalue2; 
int sensorvalue3; 
   
float sensorvalue0f; //variaveis com o mesmo signifcado das variaveis acima mas do tipo float
float sensorvalue1f; 
float sensorvalue2f; 
float sensorvalue3f; 


int add = 0; // indice da memória onde se escreve
Timer t; // timer

int led = 13; //pin do LED

void setup(){
  Serial.begin(9600);
 pinMode(led,OUTPUT); 
 digitalWrite(led, HIGH); //o led apaga-se
 t.every(10000,takeSave); //corre um ciclo de aquisição a cada 10 segundos
}

void loop (){
t.update(); //faz update ao vector de tempo
}

 void takeSave()
 {
   digitalWrite(led, LOW); //o led apaga-se
   sensorvalue0 = analogRead(inputpin0); //leitura para os quatro sensores do valor registado por estes
   sensorvalue1 = analogRead(inputpin1);
   sensorvalue2 = analogRead(inputpin2);
   sensorvalue3 = analogRead(inputpin3);
   
  sensorvalue0f=(float)sensorvalue0; //cast para tornar os valores como tipo float
  sensorvalue1f=(float)sensorvalue1;
  sensorvalue2f=(float)sensorvalue2;
  sensorvalue3f=(float)sensorvalue3;
   
   EEPROM.writeFloat(add,sensorvalue0f); // escreve os valores da medida de calibração na memória EEPROM do Arduino
   EEPROM.writeFloat(add+4,sensorvalue1f); 
   EEPROM.writeFloat(add+8,sensorvalue2f); 
   EEPROM.writeFloat(add+12,sensorvalue3f); 
   add = add + 16;//avança 4 slots na memória para não sobrepôr valores
   delay(8000);//o ciclo começa com uma leitura. Passado 8 segundos um LED acende indicando que faltam 2 segundos para a próxima aquisição e passados 2 segundos esta occorre e o LED desliga. Este ciclo repete-se de 10 em 10 segundos
   digitalWrite(led, HIGH);
 }
