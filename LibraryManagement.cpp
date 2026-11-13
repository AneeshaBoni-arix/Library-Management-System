#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <memory>
#include <cctype>

using namespace std;

// ==================================================
//                     DATE CLASS
// ==================================================

class Date {
private:
    int day, month, year;

public:
    Date(int d = 1, int m = 1, int y = 2026) {
        day = d;
        month = m;
        year = y;
    }

    int getDay() const {
        return day;
    }

    int getMonth() const {
        return month;
    }

    int getYear() const {
        return year;
    }

    bool isLeapYear() const {
        return (year % 400 == 0 ||
               (year % 4 == 0 && year % 100 != 0));
    }

    int daysInMonth() const {
        int days[] = {31, 28, 31, 30, 31, 30,
                      31, 31, 30, 31, 30, 31};

        if (month == 2 && isLeapYear())
            return 29;

        return days[month - 1];
    }

    bool isValid() const {
        if (year < 1 || month < 1 || month > 12)
            return false;

        if (day < 1 || day > daysInMonth())
            return false;

        return true;
    }

    void addDays(int n) {
        while (n > 0) {
            int remaining = daysInMonth() - day;

            if (n <= remaining) {
                day += n;
                break;
            }

            n -= remaining + 1;
            day = 1;
            month++;

            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }

    long long serial() const {
        int y = year;
        int m = month;

        y -= (m <= 2);

        long long era = y / 400;
        long long yoe = y - era * 400;

        int mp = m + (m > 2 ? -3 : 9);

        long long doy = (153 * mp + 2) / 5 + day - 1;
        long long doe = yoe * 365 + yoe / 4
                      - yoe / 100 + doy;

        return era * 146097 + doe;
    }

    string toString() const {
        stringstream ss;

        ss << setfill('0')
           << setw(2) << day << "-"
           << setw(2) << month << "-"
           << year;

        return ss.str();
    }

    // Reads date in DD-MM-YYYY format

    static bool readDate(Date &date) {
        string input;
        char dash1, dash2, extra;

        getline(cin >> ws, input);

        stringstream ss(input);

        if (!(ss >> date.day >> dash1
                 >> date.month >> dash2 >> date.year))
            return false;

        if (dash1 != '-' || dash2 != '-')
            return false;

        if (ss >> extra)
            return false;

        return date.isValid();
    }
};

// ==================================================
//                     BOOK CLASS
// ==================================================

class Book {
private:
    int bookID;
    string title, author, genre, location;
    int totalCopies, availableCopies;

public:
    Book(int id, string t, string a, string g,
         int copies, string loc) {

        bookID = id;
        title = t;
        author = a;
        genre = g;
        totalCopies = copies;
        availableCopies = copies;
        location = loc;
    }

    // Getter functions

    int getBookID() const {
        return bookID;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    string getGenre() const {
        return genre;
    }

    string getLocation() const {
        return location;
    }

    int getTotalCopies() const {
        return totalCopies;
    }

    int getAvailableCopies() const {
        return availableCopies;
    }

    // Book availability operations

    bool isAvailable() const {
        return availableCopies > 0;
    }

    bool borrowCopy() {
        if (availableCopies <= 0)
            return false;

        availableCopies--;
        return true;
    }

    void returnCopy() {
        if (availableCopies < totalCopies)
            availableCopies++;
    }

    void displayDetails() const {
        cout << "\nBook ID: " << bookID;
        cout << "\nTitle: " << title;
        cout << "\nAuthor: " << author;
        cout << "\nGenre: " << genre;
        cout << "\nAvailable Copies: " << availableCopies;
        cout << "\nShelf Location: " << location;
        cout << "\n-----------------------------\n";
    }

    void displayFullDetails() const {
        displayDetails();
        cout << "Total Copies: " << totalCopies << endl;
    }
};


class Member {
protected:
    string name;
    string memberID;

public:
    Member(string n, string id) {
        name = n;
        memberID = id;
    }

