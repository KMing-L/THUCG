#pragma once

#include <algorithm>
#include <cstdio>
#include <image.hpp>
#include <queue>

inline void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

inline int abs(int a) { return a > 0 ? a : -a; }

class Element {
  public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {
  public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Bresenham Algorithm
        printf(
            "Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n",
            xA, yA, xB, yB, color.x(), color.y(), color.z());
        if (xA == xB) {
            if (yA > yB)
                swap(yA, yB);
            for (int i = yA; i <= yB; i++) {
                img.SetPixel(xA, i, color);
            }
        } else if (yA == yB) {
            if (xA > xB)
                swap(xA, xB);
            for (int i = xA; i <= xB; i++) {
                img.SetPixel(i, yA, color);
            }
        } else {
            int dx = xB - xA;
            int dy = yB - yA;
            if (abs(dx) > abs(dy)) {
                if (xA > xB) {
                    swap(xA, xB);
                    swap(yA, yB);
                    dx = xB - xA;
                    dy = yB - yA;
                }
                int x = xA, y = yA;
                bool flag = false;
                if (dy < 0) {
                    dy = -dy;
                    flag = true;
                }
                int e = -dx;
                for (int i = 0; i <= dx; i++) {
                    img.SetPixel(x, y, color);
                    x++;
                    e += 2 * dy;
                    if (e >= 0) {
                        if (flag)
                            y--;
                        else
                            y++;
                        e -= 2 * dx;
                    }
                }
            } else {
                if (yA > yB) {
                    swap(xA, xB);
                    swap(yA, yB);
                    dx = xB - xA;
                    dy = yB - yA;
                }
                int x = xA, y = yA;
                bool flag = false;
                if (dx < 0) {
                    dx = -dx;
                    flag = true;
                }
                int e = -dy;
                for (int i = 0; i <= dy; i++) {
                    img.SetPixel(x, y, color);
                    y++;
                    e += 2 * dx;
                    if (e >= 0) {
                        if (flag)
                            x--;
                        else
                            x++;
                        e -= 2 * dy;
                    }
                }
            }
        }
    }
};

class Circle : public Element {
  protected:
    void drawCirclePoint(Image &img, int x, int y) {
        img.SetPixel(cx + x, cy + y, color);
        img.SetPixel(cx + x, cy - y, color);
        img.SetPixel(cx - x, cy + y, color);
        img.SetPixel(cx - x, cy - y, color);
        img.SetPixel(cx + y, cy + x, color);
        img.SetPixel(cx + y, cy - x, color);
        img.SetPixel(cx - y, cy + x, color);
        img.SetPixel(cx - y, cy - x, color);
    }

  public:
    int cx, cy;
    int radius;
    Vector3f color;

    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        printf("Draw a circle with center (%d, %d) and radius %d using color "
               "(%f, %f, %f)\n",
               cx, cy, radius, color.x(), color.y(), color.z());
        int x = 0, y = radius;
        int d = 5 - 4 * radius;
        drawCirclePoint(img, x, y);
        while (x <= y) {
            if (d < 0) {
                d += 8 * x + 12;
            } else {
                d += 8 * (x - y) + 20;
                y--;
            }
            x++;
            drawCirclePoint(img, x, y);
        }
    }
};

struct Seed {
    int x, y;
    Seed(int x, int y) : x(x), y(y) {}
};

class Fill : public Element {
  public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Flood fill
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n",
               cx, cy, color.x(), color.y(), color.z());

        Vector3f oldColor = img.GetPixel(cx, cy);

        int xl, xr, x, y;
        bool spanNeedFIll;
        Seed pt(cx, cy);
        std::queue<Seed> q;
        q.push(pt);

        while (!q.empty()) {
            pt = q.front();
            q.pop();
            xl = pt.x;
            xr = pt.x + 1;

            // find left and right boundary
            while (xl >= 0 && img.GetPixel(xl, pt.y) == oldColor) {
                img.SetPixel(xl, pt.y, color);
                xl--;
            }
            while (xr < img.Width() && img.GetPixel(xr, pt.y) == oldColor) {
                img.SetPixel(xr, pt.y, color);
                xr++;
            }

            x = xl + 1;
            y = pt.y + 1;
            if (y < img.Height()) {
                while (x < xr) {
                    spanNeedFIll = false;
                    while (x < xr && img.GetPixel(x, y) == oldColor) {
                        spanNeedFIll = true;
                        x++;
                    }
                    if (spanNeedFIll) {
                        q.push(Seed(x - 1, y));
                        spanNeedFIll = false;
                    }
                    while (x < xr && img.GetPixel(x, y) != oldColor) {
                        x++;
                    }
                }
            }

            x = xl + 1;
            y = pt.y - 1;
            if (y >= 0) {
                while (x < xr) {
                    spanNeedFIll = false;
                    while (x < xr && img.GetPixel(x, y) == oldColor) {
                        spanNeedFIll = true;
                        x++;
                    }
                    if (spanNeedFIll) {
                        q.push(Seed(x - 1, y));
                        spanNeedFIll = false;
                    }
                    while (x < xr && img.GetPixel(x, y) != oldColor) {
                        x++;
                    }
                }
            }
        }
    }
};