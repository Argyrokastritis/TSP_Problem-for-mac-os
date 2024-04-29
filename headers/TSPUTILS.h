#ifndef TSPUTILS_H
#define TSPUTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_ALGORITHM_NAME 10
#define MAX_FILENAME_LENGTH 30

#define MAX_NODES 1100
//#define MAX_FILES 1000  // Adjust this value as needed

struct Node {
    int id;
    double x;
    double y;
};

struct Graph {
    struct Node nodes[MAX_NODES];
    int numNodes;
};

double calculateDistance(struct Node node1, struct Node node2) {
    double x_diff = node1.x - node2.x;
    double y_diff = node1.y - node2.y;
    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

// Function to prompt the user to select a file from a directory
void selectInputFile(char *filename) {
    DIR *dir;
    struct dirent *ent;
    int count = 0;

    // Open the "input_problems" directory
    if ((dir = opendir("input_problems")) != NULL) {
        // List files in the directory
        printf("Select an input file from the \"input_problems\" folder:\n\n");
        while ((ent = readdir(dir)) != NULL) {
            // Ignore directories and special entries
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            // Check if the entry is a regular file
            struct stat file_stat;
            char filepath[MAX_FILENAME_LENGTH];
            snprintf(filepath, MAX_FILENAME_LENGTH, "input_problems/%s", ent->d_name);
            if (stat(filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                printf("  %d. %s\n", count + 1, ent->d_name);
                count++;
            }
        }
        closedir(dir);

        // Prompt the user to select a file by number
        int selectedFile;
        printf("Insert the number of the file you want to select and press Enter: ");
        scanf("%d", &selectedFile);
        printf("\n\n");


        // Find the selected file by number
        count = 0;
        if ((dir = opendir("input_problems")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                // Ignore directories and special entries
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                    continue;
                }

                // Check if the entry is a regular file
                struct stat file_stat;
                char filepath[MAX_FILENAME_LENGTH];
                snprintf(filepath, MAX_FILENAME_LENGTH, "input_problems/%s", ent->d_name);
                if (stat(filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    count++;
                    if (count == selectedFile) {
                        strncpy(filename, filepath, MAX_FILENAME_LENGTH);
                        closedir(dir);
                        return;
                    }
                }
            }
            closedir(dir);
        }
    } else {
        printf("Failed to open the \"input_problems\" folder.\n");
        exit(1);
    }
}
/*// Function to compare two strings for qsort
int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void selectInputFile(char *filename) {
    DIR *dir;
    struct dirent *ent;
    int count = 0;

    // Open the "input_problems" directory
    if ((dir = opendir("input_problems")) != NULL) {
        // Create an array to hold the filenames
        char *filenames[MAX_FILES];
        while ((ent = readdir(dir)) != NULL) {
            // Ignore directories and special entries
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            // Check if the entry is a regular file
            struct stat file_stat;
            char filepath[MAX_FILENAME_LENGTH];
            snprintf(filepath, MAX_FILENAME_LENGTH, "input_problems/%s", ent->d_name);
            if (stat(filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                // Add the filename to the array
                filenames[count] = malloc(strlen(ent->d_name) + 1);
                strcpy(filenames[count], ent->d_name);
                count++;
            }
        }
        closedir(dir);

        // Sort the filenames
        qsort(filenames, count, sizeof(char *), compareStrings);

        // Print the sorted filenames
        printf("Select an input file from the \"input_problems\" folder:\n\n");
        for (int i = 0; i < count; i++) {
            printf("  %d. %s\n", i + 1, filenames[i]);
            free(filenames[i]);
        }

        // Prompt the user to select a file by number
        int selectedFile;
        printf("Insert the number of the file you want to select and press Enter: ");
        scanf("%d", &selectedFile);
        printf("\n\n");

        // Reopen the directory to find the selected file by number
        if ((dir = opendir("input_problems")) != NULL) {
            count = 0;
            while ((ent = readdir(dir)) != NULL) {
                // Ignore directories and special entries
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                    continue;
                }

                // Check if the entry is a regular file
                struct stat file_stat;
                char filepath[MAX_FILENAME_LENGTH];
                snprintf(filepath, MAX_FILENAME_LENGTH, "input_problems/%s", ent->d_name);
                if (stat(filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    count++;
                    if (count == selectedFile) {
                        strncpy(filename, filepath, MAX_FILENAME_LENGTH);
                        closedir(dir);
                        return;
                    }
                }
            }
            closedir(dir);
        }
    } else {
        printf("Failed to open the \"input_problems\" folder.\n");
        exit(1);
    }
}*/

void readInput(struct Graph *graph, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the input file.\n");
        exit(1);
    }

    int id;
    double x, y;
    int count = 0;

    while (fscanf(file, "%d %lf %lf", &id, &x, &y) == 3) {
        graph->nodes[count].id = id;
        graph->nodes[count].x = x;
        graph->nodes[count].y = y;
        count++;
    }

    graph->numNodes = count;
    fclose(file);
}

void writeOutput(struct Graph *graph, int *tour, double tourLength, double executionTime, const char *algorithmName, const char *inputFilename, const char *outputFolder) {

    // Extract the filename from the input path
    const char *inputFilenameOnly = strrchr(inputFilename, '/');
    if (inputFilenameOnly == NULL) {
        // No '/' found, use the whole inputFilename as the filename
        inputFilenameOnly = inputFilename;
    } else {
        // Move past the '/' character
        inputFilenameOnly++;
    }

    // Extract everything before the ".tsp" extension
    // Find the position of the dot before the ".tsp" extension
    const char *dot = strrchr(inputFilenameOnly, '.');
    if (dot == NULL) {
        // No ".tsp" found, use the whole inputFilenameOnly as the base name
        dot = inputFilenameOnly + strlen(inputFilenameOnly);
    }

    // Calculate the length of the base name (without extension)
    size_t length = dot - inputFilenameOnly;

    // Create a buffer for the base name
    char inputFileBaseName[length + 1]; // +1 for the null terminator
    strncpy(inputFileBaseName, inputFilenameOnly, length);
    inputFileBaseName[length] = '\0'; // Null-terminate the string
    // Create the "results" folder if it doesn't exist
    struct stat st = {0};
    /*if (stat(outputFolder, &st) == -1) {
        //mkdir(outputFolder, 0700);
        mkdir(outputFolder);
    }*/
    if (stat(outputFolder, &st) == -1) {
        char command[256];
        sprintf(command, "mkdir %s", outputFolder);
        system(command);
    }

    // Build the output filename
    char outputFilename[100];  // Adjust the size as needed
    snprintf(outputFilename, sizeof(outputFilename), "%s/%s_%s_results.txt", outputFolder, algorithmName, inputFileBaseName);


    FILE *file = fopen(outputFilename, "w");
    if (file == NULL) {
        printf("Failed to open the output file.\n");
        exit(1);
    }


    fprintf(file, "--- %s algorithm results for the problem %s ---\n\n", algorithmName, inputFilenameOnly);
    fprintf(file, "Node_ID    X-coo        Y-coo\n");
    for (int i = 0; i < graph->numNodes; i++) {
        fprintf(file, "%d         %lf   %lf\n", graph->nodes[i].id, graph->nodes[i].x, graph->nodes[i].y);
    }


    fprintf(file, "\nSize of tour: %d\n", graph->numNodes);
    fprintf(file, "MSTlen: %lf\n", tourLength);

    fprintf(file, "\nNumber of unique elements: %d\n", graph->numNodes);

    fprintf(file, "\n---Final Tour---\n");
    for (int i = 0; i < graph->numNodes; i++) {
        fprintf(file, "%d ", graph->nodes[tour[i]].id);
    }
    fprintf(file, "%d\n", graph->nodes[tour[0]].id); // Return to the starting node to complete the cycle

    fprintf(file, "\nFinal Tour Length: %lf\n", tourLength);

    fprintf(file, "\n---Execution Time---\n");
    fprintf(file, "%lf seconds\n", executionTime);


    fclose(file);
}

double calculateTourLength(struct Graph *graph, int *tour) {
    double tourLength = 0.0;
    for (int i = 0; i < graph->numNodes; i++) {
        int currNode = tour[i];
        int nextNode = tour[(i + 1) % graph->numNodes];

        double distance = calculateDistance(graph->nodes[currNode], graph->nodes[nextNode]);
        tourLength += distance;

        //tourLength += calculateDistance(graph->nodes[currNode], graph->nodes[nextNode]);

        // Display the distance for each pair of nodes
        //printf("Distance between nodes %d and %d: %.4lf\n", currNode, nextNode, distance);
        //printf("Current node: %d, Next node: %d\n", currNode, nextNode);
    }
    //printf("\nTotal tour length: %.4lf\n", tourLength); // Print the total tour length
    return tourLength;
}


#endif

