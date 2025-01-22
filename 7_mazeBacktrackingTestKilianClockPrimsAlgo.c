#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> // Für Mikrosekunden-genaue Zeitmessung

#define WALL 1  // Wand wird als 1 dargestellt
#define PATH 0  // Pfad wird als 0 dargestellt

// Bewegungsrichtungen: Oben, Unten, Links, Rechts (jeweils 2 Felder weiter)
const int richtungen[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

// Struktur für eine Wand (x, y)
typedef struct {
    int x, y;
} Wand;

// Funktion zum zufälligen Mischen eines Arrays (Fisher-Yates Algorithmus)
void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Rekursive Backtracking-Methode zur Labyrinth-Erstellung
void generateMaze_Backtracking(int **maze, int breite, int hoehe, int x, int y) {
    maze[y][x] = PATH;  // Aktuellen Punkt als Pfad setzen
    int order[4] = {0, 1, 2, 3};
    shuffle(order, 4);  // Zufällige Bewegungsrichtungen

    for (int i = 0; i < 4; i++) {
        int nx = x + richtungen[order[i]][0];
        int ny = y + richtungen[order[i]][1];

        if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1 && maze[ny][nx] == WALL) {
            maze[y + richtungen[order[i]][1] / 2][x + richtungen[order[i]][0] / 2] = PATH;
            generateMaze_Backtracking(maze, breite, hoehe, nx, ny);
        }
    }
}

// Prim's Algorithmus zur Labyrinth-Erstellung
void generateMaze_Prims(int **maze, int breite, int hoehe) {
    // Startpunkt setzen
    int startX = 1, startY = 1;
    maze[startY][startX] = PATH;

    // Dynamische Liste für Wände
    Wand *wände = (Wand *)malloc(breite * hoehe * sizeof(Wand));
    int wändeAnzahl = 0;

    // Startwände zur Liste hinzufügen
    for (int i = 0; i < 4; i++) {
        int nx = startX + richtungen[i][0];
        int ny = startY + richtungen[i][1];
        if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1) {
            wände[wändeAnzahl++] = (Wand){nx, ny};
        }
    }

    while (wändeAnzahl > 0) {
        // Zufällige Wand wählen
        int index = rand() % wändeAnzahl;
        Wand wand = wände[index];

        // Prüfen, ob die Wand zu einem unbesuchten Bereich führt
        int count = 0, px = 0, py = 0;
        for (int i = 0; i < 4; i++) {
            int nx = wand.x + richtungen[i][0];
            int ny = wand.y + richtungen[i][1];
            if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1 && maze[ny][nx] == PATH) {
                count++;
                px = nx;
                py = ny;
            }
        }

        if (count == 1) {
            maze[wand.y][wand.x] = PATH;
            maze[(wand.y + py) / 2][(wand.x + px) / 2] = PATH;

            // Neue Wände zur Liste hinzufügen
            for (int i = 0; i < 4; i++) {
                int nx = wand.x + richtungen[i][0];
                int ny = wand.y + richtungen[i][1];
                if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1 && maze[ny][nx] == WALL) {
                    wände[wändeAnzahl++] = (Wand){nx, ny};
                }
            }
        }

        // Entferne die Wand aus der Liste
        wände[index] = wände[--wändeAnzahl];
    }

    free(wände);
}

// Funktion zur Konsolenausgabe des Labyrinths
void printMaze(int **maze, int breite, int hoehe) {
    printf("\n\n");
    for (int i = 0; i < hoehe; i++) {
        printf("  ");
        for (int j = 0; j < breite; j++) {
            if (maze[i][j] == WALL) {
                printf("\u2588\u2588");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    int breite, hoehe;
    int algorithmus;

    printf("Breite des Labyrinths (ungerade Zahl) ein: ");
    scanf("%d", &breite);
    if (breite % 2 == 0) breite++;

    printf("Höhe des Labyrinths (ungerade Zahl) ein: ");
    scanf("%d", &hoehe);
    if (hoehe % 2 == 0) hoehe++;

    printf("Algorithmus wählen (1 = Recursive Backtracking, 2 = Prim's Algorithmus): ");
    scanf("%d", &algorithmus);

    // Speicher reservieren
    int **maze = (int **)malloc(hoehe * sizeof(int *));
    for (int i = 0; i < hoehe; i++) {
        maze[i] = (int *)malloc(breite * sizeof(int));
        for (int j = 0; j < breite; j++) {
            maze[i][j] = WALL;
        }
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    if (algorithmus == 1) {
        generateMaze_Backtracking(maze, breite, hoehe, 1, 1);
    } else {
        generateMaze_Prims(maze, breite, hoehe);
    }

    gettimeofday(&end, NULL);

    maze[1][0] = PATH;
    maze[hoehe - 2][breite - 1] = PATH;

    printMaze(maze, breite, hoehe);

    double zeit = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Labyrinthgenerierung dauerte: %.6f Sekunden\n", zeit);

    for (int i = 0; i < hoehe; i++) free(maze[i]);
    free(maze);

    return 0;
}
