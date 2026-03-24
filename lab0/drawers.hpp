#pragma once
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <array>
#include <vector>
#include <sstream>

static const int WIDTH = 25;
static const int HEIGHT = 20;


class Drawer {
public:
    static void clearScreen();
    static void drawScreen();

protected:
    static std::array<std::array<char, WIDTH>, HEIGHT> screen;
    

    static void putPixel(int x, int y);
};

class LineDrawer : public Drawer {
public:
    static void draw(int x0, int y0, int x1, int y1);

private:
    static void drawLineH(int x0, int y0, int x1, int y1);
    static void drawLineV(int x0, int y0, int x1, int y1);
};

class CircleDrawer : public Drawer {
public:
    static void draw(int x0, int y0, int radius);

private:
    static void drawCirclePoints(int x0, int y0, int x, int y);

};

class TriangleDrawer : public LineDrawer {
public:
    static void draw(int x0, int y0, int x1, int y1, int x2, int y2);

private:
    static void drawEdge(int x0, int y0, int x1, int y1);
};