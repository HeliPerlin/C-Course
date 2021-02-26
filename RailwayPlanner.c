#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ERROR_MSG_ARGS "Usage: RailwayPlanner <InputFilePath>"
#define ERROR_MSG_FILE "File does not exist."
#define ERROR_MSG_INPUT "Invalid input in line: %lu."
#define ERROR_MSG_EMPTY "File is empty."
#define ANSWER "The minimal price is: %d."

#define NUM_OF_ARGS 2
#define MAX_LINE_LENGTH 1026 // or 1025?
#define DELIMITERS ",\n\r\n"


/**
 * represents a single rail and keeps it's price, length and connections, and the index of the
 * left connection as well
 */
typedef struct Rail
{
    int price;
    int distance;
    char start;
    char end;
    size_t startConnectionIndex;
} Rail;


/**
 * frees a 2D array
 * @param the 2D array to free
 * @param pLength the num of the inner arrays
 */
void freeTable(int **table, int const *pLength)
{
    for (int l = 0; l <= *pLength; l++)
    {
        free(table[l]);
        table[l] = NULL;
    }
    free(table);
    table = NULL;

}
/**
 * frees the array of all single rails,the array of possible connections, the input and output file.
 * @param pConnections the possible connections array to free
 * @param railsArr the rails array to free
 * @param railsInside the length of railsArr
 * @param fInput the input file to free
 * @param fOutput the output file to free
 */
void freeResources(char *pConnections, Rail **railsArr, size_t const railsInside, FILE *fInput,
                   FILE *fOutput)
{
    free(pConnections);
    pConnections = NULL;
    for (size_t i = 0; i < railsInside; i ++)
    {
        free(railsArr[i]);
        railsArr[i] = NULL;
    }
    free(railsArr);
    railsArr = NULL;
    fclose(fInput);
    fclose(fOutput);
}

/**
 * looks for the minimum value in the last sub-array of a given 2D array
 * @param table the 2D array to search in
 * @param length the length of the array
 * @param ConNum the length of a sub array
 * @return the minimum val(price) in the last sub array
 */
int getMinPrice(int **table, int length, size_t ConNum)
{
    size_t i = 0;
    int min = table[length][0];
    for (; i < ConNum; i++)
    {
        if(table[length][i] < min)
        {
            min = table[length][i];
        }
    }
    return min;
}


/**
 * finds the minimal value to fit the table
 * @param railsArr the array of all given single rails
 * @param table the array in which the minimum found val should be put into
 * @param pConnections an array of possible connections to use
 * @param l num of rows
 * @param k num of columns
 * @param pRailsInside length og railsArr
 * @return the minimal value to fit the table
 */
int findMinVal(Rail **railsArr, int **table, char const *pConnections, int l, size_t k, size_t
               const *pRailsInside)
{
    int curMin = INT_MAX;
    for (size_t i = 0; i < *pRailsInside; i++) // iterates over the railsArr and finds matching
        // rails.
    {
        Rail *tempRail = railsArr[i];
        if(tempRail->end == (pConnections[k]) && tempRail->distance == l && tempRail->price <
           curMin)
        {
            curMin = tempRail->price;
        }

        if( tempRail->end == (pConnections[k]) && tempRail->distance < l)
        {
            if((table[l-(tempRail->distance)][tempRail->startConnectionIndex]) != INT_MAX )
            {
                if((table[l-(tempRail->distance)][tempRail->startConnectionIndex] +
                   tempRail->price) < curMin)
                {
                    curMin = (table[l-(tempRail->distance)][tempRail->startConnectionIndex] +
                              tempRail->price);
                }
            }
        }
    }
    return (curMin);
}

/**
 * creates a 2D array from which it's easy to extract to minimal price to build the railway with
 * @param railsArr the array of all single rails
 * @param pConnections the array of all possible connections to use
 * @param pLength the wanted length of the railway
 * @param pConNum the length of pConnections
 * @param pRailsInside the length of railsArr
 * @return a 2D array of integers
 */
