#include <stdio.h>

void fillVector (size_t n, int vector []);
void vectorProduct ();
void printVector ( size_t n, int vector []);
void fillMatrix (size_t n, double matrix[n][n]);
void matrixVectorProduct();
void printMatrix(size_t n, double matrix [n][n]);
void matrixInversion();
void augmentationMatrix(size_t n, double m[n][n], double aug_m[n][2*n]);
int normalizeAugMatrix(size_t n, double aug_m[n][2*n], size_t row);
void eliminateMatrix(size_t n, double aug_m[n][2*n]);
void printAugmentedMatrix(size_t n, double matrix[n][2*n]);

size_t getSize ();

int main (){
    
    printf("Select a function: \n"
        "vector-to-vector product    - 1\n"
        "matrix-to-vector product    - 2\n"
        "Gaussian elimination        - 3\n");

    size_t choice;
    scanf("%zu", &choice);

    switch (choice){
        case 1: vectorProduct();        break;
        case 2: matrixVectorProduct();  break;
        case 3: matrixInversion();      break;
        default:
            printf("Invalid choice\n");
    }

    return 0;
}

size_t getSize (){
    size_t n;
    printf("Enter size of vector/matrix: ");
    scanf("%zu", &n);
    return n;
}

void fillVector (size_t n, int* vector){
     
    printf("Vector filling - enter %zu elements:\n", n); 
    for(size_t i = 0; i < n; i++){
        printf("Index %zu: ", i);
        scanf("%d", &vector[i]);
    }
    printVector(n, vector);
}

void vectorProduct (){
    size_t n = getSize();

    int v1[n];  
    int v2[n]; 
    fillVector(n, v1);
    fillVector(n, v2);

    int product[n];
    for(size_t i = 0; i < n; i++){
        product[i] = v1[i] * v2[i];    
    }
    printVector(n, product);
}

void printVector(size_t n, int* v){
    printf("Vector: ");
    for(size_t i = 0; i < n; i++){
        printf("%d ", v[i]);
    }
    printf("\n");
}

void fillMatrix(size_t n, double matrix[n][n]){
    printf("Matrix filling:\n");
    for(size_t i = 0; i < n; i++){
        printf("row %zu\n", i+1);
        for (size_t j = 0; j < n; j++){
            scanf("%lf", &matrix[i][j]);
        }        
    }
    printMatrix(n, matrix);
}

void printMatrix(size_t n, double matrix[n][n]){
    printf("Matrix:\n");
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < n; j++){
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixVectorProduct (){
    size_t n = getSize();
    
    int vector[n]; 
    fillVector(n, vector);

    double matrix[n][n];
    fillMatrix(n, matrix);

    double result[n];

    for(size_t i = 0; i < n; i++){
        result[i] = 0;
        for(size_t j = 0; j < n; j++){
            result[i] += matrix[i][j] * vector[j];
        }
    }

    printf("Result vector:\n");
    for(size_t i = 0; i < n; i++){
        printf("%6.2f ", result[i]);
    }
    printf("\n");
}

void matrixInversion (){
    size_t n = getSize();

    double matrix[n][n];
    fillMatrix(n, matrix);

    double augment_matrix[n][2*n];
    augmentationMatrix(n, matrix, augment_matrix);

    eliminateMatrix(n, augment_matrix);
}

void augmentationMatrix (size_t n, double m [n][n], 
                            double aug_m [n][2* n]){
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < 2*n; j++){
            if(j < n){
                aug_m[i][j] = m[i][j];
            }
            else if (j - n == i){
                aug_m[i][j] = 1.0;
            } 
            else {
                aug_m[i][j] = 0.0;
            }
        }
    }
    printAugmentedMatrix(n, aug_m);
}

int normalizeAugMatrix(size_t n, double aug_m[n][2 * n], size_t row){
  
    double pivot = aug_m[row][row];

    if(pivot == 0){
        printf("Zero pivot encountered!\n");
        return 0;
    }

    for (size_t j = 0; j < 2 * n; j++) {
        aug_m[row][j] /= pivot;
    }

    printf("\nAfter normalization:\n");
    printAugmentedMatrix(n, aug_m);

    return 1;
}

void eliminateMatrix (size_t n, double aug_m [n][2 * n]) {

    for (size_t i = 0; i < n; i++) {

        if(!normalizeAugMatrix(n, aug_m, i)) return;

        for (size_t j = 0; j < n; j++) {

            if (j == i) continue;

            double factor = aug_m[j][i];

            for (size_t k = 0; k < 2 * n; k++) {
                aug_m[j][k] -= factor * aug_m[i][k];
            }
        }
    }
    printf("\nAfter elimination:\n");
    printAugmentedMatrix(n, aug_m);
}

void printAugmentedMatrix(size_t n, double matrix[n][2*n]){
    printf("Augmented Matrix:\n");

    for(size_t i = 0; i < n; i++){
        printf("| ");

        for(size_t j = 0; j < 2*n; j++){
            if(j == n) printf("| ");
            printf("%6.2f ", matrix[i][j]);
        }

        printf("|\n");
    }
}