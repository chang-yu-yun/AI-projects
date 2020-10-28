function [data] = readData(filename)
keySet = {'L', 'R'};
valueSet = [0, 1];
table = containers.Map(keySet, valueSet); % mapping table for laterality

[data, text, raw] = xlsread(filename);

index = find(string(text(1, :)) == 'Laterality');
numOfData = size(data, 1);

% convert characters to numeric values
for i = 1:numOfData
    data(i, index) = table(string(text{i + 1, index}));
end

% there is a missing value in 'Segment' attribute
% convert it to zero
data(isnan(data)) = 0;
data = data(:, 2:end); % drop 'No' column
end