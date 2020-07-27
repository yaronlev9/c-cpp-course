// Matrix.h

#ifndef RECOMMENDER_SYSTEM_H
#define RECOMMENDER_SYSTEM_H

#include <iostream>
#include <map>
#include <vector>
#include <numeric>


/**
 * @class Matrix the class of the Matrix
 * @brief The class object of the matrix.
 */
class RecommenderSystem
{

private:

    /**
    * @var _criteriaNum the number of criterias of a movie.
    * @brief the number of criterias of a movie.
    */
    int _criteriaNum;

    /**
    * @var _movies a map of movies and their score attributes.
    * @brief a map of movies and their score attributes.
    */
    std::map<std::string, std::vector<int>> _movies;

    /**
    * @var _ranks a map where each key is a user name and the value is a map of movies and the ranks the user gave them.
    * @brief a map where each key is a user name and the value is a map of movies and the ranks the user gave them.
    */
    std::map<std::string, std::vector<int>> _ranks;

    /**
    * @var _movieNames a vector of movie names.
    * @brief a vector of movie names.
    */
    std::vector<std::string> _movieNames;

    /**
     * @fn void _readFirstFile(std::ifstream &os1, std::string &line)
     * @brief reads the file with the score attributes of all the movies.
     * @param os1 the stream object of the first file.
     */
    void _readFirstFile(std::ifstream &os1);

    /**
     * @fn void _readSecondFile(std::ifstream &os2, std::string &line)
     * @brief reads the file with the score all the users gave to each movies.
     * @param os2 the stream object of the second file.
     */
    void _readSecondFile(std::ifstream &os2);

    /**
     * @fn std::vector<int> & _getPriorityVector(const std::string &userName, int numOfMovies, int sum, int moviesSeen,
        std::vector<int> &priorityVector) const
     * @brief creates the priority vector of the user.
     * @param userName the name of the user.
     * @param sum the sum of the scores.
     * @param moviesSeen the number of movies seen.
     * @param priorityVector the priority vector.
     * @return the priority vector.
     */
    std::vector<double> & _getPriorityVector(const std::string &userName, int sum, int moviesSeen,
                                             std::vector<double> &priorityVector) const;

    /**
     * @fn void _calculateNorm(const std::vector<int> &priorityVector, const std::vector<int> &movieVector,
           int &scalarResult, double &movieVectorSize, double &priorityVectorSize) const
     * @brief calculates the norm between the 2 vectors.
     * @param priorityVector the priority vector.
     * @param movieVector the movie vector.
     * @param scalarResult the result of the scalar multiplication.
     * @param movieVectorSize the size of the movie vector.
     * @return the priority vector.
     */
    void _calculateNorm(const std::vector<int> &priorityVector, const std::vector<int> &movieVector,
                        int &scalarResult, double &movieVectorSize) const;

public:


    /**
     * @fn int loadData(const std::string& moviesAttributesFilePath, const std::string& userRanksFilePath)
     * @brief loads the data in the 2 files containing the attributes of movies and the scores users gave them.
     * @param moviesAttributesFilePath the first file.
     * @param userRanksFilePath the second file.
     * @return 0 if loading was successful or 1 if otherwise.
     */
    int loadData(const std::string &moviesAttributesFilePath, const std::string &userRanksFilePath);

    /**
     * @fn std::string recommendByContent(const std::string& userName) const
     * @brief recommends the user what movie to watch based on the content.
     * @param userName the name of the user.
     * @return the string of the movie recommended to watch.
     */
    std::string recommendByContent(const std::string &userName) const;

    /**
     * @fn double predictMovieScoreForUser(const std::string& movieName, const std::string& userName, int k) const
     * @brief predicts the users score for a movie he did not see.
     * @param movieName the name of the movie.
     * @param userName the name of the user.
     * @param k the number of movies the user watched that are more similar to the movie to predict.
     * @return the score prediction of the user to the movie.
     */
    double predictMovieScoreForUser(const std::string &movieName, const std::string &userName, int k) const;

    /**
     * @fn std::string recommendByCF(const std::string& userName, int k) const
     * @brief finds a recommended movie to a user based on predicting movies that the user did not see.
     * @param k the number of movies the user watched that are more similar to the movie to predict.
     * @return the movie recommended to the user.
     */
    std::string recommendByCF(const std::string &userName, int k) const;

    /**
    * @fn std::string getBestMovie(const std::vector<std::pair<int, std::vector<int>>> &moviesNotSeen,
     const std::vector<double> &priorityVector, double max, std::string &betterMovie, double priorityVectorSize) const
    * @brief finds the best movie.
    * @param moviesNotSeen a vector of movies not seen.
    * @param priorityVector the priority vector.
    * @param max max value so far.
    * @param betterMovie the best movie.
    * @param priorityVectorSize the size of the priority vector.
    * @return the best movie to watch.
    */
    std::string getBestMovie(const std::vector<std::pair<int, std::vector<int>>> &moviesNotSeen,
                             const std::vector<double> &priorityVector, double max, std::string &betterMovie,
                             double priorityVectorSize) const;
};

#endif //RECOMMENDER_SYSTEM_H
