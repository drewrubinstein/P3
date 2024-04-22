#ifndef MOVIE_GRAPH_H
#define MOVIE_GRAPH_H
#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "MovieNode.h"

using namespace std;

class MovieGraph {
private:
    vector<MovieNode> movies; //List of movie names
    vector<list<pair<int, double>>> adjList;  // Each list element is a pair: (connected vertex index, weight)

public:
    //Constructor, builds the graph as well
    MovieGraph(const vector<MovieNode>& movies) {
        this->movies = movies;
        adjList.resize(movies.size());
        buildGraph();
    }

    //Builds the graph based on the vector given in constructor
    void buildGraph() {
        for (int i = 0; i < movies.size(); i++) {
            for (int j = i + 1; j < movies.size(); j++) {
                double score = similarity(movies[i], movies[j]);
                if (score >= 0 && score <= 1) {
                    adjList[i].emplace_back(j, score);
                    adjList[j].emplace_back(i, score);
                }
            }
        }
    }

    //Calculates a similarity score between 2 movies
    double similarity(MovieNode movie1, MovieNode movie2) {
        // Calculate genre similarity
        set<string> genres1(movie1.genres.begin(), movie1.genres.end());
        set<string> genres2(movie2.genres.begin(), movie2.genres.end());

        vector<string> sharedGenres;
        set_intersection(genres1.begin(), genres1.end(), genres2.begin(), genres2.end(), back_inserter(sharedGenres));
        double genreSimilarity = static_cast<double>(sharedGenres.size()) / (genres1.size() + genres2.size() - sharedGenres.size());

        // Calculate rating similarity (scaled difference)
        double ratingDifference = fabs(movie1.rating - movie2.rating);
        double ratingSimilarity = max(0.0, 1.0 - (ratingDifference / 5.0));  // Assumes ratings are out of 5 because of the csvs

        // Combined similarity score (with genre being more important than the ranking of the movie)
        return 0.75 * genreSimilarity + 0.25 * ratingSimilarity;
    }

    //Finds the 10 most similar movies based on BFS Algorithm for the graph (from class w/ minor adjustments)
    vector<string> findSimilarMoviesBFS(const MovieNode& startMovie) {
        vector<string> result;
        if (movies.empty()) return result;

        map<string, int> movieIndexMap;
        for (int i = 0; i < movies.size(); i++) {
            movieIndexMap[movies[i].movieTitle] = i;
        }

        if (movieIndexMap.find(startMovie.movieTitle) == movieIndexMap.end())
            return result; // Start movie not found

        int startIdx = movieIndexMap[startMovie.movieTitle];
        vector<bool> visited(movies.size(), false);
        queue<int> q;
        visited[startIdx] = true;
        q.push(startIdx);
        int count = 0;

        while (!q.empty() && count < 10) {
            int current = q.front();
            q.pop();
            for (const auto& neighbor : adjList[current]) {
                int neighborIndex = neighbor.first;
                if (!visited[neighborIndex]) {
                    visited[neighborIndex] = true;
                    q.push(neighborIndex);
                    result.push_back(movies[neighborIndex].movieTitle);
                    if (++count == 10) break;  // Stop after finding 10 movies
                }
            }
        }
        return result;
    }

    //Finds the 10 most similar movies based on Dijkstras Algorithm for the graph (from class w/ minor adjustments)
    vector<string> findSimilarMoviesDijkstra(const MovieNode& startMovie) {
        vector<string> result;
        if (movies.empty()) return result;  // Early exit if no movies

        map<string, int> movieIndexMap;
        for (int i = 0; i < movies.size(); i++) {
            movieIndexMap[movies[i].movieTitle] = i;
        }

        if (movieIndexMap.find(startMovie.movieTitle) == movieIndexMap.end())
            return result;  // Exit if start movie is not found

        int startIdx = movieIndexMap[startMovie.movieTitle];
        vector<double> dist(movies.size(), numeric_limits<double>::infinity());
        vector<bool> visited(movies.size(), false);
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

        dist[startIdx] = 0.0;
        pq.push(make_pair(0.0, startIdx));

        while (!pq.empty()) {
            auto [currentDist, currentIdx] = pq.top();
            pq.pop();

            if (visited[currentIdx]) continue;  // Skip already visited nodes
            visited[currentIdx] = true;

            // Skip the start movie from being added to the result vector
            if (currentIdx != startIdx) {
                result.push_back(movies[currentIdx].movieTitle);
                if (result.size() >= 10) break;  // Stop if we have enough results
            }

            for (const auto& neighbor : adjList[currentIdx]) {
                int neighborIdx = neighbor.first;
                double weight = 1 - neighbor.second;  // Assuming weight is (1 - similarity score)
                double newDist = currentDist + weight;

                if (newDist < dist[neighborIdx] && !visited[neighborIdx]) {
                    dist[neighborIdx] = newDist;
                    pq.push(make_pair(newDist, neighborIdx));
                }
            }
        }

        return result;
    }
};

#endif
