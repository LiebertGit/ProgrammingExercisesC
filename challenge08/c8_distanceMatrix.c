#include <stdio.h>
#include <stddef.h>
#include <limits.h>

/*
 * Function declarations
 *
 * The graph is represented as a distance matrix.
 * INT_MAX is used when there is no direct connection
 * between two vertices.
 */
void initializeMatrix(size_t n, int matrix[n][n]);
void editMatrix(size_t n, int matrix[n][n]);
void fillMatrix(size_t n, int matrix[n][n]);
void printMatrix(size_t n, int matrix[n][n]);
void print_menu(void);
void shortestPath(size_t n, int matrix[n][n]);
int ShPthcheckVisitors(size_t n, int visitors[n]);


int main(void) {

    size_t n;

    printf("Enter amount of vertices: ");
    scanf("%zu", &n);

    /* Create the graph matrix and set its initial values */
    int matrix[n][n];
    initializeMatrix(n, matrix);

    size_t running = 1;
    size_t choice;

    /* Main menu loop */
    while (running) {

        print_menu();

        /* Check whether the menu input is valid */
        if (scanf("%zu", &choice) != 1) {

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        switch (choice) {

            case 1:
                /* Change a single connection */
                editMatrix(n, matrix);
                break;

            case 2:
                /* Enter the complete matrix */
                fillMatrix(n, matrix);
                break;

            case 3:
                /* Calculate the shortest path */
                shortestPath(n, matrix);
                break;

            case 4:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}


/* Display the available menu options */
void print_menu(void) {

    printf("\nMenu:\n");
    printf("1: Edit Matrix\n");
    printf("2: Fill Matrix\n");
    printf("3: Shortest Path\n");
    printf("4: Exit\n");
    printf("Choice: ");
}


/*
 * Initialize the distance matrix.
 *
 * The diagonal is 0 because the distance from a
 * vertex to itself is always 0.
 *
 * All other entries start as INT_MAX, meaning
 * that no connection exists yet.
 */
void initializeMatrix(size_t n, int matrix[n][n]) {

    for (size_t i = 0; i < n; i++) {

        for (size_t j = 0; j < n; j++) {

            if (i == j)
                matrix[i][j] = 0;
            else
                matrix[i][j] = INT_MAX;
        }
    }

    printMatrix(n, matrix);
}


/*
 * Edit the distance between two vertices.
 *
 * Example:
 * A B 5
 *
 * creates an edge from A to B with distance 5.
 */
void editMatrix(size_t n, int matrix[n][n]) {

    char c1, c2;
    int val;

    if (scanf(" %c %c %d", &c1, &c2, &val) != 3) {

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        return;
    }

    /* Convert vertex names (A, B, C...) into indices (0, 1, 2...) */
    int i1 = c1 - 'A';
    int i2 = c2 - 'A';

    /*
     * Check the signed values before converting them to size_t.
     * This prevents problems caused by size_t being unsigned.
     */
    if (val < 0 || i1 < 0 || i2 < 0)
        return;

    if ((size_t)i1 >= n || (size_t)i2 >= n)
        return;

    /*
     * A distance of 0 means that there is no edge.
     * Otherwise, create the edge with the given distance.
     *
     * Both directions are changed because the graph
     * is undirected.
     */
    if (val == 0) {

        matrix[i1][i2] = INT_MAX;
        matrix[i2][i1] = INT_MAX;

    } else {

        matrix[i1][i2] = val;
        matrix[i2][i1] = val;
    }

    printMatrix(n, matrix);
}


/*
 * Print the current distance matrix.
 *
 * INT_MAX is displayed as "INF" so that the matrix
 * is easier for the user to read.
 */
void printMatrix(size_t n, int matrix[n][n]) {

    printf("  ");

    /* Print column labels */
    for (size_t j = 0; j < n; j++) {
        printf("%c ", (char)('A' + j));
    }

    printf("\n");

    /* Print matrix rows */
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


/*
 * Fill the entire distance matrix from user input.
 *
 * A positive value represents an edge.
 * A 0 outside the diagonal represents no edge.
 */
void fillMatrix(size_t n, int matrix[n][n]) {

    printf("Paste %zux%zu distance matrix:\n", n, n);

    int valid = 1;

    /*
     * Read the complete matrix before returning.
     *
     * This is important when the user pastes an entire
     * matrix. We don't want leftover values to become
     * menu input if an invalid value is found.
     */
    for (size_t i = 0; i < n; i++) {

        for (size_t j = 0; j < n; j++) {

            int value;

            if (scanf("%d", &value) != 1) {

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                printf("Invalid matrix input.\n");
                return;
            }

            /*
             * Remember that the matrix is invalid,
             * but continue reading the remaining values.
             */
            if (value < 0)
                valid = 0;

            matrix[i][j] = value;
        }
    }

    /*
     * All matrix values have now been consumed.
     * We can safely return to the menu.
     */
    if (!valid) {

        printf("Negative distances are not allowed.\n");
        return;
    }

    /*
     * Convert zero values outside the diagonal
     * into INT_MAX, which represents no connection.
     */
    for (size_t i = 0; i < n; i++) {

        for (size_t j = 0; j < n; j++) {

            if (i != j && matrix[i][j] == 0)
                matrix[i][j] = INT_MAX;
        }
    }

    printf("Matrix loaded:\n");
    printMatrix(n, matrix);
}


/*
 * Find the shortest path between two vertices.
 *
 * This uses Dijkstra's algorithm.
 *
 * distance[]  -> shortest known distance to each vertex
 * visited[]   -> whether a vertex has already been processed
 * previous[]  -> previous vertex on the shortest path
 */
void shortestPath(size_t n, int matrix[n][n]) {

    char from, to;

    printf("Start vertex: ");
    scanf(" %c", &from);

    printf("Target vertex: ");
    scanf(" %c", &to);

    /* Convert A/B/C... into 0/1/2... */
    int start = from - 'A';
    int end = to - 'A';

    /* Make sure both vertices exist */
    if (start < 0 || end < 0 ||
        (size_t)start >= n || (size_t)end >= n) {

        printf("Invalid vertex.\n");
        return;
    }

    int distance[n];
    int visited[n];
    int previous[n];

    /*
     * Initially, we do not know any distances.
     *
     * previous[] is initialized to -1 because
     * no previous vertex exists yet.
     */
    for (size_t i = 0; i < n; i++) {

        distance[i] = INT_MAX;
        visited[i] = 0;
        previous[i] = -1;
    }

    /* The distance from the start vertex to itself is 0 */
    distance[start] = 0;

    int current = start;

    /*
     * Continue until all vertices have been visited
     * or no reachable vertex remains.
     */
    while (!ShPthcheckVisitors(n, visited)) {

        /* The current vertex is now processed */
        visited[current] = 1;

        /* We can stop as soon as we reach the target */
        if (current == end)
            break;

        /*
         * Check every possible neighbor of current.
         *
         * If going through current gives a shorter
         * distance, update the distance and remember
         * how we reached that vertex.
         */
        for (size_t j = 0; j < n; j++) {

            if (!visited[j] &&
                matrix[current][j] != INT_MAX &&
                distance[current] != INT_MAX) {

                int new_distance =
                    distance[current] + matrix[current][j];

                if (new_distance < distance[j]) {

                    distance[j] = new_distance;
                    previous[j] = current;
                }
            }
        }

        /*
         * Find the unvisited vertex with the smallest
         * known distance.
         *
         * This vertex becomes the next current vertex.
         */
        int smallest = INT_MAX;
        int next = -1;

        for (size_t i = 0; i < n; i++) {

            if (!visited[i] && distance[i] < smallest) {

                smallest = distance[i];
                next = (int)i;
            }
        }

        /*
         * No unvisited vertex can be reached.
         */
        if (next == -1)
            break;

        current = next;
    }

    /*
     * If the target still has distance INT_MAX,
     * there is no path from start to target.
     */
    if (distance[end] == INT_MAX) {

        printf("No path exists.\n");
        return;
    }

    printf("Shortest distance: %d\n", distance[end]);

    /*
     * Reconstruct the path using previous[].
     *
     * Start at the target and follow the previous
     * vertices backwards until we reach the start.
     */
    int path[n];
    int count = 0;

    current = end;

    while (current != -1) {

        path[count++] = current;
        current = previous[current];
    }

    /*
     * The path was stored backwards, so print it
     * from the last element to the first.
     */
    printf("Path: ");

    for (int i = count - 1; i >= 0; i--) {

        printf("%c", path[i] + 'A');

        if (i > 0)
            printf(" -> ");
    }

    printf("\n");
}


/*
 * Check whether every vertex has been visited.
 *
 * Returns 0 if an unvisited vertex exists,
 * otherwise returns 1.
 */
int ShPthcheckVisitors(size_t n, int visitors[n]) {

    for (size_t i = 0; i < n; i++) {

        if (visitors[i] == 0)
            return 0;
    }

    return 1;
}