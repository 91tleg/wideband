#include "lambda.h"

#define LAMBDA_LOOKUP_SIZE (10)

struct lookup_entry
{
    float amps;
    float lambda;
};

const struct lookup_entry lambda_lookup[] __attribute__((section(".rodata"))) = {
    {-2.22f, 0.65f},
    {-1.82f, 0.70f},
    {-1.11f, 0.80f},
    {-0.50f, 0.90f},
    {0.00f, 1.016f},
    {0.33f, 1.18f},
    {0.67f, 1.43f},
    {0.94f, 1.70f},
    {1.38f, 2.42f},
    {2.54f, 4.99f}};

float get_lambda(float amps)
{
    if ((amps <= lambda_lookup[0].amps) || (amps >= lambda_lookup[LAMBDA_LOOKUP_SIZE - 1].amps))
    {
        return (amps <= lambda_lookup[0].amps)
                    ? lambda_lookup[0].lambda
                    : lambda_lookup[LAMBDA_LOOKUP_SIZE - 1].lambda;
    }

    for (int x = 0; x < LAMBDA_LOOKUP_SIZE - 1; x++)
    {
        float x0 = lambda_lookup[x].amps;
        float y0 = lambda_lookup[x].lambda;
        float x1 = lambda_lookup[x + 1].amps;
        float y1 = lambda_lookup[x + 1].lambda;

        if (amps >= x0 && amps <= x1)
        {
            float slope = (y1 - y0) / (x1 - x0);
            return y0 + slope * (amps - x0);
        }
    }
    return 0.0f;
}
