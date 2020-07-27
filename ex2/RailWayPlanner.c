#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @brief The struct object of a single Track with it's attributes.
 */
typedef struct Track
{
    char start;
    char end;
    int cost;
    int size;
} Track;

int getInput(const char* file, FILE* output);
int getLegalNum(FILE* f, FILE* output, int *num, int numLine);
int checkLegalConnections(FILE* f, FILE* output, char connectionsArr[], int numLine, int numConnections);
int getLegalParts(FILE* f, FILE* output, char connectionsArr[], int numLine, int length, int numConnections);
int checkLegalSizeOrCost(char str[], FILE* output, int *num, int numLine);
int getMinimumPrice(FILE* output, Track* parts, int length, int num_parts, int numConnections, char connectionsArr[]);
int inConnections(const char *connectionsArr, const char str[], int numConnections);
int getMinimumPriceHelper(Track* parts, char connections[], int *min, int length, int num_parts, int numConnections);
int findChar(char connectionsArr[], int numConnections, char chr);
void freeTable(int **table, int i);
void findMin(int *min, int length, int numConnections, int *const *table);
int createTable(int length, int numConnections, int **table);
void findParts(const Track *parts, char *connectionsArr, int length, int numParts, int numConnections,
               int **table, int i, int j);

/**
 * @def int MAX_LINE_LENGTH 1025
 * @brief The maximum number of chars in a line.
 */
#define MAX_LINE_LENGTH 1025

/**
 * @def int NUM_OF_ARGS 2
 * @brief The number of arguments.
 */
#define NUM_OF_ARGS 2

/**
 * @def int NEW_ARRAY_SIZE 2
 * @brief The addition to the array size.
 */
#define NEW_ARRAY_SIZE 2

/**
 * @def int SECOND_LINE 2
 * @brief The num of the second line.
 */
#define SECOND_LINE 2

/**
 * @def int MIN_LEGAL_LINES 4
 * @brief The minimum number of legit lines.
 */
#define MIN_LEGAL_LINES 4

/**
 * @def char COMMA ','
 * @brief a comma sign.
 */
#define COMMA ","

/**
 * @def int ZERO 0
 * @brief ASCII code for 0.
 */
#define ZERO 48

/**
 * @def int NINE 9
 * @brief ASCII code for 9.
 */
#define NINE 57

/**
 * @def int DECIMAL_ BASE 10
 * @brief base of 10.
 */
#define DECIMAL_BASE 10

/**
 * @def char[] OUTPUT_FILE "railway_planner_output.txt"
 * @brief The path to the output file.
 */
#define OUTPUT_FILE "railway_planner_output.txt"

/**
 * @def char[] FILE_DOESNT_EXIST "File doesn't exists."
 * @brief Error msg when file doesnt exist.
 */
#define FILE_DOESNT_EXIST "File doesn't exists."

/**
 * @def char[] EMPTY_FILE "File is empty."
 * @brief Error msg when file is empty.
 */
#define EMPTY_FILE "File is empty."

/**
 * @def char[] USAGE_ERROR "Usage: RailwayPlanner"
 * @brief Error msg when num of args given is wrong.
 */
#define USAGE_ERROR "Usage: RailwayPlanner <InputFile>"

/**
 * @def char[] LINE_ERROR "Invalid input in line: "
 * @brief Error msg when an error in line occurs.
 */
#define LINE_ERROR "Invalid input in line: "

/**
 * @def char[] MINIMUM_PRICE_MSG "The minimal price is: "
 * @brief The minimum price msg.
 */
#define MINIMUM_PRICE_MSG "The minimal price is: "

/**
 * @var char comma
 * @brief A char of a comma.
 */
const char PAUSA = ',';

/**
 * @brief The main function. Starts the program, checks the arguments given.
 * @param argc the number of arguments
 * @param args a char array of the arguments
 * @return 0, to tell the system the execution ended without errors or 1 if there was an error.
 */
int main(int argc, char *args[])
{
    FILE* output = fopen(OUTPUT_FILE, "w");
    if (output == NULL)
    {
        return EXIT_FAILURE;
    }
    if (argc != NUM_OF_ARGS)
    {
        fprintf(output, USAGE_ERROR);
        fclose(output);
        return EXIT_FAILURE;
    }
    return getInput(args[1], output);
}

/**
 * @brief gets the input.
 * @param file path to the input file.
 * @param output the output file.
 * @return returns 0 if there are no errors and 1 if there was an error
 */
