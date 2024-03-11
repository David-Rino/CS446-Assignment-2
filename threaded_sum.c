#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/time.h>

static int numberArray[100000000];
//Have to declare it outside as there isn't enough space in the stack for such a large array.
//However, if not using the million file this static global decleration is not needed.


typedef struct _thread_data_t {
    const int *data; //pointer to array of data read from file (ALL)
    int startInd; //starting index of thread’s slice
    int endInd; //ending index of thread’s slice
    long long int *totalSum; //pointer to the total sum variable in main
    pthread_mutex_t *lock; //critical region lock
} thread_data_t;

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

void* arraySum(void* param) {
    thread_data_t* data = (thread_data_t*) param;
    long long int threadSum = 0;

    for (int i = data->startInd; i < data->endInd; ++i) {
        threadSum += data->data[i];
    }

    if (data->lock != NULL) {
        pthread_mutex_lock(data->lock);
        *data->totalSum += threadSum;
        pthread_mutex_unlock(data->lock);
    } else {
        *data->totalSum += threadSum;
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    if (argc == 2) {
        printf("File not found... \n");
        return -1;
    }

    if (argc == 3) {
        printf("Please enter a number of threads that you'd like to calculate with number of threads, filename to open and locks use value");
        return -1;
    }

    int arrayCount = readFile(argv[2], numberArray);

    int numThreads = atoi(argv[1]);
    int useLock = atoi(argv[3]);

    if (numThreads > arrayCount) {
        printf("Too many threads requested\n");
        return -1;
    }

    long long int totalSum = 0;

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    pthread_mutex_t lock;
    pthread_mutex_t *lockPtr = NULL;

    if (useLock) {
        pthread_mutex_init(&lock, NULL);
        lockPtr = &lock;
    }

    thread_data_t threadArray[numThreads];
    pthread_t* threads = malloc(numThreads * sizeof(pthread_t));

    for (int i = 0; i < numThreads; ++i) {
        threadArray[i].totalSum = &totalSum;
        threadArray[i].data = numberArray;
        threadArray[i].lock = lockPtr;
        threadArray[i].startInd = i * (arrayCount / numThreads);
        if (i == numThreads - 1) {
            threadArray[i].endInd = arrayCount;
        } else {
            threadArray[i].endInd = (i + 1) * (arrayCount / numThreads);
        }

        pthread_create(&threads[i], NULL, arraySum, (void*) &threadArray[i]);
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&endTime, NULL);

    double convertedStart = (startTime.tv_sec * 1000000 + startTime.tv_usec);
    double convertedEnd = (endTime.tv_sec * 1000000 + endTime.tv_usec);

    double elapsedTime = (convertedEnd - convertedStart) / 1000;

    printf("Total value of array: %lld\n", totalSum);
    printf("Time taken (ms): %f ms\n", elapsedTime);
    
    if (useLock) {
        pthread_mutex_destroy(&lock);
    } 

    return 0;

}