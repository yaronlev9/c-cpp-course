//
// Created by user on 11/06/2020.
//

#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "RecommenderSystem.h"

/**
 * @def std::string NOT_SCORED "NA"
 * @brief did not score the movie
 */
#define NOT_SCORED "NA"

/**
 * @def std::string CANT_OPEN "Unable to open file "
 * @brief cant open the file
 */
#define CANT_OPEN "Unable to open file "

/**
 * @def std::string USER_NOT_FOUND  "USER NOT FOUND"
 * @brief user not found error
 */
#define USER_NOT_FOUND "USER NOT FOUND"

/**
 * @def int BAD_PARAM_ERR  -1
 * @brief bad parameters
 */
#define BAD_PARAM_ERR  -1

/**
 * @def double LOW_COS_LIMIT  -1.1
 * @brief low limit for cos function
 */
#define LOW_COS_LIMIT  -1.1

/**
* @fn int loadData(const std::string& moviesAttributesFilePath, const std::string& userRanksFilePath)
* @brief loads the data in the 2 files containing the attributes of movies and the scores users gave them.
* @param moviesAttributesFilePath the first file.
* @param userRanksFilePath the second file.
* @return 0 if loading was successful or 1 if otherwise.
*/
int RecommenderSystem::loadData(const std::string &moviesAttributesFilePath, const std::string &userRanksFilePath)
{
    std::ifstream os1, os2;
    std::string line;
    os1.open(moviesAttributesFilePath);
    os2.open(userRanksFilePath);
    if (!os1)
    {
        std::cerr << CANT_OPEN << moviesAttributesFilePath << std::endl;
        return BAD_PARAM_ERR;
    }
    if (!os2)
    {
        std::cerr << CANT_OPEN << userRanksFilePath << std::endl;
        return BAD_PARAM_ERR;
    }
    _readFirstFile(os1);
    os1.close();
    _readSecondFile(os2);
    os2.close();
    return 0;
}

/**
* @fn std::string recommendByContent(const std::string& userName) const
* @brief recommends the user what movie to watch based on the content.
* @param userName the name of the user.
* @return the string of the movie recommended to watch.
*/
std::string RecommenderSystem::recommendByContent(const std::string &userName) const
{
    if (_ranks.find(userName) == _ranks.end())
    {
        return USER_NOT_FOUND;
    }
    int sum = 0;
    int moviesSeen = 0;
    std::vector<std::pair<int, std::vector<int>>> moviesNotSeen;
    for (auto i = 0; i < (int)_movieNames.size(); i++)
    {
        int scoreForMovie = _ranks.at(userName)[i];
        if (scoreForMovie == 0)
        {
            moviesNotSeen.push_back(std::make_pair(i, _movies.at(_movieNames[i])));
            continue;

        }
        sum += scoreForMovie;
        moviesSeen++;
    }
    std::vector<double> priorityVector;
    priorityVector = _getPriorityVector(userName, sum, moviesSeen, priorityVector);
    double max = LOW_COS_LIMIT;
    std::string betterMovie;
    double priorityVectorSize = 0;
    for (double j : priorityVector)
    {
        priorityVectorSize += j * j;
    }
    return getBestMovie(moviesNotSeen, priorityVector, max, betterMovie, priorityVectorSize);
}

/**
* @fn double predictMovieScoreForUser(const std::string& movieName, const std::string& userName, int k) const
* @brief predicts the users score for a movie he did not see.
* @param movieName the name of the movie.
* @param userName the name of the user.
* @param k the number of movies the user watched that are more similar to the movie to predict.
* @return the score prediction of the user to the movie.
*/
double RecommenderSystem::predictMovieScoreForUser(const std::string &movieName, const std::string &userName, int k)
const
{
    if (_ranks.find(userName) == _ranks.end() || _movies.find(movieName) == _movies.end())
    {
        return BAD_PARAM_ERR;
    }
    std::vector<std::pair<double, int>> movieScores;
    std::vector<int> notSeenMovieVector = _movies.at(movieName);
    double notSeenMovieSize = 0;
    for (int j : notSeenMovieVector)
    {
        notSeenMovieSize += j * j;
    }
    for (auto i = 0; i < (int)_movieNames.size(); i++)
    {
        int scalarResult = 0;
        double movieVectorSize = 0;
        double priority = 0;
        if (_ranks.at(userName)[i] != 0)
        {
            std::vector<int> currentMovieVector = _movies.at(_movieNames[i]);
            _calculateNorm(notSeenMovieVector, currentMovieVector, scalarResult, movieVectorSize);
            priority = scalarResult / (sqrt(notSeenMovieSize) * sqrt(movieVectorSize));
            movieScores.push_back(std::make_pair(priority, i));
        }
    }
    std::sort(movieScores.begin(), movieScores.end());
    double divided = 0;
    double divisor = 0;
    for (int i = (int)movieScores.size() - 1; i >= (int)movieScores.size() - k; i--)
    {
        divided += _ranks.at(userName)[movieScores[i].second] * movieScores[i].first;
        divisor += movieScores[i].first;
    }
    return divided / divisor;
}

