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
    int startX = 1, startY = 1;
    maze[startY][startX] = PATH;

    Wand *frontier = (Wand *)malloc(breite * hoehe * sizeof(Wand));
    int frontierCount = 0;

    for (int i = 0; i < 4; i++) {
        int nx = startX + richtungen[i][0];
        int ny = startY + richtungen[i][1];
        if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1) {
            frontier[frontierCount++] = (Wand){nx, ny};
        }
    }

    while (frontierCount > 0) {
        int index = rand() % frontierCount;
        Wand wand = frontier[index];
        frontier[index] = frontier[--frontierCount];

        int px = 0, py = 0;
        for (int i = 0; i < 4; i++) {
            int nx = wand.x + richtungen[i][0];
            int ny = wand.y + richtungen[i][1];
            if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1 && maze[ny][nx] == PATH) {
                px = nx;
                py = ny;
                break;
            }
        }

        maze[wand.y][wand.x] = PATH;
        maze[(wand.y + py) / 2][(wand.x + px) / 2] = PATH;

        for (int i = 0; i < 4; i++) {
            int nx = wand.x + richtungen[i][0];
            int ny = wand.y + richtungen[i][1];
            if (nx > 0 && ny > 0 && nx < breite - 1 && ny < hoehe - 1 && maze[ny][nx] == WALL) {
                frontier[frontierCount++] = (Wand){nx, ny};
            }
        }
    }

    free(frontier);
}

// Funktion zur Konsolenausgabe des Labyrinths
void printMaze(int **maze, int breite, int hoehe, char wandsymbol, char pfadsymbol) {
    printf("\n\n");
    for (int i = 0; i < hoehe; i++) {
        printf("  ");
        for (int j = 0; j < breite; j++) {
            printf("%c%c", maze[i][j] == WALL ? wandsymbol : pfadsymbol, maze[i][j] == WALL ? wandsymbol : pfadsymbol);

        }
        printf("\n");
    }
    printf("\n");
}

// Funkion für eine zusätzliche Konsolenausgabe mit Loesungsweg je Wahl
void PrintMazesolution(int **maze, int breite, int hoehe, char wandsymbol, char pfadsymbol) {
    printf("\n\n");
    for (int i = 0; i < hoehe; i++) {
        printf("  ");
        for (int j = 0; j < breite; j++) {
            printf("%c%c", maze[i][j] == WALL ? wandsymbol : maze[i][j] == 2 ? '.' : pfadsymbol, maze[i][j] == WALL ? wandsymbol : maze[i][j] == 2 ? '.' : pfadsymbol);

        }
        printf("\n");
    }
    printf("\n");
}

// Funktion zum Speichern des Labyrinths in einer Datei
void SaveMazeToFile(int **maze, int breite, int hoehe, char wandsymbol, char pfadsymbol, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Fehler beim Öffnen der Datei.\n");
        return;
    }

    for (int i = 0; i < hoehe; i++) {
        for (int j = 0; j < breite; j++) {
            fputc(maze[i][j] == WALL ? wandsymbol : maze[i][j] == 2 ? '.' : pfadsymbol, file);
        }
        fputc('\n', file);
    }

    fclose(file);
    printf("Labyrinth wurde in '%s' gespeichert.\n", filename);
}

// Funkion um ein Loesungsweg zu finden (DFS)
int SolveMaze(int **maze, int breite, int hoehe, int x, int y, int exitx, int exity, int *langeStraße){

    if (x == exitx && y == exity){

        maze[y][x] = 2; // loesungspfad wird markiert
        return 1;
    }

    if (x < 0 || y < 0 || x >= breite || y >= hoehe || maze[y][x] != PATH) return 0;

    maze[y][x] = 2; // Punkt als loesungspfad markiert
    (*langeStraße)++;
    

    if (SolveMaze(maze, breite, hoehe, x, y - 1, exitx, exity, langeStraße) ||   // oben
        SolveMaze(maze, breite, hoehe, x, y + 1, exitx, exity, langeStraße) ||   // unten
        SolveMaze(maze, breite, hoehe, x - 1, y, exitx, exity, langeStraße) ||   // links
        SolveMaze(maze, breite, hoehe, x + 1, y, exitx, exity, langeStraße))     // rechts
        {
            return 1;
        }
    
    maze[y][x] = PATH;
    (*langeStraße)--;
    return 0;

}

