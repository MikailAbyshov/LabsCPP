#include <iostream>
#include <fstream>
#include <cmath>
#include <optional>
#include <vector>
#include <string>
#include <sstream>

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
        }
        if (discriminant == 0) {
            double x = -b / (2.0 * a);
            return vector<double>{x};
        }
        return vector<double>{};
    }
};

int main() {
    ifstream inputFile("../input.txt");
    if (!inputFile.is_open()) {
        cerr << "Ошибка открытия файла input.txt" << endl;
        return 1;
    }

    ofstream outputFile("../output.txt");
    if (!outputFile.is_open()) {
        cerr << "Ошибка открытия файла output.txt" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        double a, b, c;

        if (!(iss >> a >> b >> c)) {
            outputFile << "Ошибка чтения коэффициентов из строки: " << line << endl;
            continue;
        }

        QuadraticEquation equation(a, b, c);
        auto roots = Solver::solveEquation(equation);

        if (roots) {
            auto rootsNumber = roots->size();

            if (rootsNumber != 0) {
                outputFile << "Корни квадратного уравнения с коэффициентами (" << a << ", " << b << ", " << c << "):" << endl;
                for (int i = 0; i < rootsNumber; i++) {
                    outputFile << roots->data()[i] << endl;
                }
            } else {
                outputFile << "У квадратного уравнения с коэффициентами (" << a << ", " << b << ", " << c << ") нет вещественных корней" << endl;
            }
        } else {
            outputFile << "У квадратного уравнения с коэффициентами (" << a << ", " << b << ", " << c << ") бесконечное множество решений" << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    cout << "Результаты записаны в файл output.txt" << endl;
    return 0;
}