    virtual ~Member() {}

    string getName() const {
        return name;
    }

    string getMemberID() const {
        return memberID;
    }

    // Pure virtual functions: abstraction

    virtual string getType() const = 0;
    virtual int getBorrowLimit() const = 0;
    virtual int getBorrowDays() const = 0;
    virtual int getRenewalLimit() const = 0;
    virtual double getFineRate() const = 0;

    virtual void displayPolicy() const {
        cout << "\nMember Type: " << getType();
        cout << "\nMaximum Books: " << getBorrowLimit();
        cout << "\nBorrowing Period: " << getBorrowDays()
             << " days";
        cout << "\nRenewal Limit: " << getRenewalLimit();
        cout << "\nFine Per Day: Rs." << getFineRate();
        cout << "\n";
    }
};

class Student : public Member {
public:
    Student(string n, string id) : Member(n, id) {}

    string getType() const override {
        return "Student";
    }

    int getBorrowLimit() const override {
        return 2;
    }

    int getBorrowDays() const override {
        return 14;
    }

    int getRenewalLimit() const override {
        return 1;
    }

    double getFineRate() const override {
        return 5.0;
    }
};


class Faculty : public Member {
public:
    Faculty(string n, string id) : Member(n, id) {}

    string getType() const override {
        return "Faculty";
    }

    int getBorrowLimit() const override {
        return 5;
    }

    int getBorrowDays() const override {
        return 30;
    }

    int getRenewalLimit() const override {
        return 2;
    }

    double getFineRate() const override {
        return 2.0;
    }
};

class Staff : public Member {
public:
    Staff(string n, string id) : Member(n, id) {}

    string getType() const override {
        return "Staff";
    }

    int getBorrowLimit() const override {
        return 3;
    }

    int getBorrowDays() const override {
        return 21;
    }

    int getRenewalLimit() const override {
        return 1;
    }

    double getFineRate() const override {
        return 3.0;
    }
};

// ==================================================
//                  BORROWER CLASS
// ==================================================

class Borrower {
private:
    string name, borrowerID, type;
    int bookID;

    Date borrowDate, dueDate, returnDate;

    int renewals;
    bool returned;
    double fine;

public:
    Borrower(string n, string id, string t, int bID,
             Date bDate, Date dDate) {

        name = n;
        borrowerID = id;
        type = t;
        bookID = bID;

        borrowDate = bDate;
        dueDate = dDate;

        renewals = 0;
        returned = false;
        fine = 0;
    }

    string getName() const {
        return name;
    }

    string getBorrowerID() const {
        return borrowerID;
    }

    string getType() const {
        return type;
    }

    int getBookID() const {
        return bookID;
    }

    Date getBorrowDate() const {
        return borrowDate;
    }

    Date getDueDate() const {
        return dueDate;
    }

    Date getReturnDate() const {
        return returnDate;
    }

    int getRenewals() const {
        return renewals;
    }

    bool isReturned() const {
        return returned;
    }

    double getFine() const {
        return fine;
    }

    void renew(int days) {
        dueDate.addDays(days);
        renewals++;
    }

    void completeReturn(Date date, double amount) {
        returnDate = date;
        returned = true;
        fine = amount;
    }
};

// ==================================================
//                 LIBRARY CLASS
// ==================================================

class Library {
private:
    vector<Book> books;
    vector<Borrower> borrowers;

    string adminUsername = "admin";
    string adminPassword = "library123";

    // Convert string to lowercase

    string toLowerCase(string text) const {
        transform(text.begin(), text.end(), text.begin(),
                  [](unsigned char c) {
                      return tolower(c);
                  });

        return text;
    }

    // Create derived Member objects

    unique_ptr<Member> createMember(
        string name, string id, string type) {

        if (type == "Student")
            return unique_ptr<Member>(new Student(name, id));

        if (type == "Faculty")
            return unique_ptr<Member>(new Faculty(name, id));

        if (type == "Staff")
            return unique_ptr<Member>(new Staff(name, id));

        return nullptr;
    }

