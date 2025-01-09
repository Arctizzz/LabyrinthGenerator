#include <stdio.h>
#include <stdlib.h>
#include <time.h> //random genrator
#include <windows.h>

enum Weg {
    
    Links,
    Rechts,
    Oben,
    Unten

};

int main(){

int maze[10][20];
int iZahl;

srand(time(NULL));

   for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {

            if (i == 0 || i == 9 || j == 0 || j == 19)
            {
                iZahl = 1;
                maze[i][j] = iZahl;

            }else{

            iZahl = rand() % 2; // 0 oder 1 für Wege und Wände
            maze[i][j] = iZahl;

            }

        }
    }

    // Ausgabe des Labyrinths
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {

            if (maze[i][j] == 0) {
                printf("."); //Weg
            } else {
                printf("#"); //Wand
            }                   
            
            Sleep(50); // 50milisec delay

        }
        printf("\n"); 
    }

    return 0;
}