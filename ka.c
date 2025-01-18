#include <stdio.h>
#define N 4

// Function prototypes
int solveMaze(int maze[N][N], int x, int y, int solution[N][N], int visited[N][N]);
void printSolution(int solution[N][N]);

// Directions for moving in the maze (right, down, left, up)
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

// Utility function to check if a move is valid
int isValidMove(int maze[N][N], int visited[N][N], int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1 && !visited[x][y];
}

// Backtracking function to solve the maze
int solveMaze(int maze[N][N], int x, int y, int solution[N][N], int visited[N][N]) {
    // Base case: if destination (bottom-right corner) is reached
    if (x == N - 1 && y == N - 1) {
        solution[x][y] = 1;
        return 1;
    }

    // Mark current cell as part of the solution and visited
    solution[x][y] = 1;
    visited[x][y] = 1;

    // Try all possible moves
    for (int i = 0; i < 4; i++) {
        int nextX = x + dx[i];
        int nextY = y + dy[i];

        if (isValidMove(maze, visited, nextX, nextY)) {
            if (solveMaze(maze, nextX, nextY, solution, visited)) {
                return 1; // Path found
            }
        }
    }

    // Backtrack: Unmark current cell
    solution[x][y] = 0;
    visited[x][y] = 0;
    return 0;
}

// Function to print the solution matrix
void printSolution(int solution[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", solution[i][j]);
        }
        printf("\n");
    }
}

// Main function
int main() {
    int maze[N][N] = {
        {1, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {1, 1, 1, 1}
    };

    int solution[N][N] = {0};   // Solution matrix
    int visited[N][N] = {0};    // Visited matrix

    if (solveMaze(maze, 0, 0, solution, visited)) {
        printf("Solution:\n");
        printSolution(solution);
    } else {
        printf("No solution exists.\n");
    }

    return 0;
}
