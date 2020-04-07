clc
clear all

for i=1:n_gt
    [thrs,cntR,sumR,cntP,sumP,V] = edgesEvalImg(lol,gt.groundTruth{i}.Boundaries); 
    R_array = cntR./sumR;
    P_array = cntP./sumP;
    F_array = 2 * ((P_array .* R_array)./(P_array + R_array));
    [max_value, max_index] = max(F_array)
    recall = R_array(max_index);
    precision = P_array(max_index);
    recall_array(i) = recall;
    precision_array(i) = precision;
end

mean_recall = mean(recall_array)
mean_precision = mean(precision_array)
F_measure = 2 * ((mean_precision * mean_recall)/(mean_precision + mean_recall))