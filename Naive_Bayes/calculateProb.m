function [prob, edges] = calculateProb(data, numeric, edges)
% calculate the conditional probability
% @param data: training data
% @param numeric: a vector indicating whether attribute is numeric or not
% @param edges (optional): set of interval boundaries (numeric) or values
% (discretization)
% @return prob: conditional probabilites of each attribute
% @return edges: a fixed set of intervals

numOfAttr = size(data, 2);
prob = cell(numOfAttr, 1);
flag = 1;
if nargin == 2
    edges = cell(numOfAttr, 1);
    flag = 0;
end

infinity = 100000;
for i = 1:numOfAttr
    if flag == 0
        if find(numeric == i)
            % numeric attributes
            % discretization: divide the range into a fixed set of intervals
            [count, edge] = histcounts(data(:, i));
        else
            % integer attributes
            [count, edge] = histcounts(data(:, i), 'BinMethod', 'integers');
        end
        edge = [-infinity, edge, infinity];
        count = [0, count, 0];
    else
        [count, edge] = histcounts(data(:, i), edges{i});
    end
    edges{i} = edge;
    
    % Laplace correction
    count = count + 1;
    total = sum(count);
    prob{i} = count / total;
end
end