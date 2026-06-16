#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Forward declaration (needed because used before definition) */
void printArray(size_t parent[], size_t n);

/*
 * ROOT marker:
 * SIZE_MAX indicates that an element has no parent (is a root).
 */
#define ROOT SIZE_MAX

/* =========================================================
   Initialization
   ========================================================= */

/*
 * init
 * ----
 * Initializes the parent array so that every element is its own root.
 */
void init(size_t parent[], size_t n){
    for (size_t i = 0; i < n; i++)
        parent[i] = ROOT;
}

/* =========================================================
   Find Root
   ========================================================= */

/*
 * Find
 * ----
 * Traverses parent pointers until a root is found.
 * Does not modify the structure.
 */
size_t Find(size_t parent[], size_t i){
    while (parent[i] != ROOT) {
        i = parent[i];
    }
    return i;
}

/* =========================================================
   Find & Replace (Path Rewrite)
   ========================================================= */

/*
 * FindReplace
 * -----------
 * Rewrites all nodes on the path from i to the root so they point
 * to 'value'. Stops before modifying the root itself.
 */
void FindReplace(size_t parent[], size_t i, size_t value){
    size_t tmp;

    while (parent[i] != ROOT) {
        tmp = parent[i];
        parent[i] = value;
        i = tmp;
    }
}

/* =========================================================
   Find & Compress (Path Compression)
   ========================================================= */

/*
 * FindCompress
 * ------------
 * Finds the root of i and compresses the path so all nodes
 * point directly to that root.
 */
void FindCompress(size_t parent[], size_t i){
    size_t root = Find(parent, i);
    FindReplace(parent, i, root);
}

/* =========================================================
   Union
   ========================================================= */

/*
 * Union
 * -----
 * Merges the sets containing a and b.
 * Links root of b to root of a, then updates paths.
 */
void Union(size_t parent[], size_t a, size_t b){
    size_t a_root = Find(parent, a);
    size_t b_root = Find(parent, b);

    /* If already in same set, do nothing */
    if (a_root == b_root)
        return;

    /* Attach b's root under a's root */
    parent[b_root] = a_root;

    /* Update paths */
    FindCompress(parent, b);
    FindReplace(parent, a, b_root);
}

/* =========================================================
   Output
   ========================================================= */

/*
 * printArray
 * ----------
 * Displays the current parent structure.
 */
void printArray(size_t parent[], size_t n){
    printf("\nParent array:\n");

    for (size_t i = 0; i < n; i++){
        if(parent[i] != ROOT)
            printf("parent[%zu] = %zu\n", i, parent[i]);
        else
            printf("parent[%zu] = ROOT\n", i);
    }
}

/* =========================================================
   Main (CLI Interface)
   ========================================================= */

int main(void){
    size_t n;

    printf("Type help to show all commands.\n\n");
    printf("Enter number of elements: ");
    scanf("%zu", &n);

    size_t parent[n];

    /* Initialize structure */
    init(parent, n);
    printArray(parent, n);

    char command[20];

    /* Command loop */
    while (1) {
        printf("\n> ");

        if (scanf("%19s", command) != 1)
            break;

        if(strcmp(command, "union") == 0) {
            size_t a, b;
            scanf("%zu %zu", &a, &b);
            Union(parent, a, b);
            printArray(parent, n);
        }
        else if (strcmp(command, "find") == 0) {
            size_t i;
            scanf("%zu", &i);
            printf("Root: %zu\n", Find(parent, i));
        }
        else if(strcmp(command, "replace") == 0){
            size_t i, value;
            scanf("%zu %zu", &i, &value);
            FindReplace(parent, i, value);
            printArray(parent, n);
        }
        else if (strcmp(command, "print") == 0){
            printArray(parent, n);
        }
        else if (strcmp(command, "quit") == 0){
            break;
        }
        else if (strcmp(command, "help") == 0){
            printf("\n=== Union-Find CLI Help ===\n\n");

            printf("This program manages a disjoint-set (Union-Find) structure.\n");
            printf("Each element initially belongs to its own set (ROOT).\n");
            printf("The 'parent' array stores the structure.\n\n");

            printf("Commands:\n\n");

            printf("  help\n");
            printf("    Show this help menu\n\n");

            printf("  union <a> <b>\n");
            printf("    Merges the sets containing elements a and b.\n");
            printf("    Internally:\n");
            printf("      - Finds root of a and b\n");
            printf("      - Attaches root of b under root of a\n");
            printf("      - Applies path compression updates\n\n");

            printf("  find <i>\n");
            printf("    Returns the root of element i.\n");
            printf("    Does NOT modify the structure.\n\n");

            printf("  replace <i> <value>\n");
            printf("    Rewrites the path from element i up to the root.\n");
            printf("    Each visited node will point to 'value'.\n");
            printf("    The root itself is NOT modified.\n\n");

            printf("  quit\n");
            printf("    Exit the program\n\n");

            printf("Notes:\n");
            printf("  - Valid indices: 0 to n-1\n");
            printf("  - ROOT means the element has no parent (set representative)\n");
            printf("  - After operations, the parent array is printed\n");
        }
        else {
            printf("Unknown command\n");
        }
    }
    return 0;
}