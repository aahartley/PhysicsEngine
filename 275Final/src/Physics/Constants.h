/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

const int PIXELS_PER_METER = 50;
const int PI = 3.14159265359f;

inline float randomFloat() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0f);
}

inline float randomFloat(float min, float max) {
    // Returns a random real in [min,max).
    return min + (max - min) * randomFloat();
}
inline Vec2 randomDirection() {
    float angle = randomFloat() * (2 * PI);
    int length = 1;
    return (Vec2(length * std::cos(angle), length * std::sin(angle)));
}
inline int randomColor() {
    int pick = randomFloat(0, 256);
    int pick1 = randomFloat(0, 256);
    int pick2= randomFloat(0, 256);

    //switch (pick) {
    //case 1:
    //    return 0xFFFF0000;
    //case 2:
    //    return 0xFF00FF00;
    //case 3:
    //    return 0xFF0000FF;
    //default:
    //    return 0xFFFFFFFF;
    //}
    int color = (255 << 24) + (pick2 << 16) + (pick1 << 8) + pick;
    return color;

}
#endif
