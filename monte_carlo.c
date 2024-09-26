#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// May add some comments about this later but naming should suggest enough for now ig
int main(void) {
    srand(time(0));
    double inner = 0;
    const double total = 1e8;

    for (int i = 0; i < total; i++) {
        double const xi = (double) rand() / (double) RAND_MAX;
        double const yi = (double) rand() / (double) RAND_MAX;

        if (xi*xi + yi*yi > 1)
            continue;

        inner++;
    }

    printf("Inner: %.1f\tTotal: %.1f\n", inner, total);
    printf("Aprox: %.8f\t", 4*inner/total);
    printf("Pi: %.8f\n\n", M_PI);

    return EXIT_SUCCESS;
}