#include <stdio.h>
#include <math.h>
#include <complex.h>

double *currentCoefficients;
int currentDegree;

/* Real generic derivative */
double derivative(double (*F)(double), double x);

/* Complex generic derivative */
double complex derivate(
    double complex (*F)(double complex),
    double complex z
);

double Newton(
    double (*F)(double),
    double (*dF)(double),
    double x
);

double complex NewtonComplex(
    double complex (*F)(double complex),
    double complex (*dF)(double complex),
    double complex z
);

double polynomial(double x);
double polynomialDerivative(double x);

double complex complexPolynomial(double complex z);
double complex complexPolynomialDerivative(double complex z);

void findRealRoots(
    double (*F)(double),
    double (*dF)(double),
    double min,
    double max,
    double step
);

void findComplexRoots(
    double complex (*F)(double complex),
    double complex (*dF)(double complex),
    double min,
    double max,
    double step
);


/* Evaluate a real polynomial using Horner's method */
double polynomial(double x) {
    double result = 0.0;

    for (int i = currentDegree; i >= 0; i--) {
        result = result * x + currentCoefficients[i];
    }

    return result;
}


/* Evaluate the derivative of a real polynomial */
double polynomialDerivative(double x) {
    double result = 0.0;

    for (int i = currentDegree; i >= 1; i--) {
        result = result * x + i * currentCoefficients[i];
    }

    return result;
}


/* Evaluate a polynomial at a complex point */
double complex complexPolynomial(double complex z) {
    double complex result = 0.0;

    for (int i = currentDegree; i >= 0; i--) {
        result = result * z + currentCoefficients[i];
    }

    return result;
}


/* Evaluate the derivative at a complex point */
double complex complexPolynomialDerivative(double complex z) {
    double complex result = 0.0;

    for (int i = currentDegree; i >= 1; i--) {
        result = result * z + i * currentCoefficients[i];
    }

    return result;
}


int main(void) {
    printf("Polynomial Derivative and Root Finder\n");
    printf("=====================================\n");
    printf("This program analyses a polynomial with real coefficients.\n");
    printf("It evaluates the polynomial and its derivative at real or complex points.\n");
    printf("It then searches numerically for real and complex roots.\n");
    printf("The search may not find every root for every polynomial.\n");
    printf("\n");
    printf("Enter the polynomial's degree, followed by its coefficients\n");
    printf("in descending order of powers, starting with the highest.\n");
    printf("For missing powers, enter 0 as the coefficient.\n");
    printf("\n");

    int degree;

    printf("Enter polynomial degree: ");

    if (scanf("%d", &degree) != 1 || degree < 0 || degree > 1000) {
        printf("Invalid degree. Enter a value between 0 and 1000.\n");
        return 1;
    }

    double coefficients[degree + 1];

    for (int i = degree; i >= 0; i--) {
        printf("Coefficient for x^%d: ", i);

        if (scanf("%lf", &coefficients[i]) != 1) {
            printf("Invalid coefficient.\n");
            return 1;
        }
    }

    currentCoefficients = coefficients;
    currentDegree = degree;

    int choice;

    printf("\nChoose an evaluation type:\n");
    printf("1. Real number\n");
    printf("2. Complex number\n");
    printf("Your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    if (choice == 1) {
        double x;

        printf("Enter a real evaluation point x: ");

        if (scanf("%lf", &x) != 1) {
            printf("Invalid evaluation point.\n");
            return 1;
        }

        printf("P(%.6f) = %.6f\n", x, polynomial(x));
        printf("P'(%.6f) = %.6f\n", x, polynomialDerivative(x));

    } else if (choice == 2) {
        double realPart;
        double imaginaryPart;

        printf("Enter the real part of the complex evaluation point: ");

        if (scanf("%lf", &realPart) != 1) {
            printf("Invalid real part.\n");
            return 1;
        }

        printf("Enter the imaginary part of the complex evaluation point: ");

        if (scanf("%lf", &imaginaryPart) != 1) {
            printf("Invalid imaginary part.\n");
            return 1;
        }

        double complex z = realPart + imaginaryPart * I;

        double complex pz = complexPolynomial(z);
        double complex dpz = complexPolynomialDerivative(z);

        printf("P(%.6f %+.6fi) = %.6f %+.6fi\n",
               creal(z), cimag(z),
               creal(pz), cimag(pz));

        printf("P'(%.6f %+.6fi) = %.6f %+.6fi\n",
               creal(z), cimag(z),
               creal(dpz), cimag(dpz));

    } else {
        printf("Invalid choice. Enter 1 or 2.\n");
        return 1;
    }

    printf("\nSearching for real roots...\n");

    findRealRoots(
        polynomial,
        polynomialDerivative,
        -10.0,
        10.0,
        0.5
    );

    printf("\nSearching for complex roots...\n");

    findComplexRoots(
        complexPolynomial,
        complexPolynomialDerivative,
        -5.0,
        5.0,
        0.5
    );

    return 0;
}


