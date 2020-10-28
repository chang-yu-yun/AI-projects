function [pred] = predict(data, prior, prob, edges)
% naive Bayes classifier
% calculate (log) posterior probability
% use log to avoid underflow
% @param data: use data to predict target value
% @param prior: prior probabilities
% @param prob: conditional probabilities given target value
% @param edges: fixed set of interval boundaries (numeric) or values
% @return pred: prediction

posterior = log2(prior);
for i = 1:length(posterior)
    for j = 1:length(data)
        index = discretize(data(j), edges{j});
        % multiplication -> log -> addition
        posterior(i) = posterior(i) + log2(prob{i}{j}(index));
    end
end

% predict by choosing the class with the highest posterior probability
[value, index] = max(posterior);
% index = 1 -> predict 0
% index = 2 -> predict 1
pred = index - 1;
end