/**
* @fn std::string recommendByCF(const std::string& userName, int k) const
* @brief finds a recommended movie to a user based on predicting movies that the user did not see.
* @param k the number of movies the user watched that are more similar to the movie to predict.
* @return the movie recommended to the user.
*/
std::string RecommenderSystem::recommendByCF(const std::string &userName, int k) const
{
    std::string movie;
    if (_ranks.find(userName) == _ranks.end())
    {
        return USER_NOT_FOUND;
    }
    double max = 0.0;
    for (auto i = 0; i < (int)_movieNames.size(); i++)
    {
        if (_ranks.at(userName)[i] == 0)
        {
            double result = predictMovieScoreForUser(_movieNames[i], userName, k);
            if (result > max)
            {
                max = result;
                movie = _movieNames[i];
            }
        }
    }
    return movie;
}

/**
* @fn std::vector<int> &getPriorityVector(const std::string &userName, int numOfMovies, int sum, int moviesSeen,
std::vector<int> &priorityVector) const
* @brief creates the priority vector of the user.
* @param userName the name of the user.
* @param sum the sum of the scores.
* @param moviesSeen the number of movies seen.
* @param priorityVector the priority vector.
* @return the priority vector.
*/
std::vector<double> &RecommenderSystem::_getPriorityVector(const std::string &userName, int sum,
                                                           int moviesSeen, std::vector<double> &priorityVector) const
{
    for (int i = 0; i < _criteriaNum; i++)
    {
        priorityVector.push_back(0);
    }
    double avg = (double) sum / moviesSeen;
    for (auto i = 0; i < (int)_movieNames.size(); i++)
    {
        std::string movie = _movieNames[i];
        if (_ranks.at(userName)[i] != 0)
        {
            for (int j = 0; j < (int)priorityVector.size(); j++)
            {
                priorityVector[j] += (_ranks.at(userName)[i] - avg) * _movies.at(movie)[j];
            }
        }
    }
    return priorityVector;
}

/**
* @fn std::string RecommenderSystem::getBestMovie(const std::vector<std::pair<int, std::vector<int>>> &moviesNotSeen,
const std::vector<double> &priorityVector, double max, std::string &betterMovie, double priorityVectorSize) const
* @brief finds the best movie.
* @param moviesNotSeen a vector of movies not seen.
* @param priorityVector the priority vector.
* @param max max value so far.
* @param betterMovie the best movie.
* @param priorityVectorSize the size of the priority vector.
* @return the best movie to watch.
*/
std::string RecommenderSystem::getBestMovie(const std::vector<std::pair<int, std::vector<int>>> &moviesNotSeen,
                                            const std::vector<double> &priorityVector, double max,
                                            std::string &betterMovie, double priorityVectorSize) const
{
    for (auto &iter : moviesNotSeen)
    {
        double scalarResult = 0;
        double movieVectorSize = 0;
        double priority;
        std::vector<int> movieVector = iter.second;
        for (int j = 0; j < (int)movieVector.size(); j++)
        {
            scalarResult += movieVector[j] * priorityVector[j];
            movieVectorSize += movieVector[j] * movieVector[j];
        }
        priority = scalarResult / (sqrt(priorityVectorSize) * sqrt(movieVectorSize));
        if (max == LOW_COS_LIMIT || priority > max)
        {
            max = priority;
            betterMovie = _movieNames[iter.first];
        }
    }
    return betterMovie;
}

/**
* @fn void calculateNorm(const std::vector<int> &priorityVector, const std::vector<int> &movieVector,
   int &scalarResult, double &movieVectorSize, double &priorityVectorSize) const
* @brief calculates the norm between the 2 vectors.
* @param priorityVector the priority vector.
* @param movieVector the movie vector.
* @param scalarResult the result of the scalar multiplication.
* @param movieVectorSize the size of the movie vector.
* @return the priority vector.
*/
void RecommenderSystem::_calculateNorm(const std::vector<int> &priorityVector, const std::vector<int> &movieVector,
                                       int &scalarResult, double &movieVectorSize) const
{
    for (int j = 0; j < (int)movieVector.size(); j++)
    {
        scalarResult += movieVector[j] * priorityVector[j];
        movieVectorSize += movieVector[j] * movieVector[j];
    }
}

/**
* @fn void readFirstFile(std::ifstream &os1, std::string &line)
* @brief reads the file with the score attributes of all the movies.
* @param os1 the stream object of the first file.
*/
void RecommenderSystem::_readFirstFile(std::ifstream &os1)
{
    std::string line;
    bool flag = false;
    while (std::getline(os1, line))
    {
        std::istringstream iss(line);
        std::string movieName;
        iss >> movieName;
        _movies[movieName] = std::vector<int>();
        for (int score; iss >> score; )
        {
            _movies[movieName].push_back(score);
        }
        if (!flag)
        {
            _criteriaNum = _movies.at(movieName).size();
            flag = true;
        }
    }
}

/**
* @fn void readSecondFile(std::ifstream &os2, std::string &line)
* @brief reads the file with the score all the users gave to each movies.
* @param os2 the stream object of the second file.
*/
void RecommenderSystem::_readSecondFile(std::ifstream &os2)
{
    std::string line;
    if (std::getline(os2, line))
    {
        std::istringstream iss(line);
        for (std::string movieName; iss >> movieName; )
        {
            _movieNames.push_back(movieName);
        }
    }
    while (std::getline(os2, line))
    {
        std::istringstream iss(line);
        std::string userName;
        iss >> userName;
        int i = 0;
        for (std::string score; iss >> score; )
        {
            if (score == NOT_SCORED)
            {
                _ranks[userName].push_back(0);
                i++;
                continue;
            }
            int newScore;
            std::istringstream(score) >> newScore;
            _ranks[userName].push_back(newScore);
            i++;
        }
    }
}
