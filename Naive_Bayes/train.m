function [prior, prob, edges] = train(data, target)
%% initialization
numeric = [1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 23, 36];
numOfData = size(data, 1);
prior = zeros(2, 1);

%% training
prior(2) = sum(target(:, 1));
prior(1) = numOfData - prior(2);
prior = prior / numOfData;

prob = cell(2, 1);
class = cell(2, 1);
class{1} = data(target == 0, :);
class{2} = data(target == 1, :);
[prob{1}, edges] = calculateProb(class{1}, numeric);
[prob{2}, edges] = calculateProb(class{2}, numeric, edges);
end