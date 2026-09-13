#pragma once
#include <cstdint>

namespace nova::ps1 {

struct Vertex {
    int16_t x;
    int16_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t u;
    uint8_t v;

    Vertex()
        : x(0), y(0), r(0xFF), g(0xFF), b(0xFF), u(0), v(0) {}

    Vertex(int16_t px, int16_t py)
        : x(px), y(py), r(0xFF), g(0xFF), b(0xFF), u(0), v(0) {}
};

struct VertexColor {
    int16_t x;
    int16_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    VertexColor() : x(0), y(0), r(0), g(0), b(0) {}
};

struct VertexTextured {
    int16_t x;
    int16_t y;
    uint8_t u;
    uint8_t v;
    uint16_t clut;

    VertexTextured() : x(0), y(0), u(0), v(0), clut(0) {}
};

struct VertexTexturedColor {
    int16_t x;
    int16_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t u;
    uint8_t v;
    uint16_t clut;

    VertexTexturedColor()
        : x(0), y(0), r(0xFF), g(0xFF), b(0xFF), u(0), v(0), clut(0) {}
};

}