function [] = 0886006()
clc;
clear;
close all;

%% read the training data
data = readData('Project_NB_Tr.xlsx');

%% k-fold cross-validation
k = 10;
dataSet = generateDataSet(data, k);
accV = zeros(k, 1);
for i = 1:k
    % partitioning: stratified subsets
    validData = dataSet{i};
    trainData = [];
    for j = 1:length(dataSet)
        if j == i
            continue;
        end
        trainData = [trainData; dataSet{j}];
    end
    [prior, prob, edges] = train(trainData(:, 2:end), trainData(:, 1));
    pred = getPrediction(validData(:, 2:end), prior, prob, edges);
    correct = sum(pred == validData(:, 1));
    acc = correct / size(validData, 1);
    accV(i) = acc;
    disp(['Accuracy: ' num2str(acc * 100) '%']);
end
% display the overall accuracy
avgAcc = mean(accV);
disp(['Overall accuracy: ' num2str(avgAcc * 100) '%']);

%% prediction
testData = readData('Project_NB_Ts.xlsx');
[prior, prob, edges] = train(data(:, 2:end), data(:, 1));
pred = getPrediction(testData, prior, prob, edges);
fid = fopen('Submission.csv', 'w');
fprintf(fid, '%s,%s\n', 'No', 'Target');
for i = 1:length(pred)
    fprintf(fid, '%d,%d\n', i, pred(i));
end
fclose(fid);
end