int **createTable(Rail **railsArr, char *pConnections, int const *pLength, size_t const
                  *pConNum, size_t *pRailsInside)
{
    int **table = (int**)malloc(sizeof(int *)*(*pLength + 1)); // includes 0
    if (table == NULL)
    {
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <= *pLength; i++)
    {
        table[i] = (int *)malloc(sizeof(int)*(*pConNum));
        if(table[i] == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }
    for (int l = 0; l <= *pLength; l++)
    {
        for (size_t k = 0; k < *pConNum; k++)
        {
            if (l == 0) // base case
            {
                table[l][k] = 0;
            }
            else
            {
                table[l][k] = INT_MAX;
                table[l][k] = findMinVal(railsArr, table, pConnections, l, k, pRailsInside);
            }
        }
    }
    return table;
    // memory is freed later
}


/**
 * checks if a given string is a natural number and converts is to int
 * @param num the string to check
 * @return -1 if fails, the converted number if succeeds.
 */

int checkIfInt(char *num)
{
     char *number = num;
     char *p = NULL;
     if(num == NULL)
     {
         return -1;
     }
     if(*num == '0')
     {
         return 0;
     }
     strtol(num, &p, 10);
     if (*p != '\n' && *p != '\0' && p[0] != '\r')
     {
         return - 1;
     }
     int temp = (int) strtol(number, NULL, 10);
     if (temp < 0)
     {
         return - 1;
     }
     return temp; // num is int
}
/**
 * looks for the index in an array of a given char
 * @param sign the char to look for it's index
 * @param pConnections the array to look in
 * @param pConNum the length ot pConnections
 * @param pErr a flag to return as -1 in case of failure
 * @return the index of the char in the array, and 0+changes the flag if it's not found
 */
size_t findConnectionIndex(char sign, char const* pConnections, size_t const *pConNum, int *pErr)
{
    *pErr = 0;
    size_t ind = 0;
    for (; ind < *pConNum; ind++)
    {
        if (sign == pConnections[ind])
        {
            return ind;
        }
    }
    *pErr = 1;
    return 0;
}
/**
 * checks if a char exists in an array
 * @param sign the char to look for
 * @param pConnections the array to look in
 * @param pConNum the length of pConnections
 * @param pErr a flag to change in case of failure
 * @return the index og the char in the array if found,0+changes the flag if fails
 */
size_t checkConnectionExistence(char sign, char const* pConnections, size_t const *pConNum, int
                                *pErr)
{
    *pErr = 0;
    size_t ind = 0;
    for (; ind < *pConNum; ind++)
    {
        if (sign == pConnections[ind])
        {
            return ind;
        }
    }
    *pErr = 1;
    return 0;
}

/**
 * extracts a number from the first line in a file and makes sure it's not empty
 * @param pLength the value into which to put the number
 * @param fInput the file to extract the number from
 * @param fOutput a file to write to in case of error
 * @return 0 for success, -1 for failure
 */
int processFirstLine(int *pLength, FILE *fInput, FILE *fOutput) // add fOutput
{
    // extract the length of the railway from the first line of the input:
    char buf1[MAX_LINE_LENGTH] = {0};
    if (fgets(buf1, sizeof(buf1), fInput) == NULL)
    {
        fprintf(fOutput, ERROR_MSG_EMPTY);
        return -1;
    }
    if( checkIfInt(buf1) == -1)
    {
        fprintf(fOutput, ERROR_MSG_INPUT, (size_t)1);
        return -1;
    }
    *pLength = (int)(strtol(buf1, NULL, 10));
    return 0;
}


/**
 * extracts an array of possible connections out of an input file
 * @param pConNum keeps the length of the array
 * @param fInput the file to take the information from
 * @param pFlag a flag to change in case of failure
 * @param fOutput a file to write to in case of failure
 * @return an array of chars representing possible connections
 */
char *processSecondLine(size_t *pConNum, FILE *fInput, int *pFlag, FILE *fOutput)
{
    // extract the connections list from the second line:
    size_t capacity = 10;
    char *pConnections = (char*)malloc(sizeof(char)*capacity);
    if (pConnections == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char *token = NULL;
    char buf2[MAX_LINE_LENGTH];
    fgets(buf2, sizeof(buf2), fInput);
    token = strtok(buf2, DELIMITERS);
    if (strlen(token) > 1)
    {
        fprintf(fOutput, ERROR_MSG_INPUT, (size_t)2);
        *pFlag = 1;
        return pConnections;
    }
    size_t charsInside = 0;
    pConnections[charsInside] = *token; // to check if that is ok
    charsInside = 1;
    while ((token = strtok(NULL, DELIMITERS)))
    {
        if(capacity <= charsInside)
        {
            capacity *= 2;
            char *pTempConnections = NULL;
            pTempConnections = (char *)realloc(pConnections, sizeof(char)*(capacity));
            if(pConnections == NULL)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                pConnections = pTempConnections;
            }
        }
        if (strlen(token) > 1)
        {
            fprintf(fOutput, ERROR_MSG_INPUT, (size_t)2);
            *pFlag = 1;
            return pConnections;
        }
        checkConnectionExistence(*token, pConnections, &charsInside, pFlag);
        if(*pFlag == 1)
        {
            *pConNum = charsInside;
            pConnections[charsInside] = *token;
            charsInside ++;
        }
    }
    *pConNum = charsInside;
    *pFlag = 0;
    return pConnections;
    // memory is freed later
}

/**
 * creates an array of rails and keeps their data from a given input file
 * @param fInput the original data file
 * @param fOutput a file to write to in case of failure
 * @param pRailsInside keeps the num of the rails in the array
 * @param pConnections the array of possible connections (to check that the rail data is valid)
 * @param pConNum the length of pConnections
 * @param pFlag a flag to change in case of failure
 * @return an array of rails
 */
Rail **processInput(FILE *fInput, FILE *fOutput, size_t *pRailsInside, char *pConnections, size_t
                    *pConNum, int *pFlag)
{
    // create a Rail out of a line, starting from the third. Add each such Rail to the railsArr:
    size_t capacity = 10;
    Rail **railsArr  = (Rail**) malloc(sizeof(Rail*) * capacity);
    if(railsArr == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char buf3[MAX_LINE_LENGTH] = {0};
    while (fgets(buf3, sizeof(buf3), fInput) != NULL)
    {
        Rail *rail = (Rail*)malloc(sizeof(Rail));
        rail->price = 0;
        rail->distance = 0;
        if (rail == NULL)
        {
            exit(EXIT_FAILURE);
        }
        char *tok0 = strtok(buf3, DELIMITERS);
        char *tok1 = strtok(NULL, DELIMITERS);
        rail->start = *tok0;
        rail->end = *tok1;
        rail->distance = checkIfInt(strtok(NULL, DELIMITERS));
        rail->price = checkIfInt(strtok(NULL, DELIMITERS));
        int err = 0;
        int *pErr = &err;
        rail->startConnectionIndex = findConnectionIndex(rail->start, pConnections, pConNum, pErr);
        if(rail->distance <= 0 || rail->price <= 0 || strlen(tok0) != 1 || strlen(tok1) != 1 ||
           tok0 == NULL || tok1 == NULL) // can take away two last checks
        {
            fprintf(fOutput, ERROR_MSG_INPUT, *pRailsInside + 3);
            *pFlag = 1;
            free(rail);
            rail = NULL;
            return railsArr;
        }

        if ((rail->startConnectionIndex == 0 && err == 1) || ((checkConnectionExistence
            (rail->end, pConnections, pConNum, pErr) == 0) && err == 1))
        {
            fprintf(fOutput, ERROR_MSG_INPUT, *pRailsInside +
            3);
            *pFlag = 1;
            free(rail);
            rail = NULL;
            return railsArr;
        }
        if(*pRailsInside >= capacity)
        {
            capacity *= 2;
            Rail **tempRailsArr = NULL;
            tempRailsArr = (Rail**)realloc(railsArr, sizeof(Rail*)*capacity);
            if(railsArr == NULL)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                railsArr = tempRailsArr;
            }
        }
        railsArr[*pRailsInside] = rail;
        (*pRailsInside) ++;
    }
    *pFlag = 0;
    return railsArr;
    // memory is freed later
}

///////////////////////////////////////////////////////////////////////////////////////////


/**
 * passes the input file to processing and then to the different functions that build the table
 * and analyze it. creates an output file in which the final result or error messages are written
 * to.
 * @param argc number of arguments
 * @param argv the arguments
 * @return 0 if succeeded.
 */
int main(int argc, char *argv[])
{
    FILE *fOutput = fopen("rwp_output.out", "w");
    if( fOutput == NULL)
    {
        return EXIT_FAILURE;
    }
    if(argc != NUM_OF_ARGS)
    {
        fprintf(fOutput, ERROR_MSG_ARGS);
        fclose(fOutput);
        return EXIT_FAILURE;
    }
    if (argv[1] == NULL)
    {
        fprintf(fOutput, ERROR_MSG_FILE);
        fclose(fOutput);
        return EXIT_FAILURE;
    }
    FILE *fInput = fopen(argv[1], "r");
    if (fInput == NULL)
    {
        fprintf(fOutput, ERROR_MSG_FILE);
        fclose(fOutput);
        return EXIT_FAILURE;
    }
    {
        int length = 0;
        size_t conNum = 0;
        int flag = 0;
        int *pLength = &length;
        size_t *pConNum = &conNum;
        int *pFlag = &flag;
        size_t railsInside = 0;
        size_t *pRailsInside = &railsInside;
        if( processFirstLine(pLength, fInput, fOutput) == -1)
        {
            freeResources(NULL, NULL, railsInside, fInput, fOutput);
            return EXIT_FAILURE;
        }// errors

        char *pConnections = processSecondLine(pConNum, fInput, pFlag, fOutput);
        if(*pFlag == 1)
        {
            freeResources(pConnections, NULL, railsInside, fInput, fOutput);
            return EXIT_FAILURE;
        }
        Rail **railsArr = processInput(fInput, fOutput, pRailsInside, pConnections, pConNum, pFlag);
        if(*pFlag == 1)
        {
            freeResources(pConnections, railsArr, railsInside, fInput, fOutput);
            return EXIT_FAILURE;
        }
        int **table = createTable(railsArr, pConnections, pLength, pConNum, pRailsInside);
        int minimum = getMinPrice(table, length, conNum);
        if(minimum == INT_MAX)
        {
            minimum = -1;
        }
        fprintf(fOutput, ANSWER, minimum);
        freeTable(table, pLength);
        freeResources(pConnections, railsArr, railsInside, fInput, fOutput);
    }
    return 0;
}
