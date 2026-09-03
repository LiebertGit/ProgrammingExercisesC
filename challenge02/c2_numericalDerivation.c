#include <stdio.h>
#include <math.h>

// original functions
double Fsin(double x);
double Fsquare(double x);
double Froot(double x);

int main() {
    double x;

    printf("Enter a value for x: ");
    scanf("%lf", &x);

    printf("This program approximates derivatives using h = 1e-6\n\n");

    double h = 1e-6;

    double result_sin = (Fsin(x + h) - Fsin(x)) / h;
    double result_square = (Fsquare(x + h) - Fsquare(x)) / h;
    double result_root = (Froot(x + h) - Froot(x)) / h;

    // print results
    printf("At x = %f:\n\n", x);

    printf("sin(x) derivative approx: %f (exact: %f)\n", result_sin, cos(x));
    printf("x^2 derivative approx:   %f (exact: %f)\n", result_square, 2*x);
    printf("sqrt(x) derivative approx: %f (exact: %f)\n", result_root, 1.0/(2.0*sqrt(x)));

    return 0;
}

double Fsin(double x){
    return sin(x);
}

double Fsquare(double x){
    return x * x;
}

double Froot(double x){
    return sqrt(x);
}