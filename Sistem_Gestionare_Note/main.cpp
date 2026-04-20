#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

class Student {
    int id;
    string name;
    map<string, vector<double>> grades;

public:
    Student(int id, string name) : id(id), name(move(name)) {}

    int getId() const {
        return id;
    }

    const string &getName() const {
        return name;
    }

    void addGrade(const string &subject, double grade) {
        grades[subject].push_back(grade);
    }

    double subjectAverage(const string &subject) const {
        auto it = grades.find(subject);
        if (it == grades.end() || it->second.empty()) {
            return 0.0;
        }
        double sum = accumulate(it->second.begin(), it->second.end(), 0.0);
        return sum / static_cast<double>(it->second.size());
    }

    double generalAverage() const {
        double total = 0.0;
        int count = 0;

        for (const auto &entry : grades) {
            for (double grade : entry.second) {
                total += grade;
                ++count;
            }
        }

        if (count == 0) {
            return 0.0;
        }
        return total / static_cast<double>(count);
    }

    const map<string, vector<double>> &getGrades() const {
        return grades;
    }
};

class GradeBook {
    vector<Student> students;
    int nextStudentId = 1;

    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static int readInt(const string &prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                clearInput();
                return value;
            }
            cout << "Valoare invalida. Incearca din nou.\n";
            clearInput();
        }
    }

    static double readDouble(const string &prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= 1.0 && value <= 10.0) {
                clearInput();
                return value;
            }
            cout << "Nota trebuie sa fie intre 1 si 10.\n";
            clearInput();
        }
    }

    static string readLine(const string &prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
        } while (value.empty());
        return value;
    }

    Student *findStudent(int id) {
        for (auto &student : students) {
            if (student.getId() == id) {
                return &student;
            }
        }
        return nullptr;
    }

public:
    void seedData() {
        students.emplace_back(nextStudentId++, "Elena Stoica");
        students.emplace_back(nextStudentId++, "Radu Dumitrescu");
        students[0].addGrade("Matematica", 9.0);
        students[0].addGrade("Matematica", 10.0);
        students[0].addGrade("Fizica", 8.5);
        students[1].addGrade("Programare", 9.5);
        students[1].addGrade("Programare", 8.0);
        students[1].addGrade("Electronica", 9.0);
    }

    void addStudent() {
        string name = readLine("Nume student: ");
        students.emplace_back(nextStudentId++, name);
        cout << "Student adaugat cu ID " << students.back().getId() << ".\n";
    }

    void addGrade() {
        int id = readInt("ID student: ");
        Student *student = findStudent(id);
        if (student == nullptr) {
            cout << "Studentul nu exista.\n";
            return;
        }

        string subject = readLine("Materie: ");
        double grade = readDouble("Nota: ");
        student->addGrade(subject, grade);
        cout << "Nota a fost adaugata.\n";
    }

    void listStudents() const {
        if (students.empty()) {
            cout << "Nu exista studenti.\n";
            return;
        }

        cout << left << setw(8) << "ID" << setw(24) << "Nume" << "Medie generala\n";
        cout << string(50, '-') << '\n';
        for (const auto &student : students) {
            cout << left << setw(8) << student.getId()
                 << setw(24) << student.getName()
                 << fixed << setprecision(2) << student.generalAverage() << '\n';
        }
    }

    void showStudentReport() {
        int id = readInt("ID student: ");
        Student *student = findStudent(id);
        if (student == nullptr) {
            cout << "Studentul nu exista.\n";
            return;
        }

        cout << "\nRaport pentru " << student->getName() << '\n';
        cout << "Medie generala: " << fixed << setprecision(2) << student->generalAverage() << '\n';

        if (student->getGrades().empty()) {
            cout << "Nu exista note inregistrate.\n";
            return;
        }

        for (const auto &entry : student->getGrades()) {
            cout << entry.first << ": ";
            for (size_t i = 0; i < entry.second.size(); ++i) {
                cout << entry.second[i];
                if (i + 1 < entry.second.size()) {
                    cout << ", ";
                }
            }
            cout << " | Medie: " << fixed << setprecision(2) << student->subjectAverage(entry.first) << '\n';
        }
    }

    void showRanking() const {
        if (students.empty()) {
            cout << "Nu exista studenti.\n";
            return;
        }

        vector<const Student *> ranking;
        for (const auto &student : students) {
            ranking.push_back(&student);
        }

        sort(ranking.begin(), ranking.end(), [](const Student *a, const Student *b) {
            return a->generalAverage() > b->generalAverage();
        });

        cout << "\nClasament studenti\n";
        for (size_t i = 0; i < ranking.size(); ++i) {
            cout << i + 1 << ". " << ranking[i]->getName()
                 << " - medie " << fixed << setprecision(2) << ranking[i]->generalAverage() << '\n';
        }
    }

    void run() {
        seedData();

        while (true) {
            cout << "\n=== Sistem Gestionare Note ===\n";
            cout << "1. Afiseaza studenti\n";
            cout << "2. Adauga student\n";
            cout << "3. Adauga nota\n";
            cout << "4. Raport student\n";
            cout << "5. Clasament\n";
            cout << "0. Iesire\n";

            int option = readInt("Optiune: ");

            switch (option) {
                case 1:
                    listStudents();
                    break;
                case 2:
                    addStudent();
                    break;
                case 3:
                    addGrade();
                    break;
                case 4:
                    showStudentReport();
                    break;
                case 5:
                    showRanking();
                    break;
                case 0:
                    cout << "Program incheiat.\n";
                    return;
                default:
                    cout << "Optiune invalida.\n";
                    break;
            }
        }
    }
};

int main() {
    GradeBook gradeBook;
    gradeBook.run();
    return 0;
}
