#include "image_functions.h"

int threshold(const cv::Mat& src, cv::Mat& dst, int threshold_value) {
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    cv::threshold(src_gray, dst, threshold_value, 255, cv::THRESH_BINARY);
    return 0;
}

int grass_fire_transform(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat src_bin;
    cv::threshold(src, src_bin, 0, 255, cv::THRESH_BINARY);

    cv::Mat dist;
    cv::distanceTransform(src_bin, dist, cv::DIST_L2, cv::DIST_MASK_PRECISE);

    cv::convertScaleAbs(dist, dst);

    return 0;
}


int cleanup(const cv::Mat& src, cv::Mat& dst, int steps) {
    // Create a structuring element for the closing operation
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));

    // Perform a morphological closing operation on the source image
    cv::Mat closed;
    cv::morphologyEx(src, closed, cv::MORPH_CLOSE, kernel);

    // Create a binary mask based on the distance transform of the closed image
    cv::Mat dist;
    cv::distanceTransform(closed, dist, cv::DIST_L2, 3);
    cv::Mat mask = dist > steps;

    // Apply the binary mask to the original source image to obtain the final result
    dst = src.clone();
    dst.setTo(0, mask);
    return 0;
}


// the components do not include background
int segment(cv::Mat& src, cv::Mat& dst, int component_num, std::map<int, cv::Mat>& regions, int min_area, cv::Mat& major) {
    // labels  for each component
    std::set<int> labels;
    cv::Mat label_img(src.size(), CV_32S);
    cv::Mat stats, centroids;
    // get the number of components including the background
    int n_labels = cv::connectedComponentsWithStats(src, label_img, stats, centroids, 8);
    // if only background in the image, return
    if (n_labels <= 1) {
        return -1;
    }

    // find components that are adjacent to the border
    std::set<int> border_components;
    for (int i = 0; i < label_img.rows; i++) {
        for (int j = 0; j < label_img.cols; j++) {
            if (i == 0 || i == label_img.rows - 1 || j == 0 || j == label_img.cols - 1) {
                border_components.insert(label_img.at<int>(i, j));
            }
        }
    }

    // store the components with its label and size of area(skip background)
    std::vector<std::pair<int, int>> components;
    for (int i = 1; i < n_labels; i++) {
        // skip those components that are adjacent to the border
        if (!border_components.count(i)) {
            int area = stats.at<int>(i, cv::CC_STAT_AREA);
            // skip those components whose area is less than min_area
            if (area >= min_area) {
                components.emplace_back(i, stats.at<int>(i, cv::CC_STAT_AREA));
            }
        }
    }
    std::sort(components.begin(),
        components.end(),
        [](const std::pair<int, int>& left, const std::pair<int, int>& right) {
            return left.second > right.second;
        });
    if (components.empty()) {
        return -1;
    }

    // colors for drawing different components
    std::vector<cv::Vec3b> colors(n_labels);
    // background color
    colors[0] = cv::Vec3b(0, 0, 0);
    srand(1);
    for (int i = 1; i < n_labels; i++) {
        colors[i] = cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
    }

    int size = std::min((int)(components.size()), component_num);
    // store the labels the top k components regardless of background or not
    for (int i = 0; i < size; i++) {
        labels.insert(components[i].first);
    }
    for (int i = 0; i < dst.rows; ++i) {
        for (int j = 0; j < dst.cols; ++j) {
            // draw the top k components
            int label = label_img.at<int>(i, j);
            // if the pixel belongs to the top K components
            if (labels.count(label)) {
                dst.at<cv::Vec3b>(i, j) = colors[label];
                // create the mat if it is not in the map yet
                if (!regions.count(label)) {
                    regions.emplace(std::make_pair(label, cv::Mat(src.rows, src.cols, CV_8UC1, cv::Scalar(0))));
                }
                // set the pixel in the corresponding mat as 255
                regions[label].at<uchar>(i, j) = 255;
            }
        }
    }
    // the major component should be the one with largest area
    major = regions[components[0].first];
    return 0;
}

