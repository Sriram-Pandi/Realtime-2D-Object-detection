Srinivas Peri and Sriram Pandi

OS: Windows 11
IDE: visual Studio 2022 
opencv version: 4.6.0. 2022-06-12, using build vc15

After downloading the ConsoleApplication1 zip file add it to your vs 2022 file path and
->Now go to the project icon on the top and to Properties.

->Then in properties go to C/C++ --> General 

->In General Additional Include Directories in place of C:\opencv\build\include give your_file_path\opencv\build\include. 

->Then go to Linker --> General

->In General Additional Library Directories in place of C:\opencv\build\x64\vc15\lib give your yoyu_file_path\opencv\build\x64\vc15\lib 

->Then Go to Linker --> Input

->In Input Additional Dependencies go to the dropdown and choose Edit and add opencv_world460.lib in the empty box.

->Apply all the changes and you should not see any errors.

Now go to Build  and click on build solution.
And Set your configuration manager to Release and x64
you are good to go.
------------------------------------------------------------------------------------------------------------------------------------------------------
When you run the code, you'll see five windows that show the original image, the thresholded image, the cleaned-up image, 
the segmentation image, and the classification image at various stages. In default mode, the segmentation window will show the components 
that have been retrieved and colored, as well as a bounding box, centroid, and least central axis marked by a green line.

In this project, three database files are used: "features.txt" for training data features, "test features.txt" for test data features, 
and "../evaluation.txt" for the confusion matrix for the test dataset.

-> To perform various actions, please use the following keys:

-> To exit the program, press the "q" key.
-> Save the four windows as "origin.jpg", "threshold.jpg", "cleanup.jpg", and "components.jpg" by pressing "a".
-> To enter segmentation mode (the default), press " " (space), and the segmentation window will display all of the top "n" components after filtering (those larger than "min area" and not adjacent to the border).
-> To enter training mode, press "t," and the segmentation window will only mark the major component for you to save to the test dataset.
-> Press "p" to enter evaluation-preparation mode, which displays the same information as training mode but saves the feature to the test dataset.
-> In training or evaluation-preparation mode, press "s," which prompts you to enter the name of the current feature into the terminal's standard input.
-> Press "n" to activate the nearest neighbor classifier for all of the segmentation window's components.
-> Press "k" to activate the knn classifier for all of the segmentation window's components.
-> Press "e" to start the test dataset evaluation based on the training dataset.

/////////////////////////////////////////////////////////////////////

Absolutely! Here's a template for a GitHub README for your project:

---

# Real-time 2-D Object Recognition

### Overview

This project focuses on real-time 2D object recognition. The primary goal is to identify specified objects from a video feed or images placed on a white surface in a translation, scale, and rotation invariant manner. Recognition is achieved using features, thresholding, morphological operations, and classification algorithms like Nearest Neighbor and KNN.

### Setup

For development, a development image set is used. For real-time recognition, a webcam is set up facing downwards on a flat white surface. The system differentiates at least 10 objects based on their 2D shape.

### Tasks Accomplished

1. **Thresholding:** The input video is thresholded to separate the objects from the background.
2. **Image Cleanup:** Morphological filtering is applied to clean up the thresholded image.
3. **Segmentation:** Connected components analysis is used to segment the cleaned image into regions.
4. **Feature Computation:** Features for each major region are computed using moments.
5. **Training Data Collection:** A training mode is set up to collect feature vectors from objects.
6. **Classification:** New feature vectors are classified using the nearest neighbor algorithm with a scaled Euclidean distance.
7. **KNN Classifier:** K-Nearest Neighbor matching is implemented.
8. **Performance Evaluation:** A confusion matrix is built to evaluate the system's performance on test data.
9. **Demo:** A video of the system in action can be found [here](https://drive.google.com/file/d/1ugSMoLBAD2nmhB8U0mDWrcEkVr-9JRxj/view?usp=sharing).

### Solution Details

#### Original Images
- MUG 
- BOX 
- MOUSE 
- POWER BANK 

#### Thresholded Images
- MUG 
- BOX 
- MOUSE 
- POWER BANK 

#### Cleaned Images
- MUG 
- BOX 
- MOUSE 
- POWER BANK 

#### Segmented Images
- MUG 
- BOX 
- MOUSE 
- POWER BANK 

#### Feature Extraction
- MUG 
- BOX 
- MOUSE 
- POWER BANK 

#### Nearest Neighbor Classification
- MUG 
- BOX 
- MOUSE 
- POWER BANK 

#### KNN Classification
- MUG 
- BOX 
- MOUSE 
- POWER BANK 

#### Confusion Matrix
```
          Mug    Box    Mouse  EarPods  Power bank
Mug       3      0      0      0        0
Box       0      3      0      0        0
Mouse     0      0      2      0        0
EarPods   0      0      1      0        0
Powerbank 0      0      0      0        3
```

### Summary

In this project, real-time object recognition was achieved using the nearest neighbor and KNN algorithms. The system is capable of simultaneously detecting and categorizing multiple objects, provided they are sufficiently large and distanced from the frame's edge.

### Reflection

This project provided insights into content-based image retrieval, understanding the backend of object detection, and implementing the grassfire transform. It also emphasized the importance of morphological operations in improving the detection process.

### References and Acknowledgement

- [Reading and Writing CSV files with C++](https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/)
- [OpenCV Moments](https://docs.opencv.org/3.4/d0/d49/tutorial_moments.html)
- [Grassfire Transform](https://github.com/ulikoehler/cv_algorithms/blob/master/doc/Grassfire.md)
- [Video Tutorial](https://youtu.be/yxG2df04YJk)

---

You can use this template and modify it according to your needs when you push your project to GitHub. Remember to always include a `.gitignore` file if you're pushing code to GitHub, so that you avoid uploading unnecessary or sensitive files.




																		
																^^Thank you.^^