#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void menu();
int countCommas();
int receiveInput();
int checkLegalInput();
int checkLegalId();
int checkLegalName();
int checkLegalGrade();
int checkLegalAge();
int checkLegalLocation();
void printStudent();
void merge();
void mergeSort();
void quick();
int partition();
void swap();

/**
 * @def int MAX_STUDENTS 5500
 * @brief The maximum number of students.
 */
#define MAX_STUDENTS 5500

/**
 * @def int MAX_AGE_OR_GRADE_LENGTH 3
 * @brief The maximum age or grade length.
 */
#define MAX_AGE_OR_GRADE_LENGTH 3

/**
 * @def int MAX_INPUT_LENGTH 61
 * @brief The maximum input length.
 */
#define MAX_INPUT_LENGTH 61

/**
 * @def MAX_ID_LENGTH 10
 * @brief The maximum id length.
 */
#define MAX_ID_LENGTH 10

/**
 * @def MAX_FIELD_LENGTH 43
 * @brief The maximum length of each field name.
 */
#define MAX_FIELD_LENGTH 43

/**
 * @var char BEST[4]
 * @brief A char array of the command best.
 */
const char BEST[6] =  "best\0";

/**
 * @var char MERGE[5]
 * @brief A char array of the command merge.
 */
const char MERGE[7] =  "merge\0";

/**
 * @var char QUICK[5]
 * @brief A char array of the command quick.
 */
const char QUICK[7] =  "quick\0";

/**
 * @var int FIELDS
 * @brief The number of fields an input should have.
 */
const int FIELDS = 6;

/**
 * @var int ZERO
 * @brief The number of fields an input should have.
 */
const int ZERO = 48;

/**
 * @var int NINE
 * @brief The number of fields an input should have.
 */
const int NINE = 57;

/**
 * @var int SPACE_ASCII_CODE
 * @brief The ascii code for space
 */
const int SPACE_ASCII_CODE = 32;

/**
 * @var int HYPHEN_ASCII_CODE
 * @brief The ascii code for hyphen
 */
const int HYPHEN_ASCII_CODE = 45;

/**
 * @var int BIG_A_ASCII_CODE
 * @brief The ascii code for big a
 */
const int BIG_A_ASCII_CODE = 65;

/**
 * @var int BIG_Z_ASCII_CODE
 * @brief The ascii code for big z
 */
const int BIG_Z_ASCII_CODE = 90;

/**
 * @var int SMALL_A_ASCII_CODE
 * @brief The ascii code for small a
 */
const int SMALL_A_ASCII_CODE = 97;

/**
 * @var int SMALL_Z_ASCII_CODE
 * @brief The ascii code for small z
 */
const int SMALL_Z_ASCII_CODE = 122;

/**
 * @var char EXIT
 * @brief The code for exit.
 */
const char EXIT = 'q';

/**
 * @var char HIGHEST_GRADE[3]
 * @brief The highest grade possible.
 */
const char HIGHEST_GRADE[4] = "100\0";

/**
 * @brief The struct object of a single student with attributes.
 */
typedef struct Students
{
    char id[MAX_ID_LENGTH + 1];
    char name[MAX_FIELD_LENGTH];
    int grade;
    int age;
    char country[MAX_FIELD_LENGTH];
    char city[MAX_FIELD_LENGTH];
    float successful;
}Student;

/**
 * @brief The main function. Starts the program, checks the arguments given.
 * @param args the number of arguments
 * @param args a char array of the arguments
 * @return 0, to tell the system the execution ended without errors or 1 if there was an error.
 */
int main(int argc, char *args[])
{
    if (argc != 2)
    {
        printf("USAGE: the commands are best, merge, quick\n");
        return EXIT_FAILURE;
    }
    else
    {
        if (((int)strlen(args[1]) > (int)strlen(MERGE)) || ((int)strlen(args[1]) == (int)strlen(MERGE) &&
            !strncmp(args[1], BEST, (int)strlen(BEST))) || (strncmp(args[1], BEST, (int)strlen(BEST)) &&
            strncmp(args[1], MERGE, (int)strlen(MERGE)) && strncmp(args[1], QUICK, (int)strlen(QUICK))))
        {
            printf("USAGE: the commands are best, merge, quick\n");
            return EXIT_FAILURE;
        }
    }
    return receiveInput(args[1]);
}

