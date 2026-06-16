#include <stdio.h>
#include <stddef.h>

typedef struct {
    int visited_all;
    int edge_count;
    int has_cycle;
} TreeCheck;

void initializeMatrix(size_t n, int matrix[n][n]);
void editMatrix(size_t n, int matrix[n][n]);
void fillMatrix(size_t n, int matrix[n][n]);
void printMatrix(size_t n, int matrix[n][n]);
void print_menu(void);
void breadthSearch(size_t n, int matrix[n][n],
                    int visited[n], size_t start);
int allComponents(size_t n, int matrix[n][n]);

int main() {

    size_t n;
    printf("Enter amount of vertices: ");
    scanf("%zu", &n);

    int matrix[n][n];
    initializeMatrix(n, matrix);

    size_t running = 1;
    size_t choice;

    while (running) {

        print_menu();

        if (scanf("%zu", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        switch (choice) {

            case 1:
                editMatrix(n, matrix);
                break;

            case 2:
                fillMatrix(n, matrix);
                break;

            case 3: {
                char c;
                printf("Enter start vertex (A-%c): ", (char)('A' + n - 1));
                scanf(" %c", &c);

                size_t start = c - 'A';

                if (start >= n) {
                    printf("Invalid vertex\n");
                    break;
                }

                int visited[n];
                for (size_t i = 0; i < n; i++){
                    visited[i] = 0;
                }

                breadthSearch(n, matrix, visited, start);
                printf("\n");

                break;
            }
            case 4:
                allComponents(n, matrix);
                break;
            case 5:
                running = 0;
                break;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}

/* ---------------- MENU ---------------- */

void print_menu(void) {
    printf("\nMenu:\n");
    printf("1: Edit Matrix\n");
    printf("2: Fill Matrix\n");
    printf("3: Breadth-First Search\n");
    printf("4: Connected Components\n");
    printf("5: Exit\n");
    printf("Choice: ");
}

/* ---------------- EDIT SINGLE EDGE ---------------- */

void editMatrix(size_t n, int matrix[n][n]) {
    char c1, c2;
    size_t val;

    if (scanf(" %c %c %zu", &c1, &c2, &val) != 3) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }

    int i1 = c1 - 'A';
    int i2 = c2 - 'A';

    if (val > 1 || i1 < 0 || i2 < 0 || i1 >= n || i2 >= n)
        return;

    matrix[i1][i2] = val;
    printMatrix(n, matrix);
}

/* ---------------- INITIALIZE ---------------- */

void initializeMatrix(size_t n, int matrix[n][n]) {

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }

    printMatrix(n, matrix);
}

/* ---------------- BULK INPUT ---------------- */

void fillMatrix(size_t n, int matrix[n][n]) {

    printf("Paste %zux%zu matrix (0/1 values):\n", n, n);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {

            if (scanf("%d", &matrix[i][j]) != 1) {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                return;
            }

            if (matrix[i][j] != 0 && matrix[i][j] != 1) {
                printf("Invalid value at (%c,%c)\n",
                       (char)('A' + i),
                       (char)('A' + j));
                return;
            }
        }
    }

    printf("Matrix loaded:\n");
    printMatrix(n, matrix);
}

/* ---------------- PRINT MATRIX ---------------- */

void printMatrix(size_t n, int matrix[n][n]) {

    printf("  ");

    for (size_t j = 0; j < n; j++) {
        printf("%c ", (char)('A' + j));
    }

    printf("\n");

    for (size_t i = 0; i < n; i++) {

        printf("%c ", (char)('A' + i));

        for (size_t j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }

        printf("\n");
    }
}

/* ---------------- BFS PLACEHOLDER ---------------- */

void breadthSearch(size_t n, int matrix[n][n],
                    int visited[n], size_t start) {
    
    if(n == 0){
        printf("Graph is empty.\n");
        return;
    }

    int queue [n];
    size_t front = 0; 
    size_t rear = 0;

    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear){
        int i = queue[front++];
        printf("%c", (char)('A'+i));
        
        for(size_t j = 0; j < n; j++){
            if ((matrix[i][j] == 1 || matrix[j][i] == 1) && !visited[j]){
                visited[j] = 1;
                queue[rear++] = j;
            }
        }
    }
}

int allComponents (size_t n, int matrix [n][n]){
    
    int visited[n];
    for (size_t i = 0; i < n; i++) {
        visited[i] = 0;
    }

    size_t count = 0;
    for (size_t i = 0; i < n; i++) {
        if (!visited[i]) {
            printf("Component: ");
            breadthSearch(n, matrix, visited, i);
            printf("\n");
            count++;
        }
    }
    return count;
}

TreeCheck bfsTreeCheck(size_t n, int matrix[n][n], size_t start){
    
    TreeCheck result = {0, 0, 0};

    int visited[n];
    int parent[n];

    for (size_t i = 0; i < n; i++){
        visited[i] = 0;
        parent[i] = -1;
    }

    int queue[n];
    size_t front = 0, rear = 0;

    queue[rear++] = start;
    visited[start] = 1;

    while(front < rear){
        int i = queue[front++];

        for(size_t j = 0; j < n; j++){

            if(matrix[i][j] == 1 || matrix[j][i] == 1){

                result.edge_count++;

                if(!visited[j]) {
                    visited[j] = 1;
                    parent[j] = i;
                    queue[rear++] = j;
                } else 
                if (parent[i] != j){
                    result.has_cycle = 1;
                }
            }
        }
    }

    result.visited_all = 1;

    for(size_t i = 0; i < n; i++){
        if(!visited[i]){
            result.visited_all = 0;
            break;
        }
    }

    result.edge_count /= 2;
    return;
}

