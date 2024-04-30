#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

//ö����
enum class BookStatus { AVAILABLE, BORROWED };//�����޶�ö��ֵ�������򣬱���������ͻ,BookStatus��һ��������������ȡֵ�����ͣ����ڱ�ʾͼ���״̬��
enum class BookType {EBOOK,PRINT_BOOK};
//�û�����
class User {
protected:
    string userID;
    string userName;
    string userType;

public:
    User(string id, string name, string type) : userID(id), userName(name) , userType(type){}
    virtual ~User() {}

    string getUserType() const { return userType; }
    string getUserID() const { return userID; }
    string getUserName() const { return userName; }
};

//User �����Ķ�����
class Reader : public User {
public:
    Reader() : User("", "", "") {}
    Reader(string id, string name) : User(id, name, "") {}
    Reader(string id, string name, string type) : User(id, name, type) {}
};

// ѧ����
class Student : public Reader {
public:
    Student(string id, string name) : Reader(id, name) {}
};

// ��ʦ��
class Teacher : public Reader {
public:
    Teacher(string id, string name, string teacherID_) : Reader(id, name, "Teacher"), teacherID(teacherID_) {}
    string getTeacherID() const { return teacherID; }
private:
    string teacherID;
};

//User �����Ĺ���Ա��
class Admin : public User {
public:
    Admin(string id, string name) : User(id, name, "") {}
};

//�鼮��
class Book {
private:
    string bookID;
    string bookName;
    BookStatus status;
    string borrower;
    BookType type; 

public:
    virtual ~Book() {} // ����������
    Book(string id, string name, BookType bookType) : bookID(id), bookName(name), status(BookStatus::AVAILABLE), borrower(""), type(bookType) {}

    BookType getBookType() const { return type; }
    string getBookID() const { return bookID; }
    string getBookName() const { return bookName; }
    BookStatus getStatus() const { return status; }
    string getBorrower() const { return borrower; }

    void setStatus(BookStatus newStatus) { status = newStatus; }
    void setBorrower(const string& borrowerName) { borrower = borrowerName; }
};

// ��������
class EBook : public Book {
public:
    EBook(string id, string name) : Book(id, name, BookType::EBOOK) {}
};

// ֽ������
class PrintBook : public Book {
public:
    PrintBook(string id, string name) : Book(id, name, BookType::PRINT_BOOK) {}
};

//ͼ���ϵͳ
class LibrarySystem {
private:
    unordered_map<string, string> userCredentials; // �洢�û���������
    unordered_map<string, Reader> readers; // �洢������Ϣ
    Admin admin; // ����Ա��Ϣ
    vector<Book> books; // �洢ͼ��

public:
    LibrarySystem() : admin("admin", "AdminName") {
        initializeUserCredentials();
        // ��ʼ��������Ϣ
        readers["user"] = Reader("user", "UserName");
    }

    void initializeUserCredentials() {
        // ��ʼ���û���������
        userCredentials["students"] = "students";
        userCredentials["teacher"] = "teacherpass";
        userCredentials["admin"] = "adminpass";
    }
        string getUserType(const string &username) {
        if (username == "user") {
            return "ѧ��"; // ���衰user����ѧ��
        } else if (username == "teacher" || readers.find(username) != readers.end()) {
            return "��ʦ"; // ���衰teacher���ǽ�ʦ
        }
        return ""; // δ֪�û�
    }

