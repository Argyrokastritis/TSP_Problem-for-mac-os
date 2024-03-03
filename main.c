#include  <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "headers/TSPUTILS.h"
#include "headers/LK.h"
#include "headers/VNS.h"
#include "headers/GPX.h"
#include "headers/SA2OPT.h"


int main() {
    struct Graph graph;

    char inputFilename[MAX_FILENAME_LENGTH];
    selectInputFile(inputFilename);

    readInput(&graph, inputFilename);

    int tour[MAX_NODES];
    for (int i = 0; i < graph.numNodes; i++) {
        tour[i] = i;
    }

    struct timeval start, end;

    //select algorithm
    int choice;
    char algorithmName[MAX_ALGORITHM_NAME];

    printf("Select the desired algorithm:\n");
    printf("  1. Lin-Kernighan (LK)\n");
    printf("  2. Variable Neighborhood Search (VNS)\n");
    printf("  3. Generalized Partition Crossover (GPX)\n");
    printf("  4. Simulated Annealing on 2-Opt Algorithm (SA2OPT)\n");
    printf("Insert your choice (1, 2, 3 or 4) and press Enter: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            strncpy(algorithmName, "LK", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL); //start timer
            lkhAlgorithm(&graph, tour);
            gettimeofday(&end, NULL); //stop timer
            break;
        case 2:
            srand(time(NULL));
            strncpy(algorithmName, "VNS", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL); //start timer
            vnsAlgorithm(&graph, tour, kmax, maxIterations);
            gettimeofday(&end, NULL); //stop timer
            break;
        case 3:
            strncpy(algorithmName, "GPX", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL); //start timer
            gpcxAlgorithm(&graph, tour);
            gettimeofday(&end, NULL); //stop timer
            break;
        case 4:
            strncpy(algorithmName, "SA2OPT", MAX_ALGORITHM_NAME);
            gettimeofday(&start, NULL); //start timer
            twoOpt(&graph, tour);
            gettimeofday(&end, NULL); //stop timer
            break;
        default:
            printf("Invalid choice. Exiting...\n");
            return 1;
    }

    //calculating execution time and tour length
    double executionTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    double finalTourLength = calculateTourLength(&graph, tour);


    writeOutput(&graph, tour, finalTourLength, executionTime, algorithmName, inputFilename, "results");



    printf("\n%s executed successfully, with execution time: %lf seconds\n", algorithmName, executionTime);

    return 0;
}

