function [ classified ] = Detectseizure( modeloprob,signal,t, l )
%Fun��o que recebe um modelo de probabilidades (modeloprob)e um sinal (signal)
%a classificar e
%devolve o sinal com informa��o onde � prov�vel que tenha occorrido uma
%seizure. l � o tamanho da janela que percorre o sinal para somar o n�mero
%de seizures. t � o threshold para considerar que uma janela corresponde a
%uma seizure

signal=signal(1,:)+signal(2,:) + signal(3,:)+signal(4,:); %soma os dados referentes aos
% sensores

[S,T,F,P]=STFT(signal-mean(signal), 1, 512,50); % Calcula a short time fourier transform para a soma
% dos 4 canais, com tamanho da janela de 1 segundo, com 512 pontos e uma
% frequ�ncia de amostragem 50Hz


rawclassified=predict(modeloprob, P(1:128,:)'); %calcula, atrav�s do modelo de probabilidades,
%se cada entrada do sinal, em bruto, � ('seizure')ou n�o ('non seizure') uma convuls�o

L=length(rawclassified);
%l=10; %tamanho janela para calcular threesholds
%t=5; %nr de eventos minimos (threshold)
aux=zeros(L,1);
classified=zeros(floor(L/l),1);
for i=1:length(rawclassified) %ciclo que converte a classficacao para 0's e 1's
    if strcmp(rawclassified{i,1},'s')
        aux(i)=1;
    else
        aux(i)=0;
    end
end

for j=1:floor(L/l) % ciclo que permite definir per�odos de convuls�o no sinal
    if sum(aux(1+l*(j-1):l*(j)))>= t %threshold para considerar convulsao
        classified(j)=1;
    else classified(j)=0;
    end
end

stem(classified)

pause

close all

end

