
/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Özkan Gezmiş
 * 150200033
*/

#include <iostream>   // For input/output
#include <cmath>      // For distance calculations
#include <vector>     // For vector operations
#include <algorithm>  // For sorting operations
#include <limits>     // For min(), max() functions
#include <fstream>    // To read/write files
#include <chrono>     // For time measurement

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point {
    double x, y;
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2) {
    int x_diff = p1.x - p2.x;
    int y_diff = p1.y - p2.y;
    int dist = x_diff * x_diff + y_diff * y_diff;
    return sqrt(dist);
    //YOUR CODE HERE
}

// Helper function to compare points based on x-coordinate, can use this for sorting.
bool compareX(Point p1, Point p2) {  // if p1.x <= p2.x, return true, else return false
    return p1.x <= p2.x;  // if p1.x >= p2.x, return true, else return false
    //YOUR CODE HERE

}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2) {  // if p1.y <= p2.y, return true, else return false
    return p1.y <= p2.y;  // if p1.y >= p2.y, return true, else return false
    //YOUR CODE HERE

}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point>& points, int start, int end) {
    double min_distance = 1e10;  // set the initial minimum distance as a very large number
    pair<Point, Point> min_pair;  // set the initial minimum pair as an empty pair
    for(int i = start; i < end; i++){
        for(int j = i+1; j < end; j++){
            double dist = distance(points[i], points[j]);
            if(dist < min_distance){
                min_distance = dist;
                min_pair = pair<Point,Point>(points[i], points[j]);
            }
        }
    }
    return min_pair;
    //YOUR CODE HERE
}

// The main objective of the assignment. This is the function to find the closest pair of points recursively using divide and conquer.
pair<Point, Point> closestPair(vector<Point>& points, int start, int end) {

    if(end - start <= 3){  // if there are at most 3 points in the vector
        return bruteForceClosestPair(points, start, end);  // find the closest pair of points using brute force
    }

    int middle = (start + end) / 2;  // find the middle point
    pair<Point,Point> pair1 = closestPair(points, start, middle);  // find the closest pair of points in the left side of the vector
    pair<Point,Point> pair2 = closestPair(points, middle, end);  // find the closest pair of points in the right side of the vector

    double min_dist;  // set the minimum distance as the minimum distance between the closest pairs of points in the left and right sides of the vector
    pair<Point, Point> min_pair;  // set the initial minimum pair
    if(distance(pair1.first, pair1.second) < distance(pair2.first, pair2.second)){  // if the distance between the closest pairs of points in the left side is smaller than the distance between the closest pairs of points in the right side
        min_dist = distance(pair1.first, pair1.second);  // set the minimum distance as the distance between the closest pairs of points in the left side
        min_pair = pair1;  // set the minimum pair as the pair in the left side
    }
    else{
        min_dist = distance(pair2.first, pair2.second);  // set the minimum distance as the distance between the closest pairs of points in the right side
        min_pair = pair2;  // set the minimum pair as the pair in the right side
    }
    int left_bound = points[middle].x - min_dist;  // find the left bound of the strip
    int right_bound = points[middle].x + min_dist;  // find the right bound of the strip
    int leftmost_point = 0;  // set the leftmost point as the first point in the vector
    int rightmost_point = end-1;  // set the rightmost point as the last point in the vector
    while (points[leftmost_point].x < left_bound )  // find the leftmost point in the strip
    {
        leftmost_point++;
    }
    while (points[rightmost_point].x > right_bound)  // find the rightmost point in the strip
    {
        rightmost_point--;
    }
    vector<Point> new_vector = vector<Point>(points.begin() + leftmost_point, points.begin() + rightmost_point+1);  // create a new vector with the points in the strip
    vector<Point> y_sorted = new_vector;  // create a new vector with the points in the strip
    sort(y_sorted.begin(), y_sorted.end(), compareY);  // sort the new vector according to y coordinates

    int max_check_num = 8; // set the maximum number of points to check as 8 for upper and below points
    double min_distance = min_dist;  // set the initial minimum distance as the minimum distance between the closest pairs of points in the left and right sides of the vector
    for(int i=0;i<y_sorted.size();i++){  // for each point in the strip
        for(int j=1 ;j<=max_check_num;j++){  // check the upper points
            if(i+j < y_sorted.size()){  // if the point is in the vector
                double dist = distance(y_sorted[i], y_sorted[i+j]);  // find the distance between the points
                if(dist < min_distance){
                    min_distance = dist;
                    min_pair = pair<Point,Point>(y_sorted[i], y_sorted[i+j]);  // set the minimum distance and the minimum pair
                }
            }
            else 
                break;  // if the point is not in the vector, break the loop
        }
        for(int j=1;j<=max_check_num;j++){  // check the below points
            if(i-j >= 0){  // if the point is in the vector
                double dist = distance(y_sorted[i], y_sorted[i-j]);    // find the distance between the points
                if(dist < min_distance){  
                    min_distance = dist;
                    min_pair = pair<Point,Point>(y_sorted[i], y_sorted[i-j]);  // set the minimum distance and the minimum pair
                }
            }
            else 
                break;  // if the point is not in the vector, break the loop
        }
    }
    pair<Point,Point> sorted_pair;  // set the sorted pair
    if(min_pair.first.y < min_pair.second.y){  // if the y coordinate of the first point is smaller than the y coordinate of the second point
        sorted_pair = min_pair;  // return the pair
    }
    else if(min_pair.first.y == min_pair.second.y){  // if the y coordinatesare the same
        if(min_pair.first.x < min_pair.second.x){  // if the x coordinate of the first point is smaller than the x coordinate of the second point
            sorted_pair = min_pair;  // return the pair
        }
        else{  // if the x coordinate of the first point is greater than the x coordinate of the second point
            sorted_pair = pair<Point,Point>(min_pair.second, min_pair.first);  // return the pair with the first point as the second point and the second point as the first point
        }
    }
    else{  // if the y coordinate of the first point is greater than the y coordinate of the second point
        sorted_pair = pair<Point,Point>(min_pair.second, min_pair.first);  // return the pair with the first point as the second point and the second point as the first point
    }

    return sorted_pair;

    //YOUR CODE HERE

}

// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point>& point_vector, pair<Point,Point> point_pair) {  // remove the pair from the vector
    int position1 = 0, position2 = 0;  // set the initial positions as 0
    for(int i = 0; i < point_vector.size(); i++){  // find the positions of the points in the vector
        if(point_vector[i].x == point_pair.first.x && point_vector[i].y == point_pair.first.y){  // if the first point is found, set the position1 as i
            position1 = i;
        }
        if(point_vector[i].x == point_pair.second.x && point_vector[i].y == point_pair.second.y){  // if the second point is found, set the position2 as i
            position2 = i;
        }
    }
    point_vector.erase(point_vector.begin() + position1);  // erase the first point from the vector
    point_vector.erase(point_vector.begin() + position2-1);  // erase the second point from the vector, since the first point is already erased, the position of the second point is decreased by 1
    return point_vector;
    //YOUR CODE HERE

}

// This function should wrap up the entire algorithm. It should:
//    1- Find the closest pair
//    2- Add the pair to the "pairs" vector
//    3- Remove the paired result from the map
// at each iteration.
// Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points) {
    vector<pair<Point,Point> > pairs; //add your pairs to this vector
    Point unconnected = {-1,-1}; //set this as the unconnected point if it exists, leave as is otherwise.

    //YOUR CODE HERE

    bool brute_force = false;  // if brute force is selected, set the boolean as true
    // brute force part
    if(brute_force){  // if brute force is selected
        sort(points.begin(), points.end(), compareY);  // first sort the array according to y coordinates
        while (points.size() >= 2){  // while there are at least 2 points in the vector
            pair<Point, Point> closest_pair = bruteForceClosestPair(points, 0, points.size());  // find the closest pair of points
            pairs.push_back(closest_pair);  // add the pair to the pairs vector
            points = removePairFromVector(points, closest_pair);  // remove the pair from the vector
        }
        if (points.size() == 1){  // if there is only one point left in the vector
            unconnected = points[0];  // set the unconnected point as the only point left in the vector
        }
    }
    

    // divide and conquer part
    if(!brute_force){  // if divide and conquer is selected
        sort(points.begin(), points.end(), compareX);  // first sort the array according to x coordinates
        while (points.size() >= 2){  // while there are at least 2 points in the vector
            pair<Point, Point> closest_pair = closestPair(points, 0, points.size());  // find the closest pair of points using divide and conquer
            pairs.push_back(closest_pair);  // add the pair to the pairs vector
            points = removePairFromVector(points, closest_pair);  // remove the pair from the vector
        }
        if (points.size() == 1){  // if there is only one point left in the vector
            unconnected = points[0];  // set the unconnected point as the only point left in the vector
        }
    }



    //before printing, please make sure that within each pair, the city with the smaller y coordinate is printed first...
    //...if the y coordinates are the same, print the city with the smaller x coordinate first.

    //This part prints the output, don't modify.    
    for(size_t i = 0; i < pairs.size(); i++){
        cout << "Pair " << i+1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1){
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y<<endl;
    }


}

//Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
vector<Point> readCoordinatesFromFile(const string& filename) {
    vector<Point> points;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "File cannot be opened!"<<endl;
        exit(1);
    }
    string line;

    while (getline(file, line)) {  // read line by line
        Point p;
        sscanf(line.c_str(), "%lf %lf", &p.x, &p.y);  // read coordinates from file
        points.push_back(p);
    }
    //YOUR CODE HERE

    return points;
}

// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    // argv[1] = "case0.txt";
    vector<Point> points = readCoordinatesFromFile(argv[1]);
    findClosestPairOrder(points);
    return 0;
}
