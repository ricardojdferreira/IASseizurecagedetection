#include <MatrixMath.h>

#include <EEPROMex.h> //biblioteca extensiva para trabalhar com a memória EEPROM (memória do arduíno que não se apaga quando se desliga)
#include <EEPROMVar.h>//biblioteca extensiva de operações na memória EEPROM
#include <MatrixMath.h>//biblioteca de cálculo sobre matrizes
//include <EEPROM.h>// biblioteca de escrita e leitura na EEPROM

# define calibracao (6)// tamanho do vector de calibração(número de calibrações que se fizerem)

float vectorcoefs[calibracao] = {0, 100, 200, 300, 400, 500}; //vector das calibrações (preencher com os pesos das massas conhecidas - 50, 100 200 g etc). Os valores indicados foram os utilizados para a calibração
float vectortodos[calibracao*4]; // vector que tem os valores todos dos sensores quando se faz as calibrações - são 4x o número de calibrações porque são 4 sensores

float matrizcoef[calibracao][2];// matriz de dimensão 4x2 em que na primeira coluna tem 1's e na segunda os valores dos pesos (se tivesse mais colunas eram os termos de ordem superior-ver equação)
float matrizcoefT[2][calibracao];// matriz anterior transposta
float matrizcoefInv[2][2];//(multiplicação das duas matrizes anteriores)^-1
float intermedio[2][calibracao];//multiplicação da matriz anterior pela matrizcoefT

float matrizMedA0[calibracao][1]; //vector com os valores do sensor AO
float matrizMedA1[calibracao][1]; //vector com os valores do sensor A1
float matrizMedA2[calibracao][1]; //vector com os valores do sensor A2
float matrizMedA3[calibracao][1]; //vector com os valores do sensor A3

float cA0[2][1];//coeficientes de calibração do sensor AO
float cA1[2][1];//coeficientes de calibração do sensor A1
float cA2[2][1];//coeficientes de calibração do sensor A2
float cA3[2][1];//coeficientes de calibração do sensor A3

void setup()
{
  int add= 0;
  Serial.begin(9600);
    for (int j= 0; j < calibracao*4; j++){
    vectortodos[j] = EEPROM.readFloat(add); //lê os valores todos das medidas de calibração guardados na memória EEPROM
    add=add+4;
}
  for ( int i = 0; i < calibracao; i ++){
  
    matrizcoef[i][0] = 1;
    matrizcoef[i][1] = vectorcoefs[i]; //constroi a matrizcoef
}
  int j = 0;
   for(int l = 0; l < calibracao*4; l = l + 4){
  
    matrizMedA0[j][0] = vectortodos[l];
    matrizMedA1[j][0] = vectortodos[l+1];
    matrizMedA2[j][0] = vectortodos[l+2];
    matrizMedA3[j][0] = vectortodos[l+3];  
    //constroi os quatro vectores com as medidas
    j = j + 1;
}
  
  //Cáculo das matrizes necessárias para obter a pseudo-inversa
  Matrix.Transpose((float*)matrizcoef,calibracao,2,(float*)matrizcoefT);
  Matrix.Multiply((float*)matrizcoefT,(float*)matrizcoef,2,calibracao,2,(float*)matrizcoefInv); 
  Matrix.Invert((float*)matrizcoefInv,2);
  Matrix.Multiply((float*)matrizcoefInv,(float*)matrizcoefT,2,2,calibracao,(float*)intermedio); 

  Matrix.Multiply((float*)intermedio,(float*)matrizMedA0,2,calibracao,1,(float*)cA0); 
  Matrix.Multiply((float*)intermedio,(float*)matrizMedA1,2,calibracao,1,(float*)cA1); 
  Matrix.Multiply((float*)intermedio,(float*)matrizMedA2,2,calibracao,1,(float*)cA2); 
  Matrix.Multiply((float*)intermedio,(float*)matrizMedA3,2,calibracao,1,(float*)cA3); 
  
  int address = 511;
  
  for(int j = 0; j <1 ; j++){ //envia os valores de ajuste para cada sensor para a memória EEOPROMdo Arduíno. Escreve no extremo oposto da memória onde estão as medidas lidas.
  EEPROM.writeFloat(address-4,cA0[j][0]);
  EEPROM.writeFloat(address-8,cA1[j][0]);
  EEPROM.writeFloat(address-12,cA2[j][0]);
  EEPROM.writeFloat(address-16,cA3[j][0]);
  
  EEPROM.writeFloat(address-20,cA0[j+1][0]);
  EEPROM.writeFloat(address-24,cA1[j+1][0]);
  EEPROM.writeFloat(address-28,cA2[j+1][0]);
  EEPROM.writeFloat(address-32,cA3[j+1][0]);
}
  
}

void loop(){
  Matrix.Print((float*)matrizcoef,calibracao,2,"matrizcoef"); //O Arduino imprime na consola as várias matrizes utilizadas no uso 
  Matrix.Print((float*)matrizcoefT,2,calibracao,"matrizcoefT"); //do método dos mínimos quadrados
  Matrix.Print((float*)matrizcoefInv,2,2,"matrizcoefInv");
  Matrix.Print((float*)intermedio,2,4,"intermedio"); 
  Matrix.Print((float*)vectortodos,calibracao*4,1,"vectortodos");  
  Matrix.Print((float*)matrizMedA0,calibracao,1,"matrizMedA0");
  Matrix.Print((float*)matrizMedA1,calibracao,1,"matrizMedA1");
  Matrix.Print((float*)matrizMedA2,calibracao,1,"matrizMedA2");
  Matrix.Print((float*)matrizMedA3,calibracao,1,"matrizMedA3");
  Matrix.Print((float*)cA0,2,1,"cA0");
  Matrix.Print((float*)cA1,2,1,"cA1");
  Matrix.Print((float*)cA2,2,1,"cA2");
  Matrix.Print((float*)cA3,2,1,"cA3");
  
}



