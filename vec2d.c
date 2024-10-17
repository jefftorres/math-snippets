#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double x, y;
} Vec2d;

double length(Vec2d const*);
Vec2d *from_angle(double);
Vec2d *rotate(Vec2d const*, double);
Vec2d *polar(Vec2d const*);
double angle(Vec2d const*, Vec2d const*);
double dot(Vec2d const*, Vec2d const*);
double cross(Vec2d const*, Vec2d const*);
// TODO normalize

double _radians(double);
double _degrees(double);

int main(void) {
    double alpha = 90;  // angle in degrees
    Vec2d *A = calloc(1, sizeof(A));
    Vec2d E = {-1.0, -1.0};
    A->x = 1.0;
    A->y = 1.0;

    printf("Original Vec.\n");
    printf("x:  %lf, \ty:  %lf\n", A->x, A->y);

    printf("Length of Vec: %lf\n", length(A));

    alpha = 45;
    Vec2d *B = from_angle(alpha);
    printf("Unitary Vec from %.2lf degrees.\n", alpha);
    printf("x': %lf, \ty': %lf\n", B->x, B->y);
    free(B);

    B = rotate(A, alpha);
    printf("Vec rotated by %.1lf degrees.\n", alpha);
    printf("x': %lf, \ty': %lf\n", B->x, B->y);
    free(B);

    B = polar(A);
    printf("Vec in polar form.\n");
    printf("r': %lf, \tphi': %lf\n", B->x, B->y);
    free(B);

    B = from_angle(135);
    printf("Angle between 2 vecs: %lf\n", _degrees(angle(A, B)));
    free(B);

    free(A);
    return EXIT_SUCCESS;
}

// Returns module of vector
double length(Vec2d const *vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

// Returns vector in cartesian coordinates based on given angle
Vec2d *from_angle(double const phi) {
    double const rad = _radians(phi);
    Vec2d *vec = calloc(1, sizeof(vec));
    vec->x = cos(rad);
    vec->y = sin(rad);

    return vec;
}

// Rotates the vector from the origin in given degrees
Vec2d *rotate(Vec2d const *vec, double const phi) {
    double const rad = _radians(phi);

    Vec2d *vec_new = calloc(1, sizeof(vec));
    vec_new->x = vec->x*cos(rad) - vec->y*sin(rad);
    vec_new->y = vec->x*sin(rad) + vec->y*cos(rad);

    return vec_new;
}

// Returns the given vector in polar form
Vec2d *polar(Vec2d const *vec) {
    Vec2d *vec_new = calloc(1, sizeof(vec));
    vec_new->x = length(vec);                         // r
    vec_new->y = _degrees(atan2(vec->y, vec->x)); // phi

    return vec_new;
}

// Angle between 2 vectors (in radians)
double angle(Vec2d const *vec1, Vec2d const *vec2) {
    double const product = dot(vec1, vec2)/(length(vec1) * length(vec2));
    return acos(product);
}

// Dot product
double dot(Vec2d const *vec1, Vec2d const *vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y;
}

// Cross product (maybe trying to project to 2d? don't really know)
double cross(Vec2d const *vec1, Vec2d const *vec2) {
    double const phi = angle(vec1, vec2);
    return length(vec1) * length(vec2) * phi;
}

double _radians(double const phi) {
    return phi * M_PI / 180;
}

double _degrees(double const rad) {
    return rad * 180 / M_PI;
}