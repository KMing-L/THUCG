#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <utility>
#include <vecmath.h>
#include <vector>

#include <algorithm>

// TODO (PA2): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in
// all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
  protected:
    std::vector<Vector3f> controls;

  public:
    explicit Curve(std::vector<Vector3f> points)
        : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

    std::vector<Vector3f> &getControls() { return controls; }

    virtual void discretize(int resolution, std::vector<CurvePoint> &data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto &control : controls) {
            glVertex3fv(control);
        }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto &control : controls) {
            glVertex3fv(control);
        }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto &cp : sampledPoints) {
            glVertex3fv(cp.V);
        }
        glEnd();
        glPopAttrib();
    }
};

class BezierCurve : public Curve {
  public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
        n = points.size() - 1;
        k = n;

        C = new double *[n + 1];
        for (int i = 0; i < n + 1; i++) {
            C[i] = new double[n + 1];
            for (int j = 0; j < n + 1; j++) {
                C[i][j] = 0;
            }
        }
        C[0][0] = 1;

        // compute C from (0, 0) to (n, n)
        for (int i = 1; i <= n; i++) {
            C[i][0] = 1;
            for (int j = 1; j <= n; j++) {
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            }
        }
    }

    ~BezierCurve() {
        for (int i = 0; i < n + 1; i++) {
            delete[] C[i];
        }
        delete[] C;
    }

    void discretize(int resolution, std::vector<CurvePoint> &data) override {
        data.clear();
        // TODO (PA2): fill in data vector
        for (int idx = 0; idx < (n + 1) * resolution; idx++) {
            double t = (double)idx / resolution / (n + 1);
            Vector3f pos = Vector3f::ZERO;
            Vector3f tan = Vector3f::ZERO;
            for (int i = 0; i <= n; i++) {
                pos += controls[i] * B(i, k, t);
                tan += controls[i] * db(i, k, t);
            }
            data.push_back({pos, tan.normalized()});
        }
    }

  protected:
    double **C;
    int n, k;

    double B(int i, int n, double t) {
        return C[n][i] * pow(1 - t, n - i) * pow(t, i);
    }

    double db(int i, int n, double t) {
        return n * (B(i - 1, n - 1, t) - B(i, n - 1, t));
    }
};

class BsplineCurve : public Curve {
  public:
    explicit BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than "
                   "4!\n");
            exit(0);
        }
        n = points.size() - 1;
        k = 3;
        B = new double[n + k + 1];
        dB = new double[n + k + 1];
    }

    ~BsplineCurve() {
        delete[] B;
        delete[] dB;
    }

    void discretize(int resolution, std::vector<CurvePoint> &data) override {
        data.clear();
        // TODO (PA2): fill in data vector
        for (int idx = k * resolution; idx <= (n + 1) * resolution; idx++) {
            double t = double(idx) / resolution / (n + k + 1);

            for (int i = 0; i < n + k + 1; i++) {
                B[i] = t_i(i) <= t && t < t_i(i + 1) ? 1 : 0;
            }
            for (int p = 1; p <= k; p++) {
                for (int i = 0; i < n + k + 1 - p; i++) {
                    if (p == k) {
                        dB[i] = p * (B[i] / (t_i(i + p) - t_i(i)) -
                                     B[i + 1] / (t_i(i + p + 1) - t_i(i + 1)));
                    }
                    B[i] = (t - t_i(i)) / (t_i(i + p) - t_i(i)) * B[i] +
                           (t_i(i + p + 1) - t) /
                               (t_i(i + p + 1) - t_i(i + 1)) * B[i + 1];
                }
            }

            Vector3f pos = Vector3f::ZERO;
            Vector3f tan = Vector3f::ZERO;

            for (int i = 0; i <= n; i++) {
                pos += B[i] * controls[i];
                tan += dB[i] * controls[i];
            }

            data.push_back({pos, tan.normalized()});
        }
    }

  protected:
    int n, k;
    double *B, *dB;

    double t_i(int i) { return (double)i / (n + k + 1); }
};

#endif // CURVE_HPP
