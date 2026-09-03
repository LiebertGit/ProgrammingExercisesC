#include <stdio.h>
#include <stddef.h>

/* Struct to store tree check results */
typedef struct {
    int visited_all;       // 1 if graph is connected
    size_t edge_count;     // number of edges (undirected)
    int has_cycle;         // 1 if cycle detected
} TreeCheck;

/* Function declarations */
void initializeMatrix(size_t n, int matrix[n][n]);
void editMatrix(size_t n, int matrix[n][n]);
void fillMatrix(size_t n, int matrix[n][n]);
void printMatrix(size_t n, int matrix[n][n]);
void print_menu(void);
void breadthSearch(size_t n, int matrix[n][n],
                   int visited[n], size_t start);
int allComponents(size_t n, int matrix[n][n]);
TreeCheck bfsTreeCheck(size_t n, int matrix[n][n], size_t start);

int main(void) {

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

            case 2:
                fillMatrix(n, matrix);
                break;

            case 3: {
                /* BFS starting from chosen vertex */
                char c;

                printf("Enter start vertex (A-%c): ",
                       (char)('A' + n - 1));

                scanf(" %c", &c);

                /*
                 * Convert the character to an integer first.
                 * This allows us to detect values below 'A'.
                 */
                int start_index = c - 'A';

                if (start_index < 0 || (size_t)start_index >= n) {
                    printf("Invalid vertex\n");
                    break;
                }

                size_t start = (size_t)start_index;

                int visited[n];

                for (size_t i = 0; i < n; i++) {
                    visited[i] = 0;
                }

                breadthSearch(n, matrix, visited, start);
                printf("\n");

                break;
            }

            case 4:
                /* Find connected components */
                allComponents(n, matrix);
                break;

            case 5:
                running = 0;
                break;

            case 6: {
                /* Check if graph is a tree */
                TreeCheck res = bfsTreeCheck(n, matrix, 0);

                if (res.visited_all &&
                    !res.has_cycle &&
                    res.edge_count == n - 1) {

                    printf("Graph is a tree\n");
                }
                else {
                    printf("Graph is NOT a tree\n");
                }

                break;
            }

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
    printf("6: Check Tree\n");
    printf("Choice: ");
}

/* ---------------- EDIT SINGLE EDGE ---------------- */

void editMatrix(size_t n, int matrix[n][n]) {

    char c1, c2;
    size_t val;

    /* Read edge input */
    if (scanf(" %c %c %zu", &c1, &c2, &val) != 3) {

        int ch;

        while ((ch = getchar()) != '\n' && ch != EOF);

        return;
    }

    /*
     * Use int temporarily so that negative values can
     * be detected before converting to size_t.
     */
    int i1 = c1 - 'A';
    int i2 = c2 - 'A';

    /* Validate value */
    if (val > 1)
        return;

    /* Validate that the vertices are not below 'A' */
    if (i1 < 0 || i2 < 0)
        return;

    /* Convert only after checking for negative values */
    size_t row = (size_t)i1;
    size_t column = (size_t)i2;

    /* Validate that the vertices are inside the matrix */
    if (row >= n || column >= n)
        return;

    matrix[row][column] = (int)val;

    printMatrix(n, matrix);
}

/* ---------------- INITIALIZE ---------------- */

void initializeMatrix(size_t n, int matrix[n][n]) {

    /* Set all entries to 0 */
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

            int value;

            /*
             * Read the value first.
             */
            if (scanf("%d", &value) != 1) {

                int ch;

                while ((ch = getchar()) != '\n' && ch != EOF);

                printf("Invalid input\n");

                return;
            }

            /*
             * Only allow 0 or 1.
             */
            if (value != 0 && value != 1) {

                printf("Invalid value at (%c,%c)\n",
                       (char)('A' + i),
                       (char)('A' + j));

                return;
            }

            matrix[i][j] = value;
        }
    }

    printf("Matrix loaded:\n");
    printMatrix(n, matrix);
}

/* ---------------- PRINT MATRIX ---------------- */

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

            printf("%d ", matrix[i][j]);
        }

        printf("\n");
    }
}

/* ---------------- BFS ---------------- */

void breadthSearch(size_t n, int matrix[n][n],
                   int visited[n], size_t start) {

    if (n == 0) {

        printf("Graph is empty.\n");

        return;
    }

    /*
     * Queue contains vertex indices,
     * therefore size_t is the natural type.
     */
    size_t queue[n];

    size_t front = 0;
    size_t rear = 0;

    /* Initialize BFS */
    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear) {

        size_t i = queue[front++];

        printf("%c", (char)('A' + i));

        /* Explore neighbors (undirected) */
        for (size_t j = 0; j < n; j++) {

            if ((matrix[i][j] == 1 ||
                 matrix[j][i] == 1) &&
                !visited[j]) {

                visited[j] = 1;

                queue[rear++] = j;
            }
        }
    }
}

/* ---------------- CONNECTED COMPONENTS ---------------- */

int allComponents(size_t n, int matrix[n][n]) {

    int visited[n];

    for (size_t i = 0; i < n; i++) {

        visited[i] = 0;
    }

    size_t count = 0;

    /* Run BFS from each unvisited node */
    for (size_t i = 0; i < n; i++) {

        if (!visited[i]) {

            printf("Component: ");

            breadthSearch(n, matrix, visited, i);

            printf("\n");

            count++;
        }
    }

    return (int)count;
}

/* ---------------- TREE CHECK ---------------- */

TreeCheck bfsTreeCheck(size_t n,
                       int matrix[n][n],
                       size_t start) {

    TreeCheck result = {
        .visited_all = 0,
        .edge_count = 0,
        .has_cycle = 0
    };

    int visited[n];

    /*
     * Parent contains vertex indices, so size_t
     * is the appropriate type.
     *
     * The root is its own parent.
     */
    size_t parent[n];

    /* Initialize arrays */
    for (size_t i = 0; i < n; i++) {

        visited[i] = 0;
        parent[i] = i;
    }

    /*
     * Queue contains vertex indices,
     * so use size_t here as well.
     */
    size_t queue[n];

    size_t front = 0;
    size_t rear = 0;

    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear) {

        size_t i = queue[front++];

        for (size_t j = 0; j < n; j++) {

            /* Undirected edge check */
            if (matrix[i][j] == 1 ||
                matrix[j][i] == 1) {

                result.edge_count++;

                if (!visited[j]) {

                    visited[j] = 1;

                    parent[j] = i;

                    queue[rear++] = j;
                }

                /*
                 * If the neighbor was already visited
                 * and is not the current vertex's parent,
                 * we found a cycle.
                 */
                else if (parent[i] != j) {

                    result.has_cycle = 1;
                }
            }
        }
    }

    /* Check connectivity */
    result.visited_all = 1;

    for (size_t i = 0; i < n; i++) {

        if (!visited[i]) {

            result.visited_all = 0;

            break;
        }
    }

    /*
     * Because the graph is undirected, every edge
     * was encountered twice.
     */
    result.edge_count /= 2;

    return result;
}