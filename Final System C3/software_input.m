
NUMBER_OF_ITEMS = 9;

value = input('Enter the values of items as a row vector: ');
weight = input('Enter the weights of items as a row vector: ');
weight_limit = input('Enter the maximum weight limit: ');

if length(value) ~= NUMBER_OF_ITEMS || length(weight) ~= NUMBER_OF_ITEMS
    error('The number of values and weights must be %d.', NUMBER_OF_ITEMS);
end

custom_path = "C:\Users\God - 03.06\source\repos\Final System C3\Final System C3";

filename =fullfile(custom_path,'items_data.txt') ;

fileID = fopen(filename, 'w');
fprintf(fileID, 'Values: ');
fprintf(fileID, '%d ', value);
fprintf(fileID, '\nWeights: ');
fprintf(fileID, '%d ', weight);
fprintf(fileID, '\nWeight_Limit: %d\n', weight_limit);
fclose(fileID);

disp(['Data saved in ', filename]);

% mesal value [8, 6, 3, 7, 6, 9, 8, 5, 6]
% mesal weight [5, 4, 3, 9, 5, 7, 6, 3, 2]
% mesal max weigh 20