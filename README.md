Movie Search Application
This application allows users to search for movies and view recommendations based on the entered movie title using breadth-first search (BFS) and Dijkstra's algorithm. The recommendations are displayed on the same screen.
Requirements
Qt Creator
Qt framework
C++ compiler
Installation and Usage
Open the project in Qt Creator.
Ensure that the necessary Qt framework is installed and configured in your development environment.
Modify the dataset pathing in main.cpp to point to the dataset provided in the GitHub.
Build and run the project in Qt Creator.
Enter the name of a movie in the provided input field.
Click the "Search" button to view BFS and Dijkstra recommendations for the entered movie.
File Structure
main.cpp: Contains the main code for the movie search application.
CSV.h: Header file for CSV parsing functionality.
MovieGraph.h: Header file for the MovieGraph class, which implements graph-related operations.
MovieNode.h: Header file for the MovieNode class, representing a movie node in the graph.
Dependencies
Qt framework: Used for GUI components and event handling.
Credits
This application was created by Drew Rubinstein, Zach Szabo, and Sebastian Robalino.
