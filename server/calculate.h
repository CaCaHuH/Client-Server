#ifndef CALCULATE_H
#define CALCULATE_H

struct result
{
    float v;
    float tetta;
    float y;
    float x;
};

class calculate
{
public:
    calculate();
    result Run(float d, float dt, float v0, float y0, float cx0, float A, float cya, float teta, float m0, float mc, float tet, float tr, float aero);
};

#endif // CALCULATE_H
