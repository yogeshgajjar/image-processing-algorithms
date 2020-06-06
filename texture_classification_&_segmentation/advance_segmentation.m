fid = fopen('featurevector.txt', 'rt');
fid_1 = fopen('meanvector.txt', 'rt');
C = textscan(fid, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f');
D = textscan(fid_1, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f');
fclose(fid);
fclose(fid_1);

featureVector = cat(2, C{:});
meanVector = cat(2, D{:});


h = ones(270000,1);
temp =  h * meanVector;
zeroMeanVector = featureVector - temp;

[U,S,V] = svd(zeroMeanVector,'econ');
V_r = V(:, (1:5));

PCA_components = featureVector * V_r;

save('/home/yogesh/Git/image-processing-algorithms/texture classification & segmentation/PCA.txt', 'PCA_components', '-ascii','-tabs');

% [coeff,score,latent] = pca(featureVector);

