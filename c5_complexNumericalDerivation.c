#include <stdio.h>
#include <math.h>
#include <complex.h>

/*
============================================================
 Complex Function Derivative Analyzer
------------------------------------------------------------
 This program:
 1. Accepts a complex number z = x + iy
 2. Applies a selected complex function f(z)
 3. Computes the numerical derivative using central difference
 4. Compares derivatives in real and imaginary directions
 5. Determines whether the function is analytic at that point

 Author: (your name)
============================================================
*/


/* ========= Function Prototypes ========= */

/* Complex functions */
double complex FSquare(double complex z);
double complex FPoly(double complex z);
double complex FExpo(double complex z);
double complex FSin(double complex z);
double complex FFraction(double complex z);
double complex FLog(double complex z);
double complex FRoot(double complex z);
double complex FConj(double complex z);
double complex FMagSquare(double complex z);
double complex FRealOnly(double complex z);

/* Numerical derivative */
double complex derivate(double complex (*f)(double complex),
                        double complex z,
                        double complex h);


/* ===================== MAIN ===================== */

int main(void) {

    double x, y;
    int choice;

    /* ---- Input complex number ---- */
    printf("Enter complex number (real imag): ");
    if (scanf("%lf %lf", &x, &y) != 2) {
        printf("Invalid input\n");
        return 1;
    }

    double complex z = x + y * I;

    /* ---- Function selection menu ---- */
    printf("\nAvailable functions:\n");
    printf("1 = z^2\n");
    printf("2 = z^3\n");
    printf("3 = exp(z)\n");
    printf("4 = sin(z)\n");
    printf("5 = 1/z\n");
    printf("6 = log(z)\n");
    printf("7 = sqrt(z)\n");
    printf("8 = conj(z)\n");
    printf("9 = |z|^2\n");
    printf("10 = Re(z)\n");

    printf("\nChoose function: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    /* Function pointer */
    double complex (*f)(double complex) = NULL;

    /* ---- Assign selected function ---- */
    switch (choice) {
        case 1:  f = FSquare;      break;
        case 2:  f = FPoly;        break;
        case 3:  f = FExpo;        break;
        case 4:  f = FSin;         break;
        case 5:  f = FFraction;    break;
        case 6:  f = FLog;         break;
        case 7:  f = FRoot;        break;
        case 8:  f = FConj;        break;
        case 9:  f = FMagSquare;   break;
        case 10: f = FRealOnly;    break;
        default:
            printf("Invalid choice\n");
            return 1;
    }

    /*
    ------------------------------------------------------------
    Step sizes for numerical differentiation

    h_real → perturbation along real axis
    h_img  → perturbation along imaginary axis

    These allow us to test whether the derivative is
    direction-independent (analytic function)
    ------------------------------------------------------------
    */
    double complex h_real = 1e-6 + 0.0 * I;
    double complex h_img  = 0.0 + 1e-6 * I;

    /* ---- Compute derivatives ---- */
    double complex d_real = derivate(f, z, h_real);
    double complex d_img  = derivate(f, z, h_img);

    /* ---- Output ---- */
    printf("\nz = %.6f + %.6fi\n", creal(z), cimag(z));

    printf("\nDerivative (real direction): %.6f + %.6fi\n",
           creal(d_real), cimag(d_real));

    printf("Derivative (imag direction): %.6f + %.6fi\n",
           creal(d_img), cimag(d_img));

    /*
    ------------------------------------------------------------
    Analyticity test:
    A function is analytic if derivative is the same
    in all directions (Cauchy-Riemann conditions).
    ------------------------------------------------------------
    */
    if (cabs(d_real - d_img) < 1e-6) {
        printf("\nFunction appears analytic at this point.\n");
    } else {
        printf("\nFunction is NOT analytic at this point.\n");
    }

    return 0;
}


/* ===================== DERIVATIVE ===================== */

/*
------------------------------------------------------------
 Computes numerical derivative using central difference:

 f'(z) ≈ (f(z + h) - f(z - h)) / (2h)

 This is more accurate than forward difference.
------------------------------------------------------------
*/
double complex derivate(double complex (*f)(double complex),
                        double complex z,
                        double complex h)
{
    return (f(z + h) - f(z - h)) / (2.0 * h);
}


/* ===================== FUNCTIONS ===================== */

/* f(z) = z^2 */
double complex FSquare(double complex z){
    return z * z;
}

/* f(z) = z^3 */
double complex FPoly(double complex z){
    return z * z * z;
}

/* f(z) = exp(z) */
double complex FExpo(double complex z){
    return cexp(z);
}

/* f(z) = sin(z) */
double complex FSin(double complex z){
    return csin(z);
}

/*
 f(z) = 1/z
 Includes safety check for division by zero
*/
double complex FFraction(double complex z){
    if (cabs(z) < 1e-12) {
        printf("Warning: division by zero or near-zero!\n");
        return 0.0 + 0.0 * I;
    }
    return 1.0 / z;
}

/*
 f(z) = log(z)
 Undefined at z = 0
*/
double complex FLog(double complex z){
    if (cabs(z) < 1e-12) {
        printf("Warning: log undefined at zero!\n");
        return 0.0 + 0.0 * I;
    }
    return clog(z);
}

/* f(z) = sqrt(z) */
double complex FRoot(double complex z){
    return csqrt(z);
}

/*
 f(z) = conjugate(z)
 NOT analytic (violates Cauchy-Riemann)
*/
double complex FConj(double complex z){
    return conj(z);
}

/*
 f(z) = |z|^2 = z * conjugate(z)
 Real-valued function, not analytic
*/
double complex FMagSquare(double complex z){
    return z * conj(z);
}

/*
 f(z) = Re(z)
 Returns real part as complex number
*/
double complex FRealOnly(double complex z){
    return creal(z) + 0.0 * I;
}