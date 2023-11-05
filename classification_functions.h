#include <fstream>
#include <vector>
#include "iostream"
#include <map>
#include "math.h"
#include "algorithm"
#include <queue>

#ifndef PROJ3_INCLUDE_DATABASE_H_
#define PROJ3_INCLUDE_DATABASE_H_

// Define constants for the file names
#define FEATURE_FILE_NAME "../features.txt"
#define EVALUATE_FILE_NAME "../test_features.txt"
#define EVALUATE_OUTPUT_FILE_NAME "../evaluation.txt"


// Declare function prototypes
bool is_empty(std::fstream& db_file, std::string file_name);
void clearFile(std::string file_name);
int write_features(std::fstream& db_file, const std::string& feature_name,const std::vector<double>& features,std::string file_name);
std::string nearest_neighbor_classifier(std::fstream& db_file, std::vector<double>& target_feature);
int knn_classifier(std::fstream& db_file, std::vector<double>& target_feature, int k, std::string& nearest_label);
int evaluate(std::fstream& db_file, std::fstream& test_file, int k);

#endif