    // Get member details

    unique_ptr<Member> getMember() {

        string name, id;
        int choice;

        cout << "\nEnter Member Name: ";
        getline(cin >> ws, name);

        cout << "Enter Member ID: ";
        getline(cin, id);

        cout << "\nSelect Member Type:\n";
        cout << "1. Student\n";
        cout << "2. Faculty\n";
        cout << "3. Staff\n";

        cout << "Enter choice: ";
        cin >> choice;

        string type;

        switch (choice) {
            case 1:
                type = "Student";
                break;

            case 2:
                type = "Faculty";
                break;

            case 3:
                type = "Staff";
                break;

            default:
                cout << "Invalid Member Type!\n";
                return nullptr;
        }

        return createMember(name, id, type);
    }

    // Find book by ID

    int findBook(int id) const {
        for (int i = 0; i < (int)books.size(); i++) {
            if (books[i].getBookID() == id)
                return i;
        }

        return -1;
    }

    // Find book by search

    vector<int> searchBooks() {

        vector<int> results;
        int choice;

        cout << "\nSearch By:\n";
        cout << "1. Book ID\n";
        cout << "2. Book Title\n";
        cout << "3. Author Name\n";

        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {

            int id;

            cout << "Enter Book ID: ";
            cin >> id;

            int index = findBook(id);

            if (index != -1)
                results.push_back(index);
        }

        else if (choice == 2 || choice == 3) {

            string keyword;

            if (choice == 2)
                cout << "Enter Book Title: ";
            else
                cout << "Enter Author Name: ";

            getline(cin >> ws, keyword);

            keyword = toLowerCase(keyword);

            for (int i = 0; i < (int)books.size(); i++) {

                string value;

                if (choice == 2)
                    value = toLowerCase(books[i].getTitle());
                else
                    value = toLowerCase(books[i].getAuthor());

                if (value.find(keyword) != string::npos)
                    results.push_back(i);
            }
        }

        else {
            cout << "Invalid Search Option!\n";
        }

        return results;
    }

    // Display search results

    void displayResults(const vector<int> &results) const {

        for (int i : results)
            books[i].displayDetails();
    }

    // Select a book

    int selectBook(const vector<int> &results) {

        if (results.empty()) {
            cout << "\nBook Not Found!\n";
            return -1;
        }

        displayResults(results);

        if (results.size() == 1)
            return results[0];

        int id;

        cout << "\nEnter Book ID to select: ";
        cin >> id;

        for (int i : results) {
            if (books[i].getBookID() == id)
                return i;
        }

        cout << "Invalid Book ID!\n";
        return -1;
    }

    // Find active borrowing record

    int findActiveRecord(string id, int bookID) const {

        for (int i = 0; i < (int)borrowers.size(); i++) {

            if (borrowers[i].getBorrowerID() == id &&
                borrowers[i].getBookID() == bookID &&
                !borrowers[i].isReturned()) {

                return i;
            }
        }

        return -1;
    }

    // Display borrowing policy using polymorphism

    void displayRules() {

        cout << "\n========== BORROWING POLICY ==========\n";

        unique_ptr<Member> student =
            createMember("", "", "Student");

        unique_ptr<Member> faculty =
            createMember("", "", "Faculty");

        unique_ptr<Member> staff =
            createMember("", "", "Staff");

        student->displayPolicy();
        faculty->displayPolicy();
        staff->displayPolicy();

        cout << "======================================\n";
    }

public:

    // Add sample books

