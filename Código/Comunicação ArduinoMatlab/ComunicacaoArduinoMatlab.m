% Script que permite a comunica��o entre o Arduino e o Matlab, permitindo
%guardar no Matlab os registos provenientes dos sensores. Permite obter
%uma matriz com as componentes AC e DC de cada sensor, bem como as horas de
%inicio e fim de cada registo.

clc;
clear all;

numSec=30; %tempo total de aquisi��o
k=15; %variavel que define o inicio da grava��o
step=15;%intervalo do save

%Defini��es da porta serial
s1 = serial('COM4');        % porta onde o Arduino est� conectado
s1.BaudRate=128000;         % define a baud rate da porta
set(s1, 'terminator', 'LF');% terminator � uma vari�vel do tipo char que indica o fim da comunica��o com o Arduino

fopen(s1); %Abre a porta s�rie

try
    
    w=fscanf(s1,'%s');               %inicia a comunica��o com o Arduino. O Arduino
    if (w=='A')                      %envia um 'A' para garantir que a comunica��o
        display('Collecting data');  %est� estabelecida
        fprintf(s1,'%s\n','A');
    end
    
    i=0;
    t0=tic; %define tempo de inicio do script
    
    while (toc(t0)<=numSec)
        i=i+1;
        v(1,i)=fscanf(s1,'%d');       %As quatro primeiras linhas da matriz, correspondem
        v(2,i)=fscanf(s1,'%d');       %ao registo do sinal AC de cada sensor. S�o guardadas
        v(3,i)=fscanf(s1,'%d');       %na matriz como inteiros ('%d'). As quatro �ltimas entradas
        v(4,i)=fscanf(s1,'%d');       %correspondem � componente DC dos sensores e s�o
        v(5,i)=fscanf(s1,'%f');       %do tipo float ('%f')
        v(6,i)=fscanf(s1,'%f');
        v(7,i)=fscanf(s1,'%f');
        v(8,i)=fscanf(s1,'%f');
        if toc(t0)>=k                 %Salva a matriz com o registo, bem como as horas de inicio
            b=toc(t0);                %e fim
            a=num2str(round(b));
            filename=strcat('D',a);
            c=clock;
            save(filename,'v','b','c');
            k=k+step;
        end
        
    end
    fclose(s1);
    
    
catch exception
    fclose(s1);                 % fim da comunica��o com a porta
    throw (exception);
end