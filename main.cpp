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
    std::string toString() const {
        return "(" + std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + ")";
    }

    [[nodiscard]]
    std::optional<std::vector<double>> solve() const {
        if (!isQuadratic()) {
            if (b == 0) {
                if (c == 0) {
                    return std::nullopt;
                }
                return std::vector<double>{};
            }
            double x = -c / b;
            return std::vector<double>{x};
        }

        double discriminant = b * b - 4.0 * a * c;

        if (discriminant > 0) {
            double x1 = (-b + sqrt(discriminant)) / (2.0 * a);
            double x2 = (-b - sqrt(discriminant)) / (2.0 * a);
            return std::vector<double>{x1, x2};
        }
        if (discriminant == 0) {
            double x = -b / (2.0 * a);
            return std::vector<double>{x};
        }
        return std::vector<double>{};
    }
};

class Student {
protected:
    std::string name;

public:
    explicit Student(std::string name) : name(std::move(name)) {}

    virtual std::optional<std::vector<double>> solve(const QuadraticEquation& equation) = 0;

    [[nodiscard]] std::string getName() const {
        return name;
    }

    virtual ~Student() = default;
};

class GoodStudent : public Student {
public:
    explicit GoodStudent(std::string name) : Student(std::move(name)) {}

    std::optional<std::vector<double>> solve(const QuadraticEquation& equation) override {
        return equation.solve();
    }
};

class AverageStudent : public Student {
public:
    explicit AverageStudent(std::string name) : Student(std::move(name)) {}

    std::optional<std::vector<double>> solve(const QuadraticEquation& equation) override {
        if (rand() % 100 < 70) {
            return equation.solve();
        } else {
            int numRoots = rand() % 3;
            std::vector<double> roots;
            roots.reserve(numRoots);
for (int i = 0; i < numRoots; ++i) {
                roots.push_back((rand() % 200 - 100) / 10.0);
            }
            return roots;
        }
    }
};

class BadStudent : public Student {
public:
    explicit BadStudent(std::string const& name) : Student(name) {}

    std::optional<std::vector<double>> solve(const QuadraticEquation& equation) override {
        return std::vector<double>{0.0};
    }
};

class Teacher {
private:
    struct Submission {
        QuadraticEquation equation;
        std::optional<std::vector<double>> answer;
        std::shared_ptr<Student> student;
    };

    std::vector<Submission> submissions;
    std::map<std::string, int> results;
    std::vector<std::string> studentNames;

public:
    void addSubmission(const QuadraticEquation& equation, const std::shared_ptr<Student> &student) {
        auto answer = student->solve(equation);
        submissions.push_back({equation, answer, student});

        if (std::find(studentNames.begin(), studentNames.end(), student->getName()) == studentNames.end()) {
            studentNames.push_back(student->getName());
            results[student->getName()] = 0;
        }
    }

    void checkAllSubmissions() {
        for (const auto& submission : submissions) {
            auto correctAnswer = submission.equation.solve();
            bool isCorrect = false;

            if (submission.answer.has_value() && correctAnswer.has_value()) {
                auto studentRoots = submission.answer.value();
                auto correctRoots = correctAnswer.value();

                std::sort(studentRoots.begin(), studentRoots.end());
                std::sort(correctRoots.begin(), correctRoots.end());

                isCorrect = (studentRoots == correctRoots);
            } else {
                isCorrect = (submission.answer.has_value() == correctAnswer.has_value());
            }

            if (isCorrect) {
                results[submission.student->getName()]++;
            }
        }
    }

    void publishResults(const std::string& filename) {
        std::ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            std::cerr << "Ошибка открытия файла " << filename << std::endl;
            return;
        }

        outputFile << "Результаты зачета:" << std::endl;
        outputFile << "-----------------" << std::endl;

        for (const auto& name : studentNames) {
            outputFile << name << ": " << results[name] << " верных решений:" << std::endl;
        }

        outputFile.close();
        std::cout << "Результаты опубликованы в файле " << filename << std::endl;
    }
};

int main() {
    srand(time(nullptr));

    std::vector<std::shared_ptr<Student>> students = {
        std::make_shared<GoodStudent>("Микаил Абубаширов"),
        std::make_shared<AverageStudent>("Мария Балабулова"),
        std::make_shared<BadStudent>("Максим Зававидный"),
        std::make_shared<GoodStudent>("Герасим Ильин"),
        std::make_shared<GoodStudent>("Глеб Альтушков"),
        std::make_shared<AverageStudent>("Дмитрий Гешефтов"),
        std::make_shared<GoodStudent>("Александр Ждуков"),
        std::make_shared<AverageStudent>("Артем Гатчинов"),
        std::make_shared<BadStudent>("Леонид Покрасов"),
        std::make_shared<GoodStudent>("Евгений Пак-Чак-Чак"),
        std::make_shared<BadStudent>("Иван Фембоев")
    };

    std::ifstream inputFile("../input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка открытия файла input.txt" << std::endl;
        return 1;
    }

    Teacher teacher;
    std::string line;
    while (getline(inputFile, line)) {
        std::istringstream iss(line);
        double a, b, c;

        if (!(iss >> a >> b >> c)) {
            std::cerr << "Ошибка чтения коэффициентов из строки: " << line << std::endl;
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