    void addSampleBooks() {

        books.push_back(Book(101, "Dune", "Frank Herbert",
        "Science Fiction", 3, "A1"));

        books.push_back(Book(102, "The Martian", "Andy Weir",
        "Science Fiction", 2, "A2"));

        books.push_back(Book(103, "Atomic Habits", "James Clear",
        "Self Development", 3, "B1"));

        books.push_back(Book(104, "The Alchemist", "Paulo Coelho",
        "Fiction", 2, "B2"));

        books.push_back(Book(105, "1984", "George Orwell",
        "Fiction", 3, "B3"));

        books.push_back(Book(106, "C++ Primer", "Stanley B. Lippman",
        "C++ Reference", 2, "C1"));

        books.push_back(Book(107, "The C++ Programming Language",
        "Bjarne Stroustrup", "C++ Reference", 2, "C2"));

        books.push_back(Book(108, "The Republic", "Plato",
        "Philosophy", 2, "D1"));

        books.push_back(Book(109, "Meditations", "Marcus Aurelius",
        "Philosophy", 2, "D2"));

        books.push_back(Book(110, "Sapiens", "Yuval Noah Harari",
        "History", 2, "E1"));
    }

    // Display available books on main menu

    void displayAvailableBooks() const {

        cout << "\n========== AVAILABLE BOOKS ==========\n";

        cout << left
             << setw(12) << "Book ID"
             << setw(35) << "Book Title"
             << "Author Name\n";

        cout << "---------------------------------------------------------------\n";

        bool found = false;

        for (const auto &b : books) {

            if (b.isAvailable()) {

                cout << left
                     << setw(12) << b.getBookID()
                     << setw(35) << b.getTitle()
                     << b.getAuthor() << "\n";

                found = true;
            }
        }

        if (!found)
            cout << "No books are currently available!\n";

        cout << "---------------------------------------------------------------\n";
    }

    // Admin login

    bool adminLogin() {

        string username, password;

        cout << "\n========== ADMIN LOGIN ==========\n";

        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;

        if (username == adminUsername &&
            password == adminPassword) {

            cout << "\nAdmin Login Successful!\n";
            return true;
        }

        cout << "\nIncorrect Username or Password!\n";
        return false;
    }

    // Add new book

    void addBook() {

        int id, copies;
        string title, author, genre, location;

        cout << "\nEnter Book ID: ";
        cin >> id;

        if (findBook(id) != -1) {
            cout << "Book ID Already Exists!\n";
            return;
        }

        cout << "Enter Book Title: ";
        getline(cin >> ws, title);

        cout << "Enter Author: ";
        getline(cin, author);

        cout << "Enter Genre: ";
        getline(cin, genre);

        cout << "Enter Number of Copies: ";
        cin >> copies;

        if (copies <= 0) {
            cout << "Invalid Number of Copies!\n";
            return;
        }

        cout << "Enter Shelf Location: ";
        getline(cin >> ws, location);

        books.push_back(Book(id, title, author,
                             genre, copies, location));

        cout << "\nBook Added Successfully!\n";
    }

    // Display all books

    void displayBooks() const {

        if (books.empty()) {
            cout << "\nNo Books Available!\n";
            return;
        }

        cout << "\n========== BOOK CATALOGUE ==========\n";

        for (const auto &b : books)
            b.displayFullDetails();
    }

    // Locate a book

    void locateBook() {

        vector<int> results = searchBooks();

        if (results.empty()) {
            cout << "\nBook Not Found!\n";
            return;
        }

        cout << "\n========== BOOK DETAILS ==========\n";

        displayResults(results);
    }

    // Discard book

    void discardBook() {

        int id;

        cout << "\nEnter Book ID to discard: ";
        cin >> id;

        int index = findBook(id);

        if (index == -1) {
            cout << "Book Not Found!\n";
            return;
        }

        if (books[index].getAvailableCopies() !=
            books[index].getTotalCopies()) {

            cout << "Cannot Discard! Some Copies Are Borrowed.\n";
            return;
        }

        books.erase(books.begin() + index);

        cout << "\nBook Discarded Successfully!\n";
    }

    // Borrow book