int getInput(const char* file, FILE* output)
{
    int length, numOfConnections, numLine = 1;
    FILE* f = fopen(file, "r");
    if (f == NULL)
    {
        fprintf(output, FILE_DOESNT_EXIST);
        fclose(output);
        return EXIT_FAILURE;
    }
    int c = fgetc(f);
    if (c == EOF)
    {
        fprintf(output, EMPTY_FILE);
        fclose(output);
        fclose(f);
        return EXIT_FAILURE;
    }
    else
    {
        ungetc(c, f);
    }
    if (getLegalNum(f, output, &length, numLine) == EXIT_FAILURE ||
        getLegalNum(f, output, &numOfConnections, ++numLine) == EXIT_FAILURE)
    {
        fclose(f);
        fclose(output);
        return EXIT_FAILURE;
    }
    char* connectionsArr = (char*)malloc(sizeof(char)*numOfConnections);
    if (connectionsArr == NULL)
    {
        fclose(output);
        fclose(f);
        return EXIT_FAILURE;
    }
    if (checkLegalConnections(f, output, connectionsArr, ++numLine, numOfConnections) == EXIT_FAILURE ||
        getLegalParts(f, output, connectionsArr, ++numLine, length, numOfConnections) == EXIT_FAILURE)
    {
        fclose(f);
        fclose(output);
        free(connectionsArr);
        return EXIT_FAILURE;
    }
    free(connectionsArr);
    fclose(output);
    fclose(f);
    return EXIT_SUCCESS;
}

/**
 * @brief checks if the connections input is legal.
 * @param file path to the input file.
 * @param output the output file.
 * @param connectionsArr[] an empty array the size of the connections.
 * @return returns 0 if the connections are legal or 1 if otherwise.
 */
