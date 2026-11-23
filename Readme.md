# Real-Time 2D Object Recognition with OpenCV

C++ webcam application for segmenting and recognizing objects using image features, nearest-neighbor classification, and k-nearest neighbors.

Build [main.cpp](main.cpp) with [functions.cpp](functions.cpp) and [classification.cpp](classification.cpp), linking against OpenCV. The program opens camera index 0 and clears its feature and evaluation files at startup. Run in a separate writable working directory to preserve earlier experiments.

See the [project report](PRCV.pdf) for the pipeline and evaluation.

## Build and try a training session

With a graphical session, webcam, and OpenCV 4 development files, build with `c++ main.cpp functions.cpp classification.cpp -o recognize $(pkg-config --cflags --libs opencv4)`. Run from a writable working directory. Use `t` to prepare a training sample, `s` to save it, `n` for nearest neighbor, `k` for k-nearest neighbors, and `q` to quit. Use an isolated directory because startup clears project-generated feature and evaluation files.