    void borrowBook() {

        unique_ptr<Member> member = getMember();

        if (!member)
            return;

        int activeBooks = 0;

        for (const auto &br : borrowers) {

            if (br.getBorrowerID() == member->getMemberID() &&
                !br.isReturned()) {

                activeBooks++;
            }
        }

        if (activeBooks >= member->getBorrowLimit()) {

            cout << "\nBorrowing Limit Reached!\n";

            cout << "Your Maximum Limit Is "
                 << member->getBorrowLimit() << " Books.\n";

            return;
        }

        vector<int> results = searchBooks();

        int index = selectBook(results);

        if (index == -1)
            return;

        if (!books[index].isAvailable()) {
            cout << "\nBook Is Currently Unavailable!\n";
            return;
        }

        if (findActiveRecord(member->getMemberID(),
                            books[index].getBookID()) != -1) {

            cout << "\nYou Have Already Borrowed This Book!\n";
            return;
        }

        Date borrowDate;

        cout << "\nEnter Borrowing Date (DD-MM-YYYY): ";

        if (!Date::readDate(borrowDate)) {
            cout << "Invalid Date!\n";
            return;
        }

        Date dueDate = borrowDate;

        dueDate.addDays(member->getBorrowDays());

        if (!books[index].borrowCopy()) {
            cout << "Book Is No Longer Available!\n";
            return;
        }

        borrowers.push_back(Borrower(
            member->getName(),
            member->getMemberID(),
            member->getType(),
            books[index].getBookID(),
            borrowDate,
            dueDate
        ));

        cout << "\n========== BORROWING SUCCESSFUL ==========\n";

        cout << "Book: " << books[index].getTitle();
        cout << "\nBorrower: " << member->getName();
        cout << "\nCategory: " << member->getType();

        cout << "\nBorrowing Date: "
             << borrowDate.toString();

        cout << "\nDue Date: "
             << dueDate.toString();

        cout << "\n==========================================\n";
    }

    // Return book

    void returnBook() {

        string id;
        int bookID;

        cout << "\nEnter Borrower ID: ";
        cin >> id;

        cout << "Enter Book ID: ";
        cin >> bookID;

        int recordIndex = findActiveRecord(id, bookID);

        if (recordIndex == -1) {
            cout << "\nActive Borrowing Record Not Found!\n";
            return;
        }

        Date returnDate;

        cout << "Enter Return Date (DD-MM-YYYY): ";

        if (!Date::readDate(returnDate)) {
            cout << "Invalid Date!\n";
            return;
        }

        Borrower &br = borrowers[recordIndex];

        if (returnDate.serial() < br.getBorrowDate().serial()) {
            cout << "Return Date Cannot Be Before Borrowing Date!\n";
            return;
        }

        long long overdueDays =
            returnDate.serial() - br.getDueDate().serial();

        double fine = 0;

        unique_ptr<Member> member =
            createMember(br.getName(), br.getBorrowerID(),
                         br.getType());

        if (overdueDays > 0) {

            fine = overdueDays * member->getFineRate();

            cout << "\nBook Returned Late!\n";
            cout << "Overdue Days: " << overdueDays;
            cout << "\nFine: Rs." << fine;
        }
        else {
            cout << "\nBook Returned On Time!\n";
            cout << "Fine: Rs.0";
        }

        br.completeReturn(returnDate, fine);

        int bookIndex = findBook(bookID);

        if (bookIndex != -1)
            books[bookIndex].returnCopy();

        cout << "\nReturn Date: " << returnDate.toString();
        cout << "\nBook Returned Successfully!\n";
    }

    // Renew book

