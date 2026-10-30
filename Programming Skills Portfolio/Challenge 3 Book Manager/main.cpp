#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

class Book {
private:
    std::string title;
    std::string author;
    int pages;
    std::string id;
    bool isBorrowed;

public:
    Book(std::string t, std::string a, int p, std::string i, bool b)
        : title(t), author(a), pages(p), id(i), isBorrowed(b) {}

    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    int getPages() const { return pages; }
    bool getStatus() const { return isBorrowed; }

    void borrowBook() { isBorrowed = true; }
    void returnBook() { isBorrowed = false; }

    void displayDetails() const {
        std::cout << "\n------------------------------------\n";
        std::cout << "ID:        " << id << "\n";
        std::cout << "Title:     " << title << "\n";
        std::cout << "Author:    " << author << "\n";
        std::cout << "Pages:     " << pages << "\n";
        std::cout << "Status:    " << (isBorrowed ? "Borrowed" : "Available") << "\n";
        std::cout << "------------------------------------\n";
    }
};

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << "\n";
        return books;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string title, author, pagesStr, id, statusStr;

        if (std::getline(ss, title, ',') &&
            std::getline(ss, author, ',') &&
            std::getline(ss, pagesStr, ',') &&
            std::getline(ss, id, ',') &&
            std::getline(ss, statusStr)) {
            
            int pages = std::stoi(pagesStr);
            bool isBorrowed = (statusStr == "true" || statusStr == "true\r");
            books.emplace_back(title, author, pages, id, isBorrowed);
        }
    }
    file.close();
    return books;
}

void displayAllBooks(const std::vector<Book>& books) {
    std::cout << "\n" << std::left 
              << std::setw(6)  << "ID"
              << std::setw(30) << "Title"
              << std::setw(22) << "Author"
              << std::setw(8)  << "Pages"
              << std::setw(12) << "Status" << "\n";
    std::cout << std::string(78, '-') << "\n";

    for (const auto& book : books) {
        std::cout << std::left 
                  << std::setw(6)  << book.getId()
                  << std::setw(30) << book.getTitle()
                  << std::setw(22) << book.getAuthor()
                  << std::setw(8)  << book.getPages()
                  << std::setw(12) << (book.getStatus() ? "Borrowed" : "Available") << "\n";
    }
}

int main() {
    std::vector<Book> books = loadBooks("bookData.txt");
    if (books.empty()) {
        std::cerr << "No book data available.\n";
        return 1;
    }

    int choice = 0;
    while (choice != 5) {
        std::cout << "\nLibrary Management Menu\n";
        std::cout << "1. View All Books\n";
        std::cout << "2. View Individual Book\n";
        std::cout << "3. Borrow a Book\n";
        std::cout << "4. Return a Book\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            displayAllBooks(books);
        } else if (choice == 2) {
            std::cout << "Enter Book ID: ";
            std::string id;
            std::cin >> id;
            bool found = false;
            for (const auto& book : books) {
                if (book.getId() == id) {
                    book.displayDetails();
                    found = true;
                    break;
                }
            }
            if (!found) std::cout << "Book with ID " << id << " not found.\n";
        } else if (choice == 3) {
            std::cout << "Enter Book ID to borrow: ";
            std::string id;
            std::cin >> id;
            bool found = false;
            for (auto& book : books) {
                if (book.getId() == id) {
                    found = true;
                    if (book.getStatus()) {
                        std::cout << "Book is already borrowed.\n";
                    } else {
                        book.borrowBook();
                        std::cout << "Successfully borrowed \"" << book.getTitle() << "\".\n";
                    }
                    break;
                }
            }
            if (!found) std::cout << "Book with ID " << id << " not found.\n";
        } else if (choice == 4) {
            std::cout << "Enter Book ID to return: ";
            std::string id;
            std::cin >> id;
            bool found = false;
            for (auto& book : books) {
                if (book.getId() == id) {
                    found = true;
                    if (!book.getStatus()) {
                        std::cout << "Book is already available.\n";
                    } else {
                        book.returnBook();
                        std::cout << "Successfully returned \"" << book.getTitle() << "\".\n";
                    }
                    break;
                }
            }
            if (!found) std::cout << "Book with ID " << id << " not found.\n";
        } else if (choice == 5) {
            std::cout << "Exiting system.\n";
        } else {
            std::cout << "Invalid option. Select 1 to 5.\n";
        }
    }

    return 0;
}