int checkLegalConnections(FILE* f, FILE* output, char connectionsArr[], int numLine, int numConnections)
{
    int j = 1;
    char str[MAX_LINE_LENGTH];
    if ((fgets(str, MAX_LINE_LENGTH, f) == NULL))
    {
        fprintf(output, LINE_ERROR"%d.", numLine);
        return EXIT_FAILURE;
    }
    sscanf(str, "%s", str);
    if (strcmp(&str[0], COMMA) == 0 || strcmp(&str[strlen(str) - 1], COMMA) == 0)
    {
        fprintf(output, LINE_ERROR"%d.", numLine);
        return EXIT_FAILURE;
    }
    connectionsArr[0] = str[0];
    for (int i = 1; i < (int)(strlen(str) - 1); i = i + 2)
    {
        if (strncmp(&str[i], COMMA, 1) == 0 && strncmp(&str[i + 1], COMMA, 1) != 0)
        {
            connectionsArr[j] = str[i + 1];
            j++;
        }
        else
        {
            fprintf(output, LINE_ERROR"%d.", numLine);
            return EXIT_FAILURE;
        }

    }
    if (j != numConnections)
    {
        fprintf(output, LINE_ERROR"%d.", numLine);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief checks if the num is a legal num.
 * @param file path to the input file.
 * @param output the output file.
 * @param *num the number given.
 * @param numLine the number of the line.
 * @return returns 0 if the number is legal or 1 if otherwise.
 */
int getLegalNum(FILE* f, FILE* output, int *num, int numLine)
{
    char str[MAX_LINE_LENGTH];
    if ((fgets(str, MAX_LINE_LENGTH, f) == NULL))
    {
        fprintf(output, LINE_ERROR"%d.", numLine);
        return EXIT_FAILURE;
    }
    sscanf(str, "%s", str);
    if ((int)str[0] == ZERO && strlen(str) != 1)
    {
        fprintf(output, LINE_ERROR"%d.", numLine);
        return EXIT_FAILURE;
    }
    for (int i = 0; i<(int)strlen(str); i++)
    {
        if (ZERO > (int)str[i] || (int)str[i] > NINE)
        {
            fprintf(output, LINE_ERROR"%d.", numLine);
            return EXIT_FAILURE;
        }
    }
    *num = (int)strtol(str, (char **)NULL, DECIMAL_BASE);
    if ((*num >= 0 && numLine == 1) || (*num > 0 && numLine == SECOND_LINE))
    {
        return EXIT_SUCCESS;
    }
    fprintf(output, LINE_ERROR"%d.", numLine);
    return EXIT_FAILURE;
}

/**
 * @brief checks if the parts given are legal.
 * @param file path to the input file.
 * @param output the output file.
 * @param connectionsArr[] an array of all the connections.
 * @param numLine the number of the line.
 * @param length the length of the railway.
 * @return returns 0 if the part input is legal or 1 if otherwise.
 */
int getLegalParts(FILE* f, FILE* output, char connectionsArr[], int numLine, int length, int numConnections)
{
    char str[MAX_LINE_LENGTH];
    char start[MAX_LINE_LENGTH], end[MAX_LINE_LENGTH], size[MAX_LINE_LENGTH], cost[MAX_LINE_LENGTH];
    Track* parts = (Track*)malloc(sizeof(Track));
    if (parts == NULL)
    {
        return EXIT_FAILURE;
    }
    while (fgets(str, MAX_LINE_LENGTH, f) != NULL)
    {
        Track track;
        sscanf(str, "%[^,],%[^,],%[^,],%[^,\r\n]", start, end, size, cost);
        if (inConnections(connectionsArr, start, numConnections) == EXIT_SUCCESS &&
            inConnections(connectionsArr, end, numConnections) == EXIT_SUCCESS)
        {
            track.start = start[0];
            track.end = end[0];
        }
        else
        {
            fprintf(output, LINE_ERROR"%d.", numLine);
            free(parts);
            return EXIT_FAILURE;
        }
        if (checkLegalSizeOrCost(size, output, &track.size, numLine) == EXIT_FAILURE ||
            checkLegalSizeOrCost(cost, output, &track.cost, numLine) == EXIT_FAILURE)
        {
            free(parts);
            return EXIT_FAILURE;
        }
        parts[numLine - MIN_LEGAL_LINES] = track;
        Track* new_parts = (Track*)(realloc(parts, (numLine - NEW_ARRAY_SIZE) * sizeof(Track)));
        if (new_parts == NULL)
        {
            free(parts);
            return EXIT_FAILURE;
        }
        parts = new_parts;
        numLine++;
    }
    if (getMinimumPrice(output, parts, length, numLine - MIN_LEGAL_LINES, numConnections,
        connectionsArr) == EXIT_FAILURE)
    {
        free(parts);
        return EXIT_FAILURE;
    }
    free(parts);
    return EXIT_SUCCESS;
}

/**
 * @brief checks if the connection is in the array of the connections.
 * @param *connectionsArr an array of all the connections.
 * @param str[] the string of the connection.
 * @return returns 0 if the connection is in the array or 1 if otherwise.
 */
int inConnections(const char *connectionsArr, const char str[], int numConnections)
{
    if (strlen(str) != 1)
    {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < numConnections; i++)
    {
        if (connectionsArr[i] == str[0])
        {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

/**
 * @brief checks if size or cost is legals.
 * @param str[] the size or cost.
 * @param output the output file.
 * @param *num the number given.
 * @param numLine the number of the line.
 * @return returns 0 if the part size or cost is legal or 1 if otherwise.
 */
int checkLegalSizeOrCost(char str[], FILE* output, int *num, int numLine)
{
    if ((int)str[0] == ZERO)
    {
        fprintf(output, LINE_ERROR"%d.", numLine);
        return EXIT_FAILURE;
    }
    for (int i = 0; i<(int)strlen(str); i++)
    {
        if (ZERO > (int)str[i] || (int)str[i] > NINE)
        {
            fprintf(output, LINE_ERROR"%d.", numLine);
            return EXIT_FAILURE;
        }
    }
    *num = (int)strtol(str, (char **)NULL, DECIMAL_BASE);
    if (*num > 0)
    {
        return EXIT_SUCCESS;
    }
    fprintf(output, LINE_ERROR"%d.", numLine);
    return EXIT_FAILURE;
}

/**
 * @brief writes to a file the minimum price.
 * @param output the output file.
 * @param parts an array of all the parts.
 * @param length the length of the railway.
 * @param num_parts the number of parts.
 * @param numConnections the number of connections.
 * @param connectionsArr the array of all the connections.
 * @return returns 0 if successful or 1 if otherwise.
 */
int getMinimumPrice(FILE* output, Track* parts, int length, int numParts, int numConnections, char connectionsArr[])
{
    if (length == 0)
    {
        fprintf(output, MINIMUM_PRICE_MSG"0");
        return EXIT_SUCCESS;
    }
    int min = INT_MAX;
    if (getMinimumPriceHelper(parts, connectionsArr, &min, length, numParts, numConnections) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    if (min == INT_MAX || min < 0)
    {
        fprintf(output, MINIMUM_PRICE_MSG"-1");
        return EXIT_SUCCESS;
    }
    fprintf(output, MINIMUM_PRICE_MSG"%d", min);
    return EXIT_SUCCESS;
}

/**
 * @brief finds the minimal price to build a railway with the kength given.
 * @param parts an array of all the parts.
 * @param connectionsArr the array of all the connections.
 * @param min the minimum price.
 * @param length the length of the railway.
 * @param num_parts the number of parts.
 * @param numConnections the number of connections.
 * @return returns 0 if successful or 1 if otherwise.
 */
int getMinimumPriceHelper(Track* parts, char connectionsArr[], int *min, int length, int numParts, int numConnections)
{
    int **table = (int **) malloc(sizeof(int *) * (length + 1));
    if (table == NULL)
    {
        return EXIT_FAILURE;
    }
    if (createTable(length, numConnections, table) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < length + 1; i++)
    {
        for (int j = 0; j < numConnections; j++)
        {
            findParts(parts, connectionsArr, length, numParts, numConnections, table, i, j);
        }
    }
    findMin(min, length, numConnections, table);
    freeTable(table, length + 1);
    return EXIT_SUCCESS;
}

/**
 * @brief tries to assign new parts and updates the cost to the table.
 * @param parts an array of all the parts.
 * @param connectionsArr the array of all the connections.
 * @param length the length of the railway.
 * @param num_parts the number of parts.
 * @param numConnections the number of connections.
 * @param table the table.
 * @param i the index of the length in the table.
 * @param j the index of the connection in the table.
 */
void findParts(const Track *parts, char *connectionsArr, int length, int numParts, int numConnections,
               int **table, int i, int j)
{
    int size, cost, endIndex;
    for (int k = 0; k < numParts; k++)
    {
        if (parts[k].start == connectionsArr[j] && (parts[k].size + i) <= length && table[i][j] != INT_MAX)
        {
            size = parts[k].size + i;
            cost = table[i][j] + parts[k].cost;
            endIndex = findChar(connectionsArr, numConnections, parts[k].end);
            if (table[size][endIndex] > cost)
            {
                table[size][endIndex] = cost;
            }
        }
    }
}

/**
 * @brief creates the table.
 * @param length the length if the railway.
 * @param numConnections the number of connections.
 * @param table the table array.
 * @return returns 0 if successful or 1 if otherwise.
 */
int createTable(int length, int numConnections, int **table)
{
    table[0] = (int *) malloc(sizeof(int) * numConnections);
    if (table[0] == NULL)
    {
        free(table);
        table = NULL;
        return EXIT_FAILURE;
    }
    for (int j = 0; j < numConnections; j++)
    {
        table[0][j] = 0;
    }
    for (int i = 1; i < length + 1; i++)
    {
        table[i] = (int *) malloc(sizeof(int) * numConnections);
            if (table[i] == NULL)
            {
                freeTable(table, i);
                return EXIT_FAILURE;
            }
            for (int j = 0; j < numConnections; j++)
            {
                table[i][j] = INT_MAX;
            }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief find the minimum.
 * @param min the table to be freed.
 * @param length the length of the rail.
 * @param numConnections the number of connections.
 * @param table the table array.
 */
void findMin(int *min, int length, int numConnections, int *const *table)
{
    int curMin = INT_MAX;
    for (int i = 0; i < numConnections; i++)
    {
        if (table[length][i] < curMin)
        {
            curMin = table[length][i];
        }
    }
    *min = curMin;
}

/**
 * @brief frees the table.
 * @param table the table to be freed.
 * @param i the number of columns in the table.
 */
void freeTable(int **table, int i)
{
    for (int j = 0; j < i; ++j)
    {
        free(table[j]);
        table[j] = NULL;
    }
    free(table);
    table = NULL;
}

/**
 * @brief finds the char in the array.
 * @param connectionsArr the array.
 * @param numConnections the length of the array.
 * @param chr the char to find.
 * @return the index where the char is.
 */
int findChar(char connectionsArr[], int numConnections, char chr)
{
    for (int i = 0; i < numConnections; i++)
    {
        if (connectionsArr[i] == chr)
        {
            return i;
        }
    }
    return EXIT_FAILURE;
}

