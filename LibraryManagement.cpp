#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;

// ================= DATE CLASS =================

class Date {
public:
    int day, month, year;

    Date(int d = 1, int m = 1, int y = 2026) {
        day = d;
        month = m;
        year = y;
    }

    bool isLeapYear() {
        return (year % 400 == 0 ||
               (year % 4 == 0 && year % 100 != 0));
    }

    int daysInMonth() {
        int days[] = {31, 28, 31, 30, 31, 30,
                      31, 31, 30, 31, 30, 31};

        if (month == 2 && isLeapYear())
            return 29;

        return days[month - 1];
    }

    bool valid() {
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

    long long serial() {
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

    string toString() {
        stringstream ss;

        ss << setfill('0')
           << setw(2) << day << "-"
           << setw(2) << month << "-"
           << year;

        return ss.str();
    }

    // UPDATED DATE INPUT FUNCTION

    static bool readDate(Date &date) {
        string input;
        char dash1, dash2;
        char extra;

        cout << flush;

        getline(cin >> ws, input);

        stringstream ss(input);

        if (!(ss >> date.day >> dash1
                 >> date.month >> dash2 >> date.year))
            return false;

        if (dash1 != '-' || dash2 != '-')
            return false;

        if (ss >> extra)
            return false;

        return date.valid();
    }
};

// ================= BOOK CLASS =================

class Book {
public:
    int bookID;
    string title, author, genre, location;
    int totalCopies, availableCopies;

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
};

// ================= BORROWER CLASS =================

class Borrower {
public:
    string name, borrowerID, type;

    int bookID;

    Date borrowDate, dueDate, returnDate;

    int renewals;
    bool returned;
    double fine;

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
};

// ================= LIBRARY CLASS =================

class Library {

private:

    vector<Book> books;
    vector<Borrower> borrowers;

    string adminUsername = "admin";
    string adminPassword = "library123";

    // CONVERT STRING TO LOWERCASE

    string toLowerCase(string text) {
        transform(text.begin(), text.end(), text.begin(),
                  [](unsigned char c) {
                      return tolower(c);
                  });

        return text;
    }

    // BORROWING PERIOD

    int getBorrowDays(string type) {
        if (type == "Student")
            return 14;

        if (type == "Faculty")
            return 30;

        if (type == "Staff")
            return 21;

        return 0;
    }

    // BORROWING LIMIT

    int getBorrowLimit(string type) {
        if (type == "Student")
            return 2;

        if (type == "Faculty")
            return 5;

        if (type == "Staff")
            return 3;

        return 0;
    }

    // RENEWAL LIMIT

    int getRenewalLimit(string type) {
        if (type == "Faculty")
            return 2;

        if (type == "Student" || type == "Staff")
            return 1;

        return 0;
    }

    // FINE RATE

    double getFineRate(string type) {
        if (type == "Student")
            return 5;

        if (type == "Faculty")
            return 2;

        if (type == "Staff")
            return 3;

        return 0;
    }

    // FIND BOOK USING ID

    int findBook(int id) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i].bookID == id)
                return i;
        }

        return -1;
    }

    // GET MEMBER DETAILS

    bool getMember(string &name, string &id, string &type) {

        cout << "\nEnter Member Name: ";
        getline(cin >> ws, name);

        cout << "Enter Member ID: ";
        getline(cin, id);

        cout << "\nSelect Member Type:\n";
        cout << "1. Student\n";
        cout << "2. Faculty\n";
        cout << "3. Staff\n";

        cout << "Enter choice: ";

        int choice;
        cin >> choice;

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
                cout << "Invalid member type!\n";
                return false;
        }

        return true;
    }

    // SEARCH BOOKS

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

            if (keyword.empty()) {
                cout << "Search cannot be empty!\n";
                return results;
            }

            for (int i = 0; i < books.size(); i++) {

                string value;

                if (choice == 2)
                    value = toLowerCase(books[i].title);
                else
                    value = toLowerCase(books[i].author);

                if (value.find(keyword) != string::npos)
                    results.push_back(i);
            }
        }

        else {
            cout << "Invalid search option!\n";
        }

        return results;
    }

    // DISPLAY SEARCH RESULTS

    void displayResults(vector<int> results) {

        for (int i : results) {

            cout << "\nBook ID: " << books[i].bookID;
            cout << "\nTitle: " << books[i].title;
            cout << "\nAuthor: " << books[i].author;
            cout << "\nGenre: " << books[i].genre;

            cout << "\nAvailable Copies: "
                 << books[i].availableCopies;

            cout << "\nShelf Location: "
                 << books[i].location;

            cout << "\n-----------------------------\n";
        }
    }

    // SELECT BOOK

    int selectBook(vector<int> results) {

        if (results.empty()) {
            cout << "\nBook not found!\n";
            return -1;
        }

        displayResults(results);

        if (results.size() == 1)
            return results[0];

        int id;

        cout << "\nEnter Book ID to select: ";
        cin >> id;

        for (int i : results) {
            if (books[i].bookID == id)
                return i;
        }

        cout << "Invalid Book ID!\n";

        return -1;
    }

    // BORROWING POLICY

    void displayRules() {

        cout << "\n========== BORROWING POLICY ==========\n";

        cout << "\nSTUDENT";
        cout << "\nMaximum Books: 2";
        cout << "\nBorrowing Period: 14 Days";
        cout << "\nRenewals: 1";
        cout << "\nFine: Rs.5 per day";

        cout << "\n\nFACULTY";
        cout << "\nMaximum Books: 5";
        cout << "\nBorrowing Period: 30 Days";
        cout << "\nRenewals: 2";
        cout << "\nFine: Rs.2 per day";

        cout << "\n\nSTAFF";
        cout << "\nMaximum Books: 3";
        cout << "\nBorrowing Period: 21 Days";
        cout << "\nRenewals: 1";
        cout << "\nFine: Rs.3 per day";

        cout << "\n======================================\n";
    }