    void renewBook() {

        string id;
        int bookID;

        cout << "\nEnter Borrower ID: ";
        cin >> id;

        cout << "Enter Book ID: ";
        cin >> bookID;

        int recordIndex = findActiveRecord(id, bookID);

        if (recordIndex == -1) {
            cout << "\nActive Borrowing Record Not Found!\n";
            return;
        }

        Borrower &br = borrowers[recordIndex];

        Date renewalDate;

        cout << "Enter Renewal Date (DD-MM-YYYY): ";

        if (!Date::readDate(renewalDate)) {
            cout << "Invalid Date!\n";
            return;
        }

        if (renewalDate.serial() < br.getBorrowDate().serial()) {
            cout << "Invalid Renewal Date!\n";
            return;
        }

        if (renewalDate.serial() > br.getDueDate().serial()) {
            cout << "Overdue Books Cannot Be Renewed!\n";
            return;
        }

        unique_ptr<Member> member =
            createMember(br.getName(), br.getBorrowerID(),
                         br.getType());

        if (br.getRenewals() >= member->getRenewalLimit()) {
            cout << "Renewal Limit Reached!\n";
            return;
        }

        br.renew(member->getBorrowDays());

        cout << "\nBook Renewed Successfully!\n";

        cout << "New Due Date: "
             << br.getDueDate().toString();

        cout << "\nRenewals Used: "
             << br.getRenewals() << endl;
    }

    // View borrowing records

    void borrowingRecords() const {

        if (borrowers.empty()) {
            cout << "\nNo Borrowing Records Found!\n";
            return;
        }

        cout << "\n========== BORROWING RECORDS ==========\n";

        for (const auto &br : borrowers) {

            cout << "\nBorrower Name: " << br.getName();
            cout << "\nBorrower ID: " << br.getBorrowerID();
            cout << "\nCategory: " << br.getType();
            cout << "\nBook ID: " << br.getBookID();

            cout << "\nBorrowing Date: "
                 << br.getBorrowDate().toString();

            cout << "\nDue Date: "
                 << br.getDueDate().toString();

            cout << "\nRenewals Used: " << br.getRenewals();

            if (br.isReturned()) {

                cout << "\nReturn Date: "
                     << br.getReturnDate().toString();

                cout << "\nStatus: Returned";
                cout << "\nFine: Rs." << br.getFine();
            }
            else {
                cout << "\nStatus: Active";
            }

            cout << "\n---------------------------------------\n";
        }
    }

    // View overdue books

    void overdueBooks() {

        Date today;

        cout << "\nEnter Today's Date (DD-MM-YYYY): ";

        if (!Date::readDate(today)) {
            cout << "Invalid Date!\n";
            return;
        }

        bool found = false;

        cout << "\n========== OVERDUE BOOKS ==========\n";

        for (const auto &br : borrowers) {

            if (!br.isReturned() &&
                today.serial() > br.getDueDate().serial()) {

                long long days =
                    today.serial() - br.getDueDate().serial();

                unique_ptr<Member> member =
                    createMember(br.getName(), br.getBorrowerID(),
                                 br.getType());

                double fine = days * member->getFineRate();

                cout << "\nBorrower: " << br.getName();
                cout << "\nBorrower ID: " << br.getBorrowerID();
                cout << "\nBook ID: " << br.getBookID();

                cout << "\nDue Date: "
                     << br.getDueDate().toString();

                cout << "\nOverdue Days: " << days;
                cout << "\nCurrent Fine: Rs." << fine;

                cout << "\n----------------------------------\n";

                found = true;
            }
        }

        if (!found)
            cout << "\nNo Overdue Books!\n";
    }

    // Check book availability

    void checkAvailability() {

        int id;

        cout << "\nEnter Book ID: ";
        cin >> id;

        int index = findBook(id);

        if (index == -1) {
            cout << "Book Not Found!\n";
            return;
        }

        const Book &b = books[index];

        cout << "\nBook: " << b.getTitle();

        cout << "\nTotal Copies: "
             << b.getTotalCopies();

        cout << "\nBorrowed Copies: "
             << b.getTotalCopies() - b.getAvailableCopies();

        cout << "\nAvailable Copies: "
             << b.getAvailableCopies();

        if (b.isAvailable())
            cout << "\nStatus: Available\n";
        else
            cout << "\nStatus: Not Available\n";
    }

    // View member records

