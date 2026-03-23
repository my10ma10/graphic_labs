/*
## Требования

Реализовать отрисовку линии, окружности и треугольника в консоль / файл, используя \
алгоритм Брезенхема. Заранее создать изображения и отображать их по таймеру, например, 
в таком порядке: две линии, окружность, треугольник и следующий цикл.
*/

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "drawers.hpp"

std::vector<int> parseLineToInts(const std::string& line);
static std::string getNextValidLine(std::ifstream& file);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments: enter coords filepath as argument\n";
    }

    const std::string filename = argv[1];
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file '" << filename << "'\n";
        return 1;
    }

    std::string firstLine = getNextValidLine(file);
    if (firstLine.empty()) {
        std::cerr << "Error: file is empty or has no valid first line\n";
        return 1;
    }
    
    int shapesCount = std::stoi(firstLine);
    
    try {
        for (int i = 0; i < shapesCount; ++i) {
            std::string line = getNextValidLine(file);
            if (line.empty()) {
                std::cerr << "Error: unexpected end of file at shape #" << (i + 1) << "\n";
                return 1;
            }
            
            std::vector<int> coords = parseLineToInts(line);
            
            Drawer::clearScreen();
            std::cout << "\nDrawing shape #" << (i + 1) << "...\n";
            
            if (coords.size() == 4) {
                LineDrawer::draw(coords[0], coords[1], coords[2], coords[3]);
            } 
            else if (coords.size() == 3) {
                CircleDrawer::draw(coords[0], coords[1], coords[2]);
            } 
            else if (coords.size() == 6) {
                TriangleDrawer::draw(coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
            } 
            else {
                std::cerr << "Warning: shape #" << (i + 1) << " has " << coords.size() 
                          << " coordinates (expected 3, 4, or 6), skipping\n";
                continue;
            }
            
            Drawer::drawScreen();
            Drawer::clearScreen();
        }
    }
    catch (const std::runtime_error& err) {
        std::cerr << "Runtime error: " << err.what() << std::endl;
        return 1;
    }
    
    file.close();
    return 0;
}

std::vector<int> parseLineToInts(const std::string& line) {
    std::vector<int> result;
    std::istringstream iss(line);
    int val;
    while (iss >> val) {
        result.push_back(val);
    }
    return result;
}

std::string getNextValidLine(std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        return line;
    }
    return "";
}