/* Numerical derivative of a real function */
double derivative(double (*F)(double), double x) {
    double h = 1e-6;

    return (F(x + h) - F(x)) / h;
}


/* Numerical derivative of a complex function */
double complex derivate(
    double complex (*F)(double complex),
    double complex z
) {
    double complex h = 1e-6;

    return (F(z + h) - F(z - h)) / (2.0 * h);
}


/* Newton's method for real roots */
double Newton(
    double (*F)(double),
    double (*dF)(double),
    double x
) {
    for (int i = 0; i < 100; i++) {
        double fx = F(x);
        double dfx = dF(x);

        if (fabs(dfx) < 1e-12) {
            return x;
        }

        double next = x - fx / dfx;

        if (fabs(next - x) < 1e-10) {
            return next;
        }

        x = next;
    }

    return x;
}


/* Newton's method for complex roots */
double complex NewtonComplex(
    double complex (*F)(double complex),
    double complex (*dF)(double complex),
    double complex z
) {
    for (int i = 0; i < 100; i++) {
        double complex fz = F(z);
        double complex dfz = dF(z);

        if (cabs(dfz) < 1e-12) {
            return z;
        }

        double complex next = z - fz / dfz;

        if (cabs(next - z) < 1e-10) {
            return next;
        }

        z = next;
    }

    return z;
}


/* Search for real roots using multiple starting points */
void findRealRoots(
    double (*F)(double),
    double (*dF)(double),
    double min,
    double max,
    double step
) {
    double roots[100];
    int rootCount = 0;

    for (double start = min; start <= max; start += step) {
        double root = Newton(F, dF, start);

        if (fabs(F(root)) < 1e-6) {
            int alreadyFound = 0;

            for (int i = 0; i < rootCount; i++) {
                if (fabs(root - roots[i]) < 1e-6) {
                    alreadyFound = 1;
                    break;
                }
            }

            if (!alreadyFound && rootCount < 100) {
                roots[rootCount] = root;
                rootCount++;

                printf("Found root: %.10f\n", root);
            }
        }
    }

    if (rootCount == 0) {
        printf("No real roots found.\n");
    }
}


/* Search for complex roots using multiple starting points */
void findComplexRoots(
    double complex (*F)(double complex),
    double complex (*dF)(double complex),
    double min,
    double max,
    double step
) {
    double complex roots[100];
    int rootCount = 0;

    for (double real = min; real <= max; real += step) {
        for (double imag = min; imag <= max; imag += step) {
            double complex start = real + imag * I;

            double complex root = NewtonComplex(F, dF, start);

            if (cabs(F(root)) < 1e-6) {
                int alreadyFound = 0;

                for (int i = 0; i < rootCount; i++) {
                    if (cabs(root - roots[i]) < 1e-6) {
                        alreadyFound = 1;
                        break;
                    }
                }

                if (!alreadyFound && rootCount < 100) {
                    roots[rootCount] = root;
                    rootCount++;

                    printf(
                        "Found complex root: %.10f %+.10fi\n",
                        creal(root),
                        cimag(root)
                    );
                }
            }
        }
    }

    if (rootCount == 0) {
        printf("No complex roots found.\n");
    }
}