void mark_object(cv::Mat& segmented_img, std::vector<cv::Point> draw_vertices) {
    cv::circle(segmented_img, draw_vertices[0], 2, cv::Scalar(0, 0, 255), 2);
    cv::line(segmented_img, draw_vertices[1], draw_vertices[2], cv::Scalar(0, 255, 0), 2);
    cv::line(segmented_img, draw_vertices[3], draw_vertices[4], cv::Scalar(0, 255, 0), 2);
    cv::line(segmented_img, draw_vertices[4], draw_vertices[6], cv::Scalar(0, 255, 0), 2);
    cv::line(segmented_img, draw_vertices[6], draw_vertices[5], cv::Scalar(0, 255, 0), 2);
    cv::line(segmented_img, draw_vertices[5], draw_vertices[3], cv::Scalar(0, 255, 0), 2);
}

int calculateFeatures(cv::Mat& image, std::vector<double>& feature_vector, std::vector<cv::Point>& draw_vertices) {
    cv::Moments moments = cv::moments(image, true);
    // Calculate centroid
    std::pair<double, double> centroid;
    centroid.first = moments.m10 / moments.m00;
    centroid.second = moments.m01 / moments.m00;
    draw_vertices.emplace_back(cv::Point((int)centroid.first, (int)centroid.second));

    // Calculate angle
    double angle = 0.5 * std::atan2(2.0 * moments.mu11, moments.mu20 - moments.mu02);

    // Transform image vertices to align major axis with x-axis
    std::vector<std::pair<double, double>> transformedVertices;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) == 255) {
                // When transforming, the x is j and y is i
                int x = j, y = i;
                transformedVertices.emplace_back(std::make_pair(
                    std::cos(angle) * (x - centroid.first) + std::sin(angle) * (y - centroid.second),
                    std::cos(angle) * (y - centroid.second) - std::sin(angle) * (x - centroid.first)
                ));
            }
        }
    }

    // Calculate bounding box dimensions
    double quadAxis[4] = { 0., 0., 0., 0. };
    for (std::pair<double, double> p : transformedVertices) {
        // Leftmost x-axis
        quadAxis[0] = std::min(p.first, quadAxis[0]);
        // Rightmost x-axis
        quadAxis[1] = std::max(p.first, quadAxis[1]);
        // Top y-axis
        quadAxis[2] = std::max(p.second, quadAxis[2]);
        // Bottom y-axis
        quadAxis[3] = std::min(p.second, quadAxis[3]);
    }

    // Calculate width and height feature
    double height = quadAxis[2] - quadAxis[3];
    double width = quadAxis[1] - quadAxis[0];
    feature_vector.emplace_back(height / width);

    // Calculate filled ratio feature
    feature_vector.emplace_back(moments.m00 / (height * width));

    // Calculate second moment about the central axis feature
    double theta = angle + CV_PI / 2.;
    double summation = 0.;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) == 255) {
                // transform coordinate system
                double x = j - centroid.first;
                double y = i - centroid.second;
                double new_x = x * cos(theta) + y * sin(theta);
                double new_y = -x * sin(theta) + y * cos(theta);
                transformedVertices.emplace_back(std::make_pair(new_x, new_y));
            }
        }
    }
    // find the minimum area bounding rectangle
    cv::RotatedRect bounding_rect = cv::minAreaRect(transformedVertices);

    // extract features
    feature_vector.push_back(bounding_rect.size.height / bounding_rect.size.width); // aspect ratio
    feature_vector.push_back(bounding_rect.size.area() / (double)image.size().area()); // fill ratio
    feature_vector.push_back(bounding_rect.angle); // orientation angle

    // draw rotated rectangle on the image
    cv::Point2f vertices[4];
    bounding_rect.points(vertices);
    for (int i = 0; i < 4; i++) {
        draw_vertices.push_back(cv::Point((int)vertices[i].x + centroid.first, (int)vertices[i].y + centroid.second));
    }

    return 0;
}