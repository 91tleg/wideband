#include "lambda.h"

#define LAMBDA_LOOKUP_SIZE 20
const float lambda_lookup[LAMBDA_LOOKUP_SIZE] __attribute__((section(".rodata"))) = {
    -2.22, 0.65,
    -1.82, 0.70,
    -1.11, 0.80,
    -0.50, 0.90,
    0.00, 1.016,
    0.33, 1.18,
    0.67, 1.43,
    0.94, 1.70,
    1.38, 2.42,
    2.54, 4.99};

float get_lambda(float amps)
{
    if (amps < lambda_lookup[0])
    {
        return 0.0f;
    }
    if (amps > lambda_lookup[LAMBDA_LOOKUP_SIZE - 2])
    {
        return 9.99f;
    }

    for (int x = 0; x < LAMBDA_LOOKUP_SIZE - 2; x += 2)
    {
        if (amps >= lambda_lookup[x] && amps <= lambda_lookup[x + 2])
        {
            // Interpolate the lookup
            return (amps - lambda_lookup[x]) / (lambda_lookup[x + 2] - lambda_lookup[x]) * (lambda_lookup[x + 3] - lambda_lookup[x + 1]) + lambda_lookup[x + 1];
        }
    }
}