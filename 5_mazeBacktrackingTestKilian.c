#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL 1  // Wand wird als 1 dargestellt
#define PATH 0  // Pfad wird als 0 dargestellt

// Bewegungsrichtungen: Oben, Unten, Links, Rechts (jeweils 2 Felder weiter)
const int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

// Funktion zum Zufälligen Mischen eines Arrays (Fisher-Yates Algorithmus)
void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Rekursive Funktion zur Labyrinth-Erstellung
void generateMaze(int **maze, int breite, int hoehe, int x, int y) {
    maze[y][x] = PATH;  // Aktuellen Punkt als Pfad setzen
    int order[4] = {0, 1, 2, 3};
    shuffle(order, 4);  // Zufällige Bewegungsrichtungen

    for (int i = 0; i < 4; i++) {
        int nx = x + directions[order[i]][0];
        int ny = y + directions[order[i]][1];

        // Prüfen, ob sich die neue Position innerhalb des Labyrinths befindet und noch nicht besucht wurde
        if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1 && maze[ny][nx] == WALL) {
            maze[y + directions[order[i]][1] / 2][x + directions[order[i]][0] / 2] = PATH;  // Verbindung öffnen
            generateMaze(maze, breite, hoehe, nx, ny);  // Rekursion zur weiteren Generierung
        }
    }
}

// Funktion zur Konsolenausgabe des Labyrinths
void printMaze(int **maze, int breite, int hoehe) {
    printf("\n\n");
    for (int i = 0; i < hoehe; i++) {
        printf("  ");
        for (int j = 0; j < breite; j++) {
            printf(maze[i][j] == WALL ? "\u2588\u2588" : "  ");  // Wand: "██", Pfad: "  "
        }
        printf("\n");
    }
    printf("\n");
}

int main() {

    srand(time(NULL));  

    int breite, hoehe;

    
    printf("Breite des Labyrinths (ungerade Zahl) ein: ");
    scanf("%d", &breite);
    if (breite % 2 == 0) breite++;  // Sicherstellen, dass die Breite ungerade ist

    printf("Höhe des Labyrinths (ungerade Zahl) ein: ");
    scanf("%d", &hoehe);
    if (hoehe % 2 == 0) hoehe++;  // Sicherstellen, dass die Höhe ungerade ist

    // Dynamische Speicherreservierung für das Labyrinth
    int **maze = (int **)malloc(hoehe * sizeof(int *));
    for (int i = 0; i < hoehe; i++) {
        maze[i] = (int *)malloc(breite * sizeof(int));
        for (int j = 0; j < breite; j++) {
            maze[i][j] = WALL;  // Das gesamte Labyrinth mit Wänden füllen
        }
    }

    generateMaze(maze, breite, hoehe, 1, 1);  // Labyrinth generieren (Startpunkt bei (1,1))

    maze[1][0] = PATH;  // Eingang auf der linken Seite
    maze[hoehe - 2][breite - 1] = PATH;  // Ausgang auf der rechten Seite

    printMaze(maze, breite, hoehe);  // Labyrinth in der Konsole ausgeben

    // Speicherfreigabe
    for (int i = 0; i < hoehe; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}
