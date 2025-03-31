#include <iostream>
#include <fstream>
#include <cmath>
#include <optional>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>

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

    [[nodiscard]]
    string toString() const {
        return "(" + to_string(a) + ", " + to_string(b) + ", " + to_string(c) + ")";
    }

    [[nodiscard]]
    optional<vector<double>> solve() const {
        if (!isQuadratic()) {
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

class Student {
protected:
    string name;

public:
    explicit Student(string name) : name(name) {}

    virtual optional<vector<double>> solve(const QuadraticEquation& equation) = 0;

    string getName() const {
        return name;
    }

    virtual ~Student() = default;
};

class GoodStudent : public Student {
public:
    explicit GoodStudent(string name) : Student(name) {}

    optional<vector<double>> solve(const QuadraticEquation& equation) override {
        return equation.solve();
    }
};

class AverageStudent : public Student {
public:
    explicit AverageStudent(string name) : Student(name) {}

    optional<vector<double>> solve(const QuadraticEquation& equation) override {
        if (rand() % 100 < 70) {
            return equation.solve();
        } else {
            int numRoots = rand() % 3;
            vector<double> roots;
            for (int i = 0; i < numRoots; ++i) {
                roots.push_back((rand() % 200 - 100) / 10.0);
            }
            return roots;
        }
    }
};

class BadStudent : public Student {
public:
    explicit BadStudent(string name) : Student(name) {}

    optional<vector<double>> solve(const QuadraticEquation& equation) override {
        return vector<double>{0.0};
    }
};

class Teacher {
private:
    struct Submission {
        QuadraticEquation equation;
        optional<vector<double>> answer;
        shared_ptr<Student> student;
    };

    vector<Submission> submissions;
    map<string, int> results;

public:
    void addSubmission(const QuadraticEquation& equation, shared_ptr<Student> student) {
        auto answer = student->solve(equation);
        submissions.push_back({equation, answer, student});
    }

    void checkAllSubmissions() {
        for (const auto& submission : submissions) {
            auto correctAnswer = submission.equation.solve();
            bool isCorrect = false;

            if (submission.answer.has_value() && correctAnswer.has_value()) {
                auto studentRoots = submission.answer.value();
                auto correctRoots = correctAnswer.value();

                sort(studentRoots.begin(), studentRoots.end());
                sort(correctRoots.begin(), correctRoots.end());

                isCorrect = (studentRoots == correctRoots);
            } else {
                isCorrect = (submission.answer.has_value() == correctAnswer.has_value());
            }

            if (isCorrect) {
                results[submission.student->getName()]++;
            }
        }
    }

    void publishResults(const string& filename) {
        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cerr << "Ошибка открытия файла " << filename << endl;
            return;
        }

        outputFile << "Результаты зачета:" << endl;
        outputFile << "-----------------" << endl;

        for (const auto& [name, score] : results) {
            outputFile << name << ": " << score << " верных решений" << endl;
        }

        outputFile.close();
        cout << "Результаты опубликованы в файле " << filename << endl;
    }
};

int main() {
    srand(time(nullptr));

    vector<shared_ptr<Student>> students = {
        make_shared<GoodStudent>("Иван Иванов (хороший)"),
        make_shared<AverageStudent>("Петр Петров (средний)"),
        make_shared<BadStudent>("Сидор Сидоров (плохой)")
    };

    ifstream inputFile("../input.txt");
    if (!inputFile.is_open()) {
        cerr << "Ошибка открытия файла input.txt" << endl;
        return 1;
    }

    Teacher teacher;
    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        double a, b, c;

        if (!(iss >> a >> b >> c)) {
            cerr << "Ошибка чтения коэффициентов из строки: " << line << endl;
            continue;
        }

        QuadraticEquation equation(a, b, c);

        for (const auto& student : students) {
            teacher.addSubmission(equation, student);
        }
    }

    inputFile.close();

    teacher.checkAllSubmissions();

    teacher.publishResults("../results.txt");

    return 0;
}