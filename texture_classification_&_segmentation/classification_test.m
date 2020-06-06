% read file 
directory = input("Enter working directory followed by the 'PCA_test.txt: ", 's');
fid = fopen('featurevector_classT.txt', 'rt');
fid_1 = fopen('meanvector_classT.txt', 'rt');
C = textscan(fid, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
D = textscan(fid_1, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
fclose(fid);
fclose(fid_1);

featureVector = cat(2, C{:});
meanVector = cat(2, D{:});

h = ones(12,1);
temp =  h * meanVector;
%calculating zero mean matrix 

zeroMeanVector = featureVector - temp;
zeroMeanVector_matlab = featureVector - mean(featureVector, 1);

[U,S,V] = svd(zeroMeanVector,'econ');
V_r = V(:, (1:3));

PCA_components = featureVector * V_r;

save(directory, 'PCA_components', '-ascii','-tabs');