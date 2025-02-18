#include <iostream>
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
                    return vector<double>{NULL};
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
    while (true) {
        string input;
        cout << "Введите коэффициенты a, b и c (или нажмите Enter для выхода): ";
        getline(cin, input);

        if (input.empty()) {
            break;
        }

        double a, b, c;
        sscanf(input.c_str(), "%lf %lf %lf", &a, &b, &c);

        QuadraticEquation equation(a, b, c);

        auto roots = Solver::solveEquation(equation);
        auto rootsNumber = roots->size();

        if (rootsNumber != 0) {
            if (roots->data()[0] != NULL) {
                cout << "Корни введенного квадратного уравнения:" << endl;

                for (int i = 0; i < roots->size(); i++) {
                    cout << roots->data()[i] << endl;
                }
            } else {
                cout << "У введенного квадратного уравнения бесконечное множество решений" << endl;
            }
        } else {
            cout << "У введенного квадратного уравнения нет вещественных корней" << endl;
        }

        roots->clear();
    }

    return 0;
}