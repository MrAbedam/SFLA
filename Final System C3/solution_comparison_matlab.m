
fileID = fopen('solution_comparison.txt', 'r');

if fileID == -1
    error('Failed to open the file. Make sure the file exists in the current directory.');
end

data = textscan(fileID, '%s %f', 'Delimiter', ':', 'MultipleDelimsAsOne', true);
fclose(fileID);

Ug_Fitness = data{2}(1);
Us_Fitness = data{2}(2);


numItems = length(Ug_Binary);

figure;
bar([Ug_Fitness, Us_Fitness]);
set(gca, 'xticklabel', {'Ug', 'Us'});
ylabel('Fitness Value');
title('Comparison of Fitness Values');
