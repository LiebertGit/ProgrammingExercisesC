#include <stdio.h>
#include <math.h>
#include <complex.h>

double *currentCoefficients;
int currentDegree;

/* Real generic derivative */
double derivative (double (*F)(double), double x);

/* Complex generic derivative */
double complex derivate (
                    double complex (*F)(double complex),
                    double complex z);

double Newton (double (*F)(double),
                double (*dF) (double),
                double x);

double complex NewtonComplex(
            double complex (*F)(double complex),
            double complex z);

double polynomial (double x);
double polynomialDerivative (double x);

double complex complexPolynomial(double complex z);
double complex complexPolynomialDerivative (double complex z);

void findRealRoots (double (*F)(double),
                    double (*dF)(double),
                    double min,
                    double max,
                    double step);

void findComplexRoots (
                double complex (*F) (double complex),
                double complex (*dF) (double complex),
                double min,
                double max,
                double step);

double Fsin (double x) { 
    return sin (x);
}

double Fsquare (double x){
    return x * x;
}

double Ftest (double x)  {
    return x * x - 2.0;
}

double FtestDerivative (double x){
    return 2.0 * x;
}

double complex Fcube (double complex z) {
    return z * z * z; 
}

double polynomial (double x){
    double result = 0.0;
    
    for (int i = currentDegree; i >= 0; i--){
        result = result * x + currentCoefficients[i];
    }

    return result;
}

double polynomialDerivative (double x) {
    double result = 0.0;

    for (int i = currentDegree; i >= 1; i--){
        result = result * x + i * currentCoefficients[i];
    }
    return result;
}

double complex complexPolynomial (double complex z) {
    double complex result = 0.0;

    for (int i = currentDegree; i >= 0; i--){
        result = result * z + currentCoefficients[i];
    }

    return result;
}

double complex complexPolynomialDerivative(double complex z){
    double complex result = 0.0;

    for (int i = currentDegree; i >= 1; i--){
        result = result * z + i * currentCoefficients[i];
    }
    return result;
}

double complex FcomplexTest(double complex z){
    return z * z + 1.0;
}

int main (void){

    double x = 2.0;

    printf("sin'(%.2f) = %f\n", x, derivative(Fsin, x));
    printf("(x^2)' at %.2f = %f\n", x, derivative(Fsquare, x));

    double complex z = 2.0 + 1.0 * I;

    double complex result = derivate(Fcube, z);

    printf("Derivative of z^3 at %.2f + %.2fi = %.6f + %.6fi\n",
            creal(z), cimag(z),
            creal(result), cimag(result));

    int degree;
    
    printf("\nEnter polynomial degree: ");
  
    if (scanf("%d", &degree) != 1) {
        printf("Invalid degree.\n");
        return 1;
    }

    double coefficients[degree + 1];

    for (int i = degree; i >= 0; i--) {
        printf("Coefficient for x^%d: ", i);
        scanf("%lf", &coefficients[i]);
    }

    currentCoefficients = coefficients;
    currentDegree = degree;

    printf("P(2) = %f\n", polynomial(2.0));
    printf("P'(2) = %f\n", polynomialDerivative(2.0));

    printf("P'(2 + i) = %.6f + %.6fi\n",
        creal(complexPolynomialDerivative(2.0 + 1.0 * I)),
        cimag(complexPolynomialDerivative(2.0 + 1.0 * I)));

    double testRealRoot = Newton(Ftest, FtestDerivative, 1.0);
    printf("Real root of x² - 2: %.10f\n", testRealRoot);

    findRealRoots(polynomial, 
                polynomialDerivative, 
                -10.0, 
                10.0, 
                0.5);

    double complex testRoot = 
        NewtonComplex(FcomplexTest, 1.0 + 1.0 * I);

    printf("Complex root: %.10f + %.10fi\n",
        creal(testRoot), cimag(testRoot));

    findComplexRoots(complexPolynomial,
                    complexPolynomialDerivative,
                    -5.0,
                    5.0,
                    0.5);

    return 0;
}

double derivative (double (*F)(double), double x) {
    double h = 1e-6;

    return (F(x +h) -F(x)) / h;
}

double complex derivate (
                    double complex (*F) (double complex),
                    double complex z) {
    double complex h = 1e-6;

    return (F(z + h) - F(z - h)) / (2.0 * h);
}

double Newton (double (*F)(double),
                double (*dF) (double),
                double x) 
{
    for (int i = 0; i < 100; i++){
        double fx = F(x);
        double dfx = dF(x);

        if (fabs(dfx) < 1e-12) {
            return x;
        }
        double next = x - fx / dfx;

        if (fabs(next - x)  < 1e-10){
            return next;
        }
        x = next;
    }
    return x;
}

double complex NewtonComplex (
                double complex (*F)(double complex),
                double complex z) 
{
    for (int i = 0; i < 100; i++){
        double complex fz = F(z);
        double complex dfz = derivate (F, z);
    
        if (cabs(dfz) < 1e-12){
            return z;
        }

        double complex next = z - fz / dfz;

        if (cabs(next -z) < 1e-10){
            return next;
        }

        z = next;
    }

    return z;
}


void findRealRoots (double (*F)(double),
                    double (*dF)(double),
                    double min,
                    double max,
                    double step)
{
    double roots[100];
    int rootCount = 0;
    
    for (double start = min; start <= max; start += step) {
        double root = Newton(F, dF, start);

        if (fabs(F(root)) < 1e-6){

            int alreadyFound = 0;

            for (int i = 0; i < rootCount; i++){
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
}

void findComplexRoots (
                double complex (*F) (double complex),
                double complex (*dF)(double complex),
                double min,
                double max,
                double step)
{
    double complex roots[100];
    int rootCount = 0;
    
    for (double real = min; real <= max; real += step){
    for(double imag = min; imag <= max; imag += step){

        double complex start = real + imag * I;
        double complex root = NewtonComplex(F, start);    
        
        if(cabs(F(root)) < 1e-6){

            

            printf("Found complex root: %.10f + %.10fi\n",
                    creal(root), cimag(root));
            }
        }
    }
}