    void memberRecords() {

        string id;

        cout << "\nEnter Member ID: ";
        cin >> id;

        bool found = false;

        for (const auto &br : borrowers) {

            if (br.getBorrowerID() == id) {

                cout << "\nBook ID: " << br.getBookID();
                cout << "\nBorrower: " << br.getName();
                cout << "\nCategory: " << br.getType();

                cout << "\nBorrowing Date: "
                     << br.getBorrowDate().toString();

                cout << "\nDue Date: "
                     << br.getDueDate().toString();

                if (br.isReturned()) {

                    cout << "\nStatus: Returned";
                    cout << "\nFine: Rs." << br.getFine();
                }
                else {
                    cout << "\nStatus: Active";
                }

                cout << "\n-----------------------------\n";

                found = true;
            }
        }

        if (!found)
            cout << "\nNo Records Found!\n";
    }

    // Admin dashboard

    void adminMenu() {

        int choice;

        do {

            cout << "\n\n========== ADMIN DASHBOARD ==========\n";

            cout << "1. Add New Book\n";
            cout << "2. Display All Books\n";
            cout << "3. Locate a Book\n";
            cout << "4. Discard a Book\n";
            cout << "5. View All Borrowing Records\n";
            cout << "6. View Overdue Books\n";
            cout << "7. Check Book Availability\n";
            cout << "8. View Borrowing Policy\n";
            cout << "9. Logout\n";

            cout << "\nEnter Choice: ";
            cin >> choice;

            switch (choice) {

                case 1:
                    addBook();
                    break;

                case 2:
                    displayBooks();
                    break;

                case 3:
                    locateBook();
                    break;

                case 4:
                    discardBook();
                    break;

                case 5:
                    borrowingRecords();
                    break;

                case 6:
                    overdueBooks();
                    break;

                case 7:
                    checkAvailability();
                    break;

                case 8:
                    displayRules();
                    break;

                case 9:
                    cout << "\nAdmin Logged Out!\n";
                    break;

                default:
                    cout << "\nInvalid Choice!\n";
            }

        } while (choice != 9);
    }

    // Member dashboard

    void memberMenu() {

        int choice;

        do {

            cout << "\n\n========== MEMBER DASHBOARD ==========\n";

            cout << "1. Search Books\n";
            cout << "2. Borrow a Book\n";
            cout << "3. Return a Book\n";
            cout << "4. Renew a Book\n";
            cout << "5. View My Records\n";
            cout << "6. Check Book Availability\n";
            cout << "7. View Borrowing Policy\n";
            cout << "8. Exit\n";

            cout << "\nEnter Choice: ";
            cin >> choice;

            switch (choice) {

                case 1:
                    locateBook();
                    break;

                case 2:
                    borrowBook();
                    break;

                case 3:
                    returnBook();
                    break;

                case 4:
                    renewBook();
                    break;

                case 5:
                    memberRecords();
                    break;

                case 6:
                    checkAvailability();
                    break;

                case 7:
                    displayRules();
                    break;

                case 8:
                    cout << "\nLeaving Member Dashboard!\n";
                    break;

                default:
                    cout << "\nInvalid Choice!\n";
            }

        } while (choice != 8);
    }

    // Main menu

    void menu() {

        int choice;

        do {

            // Available books appear automatically

            displayAvailableBooks();

            cout << "\n====================================\n";
            cout << "       LIBRARY MANAGEMENT SYSTEM\n";
            cout << "====================================\n";

            cout << "1. Administrator Login\n";
            cout << "2. Member Access\n";
            cout << "3. Exit\n";

            cout << "\nEnter Choice: ";
            cin >> choice;

            switch (choice) {

                case 1:
                    if (adminLogin())
                        adminMenu();
                    break;

                case 2:
                    memberMenu();
                    break;

                case 3:
                    cout << "\nThank You for Using the Library!\n";
                    break;

                default:
                    cout << "\nInvalid Choice!\n";
            }

        } while (choice != 3);
    }
};

// ==================================================
//                     MAIN FUNCTION
// ==================================================

int main() {

    Library library;

    library.addSampleBooks();

    library.menu();

    return 0;
}
