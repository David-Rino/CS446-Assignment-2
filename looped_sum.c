#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/time.h>

static int numberArray[100000000];
//Have to declare it outside as there isn't enough space in the stack for such a large array.
//However, if not using the million file this static global decleration is not needed.

int readFile(char filename[], int* array) {
    int counter = 0;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("File not found...\n");
        return -1;
    }

    printf("Reading File!\n");

    while (fscanf(file, "%d", &array[counter]) != EOF) {
        counter++;
    }

    return counter;

}

long long int sumArray(int inputArray[], int length) {
    long long int totalSum = 0;

    for (int i = 0; i < length; i++) {
        totalSum += inputArray[i];
    } 

    return totalSum;
}

int main(int argc, char* argv[]) {

    int arrayCount = readFile(argv[1], numberArray);

    if (arrayCount == -1) {
        return 1;
    }

    struct timeval startTime, endTime;

    gettimeofday(&startTime, NULL);

    long long int sum = sumArray(numberArray, arrayCount);

    gettimeofday(&endTime, NULL);

    double convertedStart = (startTime.tv_sec * 1000000 + startTime.tv_usec);
    double convertedEnd = (endTime.tv_sec * 1000000 + endTime.tv_usec);

    double elapsedTime = (convertedEnd - convertedStart) / 1000;

    printf("Total value of array: %lld\n", sum);
    printf("Time taken (ms): %f ms\n", elapsedTime);

    return 0;

}

