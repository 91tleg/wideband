#include "lambda.h"

#define LAMBDA_LOOKUP_SIZE (10)

struct lookup_entry
{
    float amps;
    float lambda;
};

const struct lookup_entry lambda_lookup[] __attribute__((section(".rodata"))) = {
    {-2.22, 0.65},
    {-1.82, 0.70},
    {-1.11, 0.80},
    {-0.50, 0.90},
    {0.00, 1.016},
    {0.33, 1.18},
    {0.67, 1.43},
    {0.94, 1.70},
    {1.38, 2.42},
    {2.54, 4.99}};

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