    void loginMenu() {
        while (true) {
            cout << "��ӭ����ͼ�����ϵͳ��" << endl;
            cout << "1. ѧ����¼" << endl;
            cout << "2. ��ʦ��¼" << endl;
            cout << "3. ����Ա��¼" << endl;
            cout << "4. �˳�" << endl;
            cout << "��ѡ����Ĳ�����";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    studentsLogin();
                    break;
                case 2:
                    teacherLogin();
                    break;
                case 3:
                    adminLogin();
                    break;
                case 4:
                    cout << "�˳�ϵͳ��" << endl;
                    return;
                default:
                    cout << "���������ԡ�" << endl;
            }
        }
    }

    void studentsLogin() {
        string username, password;
        cout << "�������û�����";
        cin >> username;
        cout << "���������룺";
        cin >> password;

        if (validateCredentials(username, password)) {
            cout << "ѧ����¼�ɹ���" << endl;
            // ��¼�ɹ���Ĳ�����������ö��߲˵�����
            Reader& reader = readers[username];
            userMenu(reader);
        } else {
            cout << "�û������������" << endl;
        }
    }

    void teacherLogin() {
        string username, teacherID, password;
        cout << "�������û�����";
        cin >> username;
        cout << "�������ʦID��";
        cin >> teacherID;
        cout << "���������룺";
        cin >> password;

        // ��֤�û���������
        if (validateCredentials(username, password)) {
            // ��֤�û�����
            string userType = getUserType(username);
            if (userType == "��ʦ") {
                // ��֤��ʦID
                if (validateTeacherID(teacherID)) { // ������֤��ʦID�ĺ���
                cout << "��ʦ��¼�ɹ���" << endl;
                Reader& reader = readers[username];
                userMenu(reader);
                return;
            } else {
                cout << "��ʦID����" << endl;
                return;
            }
            } else {
                cout << "���û����ǽ�ʦ���޷���¼��" << endl;
                return;
            }
        } else {
            cout << "�û������������" << endl;
            return;
        }
    }

    void userMenu(const Reader& reader) {
        while (true) {
            cout << "��ӭ����ͼ�����ϵͳ��" << reader.getUserName() << "��" << endl;
            cout << "1. �����鼮" << endl;
            cout << "2. �����鼮" << endl;
            cout << "3. �黹�鼮" << endl;
            cout << "4. ������һ���˵�" << endl;
            cout << "��ѡ����Ĳ�����";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    searchBooks();
                    break;
                case 2:
                    borrowBook(reader);
                    break;
                case 3:
                    returnBook(reader);
                    break;
                case 4:
                    cout << "������һ���˵���" << endl;
                    return;
                default:
                    cout << "���������ԡ�" << endl;
            }
        }
    }

    void adminLogin() {
        string username, password;
        cout << "���������Ա�û�����";
        cin >> username;
        cout << "���������Ա���룺";
        cin >> password;

        if (username == admin.getUserID() && password == userCredentials["admin"]) {
            cout << "����Ա��¼�ɹ���" << endl;
            adminMenu();
        } else {
            cout << "����Ա�û������������" << endl;
        }
    }

    void adminMenu() {
    string username;  // �� switch ����ⲿ����
    auto it = readers.end();  // ��ʼ��Ϊ end()

    while (true) {
        cout << "��ӭ�������Ա�˵���" << endl;
        cout << "1. ����ѧ���û�" << endl;
        cout << "2. ���ӽ�ʦ�û�" << endl;
        cout << "3. ɾ���û�" << endl;
        cout << "4. ���ӵ����鼮" << endl;
        cout << "5. ����ֽ���鼮" << endl;
        cout << "6. ɾ���鼮" << endl;
        cout << "7. �鿴�û���������ļ�¼" << endl;
        cout << "8. �鿴�û�ֽ������ļ�¼" << endl;
        cout << "9. ������һ���˵�" << endl;
        cout << "��ѡ����Ĳ�����";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                addTeacher();
                break;    
            case 3:
                deleteUser();
                break;
            case 4:
                addEBook();
                break;
            case 5:
                addPrintBook();
                break;
            case 6:
                deleteBook();
                break;
            case 7:
                // ��ȡ�û���
                cout << "�������û�����";
                cin >> username;
                // ���Ҷ�Ӧ��Reader����
                it = readers.find(username);
                if (it != readers.end()) {
                    // ����viewUserRecords()������Reader������Ϊ����
                    viewEBookRecords(it->second);
                } else {
                    cout << "δ�ҵ��û���" << username << endl;
                }
                break;
            case 8:
                // ��ȡ�û���
                cout << "�������û�����";
                cin >> username;
                // ���Ҷ�Ӧ��Reader����
                it = readers.find(username);
                if (it != readers.end()) {
                    // ����viewUserRecords()������Reader������Ϊ����
                    viewPrintBookRecords(it->second);
                } else {
                    cout << "δ�ҵ��û���" << username << endl;
                }
                break;
            case 9:
                cout << "������һ���˵���" << endl;
                return;
            default:
                cout << "���������ԡ�" << endl;
            }
        }
    }

   bool validateTeacherID(const std::string& teacherID) {
    // �����ʦID������������������
    // ��"T"��ͷ
    if (teacherID[0] != 'T') {
        return false;
    }
        return true;
    }

    bool validateCredentials(const string& username, const string& password) {
        auto it = userCredentials.find(username);
        return it != userCredentials.end() && it->second == password;
    }

    void searchBooks() {
    string keyword;
    cout << "������Ҫ�����Ĺؼ��ʻ�������";
    cin >> keyword;

    bool found = false;
    for (const Book& book : books) {
        if (book.getBookName().find(keyword) != string::npos) {
            cout << "������" << book.getBookName() << "����ţ�" << book.getBookID() << endl;
            found = true;
            }
        }

    if (!found) {
        cout << "δ�ҵ�ƥ����鼮��" << endl;
        }
    }

    void borrowBook(const Reader& reader) {
    string bookID;
    cout << "������Ҫ���ĵ��鼮��ţ�";
    cin >> bookID;
    // ���ȼ���鼮�Ƿ����
    bool bookFound = false;
    for (Book& book : books) {
        if (book.getBookID() == bookID) {
            bookFound = true;
            // ����鼮�Ƿ��Ѿ������
            if (book.getStatus() == BookStatus::BORROWED) {
                cout << "��Ǹ�����鼮�Ѿ��������" << endl;
                return;
            }
            // ���ݶ������;����Ƿ���Խ��ĵ�����
            if (getUserType(reader.getUserName()) == "Teacher" || (getUserType(reader.getUserName()) == "Student" && dynamic_cast<const Student*>(&reader))) {
                // ��ʦ��ѧ��ֻ�ܽ�ֽ����
                if (dynamic_cast<const EBook*>(&book)) {
                    cout << "��Ǹ��" << getUserType(reader.getUserName()) << "ֻ�ܽ���ֽ���顣" << endl;
                    return;
                }
            }
            // ����鼮Ϊ�ѽ��
            book.setStatus(BookStatus::BORROWED);
            book.setBorrower(reader.getUserName()); // ��¼��������Ϣ
            cout << "�ɹ������鼮��" << book.getBookName() << endl;
            return;
            }
        }
        // ����鼮������
        if (!bookFound) {
            cout << "δ�ҵ����Ϊ " << bookID << " ���鼮��" << endl;
        }
    }

    void returnBook(const Reader& reader) {
        string bookID;
        cout << "������Ҫ�黹���鼮��ţ�";
        cin >> bookID;

        bool bookFound = false;
        for (Book& book : books) {
            if (book.getBookID() == bookID) {
                bookFound = true;
                // ����鼮�Ƿ��Ѿ��������ָ������
                if (book.getStatus() == BookStatus::BORROWED && book.getBorrower() == reader.getUserName()) {
                    // ����鼮Ϊ�ɹ�����
                    book.setStatus(BookStatus::AVAILABLE);
                    book.setBorrower(""); // �����������Ϣ
                    cout << "�ɹ��黹�鼮��" << book.getBookName() << endl;
                    return;
                } else {
                    cout << "��Ǹ������δ���ı��Ϊ " << bookID << " ���鼮��" << endl;
                    return;
                }
            }
        }

        if (!bookFound) {
            cout << "δ�ҵ����Ϊ " << bookID << " ���鼮��" << endl;
        }
    }

    void addStudent() {
        string username, password;
        cout << "�������û����û�����";
        cin >> username;
        cout << "�������û������룺";
        cin >> password;

        if (userCredentials.find(username) == userCredentials.end()) {
            userCredentials[username] = password;
            readers[username] = Reader(username, "New User");
            cout << "�û���ӳɹ���" << endl;
        } else {
            cout << "�û����Ѵ��ڣ�" << endl;
        }
    }

    void addTeacher() {
        string newTeacherUsername, newTeacherID, newPassword;
        cout << "�����½�ʦ���û�����";
        cin >> newTeacherUsername;
        cout << "�����½�ʦ�Ľ�ʦID��";
        cin >> newTeacherID;
        cout << "�����½�ʦ�����룺";
        cin >> newPassword;

        if (userCredentials.find(newTeacherUsername) == userCredentials.end()) {
            userCredentials[newTeacherUsername] = newPassword;
            readers[newTeacherUsername] = Reader(newTeacherUsername, "New Teacher", newTeacherID); // ʹ�ô��н�ʦID�����Ĺ��캯��
            cout << "��ʦ�û���ӳɹ���" << endl;
        } else {
            cout << "�û����Ѵ��ڣ�" << endl;
        }
    }

    void deleteUser() {
        string username;
        cout << "����Ҫɾ�����û��û�����";
        cin >> username;

        if (userCredentials.find(username) != userCredentials.end()) {
            userCredentials.erase(username);
            readers.erase(username);
            cout << "�û�ɾ���ɹ���" << endl;
        } else {
            cout << "�û������ڣ�" << endl;
        }
    }

    void addEBook() {
        string bookID, bookName;
        cout << "��������鼮��ţ���E��ͷ����";
        cin >> bookID;
        
        // ��֤�鼮����Ƿ���'E'��ͷ
        if (bookID.empty() || bookID[0] != 'E') {
            cout << "�����鼮��ű�����E��ͷ��" << endl;
            return;
        }

        cout << "��������鼮���ƣ�";
        cin >> bookName;

        books.push_back(EBook(bookID, bookName));
        cout << "�����鼮��ӳɹ���" << endl;
    }

    void addPrintBook() {
        string bookID, bookName;
        cout << "����ֽ���鼮��ţ���P��ͷ����";
        cin >> bookID;
        
        // ��֤�鼮����Ƿ���'P'��ͷ
        if (bookID.empty() || bookID[0] != 'P') {
            cout << "ֽ���鼮��ű�����P��ͷ��" << endl;
            return;
        }

        cout << "����ֽ���鼮���ƣ�";
        cin >> bookName;

        books.push_back(PrintBook(bookID, bookName));
        cout << "ֽ���鼮��ӳɹ���" << endl;
    }

    void deleteBook() {
        string bookID;
        cout << "����Ҫɾ�����鼮��ţ�";
        cin >> bookID;

        auto it = find_if(books.begin(), books.end(), [&](const Book& b) {
            return b.getBookID() == bookID;
        });

        if (it != books.end()) {
            // ʹ�õ�����ָ��Ķ���
            Book& bookToDelete = *it;     
            if (bookToDelete.getBookType() == BookType::EBOOK) {
                // ����ǵ����飬��ִ��ɾ��������Ĳ���
                it = books.erase(it); // ���µ�����
                cout << "������ɾ���ɹ���" << endl;
            } else if (bookToDelete.getBookType() == BookType::PRINT_BOOK) {
                // �����ֽ���飬��ִ��ɾ��ֽ����Ĳ���
                it = books.erase(it); // ���µ�����
                cout << "ֽ����ɾ���ɹ���" << endl;
            }
        } else {
            cout << "�鼮�����ڣ�" << endl;
        }
    }


    void viewEBookRecords(const Reader& reader) {
        cout << "�û� " << reader.getUserName() << " ���ĵĵ������¼��" << endl;
        bool hasBorrowedEBooks = false;

        for (const Book& book : books) {
            if (book.getBookType() == BookType::EBOOK && book.getStatus() == BookStatus::BORROWED && book.getBorrower() == reader.getUserName()) {
                cout << "������" << book.getBookName() << "����ţ�" << book.getBookID() << endl;
                hasBorrowedEBooks = true;
            }
        }

        if (!hasBorrowedEBooks) {
            cout << "�û� " << reader.getUserName() << " û�н��ĵ����顣" << endl;
        }
    }

    void viewPrintBookRecords(const Reader& reader) {
        cout << "�û� " << reader.getUserName() << " ���ĵ�ֽ�����¼��" << endl;
        bool hasBorrowedPrintBooks = false;

        for (const Book& book : books) {
            if (book.getBookType() == BookType::PRINT_BOOK && book.getStatus() == BookStatus::BORROWED && book.getBorrower() == reader.getUserName()) {
                cout << "������" << book.getBookName() << "����ţ�" << book.getBookID() << endl;
                hasBorrowedPrintBooks = true;
            }
        }

        if (!hasBorrowedPrintBooks) {
            cout << "�û� " << reader.getUserName() << " û�н���ֽ���顣" << endl;
        }
    }
};

int main() {
    LibrarySystem system;
    system.loginMenu();
    return 0;
}