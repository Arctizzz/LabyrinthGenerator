#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct Node {
int row;
int column;
int visited;
struct Node* next;
};

void addNode(struct Node** head, int row, int column) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->row = row;
    newNode->column = column;
    newNode->visited = 0;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}


struct Node* findNode(struct Node* head, int row, int column) {
    struct Node* temp = head;
    while (temp != NULL) {
        if (temp->row == row && temp->column == column) {
            return temp;  
        }
        temp = temp->next;  
    }
    return NULL;  
}
void printList(struct Node* head) {
    struct Node* temp = head;
    printf("Current Linked List:\n");
    while (temp != NULL) {
        printf("Node at (%d, %d)%d->", temp->row, temp->column,temp->visited);
        temp = temp->next;
    }
    printf("End of List\n");
}

void addNeighbors(struct Node** head, int rows, int columns, int row, int column) {
    if (row > 0 && findNode(*head, row - 1, column) == NULL) {
        addNode(head, row - 1, column);
        findNode(*head, row - 1, column)->visited = 1;  
    }
    if (row < rows - 1 && findNode(*head, row + 1, column) == NULL) {
        addNode(head, row + 1, column);
        findNode(*head, row + 1, column)->visited = 1;  
    }
    if (column > 0 && findNode(*head, row, column - 1) == NULL) {
        addNode(head, row, column - 1);
        findNode(*head, row, column - 1)->visited = 1;  
    }
    if (column < columns - 1 && findNode(*head, row, column + 1) == NULL) {
        addNode(head, row, column + 1);
        findNode(*head, row, column + 1)->visited = 1;  
    }
}

struct Node* chooseRandom(struct Node* head, int currentRow, int currentColumn) {
    int count = 0;
    struct Node* temp = head;
    while (temp != NULL) {
        if (!temp->visited && 
            ((temp->row == currentRow - 1 && temp->column == currentColumn) ||
            (temp->row == currentRow + 1 && temp->column == currentColumn) ||
            (temp->row == currentRow && temp->column == currentColumn - 1) ||
            (temp->row == currentRow && temp->column == currentColumn+ 1))) {
            count++;
        }
        temp = temp->next;
    }
    if (count == 0)
        return NULL;

    int randomIndex = rand() % count;
    temp = head;
    while (temp != NULL) {
        if (!temp->visited && 
            ((temp->row == currentRow - 1 && temp->column == currentColumn) ||
            (temp->row == currentRow + 1 && temp->column == currentColumn) ||
            (temp->row == currentRow && temp->column == currentColumn - 1) ||
            (temp->row == currentRow && temp->column == currentColumn + 1))) {
            if (randomIndex == 0)
                return temp;
            randomIndex--;
        }
        temp = temp->next;
    }
    return NULL;
}

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
    getSize(&columns, &rows);  

    int Labyrinth[rows][columns];  
    struct Node* list = NULL;

    srand(time(NULL));  

    pregenerateLabyrinth(rows, columns, Labyrinth);  

    int startingrow = rand() % rows;       
    int startingcolumn = rand() % columns; 

    addNode(&list, startingrow, startingcolumn);  
    struct Node* current = list;
    current->visited = 1;

    while (current != NULL) {
        addNeighbors(&list, rows+1, columns+1, current->row, current->column);
        current = chooseRandom(list, current->row, current->column);


        if (current != NULL) {
            current->visited = 1;
            printf("Expanded node at (%d, %d)\n", current->row, current->column);
        }
    }

    saveArrayToTxt(rows, columns, Labyrinth, "Labyrinth.txt");
    printList(list);
    printf("Labyrinth saved in Labyrinth.txt\n");
    return 0;
}