// Sackgassen und Anzahl der Wege zählen
int CountDeadEnds(int **maze, int breite, int hoehe) {
    int deadends = 0;
    int pathAnzahl = 0;

    for (int y = 1; y < hoehe - 1; y++) {  // Schleife durch alle Reihen (außer Rand)
        for (int x = 1; x < breite - 1; x++) {  // Schleife durch alle Spalten (außer Rand)
            if (maze[y][x] == PATH) {  // Wenn es ein Pfad ist
                int offeneNachbarn = 0;

                // Überprüfe Nachbarn
                if (maze[y - 1][x] == PATH) offeneNachbarn++;  // Oben
                if (maze[y + 1][x] == PATH) offeneNachbarn++;  // Unten
                if (maze[y][x - 1] == PATH) offeneNachbarn++;  // Links
                if (maze[y][x + 1] == PATH) offeneNachbarn++;  // Rechts

                //printf("Feld (%d, %d): Offene Nachbarn = %d\n", x, y, offeneNachbarn); hatte probleme

                // Wenn genau 1 offener Nachbar (Dead End), zähle es
                if (offeneNachbarn == 1){
                    //printf("-> Dead End gefunden bei (%d, %d)\n", x, y); hatte probleme
                    deadends++; 
                }
                pathAnzahl++;
            } 
        }
    }

    printf("Gesamtzahl der Dead Ends: %d\n", deadends);
    printf("Gesamtzahl der Wege: %d\n", pathAnzahl);

    return deadends;  // Gesamtanzahl der Sackgassen
}

int main() {
    srand(time(NULL));

    int breite, hoehe;
    int algorithmus;
    int loesung;
    int langeStraße = 0;

    printf("Breite des Labyrinths (ungerade Zahl) ein: ");
    scanf("%d", &breite);
    if (breite % 2 == 0) breite++;

    printf("Hoehe des Labyrinths (ungerade Zahl) ein: ");
    scanf("%d", &hoehe);
    if (hoehe % 2 == 0) hoehe++;

    printf("Algorithmus wählen (1 = Recursive Backtracking, 2 = Prim's Algorithmus): ");
    scanf("%d", &algorithmus);

    // Symbole für die Darstellung
    char wandSymbol = '#';
    char pfadSymbol = ' ';

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

    // Backtracking-Algorithmus aufrufen
     if (algorithmus == 1) {
        generateMaze_Backtracking(maze, breite, hoehe, 1, 1);
    } else {
        generateMaze_Prims(maze, breite, hoehe);
    }

    gettimeofday(&end, NULL);

    // Eingang und Ausgang setzen
    maze[1][0] = PATH;
    maze[hoehe - 2][breite - 1] = PATH;

    // Deadends und Anzahl der Wege zählen
    int deadends = CountDeadEnds(maze, breite, hoehe);

    SolveMaze(maze, breite, hoehe, 0, 1, breite - 1, hoehe - 2, &langeStraße);

    printf("Die Laenge des Loesungspfades: %d",langeStraße);

    // Labyrinth in der Konsole ausgeben
    printMaze(maze, breite, hoehe, wandSymbol, pfadSymbol);

    printf("Soll die Loesunng geziegt werden? Schreib 1 fuer Ja und 2 fuer Nein: " );
    scanf("%d", &loesung);

    if (loesung == 1)
    {PrintMazesolution(maze, breite, hoehe, wandSymbol, pfadSymbol);}

    // Labyrinth in einer Datei speichern
    SaveMazeToFile(maze, breite, hoehe, wandSymbol, pfadSymbol, "labyrinth.txt");

    // Zeit berechnen und ausgeben
    double zeit = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Labyrinthgenerierung dauerte: %.6f Sekunden\n", zeit);
    
    // Speicher freigeben
    for (int i = 0; i < hoehe; i++) free(maze[i]);
    free(maze);

    return 0;
}