/**
 * @brief Does the sort chosen based on the command that was chosen.
 * @param students[] an array of students
 * @param command[] an array of chars that represent a command
 * @param size the number of students in the array
 */
void menu(Student students[], char command[], int size)
{
    int i;
    if (size == 0)
    {
        return;
    }
    if (strncmp(command, BEST, (int)strlen(BEST)) == 0)
    {
        mergeSort(command, students, 0, size - 1);
        printf("best student info is: ");
        printStudent(size - 1, students);
        return;
    }
    if (strncmp(command, MERGE, (int)strlen(MERGE)) == 0)
    {
        mergeSort(command, students, 0, size - 1);
    }
    if (strncmp(command, QUICK, (int)strlen(QUICK)) == 0)
    {
        quick(students, 0, size - 1);
    }
    for (i = 0; i<size; ++i)
    {
        printStudent(i, students);
    }
}

/**
 * @brief gets the input and creates the students based on the input.
 * @param command[] a char array of the command
 * @return returns 0 if there are no errors and 1 if there was an error
 */
int receiveInput(char command[])
{
    Student students[MAX_STUDENTS];
    char line[MAX_INPUT_LENGTH], id[MAX_FIELD_LENGTH], name[MAX_FIELD_LENGTH], grade[MAX_FIELD_LENGTH];
    char age[MAX_FIELD_LENGTH], country[MAX_FIELD_LENGTH], city[MAX_FIELD_LENGTH];
    int counter = 0, lines = 0, j;
    while (counter < MAX_STUDENTS)
    {
        printf("Enter student info. To exit press q, then enter\n");
        if (fgets(line, MAX_INPUT_LENGTH, stdin) == NULL)
        {
            printf("ERROR: could not read info\n");
            printf("in line %d\n", lines);
            return EXIT_FAILURE;
        }
        if (strncmp(&EXIT, line, 1) == 0 && (int)strlen(line) == 2)
        {
            menu(students, command, counter);
            return EXIT_SUCCESS;
        }
        if (countCommas(line) != FIELDS - 1)
        {
            printf("ERROR: there should be 6 fields separated by a single comma in between\n");
            printf("in line %d\n", lines);
            lines++;
            continue;
        }
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,\n]", id, name, grade, age, country, city);
        if (checkLegalInput(id, name, grade, age, country, city, lines))
        {
            lines++;
            continue;
        }
        lines++;
        Student s;
        sscanf(age, "%d", &s.age);
        sscanf(grade, "%d", &s.grade);
        s.id[MAX_ID_LENGTH] = '\0';
        for (j = 0; j<MAX_FIELD_LENGTH; ++j)
        {
            if (j<MAX_ID_LENGTH)
            {
                s.id[j] = id[j];
            }
            s.name[j] = name[j];
            s.country[j] = country[j];
            s.city[j] = city[j];
        }
        s.successful = (float)s.grade / (float)s.age;
        students[counter] = s;
        counter++;
    }
    return EXIT_FAILURE;
}

/**
 * @brief counts the number of commas in the input.
 * @param line[] the line of input
 * @return the number of commas
 */
int countCommas(char line[])
{
    int counter = 0;
    char *index = strchr(line, ',');
    while (index != NULL)
    {
        counter++;
        index = strchr(index + 1, ',');
    }
    return counter;
}

/**
 * @brief Checks if the input is legal.
 * @param id[] the id of the student
 * @param name[] the name of the student
 * @param grade[] the grade of the student
 * @param age[] the age of the student
 * @param country[] the country of the student
 * @param city[] the city of the student
 * @param lines the number of input lines
 * @return 0 if the input is legal and 1 if otherwise
 */
