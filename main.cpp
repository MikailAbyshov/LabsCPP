#include <cmath>
#include <optional>
#include <vector>

using namespace std;

class QuadraticEquation {
private:
    double a, b, c;

public:
    QuadraticEquation(double a, double b, double c) : a(a), b(b), c(c) {}

    [[nodiscard]]
    double getA() const { return a; }
    [[nodiscard]]
    double getB() const { return b; }
    [[nodiscard]]
    double getC() const { return c; }

    [[nodiscard]]
    bool isQuadratic() const {
        return a != 0;
    }
};

class Solver {
public:
    static optional<vector<double>> solveEquation(const QuadraticEquation& equation) {
        double a = equation.getA();
        double b = equation.getB();
        double c = equation.getC();

        if (!equation.isQuadratic()) {
            if (b == 0) {
                if (c == 0) {
                    return nullopt;
                }
                return vector<double>{};
            }
            double x = -c / b;
            return vector<double>{x};
        }

        double discriminant = b * b - 4.0 * a * c;

        if (discriminant > 0) {
            double x1 = (-b + sqrt(discriminant)) / (2.0 * a);
            double x2 = (-b - sqrt(discriminant)) / (2.0 * a);
            return vector<double>{x1, x2};
        } else if (discriminant == 0) {
            double x = -b / (2.0 * a);
            return vector<double>{x};
        } else {
            return vector<double>{};
        }
    }
};

int main() {

}
