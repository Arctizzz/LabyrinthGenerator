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

void pregenerateLabyrinth(int rows, int columns, int labyrinth[rows][columns]){
 for(int i = 0; i < rows; i++){
    for(int j = 0; j < columns; j++){
        labyrinth[i][j] = 1111;
    }
 }
}
// 1 1 1 1 | 1 bedeutet Wand, 0 bedeutet keine Wand
// N S W O |
void removeWall(int rows, int columns, int labyrinth[rows][columns], int row, int column) {
    int direction = rand() % 4;

    switch (direction) {
        case 0:  // Nordne
            if (row > 0) {
                labyrinth[row][column] &= 0b0111;        
                labyrinth[row - 1][column] &= 0b1011;   
            }
            break;
        case 1:  // Süden
            if (row < rows - 1) {
                labyrinth[row][column] &= 0b1011;        
                labyrinth[row + 1][column] &= 0b0111;   
            }
            break;
        case 2:  // Westen
            if (column > 0) {
                labyrinth[row][column] &= 0b1101;        
                labyrinth[row][column - 1] &= 0b1110;   
            }
            break;
        case 3:  // Osten
            if (column < columns - 1) {
                labyrinth[row][column] &= 0b1110;        
                labyrinth[row][column + 1] &= 0b1101;   
            }
            break;
    }
}

void saveArrayToTxt(int rows, int columns, int array[rows][columns], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Datei konnte nicht geöffnet werden %s\n", filename);
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
    srand(time(NULL));
    getSize(&columns, &rows);
    int Labyrinth[rows][columns];
    pregenerateLabyrinth(rows, columns, Labyrinth);

    int startingcolumn = rand() % columns;
    int startingrow = rand() % rows;
    
    saveArrayToTxt(rows, columns, Labyrinth, "Labyrinth.txt");

    printf("Labyrinth saved in Labyrinth.txt\n");
    return 0;
}
