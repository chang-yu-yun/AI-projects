function [pred] = getPrediction(data, prior, prob, edges)
numOfData = size(data, 1);
pred = zeros(numOfData, 1);
for i = 1:numOfData
    pred(i) = predict(data(i, :), prior, prob, edges);
end
end

