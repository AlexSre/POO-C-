#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

class Book {
    string code;
    string title;
    string author;
    bool borrowed;
    string borrowerName;

public:
    Book(string code, string title, string author)
        : code(move(code)), title(move(title)), author(move(author)), borrowed(false) {}

    const string &getCode() const {
        return code;
    }

    const string &getTitle() const {
        return title;
    }

    const string &getAuthor() const {
        return author;
    }

    bool isBorrowed() const {
        return borrowed;
    }

    const string &getBorrowerName() const {
        return borrowerName;
    }

    bool borrowTo(const string &memberName) {
        if (borrowed) {
            return false;
        }
        borrowed = true;
        borrowerName = memberName;
        return true;
    }

    bool giveBack() {
        if (!borrowed) {
            return false;
        }
        borrowed = false;
        borrowerName.clear();
        return true;
    }
};

class Member {
    int id;
    string name;
    vector<string> borrowedCodes;

public:
    Member(int id, string name) : id(id), name(move(name)) {}

    int getId() const {
        return id;
    }

    const string &getName() const {
        return name;
    }

    const vector<string> &getBorrowedCodes() const {
        return borrowedCodes;
    }

    void addBorrowedBook(const string &code) {
        borrowedCodes.push_back(code);
    }

    bool removeBorrowedBook(const string &code) {
        auto it = find(borrowedCodes.begin(), borrowedCodes.end(), code);
        if (it == borrowedCodes.end()) {
            return false;
        }
        borrowedCodes.erase(it);
        return true;
    }
};

class Library {
    vector<Book> books;
    vector<Member> members;
    int nextMemberId = 1;

    Book *findBook(const string &code) {
        for (auto &book : books) {
            if (book.getCode() == code) {
                return &book;
            }
        }
        return nullptr;
    }

    Member *findMember(int id) {
        for (auto &member : members) {
            if (member.getId() == id) {
                return &member;
            }
        }
        return nullptr;
    }

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

    static string readLine(const string &prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
        } while (value.empty());
        return value;
    }

public:
    void seedData() {
        books.emplace_back("B101", "Programare C++", "Bjarne Stroustrup");
        books.emplace_back("B102", "Structuri de date", "Niculae Tapus");
        books.emplace_back("B103", "Algoritmi practici", "Thomas Cormen");

        members.emplace_back(nextMemberId++, "Andrei Popescu");
        members.emplace_back(nextMemberId++, "Maria Ionescu");
    }

    void addBook() {
        string code = readLine("Cod carte: ");
        if (findBook(code) != nullptr) {
            cout << "Exista deja o carte cu acest cod.\n";
            return;
        }

        string title = readLine("Titlu: ");
        string author = readLine("Autor: ");
        books.emplace_back(code, title, author);
        cout << "Cartea a fost adaugata.\n";
    }

    void addMember() {
        string name = readLine("Nume membru: ");
        members.emplace_back(nextMemberId++, name);
        cout << "Membrul a fost inregistrat cu ID " << members.back().getId() << ".\n";
    }

    void borrowBook() {
        int memberId = readInt("ID membru: ");
        string code = readLine("Cod carte: ");

        Member *member = findMember(memberId);
        Book *book = findBook(code);

        if (member == nullptr) {
            cout << "Membrul nu exista.\n";
            return;
        }
        if (book == nullptr) {
            cout << "Cartea nu exista.\n";
            return;
        }
        if (!book->borrowTo(member->getName())) {
            cout << "Cartea este deja imprumutata.\n";
            return;
        }

        member->addBorrowedBook(code);
        cout << "Imprumut realizat cu succes.\n";
    }

    void returnBook() {
        int memberId = readInt("ID membru: ");
        string code = readLine("Cod carte: ");

        Member *member = findMember(memberId);
        Book *book = findBook(code);

        if (member == nullptr || book == nullptr) {
            cout << "Date invalide pentru retur.\n";
            return;
        }
        if (!book->isBorrowed() || book->getBorrowerName() != member->getName()) {
            cout << "Cartea nu este imprumutata pe acest membru.\n";
            return;
        }

        book->giveBack();
        member->removeBorrowedBook(code);
        cout << "Cartea a fost returnata.\n";
    }

    void listBooks() const {
        if (books.empty()) {
            cout << "Nu exista carti in biblioteca.\n";
            return;
        }

        cout << left << setw(10) << "Cod" << setw(28) << "Titlu" << setw(24) << "Autor" << "Status\n";
        cout << string(80, '-') << '\n';

        for (const auto &book : books) {
            string status = book.isBorrowed() ? "Imprumutata de " + book.getBorrowerName() : "Disponibila";
            cout << left << setw(10) << book.getCode()
                 << setw(28) << book.getTitle()
                 << setw(24) << book.getAuthor()
                 << status << '\n';
        }
    }

    void listMembers() const {
        if (members.empty()) {
            cout << "Nu exista membri inregistrati.\n";
            return;
        }

        for (const auto &member : members) {
            cout << "ID: " << member.getId() << " | Nume: " << member.getName() << " | Carti imprumutate: ";
            if (member.getBorrowedCodes().empty()) {
                cout << "0";
            } else {
                for (size_t i = 0; i < member.getBorrowedCodes().size(); ++i) {
                    cout << member.getBorrowedCodes()[i];
                    if (i + 1 < member.getBorrowedCodes().size()) {
                        cout << ", ";
                    }
                }
            }
            cout << '\n';
        }
    }

    void run() {
        seedData();

        while (true) {
            cout << "\n=== Sistem Gestiune Biblioteca ===\n";
            cout << "1. Afiseaza carti\n";
            cout << "2. Afiseaza membri\n";
            cout << "3. Adauga carte\n";
            cout << "4. Adauga membru\n";
            cout << "5. Imprumuta carte\n";
            cout << "6. Returneaza carte\n";
            cout << "0. Iesire\n";

            int option = readInt("Optiune: ");

            switch (option) {
                case 1:
                    listBooks();
                    break;
                case 2:
                    listMembers();
                    break;
                case 3:
                    addBook();
                    break;
                case 4:
                    addMember();
                    break;
                case 5:
                    borrowBook();
                    break;
                case 6:
                    returnBook();
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
    Library library;
    library.run();
    return 0;
}
