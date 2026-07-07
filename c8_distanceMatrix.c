#include <stdio.h>
#include <stddef.h>
#include <limits.h>

void initializeMatrix(size_t n, int matrix[n][n]);
void editMatrix(size_t n, int matrix[n][n]);
void fillMatrix(size_t n, int matrix[n][n]);
void printMatrix(size_t n, int matrix[n][n]);
void print_menu(void);
void shortestPath(size_t n, int matrix[n][n]);
int ShPthcheckVisitors(size_t n, int visitors[n]);

int main () {
    size_t n;
    printf("Enter amount of vertices: ");
    scanf("%zu", &n);

    int matrix[n][n];
    initializeMatrix(n, matrix);

    size_t running = 1;
    size_t choice;

    while (running) {

        print_menu();

        /* Input validation for menu */
        if (scanf("%zu", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        switch (choice) {

            case 1:
                editMatrix(n, matrix);
                break;

            case 2: {
                fillMatrix(n, matrix);
                break;
            }
            case 3: {
                shortestPath(n, matrix);
                break;
            }
            case 4: 
                return 0;
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
    printf("3: Shortest Path\n");
    printf("4: Exit\n");
    printf("Choice: ");
}

/* ---------------- INITIALIZE ---------------- */

void initializeMatrix(size_t n, int matrix[n][n]) {

    /* Set all entries to 0 */
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            
            if(i == j)
                matrix[i][j] = 0;
            else    
                matrix[i][j] = INT_MAX;
        }
    }

    printMatrix(n, matrix);
}


void editMatrix(size_t n, int matrix[n][n]) {
    char c1, c2;
    int  val;

    /* Read edge input */
    if (scanf(" %c %c %d", &c1, &c2, &val) != 3) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }

    int i1 = c1 - 'A';
    int i2 = c2 - 'A';

    /* Validate indices and value */
    if (val < 0 || i1 < 0 || i2 < 0 || i1 >= n || i2 >= n)
        return;

    if(val == 0) {
        matrix[i1][i2] = INT_MAX;
        matrix[i2][i1] = INT_MAX;
    } else {
        matrix[i1][i2] = val;
        matrix[i2][i1] = val; 
    }
    printMatrix(n, matrix);
}

void printMatrix(size_t n, int matrix[n][n]) {

    /* Column labels */
    printf("  ");
    for (size_t j = 0; j < n; j++) {
        printf("%c ", (char)('A' + j));
    }
    printf("\n");

    /* Rows */
    for (size_t i = 0; i < n; i++) {

        printf("%c ", (char)('A' + i));

        for (size_t j = 0; j < n; j++) {
            if (matrix[i][j] == INT_MAX)
                printf("INF ");
            else
                printf("%d ", matrix[i][j]);
        }

        printf("\n");
    }
}

void fillMatrix(size_t n, int matrix[n][n]) {

    printf("Paste %zux%zu distance matrix:\n", n, n);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {

            int value;

            if (scanf("%d", &value) != 1) {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                return;
            }

            if (value < 0) {
                printf("Negative distances are not allowed.\n");
                return;
            }

            if (i != j && value == 0)
                matrix[i][j] = INT_MAX;
            else
                matrix[i][j] = value;
        }
    }

    printf("Matrix loaded:\n");
    printMatrix(n, matrix);
}

void shortestPath(size_t n, int matrix [n][n]){
    char from, to;
    
    printf("Start vertex: ");
    scanf (" %c", &from);

    printf("Target vertex: ");
    scanf (" %c", &to);

    int start = from - 'A';
    int end = to - 'A';

   if (start < 0 || end < 0 ||
        (size_t)start >= n || (size_t)end >= n) {
        
        printf("Invalid vertex.\n");
        return;
    }

    int distance[n];
    int visited[n];
    int previous[n];

    for(int i = 0; i < n; i++){
        distance[i] = INT_MAX;
        visited[i] = 0;
        previous[i] = -1;
    }
    
    distance[start] = 0;

    int current = start;

    while (!ShPthcheckVisitors(n, visited)){

        visited[current] = 1;

        if (current == end)
            break;
        
        
        
        for(int j = 0; j < n; j++){

            if(!visited[j] && 
                matrix[current][j] != INT_MAX &&
                distance[current] != INT_MAX){

                int new_distance = 
                    distance[current] + matrix[current][j];

                if (new_distance < distance[j]){
                    distance[j] = new_distance;
                    previous[j] = current;
                }
            }
        }
        int smallest = INT_MAX;
        int next = -1;

        for(size_t i = 0 ; i < n; i++){
            if(!visited[i] && distance[i] < smallest){
                smallest = distance[i];
                next = i; 
            }
        }

        if(next == -1)
            break;

        current = next;
    }
    if (distance[end] == INT_MAX) {
        printf("No path exists.\n");
        return;
    }
    printf("Shortest distance: %d\n", distance[end]);

    int path[n];
    int count = 0;

    current = end;

    while (current != -1){
        path[count++] = current;
        current = previous[current];
    }

    printf("Path: ");
    
    for(int i = count -1; i >= 0; i--){
        printf("%c", path[i] + 'A');

        if(i > 0)
            printf(" -> ");
    }
    printf("\n");
}

int ShPthcheckVisitors (size_t n,int visitors[n]){
    for (int i = 0; i < n; i++){
        if(visitors[i] == 0)
            return 0;
    }
    return 1;
}