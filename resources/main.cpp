#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "CSV.h"
#include "MovieGraph.h"
#include "MovieNode.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CSVReader movieReader;
    string moviesPath = "C:\\Users\\dansz\\Downloads\\Movies and Ratings-20240422T025245Z-001\\Movies and Ratings\\truncated_movies_1000.csv";
    auto movies = movieReader.parseMovies(moviesPath);

    string ratingsPath = "C:\\Users\\dansz\\Downloads\\Movies and Ratings-20240422T025245Z-001\\Movies and Ratings\\simulated_ratings_10000.csv";
    movieReader.parseRatings(ratingsPath, movies);

    vector<MovieNode> movieNodes;
    for (const auto& pair : movies) {
        movieNodes.push_back(pair.second);
    }

    MovieGraph graph(movieNodes);

    QWidget window;
    window.setWindowTitle("Movie Search");

    QGridLayout layout(&window);

    QLineEdit movieLineEdit;
    movieLineEdit.setPlaceholderText("Enter movie name");
    layout.addWidget(&movieLineEdit, 0, 0, 1, 2);

    QPushButton searchButton("Search");
    layout.addWidget(&searchButton, 0, 2, 1, 1);

    QLabel bfsTitle("BFS Recommendations:");
    layout.addWidget(&bfsTitle, 1, 0, 1, 1);

    QLabel dijkstraTitle("Dijkstra Recommendations:");
    layout.addWidget(&dijkstraTitle, 1, 1, 1, 1);

    QLabel bfsResultsLabel;
    layout.addWidget(&bfsResultsLabel, 2, 0, 1, 1);

    QLabel dijkstraResultsLabel;
    layout.addWidget(&dijkstraResultsLabel, 2, 1, 1, 1);

    QObject::connect(&searchButton, &QPushButton::clicked, [&]() {
        QString movieName = movieLineEdit.text().trimmed();

        auto searchIt = find_if(movieNodes.begin(), movieNodes.end(), [&](const MovieNode& node) {
            return QString::fromStdString(node.movieTitle) == movieName;
        });

        if (searchIt != movieNodes.end()) {
            vector<string> bfsResults = graph.findSimilarMoviesBFS(*searchIt);
            QString bfsResultText;
            for (size_t i = 0; i < bfsResults.size(); ++i) {
                bfsResultText += QString::number(i + 1) + ". " + QString::fromStdString(bfsResults[i]) + "<br>";
            }
            bfsResultsLabel.setText(bfsResultText);

            vector<string> dijkstraResults = graph.findSimilarMoviesDijkstra(*searchIt);
            QString dijkstraResultText;
            for (size_t i = 0; i < dijkstraResults.size(); ++i) {
                dijkstraResultText += QString::number(i + 1) + ". " + QString::fromStdString(dijkstraResults[i]) + "<br>";
            }
            dijkstraResultsLabel.setText(dijkstraResultText);
        } else {
            bfsResultsLabel.setText("Movie '" + movieName + "' not found.");
            dijkstraResultsLabel.setText("Movie '" + movieName + "' not found.");
        }
    });

    window.show();

    return app.exec();
}
