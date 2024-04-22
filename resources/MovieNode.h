#ifndef P3TESTING_MOVIENODE_H
#define P3TESTING_MOVIENODE_H

#endif //P3TESTING_MOVIENODE_H
#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct MovieNode {

    int movieId = -1;
    string movieTitle;
    vector<string> genres;
    double rating = 0.0; // This will store the computed average rating
    double ratingSum = 0.0; // Sum of all ratings received
    int ratingCount = 0; // Number of ratings received

    MovieNode() = default; // Default constructor

    MovieNode(int id, string title, vector<string> g, double initialRating = 0.0)
            : movieId(id), movieTitle(title), genres(g), rating(initialRating), ratingSum(0.0), ratingCount(0) {}

    //Used for rating because the CSV gives multiple user ratings and we calculate the rating as the average of all user ratings
    void addRating(double newRating) {
        ratingSum += newRating;
        ratingCount++;
        rating = ratingSum / ratingCount; // Update average rating
    }
};
