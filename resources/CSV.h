//
// Created by Drew on 4/20/2024.
//

/// https://grouplens.org/datasets/movielens/25m/

#pragma once

#ifndef CSV_H
#define CSV_H

#include "MovieNode.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

class CSVReader {
public:
    unordered_map<int, MovieNode> parseMovies(const string &filePath) {
        unordered_map<int, MovieNode> movies;
        ifstream file(filePath);
        string line, temp;
        stringstream ss;

        if (!file.is_open()) {
            cerr << "Failed to open file: " << filePath << endl;
            return movies;
        }

        getline(file, line); // Skip the header
        while (getline(file, line)) {
            ss.clear();
            ss.str(line);
            int movieId;
            string title, genreStr;

            getline(ss, temp, ',');
            temp = trim(temp);
            if (!isValidNumber(temp)) {
                cerr << "Invalid movie ID: " << temp << endl;
                continue;
            }
            movieId = stoi(temp);

            if (ss.peek() == '"') {
                ss.get();
                getline(ss, title, '"');
                ss.get(); // Skip the comma after the closing quote
            } else {
                getline(ss, title, ',');
            }
            title = trim(title);

            getline(ss, genreStr);
            vector<string> genres;
            ss.clear();
            ss.str(genreStr);
            while (getline(ss, temp, '|')) {
                genres.push_back(trim(temp));
            }

            movies[movieId] = MovieNode(movieId, title, genres);
        }

        file.close();
        return movies;
    }

    void parseRatings(const string &filePath, unordered_map<int, MovieNode> &movies) {
        ifstream file(filePath);
        string line, temp;
        stringstream ss;

        if (!file.is_open()) {
            cerr << "Failed to open file: " << filePath << endl;
            return;
        }

        getline(file, line); // Skip the header
        while (getline(file, line)) {
            ss.clear();
            ss.str(line);
            int movieId;
            double rating;

            getline(ss, temp, ','); // Skip userId
            getline(ss, temp, ',');
            temp = trim(temp);
            if (!isValidNumber(temp)) {
                cerr << "Invalid movie ID: " << temp << endl;
                continue;
            }
            movieId = stoi(temp);

            getline(ss, temp, ',');
            temp = trim(temp);
            rating = stod(temp);

            if (movies.find(movieId) != movies.end()) {
                movies[movieId].addRating(rating); // Use addRating to update sum and count
            }
        }

        file.close();
    }

    string trim(const string& str) {
        auto wsfront = find_if_not(str.begin(), str.end(), [](int c){return isspace(c);});
        auto wsback = find_if_not(str.rbegin(), str.rend(), [](int c){return isspace(c);}).base();
        return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
    }

    bool isValidNumber(const string& str) {
        return str.find_first_not_of("0123456789.-") == string::npos;
    }
};

#endif // CSV_H
