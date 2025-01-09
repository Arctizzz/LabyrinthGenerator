#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int getSize(int *columns, int *rows) {
    printf("Bitte die Länge angeben \n");
    scanf("%d", columns);

    printf("Bitte die Höhe angeben \n");
    scanf("%d", rows);
    return 0;
}

void saveArrayToTxt(int rows, int columns, int array[rows][columns], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open File %s\n", filename);
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            fprintf(file, "%d", array[i][j]);
            if (j < columns - 1) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    int rows, columns;

    getSize(&columns, &rows);

    int Labyrinth[rows][columns];

    srand(time(NULL));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            Labyrinth[i][j] = rand() % 2;
        }
    }

    saveArrayToTxt(rows, columns, Labyrinth, "Labyrinth.txt");

    printf("Labyrinth saved to Labyrinth.txt\n");
    return 0;
}
