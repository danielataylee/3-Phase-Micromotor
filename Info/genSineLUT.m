function genSineLUT(sineAmplitude, numUniqueEntries, maxPWM)

text = zeros(1, numUniqueEntries*2);
fileID = fopen('text.txt','wt');
fprintf(fileID, '{');
for i = 1:numUniqueEntries*2
    text(i) = round(maxPWM/5*(sineAmplitude/2+sineAmplitude/2*sin(i/numUniqueEntries*2*pi))); 
    
    if(i == numUniqueEntries*2)
        fprintf(fileID, ' %d};', text(i));
        fclose(fileID);
        break
    end
    fprintf(fileID, ' %d ,', text(i));
    if(~mod(i,16))
        fprintf(fileID, '\n');
        fprintf(fileID, '\t');
    end
end
