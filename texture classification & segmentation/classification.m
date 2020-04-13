% read file 
fid = fopen('featurevector_class.txt', 'rt');
fid_1 = fopen('meanvector_class.txt', 'rt');
fid_test = fopen('featurevector_classT.txt', 'rt');
% convert into cells 
C = textscan(fid, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
D = textscan(fid_1, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
test = textscan(fid_test, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
fclose(fid);
fclose(fid_1);
fclose(fid_test);
% convert into double matrix
featureVector = cat(2, C{:});
meanVector = cat(2, D{:});
featureVector_test = cat(2, test{:});

%calculating zero mean matrix 
zeroMeanVector_matlab = featureVector - mean(featureVector, 1);
h = ones(36,1);
temp =  h * meanVector;
zeroMeanVector = featureVector - temp;

% calculating SVD 
[U,S,V] = svd(zeroMeanVector,'econ');
V_r = V(:, (1:3));

PCA_components = featureVector * V_r;
PCA_components_test = featureVector_test * V_r;

save('/home/yogesh/Git/hw-4-texture/PCA_train.txt', 'PCA_components', '-ascii','-tabs');

figure(1)
biplot(V(:,1:3), 'Scores', zeroMeanVector(:,1:3));

% plotting the PCA feature vector for train images 
figure(2)
scatter3(PCA_components(1:9,1), PCA_components(1:9,2), PCA_components(1:9, 3), 'r', 'filled');
hold on
scatter3(PCA_components(10:18,1), PCA_components(10:18,2), PCA_components(10:18, 3), 'g', 'filled');
hold on
scatter3(PCA_components(19:27,1), PCA_components(19:27,2), PCA_components(19:27, 3), 'b', 'filled');
hold on
scatter3(PCA_components(27:36,1), PCA_components(27:36,2), PCA_components(27:36, 3), 'y', 'filled');
hold off
xlabel('Component 1');
ylabel('Component 2');
zlabel('Component 3');
grid on
legend('blanket', 'brick', 'grass', 'rice')

%plotting the PCA feature vector for test images. 
figure(3)
scatter3(PCA_components_test(1:1,1), PCA_components_test(1:1,2), PCA_components_test(1:1, 3), 'r', 'filled');
hold on
scatter3(PCA_components_test(6:6,1), PCA_components_test(6:6,2), PCA_components_test(6:6, 3), 'r', 'filled');
hold on
scatter3(PCA_components_test(12:12,1), PCA_components_test(12:12,2), PCA_components_test(12:12, 3), 'r', 'filled');
hold on
scatter3(PCA_components_test(2:2,1), PCA_components_test(2:2,2), PCA_components_test(2:2, 3), 'y', 'filled');
hold on
scatter3(PCA_components_test(3:3,1), PCA_components_test(3:3,2), PCA_components_test(3:3, 3), 'y', 'filled');
hold on
scatter3(PCA_components_test(11:11,1), PCA_components_test(11:11,2), PCA_components_test(11:11, 3), 'y', 'filled');
hold on
scatter3(PCA_components_test(4:4,1), PCA_components_test(4:4,2), PCA_components_test(4:4, 3), 'b', 'filled');
hold on
scatter3(PCA_components_test(7:7,1), PCA_components_test(7:7,2), PCA_components_test(7:7, 3), 'b', 'filled');
hold on
scatter3(PCA_components_test(10:10,1), PCA_components_test(10:10,2), PCA_components_test(10:10, 3), 'b', 'filled');
hold on
scatter3(PCA_components_test(5:5,1), PCA_components_test(5:5,2), PCA_components_test(5:5, 3), 'g', 'filled');
hold on
scatter3(PCA_components_test(8:8,1), PCA_components_test(8:8,2), PCA_components_test(8:8, 3), 'g', 'filled');
hold on
scatter3(PCA_components_test(9:9,1), PCA_components_test(9:9,2), PCA_components_test(9:9, 3), 'g', 'filled');
hold off
xlabel('Component 1');
ylabel('Component 2');
zlabel('Component 3');
grid on
legend('blanket', 'brick', 'grass', 'rice');
