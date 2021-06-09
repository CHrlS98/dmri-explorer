#version 460

const float PI = 3.14159265358f;
const int MAX_FACTORIAL = 18;
const float FACTORIALS_LUT[MAX_FACTORIAL + 1] =
{
    1.0f,
    1.0f,
    2.0f,
    6.0f,
    24.0f,
    120.0f,
    720.0f,
    5040.0f,
    40320.0f,
    362880.0f,
    3628800.0f,
    39916800.0f,
    479001600.0f,
    6227020800.0f,
    87178291200.0f,
    1307674368000.0f,
    20922789888000.0f,
    355687428096000.0f,
    6402373705728000.0f
};