NUMBER_OF_ITEMS = 9;
NUMBER_OF_FROGS = 6; 


solutions = randi([0, 1], NUMBER_OF_FROGS, NUMBER_OF_ITEMS);

custom_path = "C:\Users\God - 03.06\source\repos\Final System C3\Final System C3";

filename = fullfile(custom_path, 'random_generation.txt');

fileID = fopen(filename, 'w');
fprintf(fileID, 'Generated Frogs:\n');

for i = 1:NUMBER_OF_FROGS
    fprintf(fileID, '%d ', solutions(i, :));
    fprintf(fileID, '\n');
end

fclose(fileID);

disp(['Random frog solutions saved in ', filename]);
