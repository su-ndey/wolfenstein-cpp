#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

const int width = 320;
const int height = 200;
const double pi = 3.141592653589793;

// Basic level from src/base.py in wolfenstein-rl.
std::string map[] = {
    "SSSSSSSSSSSSSSSSSSSSSS",
    "S                    S",
    "S                    S",
    "S                    S",
    "SSSSSSS              S",
    "S     S              S",
    "S     S              S",
    "S     S              S",
    "S     S              S",
    "SSSSSSS              S",
    "S                    S",
    "SSSSSSSSSSSSSSSSSSSSSS"
};

bool wall(double x, double y) {
    if (x < 0 || x >= 22 || y < 0 || y >= 12) return true;
    return map[int(y)][int(x)] == 'S';
}

// Check vertical and horizontal grid lines, then take the nearer hit.
double castRay(double x, double y, double angle) {
    double dx = std::sin(angle);
    double dy = std::cos(angle);
    double vertical = 100;
    double horizontal = 100;

    if (std::abs(dx) > 0.000001) {
        int step = dx > 0 ? 1 : -1;
        double gridX = std::floor(x) + (dx > 0 ? 1 : 0);
        for (int i = 0; i < 22; i++) {
            double distance = (gridX - x) / dx;
            double hitY = y + distance * dy;
            double checkX = gridX + step * 0.000001;
            if (wall(checkX, hitY)) {
                vertical = distance;
                break;
            }
            gridX += step;
        }
    }

    if (std::abs(dy) > 0.000001) {
        int step = dy > 0 ? 1 : -1;
        double gridY = std::floor(y) + (dy > 0 ? 1 : 0);
        for (int i = 0; i < 12; i++) {
            double distance = (gridY - y) / dy;
            double hitX = x + distance * dx;
            double checkY = gridY + step * 0.000001;
            if (wall(hitX, checkY)) {
                horizontal = distance;
                break;
            }
            gridY += step;
        }
    }
    return vertical < horizontal ? vertical : horizontal;
}

void draw(double x, double y, double angle) {
    int top[width];
    int bottom[width];
    int shade[width];
    const double fov = pi / 3;

    for (int column = 0; column < width; column++) {
        double offset = fov / 2 - (column + 0.5) * fov / width;
        double distance = castRay(x, y, angle + offset);
        // Correct the distance to avoid the fisheye effect.
        distance *= std::cos(offset);
        if (distance < 0.01) distance = 0.01;
        int wallHeight = int(300 / distance);
        top[column] = (height - wallHeight) / 2;
        bottom[column] = (height + wallHeight) / 2;
        shade[column] = int(220 / (1 + distance * 0.15));
    }

    std::ofstream image("view.ppm");
    if (!image) {
        std::cerr << "Could not write view.ppm\n";
        return;
    }
    image << "P3\n" << width << ' ' << height << "\n255\n";
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            if (row >= top[column] && row < bottom[column]) {
                int value = shade[column];
                image << value << ' ' << value / 2 << ' ' << value / 3 << ' ';
            } else if (row < height / 2) {
                image << "100 150 200 ";
            } else {
                image << "55 55 55 ";
            }
        }
        image << '\n';
    }
}

int main() {
    double x = 3.3125;
    double y = 8.3125;
    double angle = pi;
    std::cout << "w/s: move, a/d: turn, q: quit. Press Enter after a command.\n";
    std::cout << "Open view.ppm in an image viewer after each move.\n";
    draw(x, y, angle);

    char command;
    while (std::cin >> command) {
        if (command == 'q') break;
        if (command == 'a') angle += 0.15;
        if (command == 'd') angle -= 0.15;
        if (command == 'w' || command == 's') {
            double step = command == 'w' ? 0.15 : -0.15;
            double nextX = x + std::sin(angle) * step;
            double nextY = y + std::cos(angle) * step;
            if (!wall(nextX, nextY)) {
                x = nextX;
                y = nextY;
            }
        }
        draw(x, y, angle);
        std::cout << "Position: " << x << ", " << y << '\n';
    }
    return 0;
}
