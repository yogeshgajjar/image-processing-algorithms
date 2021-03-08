## Texture Classification

Texture classification deals with the classifying of an image into a class. Image classification is a process in computer vision that deals with identifying the type of category of the image by looking at the features of it. For example, an image classification algorithm may be designed to identify if the image contains a dog or cat. This is a hard and difficult task and involves deep research. After the successful results of Deep Neural Networks, it has imparted a significant impact in the other visual problems such as segmentation and general object detection.

To perform texture classification, we use the law filters. The law filters designed the filter bank of size 5x5 to identify textures from it. Since the good amount of size is needed to determine the texture, 5x5 carries a lot of information.

Law filters can be represented as two different ways i.e. subspace representation and frequency decomposition and frequency band representation. The law filters from 1 to 5 are represented into 25 1D filters after performing the tensor product. In different types of images ranging from smooth to rough, the low frequency bands have more energy than the others. But for similar textures high frequency have more energy than low frequency. We then perform a dimension reduction procedure such as PCA to find the dimensions which generates the
maximum energy.

## Output 

The following is the output after performing K-Means on the feature extracted using Law's filter. The result contains classification on 15D features as well PCA reduced 3D features. 

```
------------------ Test Image with 15D features ----------------

Test Image		Class Label		Class Name		Ground Truth
1			    1	            Grass			Grass
2			    2	            Blanket			Blanket
3			    4	            Rice			Blanket
4			    3	            Brick			Brick
5			    4	            Rice			Rice
6			    1	            Grass			Grass
7			    3	            Brick			Brick
8			    4	            Rice			Rice
9			    4	            Rice			Rice
10			    3	            Brick			Brick
11			    3	            Brick			Blanket
12			    1	            Grass			Grass


Final Class Labels:
Images belong Grass: 1,6,12, and Count for each label is: 3
Images belong to Blanket: 2, and Count for each label is: 1
Images belong to Brick: 4,7,10,11, and Count for each label is: 4
Images belong to Rice: 3,5,8,9, and Count for each label is: 4

Misclassification error rate : 16.6667%


------------------ Test Image with PCA reduced 3D features ----------------

Test Image		Class Label		Class Name		Ground Truth
1			    1	            Grass			Grass
2			    2	            Blanket			Blanket
3			    4	            Rice			Blanket
4			    3	            Brick			Brick
5			    4	            Rice			Rice
6			    1	            Grass			Grass
7			    3	            Brick			Brick
8			    4			    Rice			Rice
9			    2			    Blanket			Rice
10			    3			    Brick			Brick
11			    3			    Brick			Blanket
12			    1			    Grass			Grass


Final Class Labels:
Images belong Grass: 1,6,12, and Count for each label is: 3
Images belong to Blanket: 2,9, and Count for each label is: 2
Images belong to Brick: 4,7,10,11, and Count for each label is: 4
Images belong to Rice: 3,5,8, and Count for each label is: 3

Misclassification error rate : 25%
```




