clear all
clc
fid = fopen('featurevector_class25D.txt', 'rt');
C = textscan(fid, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
fclose(fid);
featureVector = cat(2, C{:});

% X_train = featureVector;
% intra_var=zeros(2,15);
% inter_var=zeros(2,15);
% 
% for i=1:15
%     var1 = var(X_train(1:9,i));
%     var2 = var(X_train(10:18,i));
%     var3 = var(X_train(19:27,i));
%     var4 = var(X_train(28:36,i));
%     mean_var = (var1+var2+var3+var4)/4;
%     intra_var(:,i) = [i;mean_var];
% end
% %intra_var = sortrows(intra_var.',2).'; %intra class variance minimum
% 
% for i=1:15
%     m1 = mean(X_train(1:9,i));
%     m2 = mean(X_train(10:18,i));
%     m3 = mean(X_train(19:27,i));
%     m4 = mean(X_train(28:36,i));
%     varm = var([m1,m2,m3,m4]);
%     inter_var(:,i) = [i;varm];
% end
% %inter_var = sortrows(inter_var.',2,'descend').'; %inter class variance maximum
% 
% dis_pow = zeros(2,15);
% for i=1:15
%     dis_pow(1,i) = i;
%     if intra_var(2,i)==0
%         intra_var(2,i)=10^-6;
%     end
%     dis_pow(2,i) = inter_var(2,i)/intra_var(2,i);
% end
% dis_pow = sortrows(dis_pow.',2,'descend').';
% X = ['Feature with highest discriminant power is: '+string(dis_pow(1,1))+' which is E5E5'];
% disp(X);
% Y = ['Feature with lowest discriminant power is: '+string(dis_pow(1,end))+' which is L5L5'];
% disp(Y);

X_train = featureVector;
intra_var=zeros(2,25);
inter_var=zeros(2,25);

for i=1:25
    var1 = var(X_train(1:9,i));
    var2 = var(X_train(10:18,i));
    var3 = var(X_train(19:27,i));
    var4 = var(X_train(28:36,i));
    mean_var = (var1+var2+var3+var4)/4;
    intra_var(:,i) = [i;mean_var];
end
%intra_var = sortrows(intra_var.',2).'; %intra class variance minimum

for i=1:25
    m1 = mean(X_train(1:9,i));
    m2 = mean(X_train(10:18,i));
    m3 = mean(X_train(19:27,i));
    m4 = mean(X_train(28:36,i));
    varm = var([m1,m2,m3,m4]);
    inter_var(:,i) = [i;varm];
end
%inter_var = sortrows(inter_var.',2,'descend').'; %inter class variance maximum

dis_pow = zeros(2,25);
for i=1:25
    dis_pow(1,i) = i;
    if intra_var(2,i)==0
        intra_var(2,i)=10^-6;
    end
    dis_pow(2,i) = inter_var(2,i)/intra_var(2,i);
end
dis_pow = sortrows(dis_pow.',2,'descend').';
X = ['Feature with highest discriminant power is: '+string(dis_pow(1,1))+' which is E5E5'];
disp(X);
Y = ['Feature with lowest discriminant power is: '+string(dis_pow(1,end))+' which is L5E5'];
disp(Y);



