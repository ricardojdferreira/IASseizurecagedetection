function [ S,F,T,P] = STFT( signal,window_length,nfft,fs)
%window length in seconds, fs in hertz, channel of the sensor

window_length=window_length*fs;

window=blackman(window_length);

[S,F,T,P]=spectrogram(signal-mean(signal),window,window_length/2,nfft,fs);
end