public:

    // ADD SAMPLE BOOKS

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

    // DISPLAY AVAILABLE BOOKS IN MAIN MENU

    void displayAvailableBooks() {

        cout << "\n========== AVAILABLE BOOKS ==========\n";

        cout << left
             << setw(12) << "Book ID"
             << setw(35) << "Book Title"
             << "Author Name\n";

        cout << "---------------------------------------------------------------\n";

        bool found = false;

        for (auto &b : books) {

            if (b.availableCopies > 0) {

                cout << left
                     << setw(12) << b.bookID
                     << setw(35) << b.title
                     << b.author << "\n";

                found = true;
            }
        }

        if (!found)
            cout << "No books are currently available!\n";

        cout << "---------------------------------------------------------------\n";
    }

    // ADMIN LOGIN

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

    // ADD NEW BOOK

    void addBook() {

        int id, copies;

        string title, author, genre, location;

        cout << "\nEnter Book ID: ";
        cin >> id;

        if (findBook(id) != -1) {
            cout << "Book ID already exists!\n";
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
            cout << "Invalid number of copies!\n";
            return;
        }

        cout << "Enter Shelf Location: ";
        getline(cin >> ws, location);

        books.push_back(Book(id, title, author,
                             genre, copies, location));

        cout << "\nBook Added Successfully!\n";
    }

    // DISPLAY ALL BOOKS

    void displayBooks() {

        if (books.empty()) {
            cout << "\nNo Books Available!\n";
            return;
        }

        cout << "\n========== BOOK CATALOGUE ==========\n";

        for (auto &b : books) {

            cout << "\nBook ID: " << b.bookID;
            cout << "\nTitle: " << b.title;
            cout << "\nAuthor: " << b.author;
            cout << "\nGenre: " << b.genre;

            cout << "\nTotal Copies: " << b.totalCopies;
            cout << "\nAvailable Copies: " << b.availableCopies;

            cout << "\nShelf Location: " << b.location;

            cout << "\n------------------------------------\n";
        }
    }

    // LOCATE BOOK

    void locateBook() {

        vector<int> results = searchBooks();

        if (results.empty()) {
            cout << "\nBook Not Found!\n";
            return;
        }

        cout << "\n========== BOOK DETAILS ==========\n";

        displayResults(results);
    }

    // DISCARD BOOK

    void discardBook() {

        int id;

        cout << "\nEnter Book ID to discard: ";
        cin >> id;

        int index = findBook(id);

        if (index == -1) {
            cout << "Book Not Found!\n";
            return;
        }

        if (books[index].availableCopies != books[index].totalCopies) {
            cout << "Cannot discard! Some copies are borrowed.\n";
            return;
        }

        books.erase(books.begin() + index);

        cout << "\nBook Discarded Successfully!\n";
    }

    // BORROW BOOK

    void borrowBook() {

        string name, id, type;

        if (!getMember(name, id, type))
            return;

        int activeBooks = 0;

        for (auto &br : borrowers) {

            if (br.borrowerID == id && !br.returned)
                activeBooks++;
        }

        if (activeBooks >= getBorrowLimit(type)) {

            cout << "\nBorrowing Limit Reached!\n";

            cout << "Your maximum limit is "
                 << getBorrowLimit(type) << " books.\n";

            return;
        }

        vector<int> results = searchBooks();

        int index = selectBook(results);

        if (index == -1)
            return;

        if (books[index].availableCopies <= 0) {
            cout << "\nBook is currently unavailable!\n";
            return;
        }

        for (auto &br : borrowers) {

            if (br.borrowerID == id &&
                br.bookID == books[index].bookID &&
                !br.returned) {

                cout << "\nYou have already borrowed this book!\n";
                return;
            }
        }

        Date borrowDate;

        cout << "\nEnter Borrowing Date (DD-MM-YYYY): ";

        if (!Date::readDate(borrowDate)) {
            cout << "Invalid Date!\n";
            return;
        }

        Date dueDate = borrowDate;

        dueDate.addDays(getBorrowDays(type));

        borrowers.push_back(Borrower(
            name, id, type, books[index].bookID,
            borrowDate, dueDate
        ));

        books[index].availableCopies--;

        cout << "\n========== BORROWING SUCCESSFUL ==========\n";

        cout << "Book: " << books[index].title;
        cout << "\nBorrower: " << name;
        cout << "\nCategory: " << type;

        cout << "\nBorrowing Date: "
             << borrowDate.toString();

        cout << "\nDue Date: "
             << dueDate.toString();

        cout << "\n==========================================\n";
    }

    // RETURN BOOK

    void returnBook() {

        string id;
        int bookID;

        cout << "\nEnter Borrower ID: ";
        cin >> id;

        cout << "Enter Book ID: ";
        cin >> bookID;

        for (auto &br : borrowers) {

            if (br.borrowerID == id &&
                br.bookID == bookID &&
                !br.returned) {

                Date returnDate;

                cout << "Enter Return Date (DD-MM-YYYY): ";

                if (!Date::readDate(returnDate)) {
                    cout << "Invalid Date!\n";
                    return;
                }

                if (returnDate.serial() < br.borrowDate.serial()) {
                    cout << "Return date cannot be before borrowing date!\n";
                    return;
                }

                br.returnDate = returnDate;
                br.returned = true;

                long long overdueDays =
                    returnDate.serial() - br.dueDate.serial();

                if (overdueDays > 0) {

                    br.fine = overdueDays * getFineRate(br.type);

                    cout << "\nBook Returned Late!\n";
                    cout << "Overdue Days: " << overdueDays;
                    cout << "\nFine: Rs." << br.fine;
                }
                else {

                    br.fine = 0;

                    cout << "\nBook Returned On Time!\n";
                    cout << "Fine: Rs.0";
                }

                int index = findBook(bookID);

                if (index != -1)
                    books[index].availableCopies++;

                cout << "\nReturn Date: "
                     << returnDate.toString();

                cout << "\nBook Returned Successfully!\n";

                return;
            }
        }

        cout << "\nActive Borrowing Record Not Found!\n";
    }

    // RENEW BOOK

    void renewBook() {

        string id;
        int bookID;

        cout << "\nEnter Borrower ID: ";
        cin >> id;

        cout << "Enter Book ID: ";
        cin >> bookID;

        for (auto &br : borrowers) {

            if (br.borrowerID == id &&
                br.bookID == bookID &&
                !br.returned) {

                Date renewalDate;

                cout << "Enter Renewal Date (DD-MM-YYYY): ";

                if (!Date::readDate(renewalDate)) {
                    cout << "Invalid Date!\n";
                    return;
                }

                if (renewalDate.serial() > br.dueDate.serial()) {
                    cout << "Overdue books cannot be renewed!\n";
                    return;
                }

                if (br.renewals >= getRenewalLimit(br.type)) {
                    cout << "Renewal Limit Reached!\n";
                    return;
                }

                br.dueDate.addDays(getBorrowDays(br.type));

                br.renewals++;

                cout << "\nBook Renewed Successfully!\n";

                cout << "New Due Date: "
                     << br.dueDate.toString();

                cout << "\nRenewals Used: "
                     << br.renewals << endl;

                return;
            }
        }

        cout << "\nActive Borrowing Record Not Found!\n";
    }

    // VIEW BORROWING RECORDS

    void borrowingRecords() {

        if (borrowers.empty()) {
            cout << "\nNo Borrowing Records Found!\n";
            return;
        }

        cout << "\n========== BORROWING RECORDS ==========\n";

        for (auto &br : borrowers) {

            cout << "\nBorrower Name: " << br.name;
            cout << "\nBorrower ID: " << br.borrowerID;
            cout << "\nCategory: " << br.type;
            cout << "\nBook ID: " << br.bookID;

            cout << "\nBorrowing Date: "
                 << br.borrowDate.toString();

            cout << "\nDue Date: "
                 << br.dueDate.toString();

            cout << "\nRenewals Used: " << br.renewals;

            if (br.returned) {

                cout << "\nReturn Date: "
                     << br.returnDate.toString();

                cout << "\nStatus: Returned";
                cout << "\nFine: Rs." << br.fine;
            }
            else {
                cout << "\nStatus: Active";
            }

            cout << "\n---------------------------------------\n";
        }
    }

    // VIEW OVERDUE BOOKS

    void overdueBooks() {

        Date today;

        cout << "\nEnter Today's Date (DD-MM-YYYY): ";

        if (!Date::readDate(today)) {
            cout << "Invalid Date!\n";
            return;
        }

        bool found = false;

        cout << "\n========== OVERDUE BOOKS ==========\n";

        for (auto &br : borrowers) {

            if (!br.returned &&
                today.serial() > br.dueDate.serial()) {

                long long days =
                    today.serial() - br.dueDate.serial();

                double fine = days * getFineRate(br.type);

                cout << "\nBorrower: " << br.name;
                cout << "\nBorrower ID: " << br.borrowerID;
                cout << "\nBook ID: " << br.bookID;

                cout << "\nDue Date: "
                     << br.dueDate.toString();

                cout << "\nOverdue Days: " << days;
                cout << "\nCurrent Fine: Rs." << fine;

                cout << "\n----------------------------------\n";

                found = true;
            }
        }

        if (!found)
            cout << "\nNo Overdue Books!\n";
    }

    // CHECK BOOK AVAILABILITY

    void checkAvailability() {

        int id;

        cout << "\nEnter Book ID: ";
        cin >> id;

        int index = findBook(id);

        if (index == -1) {
            cout << "Book Not Found!\n";
            return;
        }

        Book &b = books[index];

        cout << "\nBook: " << b.title;
        cout << "\nTotal Copies: " << b.totalCopies;

        cout << "\nBorrowed Copies: "
             << b.totalCopies - b.availableCopies;

        cout << "\nAvailable Copies: "
             << b.availableCopies;

        if (b.availableCopies > 0)
            cout << "\nStatus: Available\n";
        else
            cout << "\nStatus: Not Available\n";
    }

    // VIEW MEMBER RECORDS

    void memberRecords() {

        string id;

        cout << "\nEnter Member ID: ";
        cin >> id;

        bool found = false;

        for (auto &br : borrowers) {

            if (br.borrowerID == id) {

                cout << "\nBook ID: " << br.bookID;
                cout << "\nBorrower: " << br.name;
                cout << "\nCategory: " << br.type;

                cout << "\nBorrowing Date: "
                     << br.borrowDate.toString();

                cout << "\nDue Date: "
                     << br.dueDate.toString();

                if (br.returned) {

                    cout << "\nStatus: Returned";
                    cout << "\nFine: Rs." << br.fine;
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

    // ADMIN DASHBOARD

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

    // MEMBER DASHBOARD

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

    // MAIN MENU

    void menu() {

        int choice;

        do {

            // AVAILABLE BOOKS DISPLAY AUTOMATICALLY

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

// ================= MAIN FUNCTION =================

int main() {

    Library lib;

    lib.addSampleBooks();

    lib.menu();

    return 0;
}