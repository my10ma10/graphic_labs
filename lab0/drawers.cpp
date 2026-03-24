#include "drawers.hpp"
#include <iostream>

std::array<std::array<char, WIDTH>, HEIGHT> Drawer::screen;

void Drawer::putPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        screen[y][x] = '*';
    }
    else {
        throw std::logic_error("Array boundaries error");
    }
}


void Drawer::clearScreen() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            screen[y][x] = '-';
        }
    }
}

void Drawer::drawScreen() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::cout << screen[y][x] << ' ';
        }
        std::cout << std::endl;
    }
}

void LineDrawer::draw(int x0, int y0, int x1, int y1) {
    if (std::abs(x1 - x0) > std::abs(y1 - y0)) {
        drawLineH(x0, y0, x1, y1);
    }
    else {
        drawLineV(x0, y0, x1, y1);
    }
}

void LineDrawer::drawLineH(int x0, int y0, int x1, int y1) {
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;

    int direction = dy < 0 ? -1 : 1;
    dy *= direction;

    if (dx == 0) return;
    
    
    int y = y0;
    int p = 2*dy - dx;
    
    for (int i = 0; i < dx + 1; ++i) {
        putPixel(x0 + i, y);
        if (p >= 0) {
            y += direction;
            p = p - 2*dx;
        }
        p = p + 2*dy;
    }
}

void LineDrawer::drawLineV(int x0, int y0, int x1, int y1) {
    if (y0 > y1) {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }
    int dy = y1 - y0;
    int dx = x1 - x0;

    int direction = dx < 0 ? -1 : 1;
    dx *= direction;

    if (dy == 0) return;
    
    
    int x = x0;
    int p = 2*dx - dy;
    
    for (int i = 0; i < dy + 1; ++i) {
        putPixel(y0 + i, x);
        if (p >= 0) {
            x += direction;
            p = p - 2*dy;
        }
        p = p + 2*dx;
    }
}

void CircleDrawer::draw(int x0, int y0, int radius) {
    if (radius < 0) return;

    int x = 0;
    int y = radius;
    int d = 1 - radius;

    drawCirclePoints(x0, y0, x, y);

    while (x < y) {
        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } 
        else {
            y--;
            d += 2 * (x - y) + 1;
        }
        drawCirclePoints(x0, y0, x, y);
    }
}

void CircleDrawer::drawCirclePoints(int x0, int y0, int x, int y) {
    putPixel(x0 + x, y0 + y);
    putPixel(x0 - x, y0 + y);
    putPixel(x0 + x, y0 - y);
    putPixel(x0 - x, y0 - y);
    putPixel(x0 + y, y0 + x);
    putPixel(x0 - y, y0 + x);
    putPixel(x0 + y, y0 - x);
    putPixel(x0 - y, y0 - x);
}


void TriangleDrawer::drawEdge(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    int dirX = (dx > 0) ? 1 : -1;
    int dirY = (dy > 0) ? 1 : -1;
    
    dx = std::abs(dx);
    dy = std::abs(dy);
    
    int des = (dx > dy) ? dx / 2 : -dy / 2;
    
    while (true) {
        putPixel(x0, y0);
        
        if (x0 == x1 && y0 == y1) break;
        
        int des2 = des;
        if (des2 > -dx) {
            des -= dy;
            x0 += dirX;
        }
        if (des2 < dy) {
            des += dx;
            y0 += dirY;
        }
    }
}

void TriangleDrawer::draw(int x0, int y0, int x1, int y1, int x2, int y2) {
    drawEdge(x0, y0, x1, y1);
    drawEdge(x1, y1, x2, y2);
    drawEdge(x2, y2, x0, y0);
}
