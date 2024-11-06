#include <stdio.h>
#include <stdlib.h>

double f(double);
void trapezoid_rule(double, double, size_t);

int main(void) {
    trapezoid_rule(0, 2, 20);
    return EXIT_SUCCESS;
}

void trapezoid_rule(double const start, double const end, size_t const samples) {
    double const delta = end/samples-start;
    double x=start, sum=0;

    for (int i=0; i<samples; i++, x+=delta) {
        double const dx = delta * 0.5 * (f(x) + f(x+delta));
        printf("f(x): %lf, f(x+1): %lf, area: %lf\n", f(x), f(x+delta), dx);
        sum += dx;
    }

    printf("sum: %lf\n", sum);
}

// Any func should do, for now, a parabola
double f(double const x) {
    return x*x + 1;
}