#include <stdio.h>
#include <math.h>

int main(void)
{
    int digits;

    printf("How many digits of pi (max 16 with double)? ");
    scanf("%d", &digits);

    if (digits > 16) {
        printf("Error: double precision only supports ~16 digits.\n");
        return 1;
    }

    double a = 1.0;
    double b = 1.0 / sqrt(2.0);
    double t = 0.25;
    double p = 1.0;

    double pi = 0.0;
    double pi_old = 0.0;

    int iter = 0;

    double tolerance = pow(10, -digits);

    while (1)
    {
        iter++;

        pi_old = pi;

        double a_next = (a + b) / 2.0;
        double b_next = sqrt(a * b);

        t = t - p * (a - a_next) * (a - a_next);
        p = 2.0 * p;

        a = a_next;
        b = b_next;

        pi = (a + b) * (a + b) / (4.0 * t);

        if (fabs(pi - pi_old) < tolerance)
            break;
    }

    printf("pi = %.*f\n", digits, pi);
    printf("iterations = %d\n", iter);
    printf("Pi computed using Gauss–Legendre iterative algorithm (double precision).\n");
    
    return 0;
}