int checkLegalInput(char id[], char name[], char grade[], char age[], char country[], char city[], int lines)
{
    if (checkLegalId(id, lines) || checkLegalName(name, lines) || checkLegalGrade(grade, lines) ||
        checkLegalAge(age, lines) || checkLegalLocation(country, lines) || checkLegalLocation(city, lines))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Checks if the id is legal.
 * @param id[] the id of the student
 * @param lines the number of input lines
 * @return 0 if the input is legal and 1 if otherwise
 */
int checkLegalId(char id[], int lines)
{
    int i;
    if ((int)strlen(id) != MAX_ID_LENGTH || (int)id[0] == 48)
    {
        printf("ERROR: id has to be at length 10 contain only digits and cannot start with 0\n");
        printf("in line %d\n", lines);
        return EXIT_FAILURE;
    }
    for(i = 0; i < (int)strlen(id) ; ++i)
    {
        if (NINE<(int)(id[i]) || (int)(id[i])<ZERO)
        {
            printf("ERROR: id has to be at length 10 contain only digits and cannot start with 0\n");
            printf("in line %d\n", lines);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Checks if name is legal.
 * @param name[] the name of the student
 * @param lines the number of input lines
 * @return 0 if the input is legal and 1 if otherwise
 */
int checkLegalName(char name[], int lines)
{
    int i;
    for(i = 0; i < (int)strlen(name); ++i)
    {
        if ((int)(name[i]) == SPACE_ASCII_CODE || (int)(name[i]) == HYPHEN_ASCII_CODE ||
            (BIG_A_ASCII_CODE <= (int)(name[i]) && (int)(name[i]) <= BIG_Z_ASCII_CODE) ||
            (SMALL_A_ASCII_CODE <= (int)(name[i]) && (int)(name[i]) <= SMALL_Z_ASCII_CODE))
        {
            continue;
        }
        printf("ERROR: name can only contain alphabetic characters or ’-’, or spaces\n");
        printf("in line %d\n", lines);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Checks if grade is legal.
 * @param grade[] the grade of the student
 * @param lines the number of input lines
 * @return 0 if the input is legal and 1 if otherwise
 */
int checkLegalGrade(char grade[], int lines)
{
    int i;
    if (((int)strlen(grade) > MAX_AGE_OR_GRADE_LENGTH - 1 &&
        strncmp(HIGHEST_GRADE, grade, (int)strlen(HIGHEST_GRADE)) != 0) || (int)strlen(grade) < 1)
    {
        printf("ERROR: grade has to be between 0 to 100\n");
        printf("in line %d\n", lines);
        return EXIT_FAILURE;
    }
    for(i = 0; i < (int)strlen(grade) ; ++i)
    {
        if (NINE<(int)(grade[i]) || (int)(grade[i])<ZERO)
        {
            printf("ERROR: grade has to be between 0 to 100\n");
            printf("in line %d\n", lines);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Checks if the age is legal.
 * @param age[] the age of the student
 * @param lines the number of input lines
 * @return 0 if the input is legal and 1 if otherwise
 */
int checkLegalAge(char age[], int lines)
{
    int i, num;
    if (((int)strlen(age) > MAX_AGE_OR_GRADE_LENGTH || (int)strlen(age) < 1))
    {
        printf("ERROR: age has to be between 18 to 120\n");
        printf("in line %d\n", lines);
        return EXIT_FAILURE;
    }
    for(i = 0; i < (int)strlen(age); ++i)
    {
        if (NINE < (int)(age[i]) || (int)(age[i]) < ZERO)
        {
            printf("ERROR: age has to be between 18 to 120\n");
            printf("in line %d\n", lines);
            return EXIT_FAILURE;
        }
    }
    sscanf(age, "%d", &num);
    if (num < 18 || num > 120)
    {
        printf("ERROR: age has to be between 18 to 120\n");
        printf("in line %d\n", lines);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Checks if the location is legal.
 * @param location[] the location of the student
 * @param lines the number of input lines
 * @return 0 if the input is legal and 1 if otherwise
 */
int checkLegalLocation(char location[], int lines)
{
    int i;
    for(i = 0; i < (int)strlen(location); ++i)
    {
        if ((int)(location[i]) == HYPHEN_ASCII_CODE || (BIG_A_ASCII_CODE <= (int)(location[i]) &&
            (int)(location[i]) <= BIG_Z_ASCII_CODE) || (SMALL_A_ASCII_CODE <= (int)(location[i]) &&
            (int)(location[i]) <= SMALL_Z_ASCII_CODE))
        {
            continue;
        }
        printf("ERROR: location can only contain alphabetic characters or ’-’\n");
        printf("in line %d\n", lines);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Prints the student's details.
 * @param index the index of the student in the array
 * @param students[] the array of the students
 */
void printStudent(int index, Student students[])
{
    printf("%s,", students[index].id);
    printf("%s,", students[index].name);
    printf("%d,", students[index].grade);
    printf("%d,", students[index].age);
    printf("%s,", students[index].country);
    printf("%s\n", students[index].city);
}

/**
 * @brief Sorts the array using merge sort.
 * @param string[] the command given by the user
 * @param students[] the array of the students
 * @param l the left index of the array
 * @param r the right index of the array
 */
void mergeSort(char string[], Student students[], int l, int r)
{
    if (l<r)
    {
        int mid = l + (r - l) / 2;
        mergeSort(string, students, l, mid);
        mergeSort(string, students, mid + 1, r);
        merge(string, students, l, mid, r);
    }
}

/**
 * @brief Sorts the array using merge sort.
 * @param string[] the command given by the user
 * @param students[] the array of the students
 * @param l the left index of the array
 * @param mid the middle index of the array
 * @param r the right index of the array
 */
void merge(char string[], Student students[], int l, int mid, int r)
{
    int i, j, k;
    int n1 = mid - l + 1;
    int n2 =  r - mid;
    Student L[MAX_STUDENTS / 2], R[MAX_STUDENTS / 2];
    for (i = 0; i < n1; i++)
    {
        L[i] = students[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = students[mid + 1 + j];
    }
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (strncmp(string, BEST, (int)strlen(BEST)) == 0)
        {
            if (L[i].successful < R[j].successful)
            {
                students[k] = L[i];
                i++;
            }
            else
            {
                students[k] = R[j];
                j++;
            }
            k++;
        }
        if (strncmp(string, MERGE, (int)strlen(MERGE)) == 0)
        {
            if (L[i].grade < R[j].grade)
            {
                students[k] = L[i];
                i++;
            }
            else
            {
                students[k] = R[j];
                j++;
            }
            k++;
        }
    }
    while (i < n1)
    {
        students[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        students[k] = R[j];
        j++;
        k++;
    }
}

/**
 * @brief Sorts the array using quick sort.
 * @param students[] the array of the students
 * @param l the left index of the array
 * @param r the right index of the array
 */
void quick(Student students[], int l, int r)
{
    if (l < r)
    {
        int p = partition(students, l, r);
        quick(students, l, p - 1);
        quick(students, p + 1, r);
    }
}

/**
 * @brief Sorts the array using quick sort.
 * @param students[] the array of the students
 * @param l the left index of the array
 * @param r the right index of the array
 * @return the partition
 */
int partition (Student students[], int l, int r)
{
    Student pivot = students[r];
    int i = (l - 1), j;
    for (j = l; j <= r - 1; j++)
    {
        if (strncmp(students[j].name, pivot.name, MAX_FIELD_LENGTH) <= 0)
        {
            i++;
            swap(&students[i], &students[j]);
        }
    }
    swap(&students[i + 1], &students[r]);
    return (i + 1);
}

/**
 * @brief Swaps between the 2 students in the array.
 * @param a the pointer to the first student in the array
 * @param b the pointer to the second student in the array
 */
void swap(Student* a, Student* b)
{
    Student t = *a;
    *a = *b;
    *b = t;
}


