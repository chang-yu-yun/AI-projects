function [dataSet] = generateDataSet(data, k)
% generate k subsets and ensure that the proportion in each subset is
% approximately equal to that of the original data
% pre-processing step for k-fold cross-validation
% @param data: the original data
% @param k: k-fold cross-validataion
% @return dataSet: partitioning of the original data

numOfData = size(data, 1);
target = [sum(data(:,1) == 0), sum(data(:, 1) == 1)];
subsetSize = size(data, 1) / k;
dataSet = cell(k, 1);

% compute the desired number of target-0 samples in one subset
% as well as target-1 samples
ratio = target(1) / sum(target);
num = zeros(2, 1);
num(1) = ceil(subsetSize * ratio(1));
num(2) = subsetSize - num(1);
index = ones(2, 1);
count = zeros(2, 1);
for i = 1:numOfData
    curr = data(i, 1) + 1;
    if count(curr) == num(curr)
        index(curr) = index(curr) + 1;
        count(curr) = 0; 
    end
    dataSet{index(curr)} = [dataSet{index(curr)}; data(i, :)];
    count(curr) = count(curr) + 1;
end
end