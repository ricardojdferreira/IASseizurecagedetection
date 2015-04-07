% Script que permite a comunicação entre o Arduino e o Matlab, permitindo
%guardar no Matlab os registos provenientes dos sensores. Permite obter
%uma matriz com as componentes AC e DC de cada sensor, bem como as horas de
%inicio e fim de cada registo.

clc;
clear all;

numSec=30; %tempo total de aquisição
k=15; %variavel que define o inicio da gravação
step=15;%intervalo do save

%Definições da porta serial
s1 = serial('COM4');        % porta onde o Arduino está conectado
s1.BaudRate=128000;         % define a baud rate da porta
set(s1, 'terminator', 'LF');% terminator é uma variável do tipo char que indica o fim da comunicação com o Arduino

fopen(s1); %Abre a porta série

try
    
    w=fscanf(s1,'%s');               %inicia a comunicação com o Arduino. O Arduino
    if (w=='A')                      %envia um 'A' para garantir que a comunicação
        display('Collecting data');  %está estabelecida
        fprintf(s1,'%s\n','A');
    end
    
    i=0;
    t0=tic; %define tempo de inicio do script
    
    while (toc(t0)<=numSec)
        i=i+1;
        v(1,i)=fscanf(s1,'%d');       %As quatro primeiras linhas da matriz, correspondem
        v(2,i)=fscanf(s1,'%d');       %ao registo do sinal AC de cada sensor. São guardadas
        v(3,i)=fscanf(s1,'%d');       %na matriz como inteiros ('%d'). As quatro últimas entradas
        v(4,i)=fscanf(s1,'%d');       %correspondem à componente DC dos sensores e são
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
    fclose(s1);                 % fim da comunicação com a porta
    throw (exception);
end