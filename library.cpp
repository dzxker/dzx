#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

//枚举类
enum class BookStatus { AVAILABLE, BORROWED };//可以限定枚举值的作用域，避免命名冲突,BookStatus是一个具有两个可能取值的类型，用于表示图书的状态。
enum class BookType {EBOOK,PRINT_BOOK};
//用户基类
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

//User 派生的读者类
class Reader : public User {
public:
    Reader() : User("", "", "") {}
    Reader(string id, string name) : User(id, name, "") {}
    Reader(string id, string name, string type) : User(id, name, type) {}
};

// 学生类
class Student : public Reader {
public:
    Student(string id, string name) : Reader(id, name) {}
};

// 教师类
class Teacher : public Reader {
public:
    Teacher(string id, string name, string teacherID_) : Reader(id, name, "Teacher"), teacherID(teacherID_) {}
    string getTeacherID() const { return teacherID; }
private:
    string teacherID;
};

//User 派生的管理员类
class Admin : public User {
public:
    Admin(string id, string name) : User(id, name, "") {}
};

//书籍类
class Book {
private:
    string bookID;
    string bookName;
    BookStatus status;
    string borrower;
    BookType type; 

public:
    virtual ~Book() {} // 虚析构函数
    Book(string id, string name, BookType bookType) : bookID(id), bookName(name), status(BookStatus::AVAILABLE), borrower(""), type(bookType) {}

    BookType getBookType() const { return type; }
    string getBookID() const { return bookID; }
    string getBookName() const { return bookName; }
    BookStatus getStatus() const { return status; }
    string getBorrower() const { return borrower; }

    void setStatus(BookStatus newStatus) { status = newStatus; }
    void setBorrower(const string& borrowerName) { borrower = borrowerName; }
};

// 电子书类
class EBook : public Book {
public:
    EBook(string id, string name) : Book(id, name, BookType::EBOOK) {}
};

// 纸质书类
class PrintBook : public Book {
public:
    PrintBook(string id, string name) : Book(id, name, BookType::PRINT_BOOK) {}
};

//图书馆系统
class LibrarySystem {
private:
    unordered_map<string, string> userCredentials; // 存储用户名和密码
    unordered_map<string, Reader> readers; // 存储读者信息
    Admin admin; // 管理员信息
    vector<Book> books; // 存储图书

public:
    LibrarySystem() : admin("admin", "AdminName") {
        initializeUserCredentials();
        // 初始化读者信息
        readers["user"] = Reader("user", "UserName");
    }

    void initializeUserCredentials() {
        // 初始化用户名和密码
        userCredentials["students"] = "students";
        userCredentials["teacher"] = "teacherpass";
        userCredentials["admin"] = "adminpass";
    }
        string getUserType(const string &username) {
        if (username == "user") {
            return "学生"; // 假设“user”是学生
        } else if (username == "teacher" || readers.find(username) != readers.end()) {
            return "教师"; // 假设“teacher”是教师
        }
        return ""; // 未知用户
    }

    void loginMenu() {
        while (true) {
            cout << "欢迎进入图书管理系统！" << endl;
            cout << "1. 学生登录" << endl;
            cout << "2. 教师登录" << endl;
            cout << "3. 管理员登录" << endl;
            cout << "4. 退出" << endl;
            cout << "请选择你的操作：";

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
                    cout << "退出系统。" << endl;
                    return;
                default:
                    cout << "错误，请重试。" << endl;
            }
        }
    }

    void studentsLogin() {
        string username, password;
        cout << "请输入用户名：";
        cin >> username;
        cout << "请输入密码：";
        cin >> password;

        if (validateCredentials(username, password)) {
            cout << "学生登录成功！" << endl;
            // 登录成功后的操作，例如调用读者菜单函数
            Reader& reader = readers[username];
            userMenu(reader);
        } else {
            cout << "用户名或密码错误！" << endl;
        }
    }

    void teacherLogin() {
        string username, teacherID, password;
        cout << "请输入用户名：";
        cin >> username;
        cout << "请输入教师ID：";
        cin >> teacherID;
        cout << "请输入密码：";
        cin >> password;

        // 验证用户名和密码
        if (validateCredentials(username, password)) {
            // 验证用户类型
            string userType = getUserType(username);
            if (userType == "教师") {
                // 验证教师ID
                if (validateTeacherID(teacherID)) { // 调用验证教师ID的函数
                cout << "教师登录成功！" << endl;
                Reader& reader = readers[username];
                userMenu(reader);
                return;
            } else {
                cout << "教师ID错误！" << endl;
                return;
            }
            } else {
                cout << "该用户不是教师，无法登录。" << endl;
                return;
            }
        } else {
            cout << "用户名或密码错误！" << endl;
            return;
        }
    }

    void userMenu(const Reader& reader) {
        while (true) {
            cout << "欢迎进入图书管理系统，" << reader.getUserName() << "！" << endl;
            cout << "1. 查找书籍" << endl;
            cout << "2. 借阅书籍" << endl;
            cout << "3. 归还书籍" << endl;
            cout << "4. 返回上一级菜单" << endl;
            cout << "请选择你的操作：";

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
                    cout << "返回上一级菜单。" << endl;
                    return;
                default:
                    cout << "错误，请重试。" << endl;
            }
        }
    }

    void adminLogin() {
        string username, password;
        cout << "请输入管理员用户名：";
        cin >> username;
        cout << "请输入管理员密码：";
        cin >> password;

        if (username == admin.getUserID() && password == userCredentials["admin"]) {
            cout << "管理员登录成功！" << endl;
            adminMenu();
        } else {
            cout << "管理员用户名或密码错误！" << endl;
        }
    }

    void adminMenu() {
    string username;  // 在 switch 语句外部声明
    auto it = readers.end();  // 初始化为 end()

    while (true) {
        cout << "欢迎进入管理员菜单！" << endl;
        cout << "1. 增加学生用户" << endl;
        cout << "2. 增加教师用户" << endl;
        cout << "3. 删除用户" << endl;
        cout << "4. 增加电子书籍" << endl;
        cout << "5. 增加纸质书籍" << endl;
        cout << "6. 删除书籍" << endl;
        cout << "7. 查看用户电子书借阅记录" << endl;
        cout << "8. 查看用户纸质书借阅记录" << endl;
        cout << "9. 返回上一级菜单" << endl;
        cout << "请选择你的操作：";

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
                // 获取用户名
                cout << "请输入用户名：";
                cin >> username;
                // 查找对应的Reader对象
                it = readers.find(username);
                if (it != readers.end()) {
                    // 调用viewUserRecords()并传递Reader对象作为参数
                    viewEBookRecords(it->second);
                } else {
                    cout << "未找到用户：" << username << endl;
                }
                break;
            case 8:
                // 获取用户名
                cout << "请输入用户名：";
                cin >> username;
                // 查找对应的Reader对象
                it = readers.find(username);
                if (it != readers.end()) {
                    // 调用viewUserRecords()并传递Reader对象作为参数
                    viewPrintBookRecords(it->second);
                } else {
                    cout << "未找到用户：" << username << endl;
                }
                break;
            case 9:
                cout << "返回上一级菜单。" << endl;
                return;
            default:
                cout << "错误，请重试。" << endl;
            }
        }
    }

   bool validateTeacherID(const std::string& teacherID) {
    // 假设教师ID必须满足以下条件：
    // 以"T"开头
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
    cout << "请输入要搜索的关键词或书名：";
    cin >> keyword;

    bool found = false;
    for (const Book& book : books) {
        if (book.getBookName().find(keyword) != string::npos) {
            cout << "书名：" << book.getBookName() << "，书号：" << book.getBookID() << endl;
            found = true;
            }
        }

    if (!found) {
        cout << "未找到匹配的书籍。" << endl;
        }
    }

    void borrowBook(const Reader& reader) {
    string bookID;
    cout << "请输入要借阅的书籍编号：";
    cin >> bookID;
    // 首先检查书籍是否存在
    bool bookFound = false;
    for (Book& book : books) {
        if (book.getBookID() == bookID) {
            bookFound = true;
            // 检查书籍是否已经被借出
            if (book.getStatus() == BookStatus::BORROWED) {
                cout << "抱歉，该书籍已经被借出。" << endl;
                return;
            }
            // 根据读者类型决定是否可以借阅电子书
            if (getUserType(reader.getUserName()) == "Teacher" || (getUserType(reader.getUserName()) == "Student" && dynamic_cast<const Student*>(&reader))) {
                // 老师或学生只能借纸质书
                if (dynamic_cast<const EBook*>(&book)) {
                    cout << "抱歉，" << getUserType(reader.getUserName()) << "只能借阅纸质书。" << endl;
                    return;
                }
            }
            // 标记书籍为已借出
            book.setStatus(BookStatus::BORROWED);
            book.setBorrower(reader.getUserName()); // 记录借阅者信息
            cout << "成功借阅书籍：" << book.getBookName() << endl;
            return;
            }
        }
        // 如果书籍不存在
        if (!bookFound) {
            cout << "未找到编号为 " << bookID << " 的书籍。" << endl;
        }
    }

    void returnBook(const Reader& reader) {
        string bookID;
        cout << "请输入要归还的书籍编号：";
        cin >> bookID;

        bool bookFound = false;
        for (Book& book : books) {
            if (book.getBookID() == bookID) {
                bookFound = true;
                // 检查书籍是否已经被借出给指定读者
                if (book.getStatus() == BookStatus::BORROWED && book.getBorrower() == reader.getUserName()) {
                    // 标记书籍为可供借阅
                    book.setStatus(BookStatus::AVAILABLE);
                    book.setBorrower(""); // 清除借阅者信息
                    cout << "成功归还书籍：" << book.getBookName() << endl;
                    return;
                } else {
                    cout << "抱歉，您并未借阅编号为 " << bookID << " 的书籍。" << endl;
                    return;
                }
            }
        }

        if (!bookFound) {
            cout << "未找到编号为 " << bookID << " 的书籍。" << endl;
        }
    }

    void addStudent() {
        string username, password;
        cout << "输入新用户的用户名：";
        cin >> username;
        cout << "输入新用户的密码：";
        cin >> password;

        if (userCredentials.find(username) == userCredentials.end()) {
            userCredentials[username] = password;
            readers[username] = Reader(username, "New User");
            cout << "用户添加成功！" << endl;
        } else {
            cout << "用户名已存在！" << endl;
        }
    }

    void addTeacher() {
        string newTeacherUsername, newTeacherID, newPassword;
        cout << "输入新教师的用户名：";
        cin >> newTeacherUsername;
        cout << "输入新教师的教师ID：";
        cin >> newTeacherID;
        cout << "输入新教师的密码：";
        cin >> newPassword;

        if (userCredentials.find(newTeacherUsername) == userCredentials.end()) {
            userCredentials[newTeacherUsername] = newPassword;
            readers[newTeacherUsername] = Reader(newTeacherUsername, "New Teacher", newTeacherID); // 使用带有教师ID参数的构造函数
            cout << "教师用户添加成功！" << endl;
        } else {
            cout << "用户名已存在！" << endl;
        }
    }

    void deleteUser() {
        string username;
        cout << "输入要删除的用户用户名：";
        cin >> username;

        if (userCredentials.find(username) != userCredentials.end()) {
            userCredentials.erase(username);
            readers.erase(username);
            cout << "用户删除成功！" << endl;
        } else {
            cout << "用户不存在！" << endl;
        }
    }

    void addEBook() {
        string bookID, bookName;
        cout << "输入电子书籍编号（以E开头）：";
        cin >> bookID;
        
        // 验证书籍编号是否以'E'开头
        if (bookID.empty() || bookID[0] != 'E') {
            cout << "电子书籍编号必须以E开头！" << endl;
            return;
        }

        cout << "输入电子书籍名称：";
        cin >> bookName;

        books.push_back(EBook(bookID, bookName));
        cout << "电子书籍添加成功！" << endl;
    }

    void addPrintBook() {
        string bookID, bookName;
        cout << "输入纸质书籍编号（以P开头）：";
        cin >> bookID;
        
        // 验证书籍编号是否以'P'开头
        if (bookID.empty() || bookID[0] != 'P') {
            cout << "纸质书籍编号必须以P开头！" << endl;
            return;
        }

        cout << "输入纸质书籍名称：";
        cin >> bookName;

        books.push_back(PrintBook(bookID, bookName));
        cout << "纸质书籍添加成功！" << endl;
    }

    void deleteBook() {
        string bookID;
        cout << "输入要删除的书籍编号：";
        cin >> bookID;

        auto it = find_if(books.begin(), books.end(), [&](const Book& b) {
            return b.getBookID() == bookID;
        });

        if (it != books.end()) {
            // 使用迭代器指向的对象
            Book& bookToDelete = *it;     
            if (bookToDelete.getBookType() == BookType::EBOOK) {
                // 如果是电子书，则执行删除电子书的操作
                it = books.erase(it); // 更新迭代器
                cout << "电子书删除成功！" << endl;
            } else if (bookToDelete.getBookType() == BookType::PRINT_BOOK) {
                // 如果是纸质书，则执行删除纸质书的操作
                it = books.erase(it); // 更新迭代器
                cout << "纸质书删除成功！" << endl;
            }
        } else {
            cout << "书籍不存在！" << endl;
        }
    }


    void viewEBookRecords(const Reader& reader) {
        cout << "用户 " << reader.getUserName() << " 借阅的电子书记录：" << endl;
        bool hasBorrowedEBooks = false;

        for (const Book& book : books) {
            if (book.getBookType() == BookType::EBOOK && book.getStatus() == BookStatus::BORROWED && book.getBorrower() == reader.getUserName()) {
                cout << "书名：" << book.getBookName() << "，书号：" << book.getBookID() << endl;
                hasBorrowedEBooks = true;
            }
        }

        if (!hasBorrowedEBooks) {
            cout << "用户 " << reader.getUserName() << " 没有借阅电子书。" << endl;
        }
    }

    void viewPrintBookRecords(const Reader& reader) {
        cout << "用户 " << reader.getUserName() << " 借阅的纸质书记录：" << endl;
        bool hasBorrowedPrintBooks = false;

        for (const Book& book : books) {
            if (book.getBookType() == BookType::PRINT_BOOK && book.getStatus() == BookStatus::BORROWED && book.getBorrower() == reader.getUserName()) {
                cout << "书名：" << book.getBookName() << "，书号：" << book.getBookID() << endl;
                hasBorrowedPrintBooks = true;
            }
        }

        if (!hasBorrowedPrintBooks) {
            cout << "用户 " << reader.getUserName() << " 没有借阅纸质书。" << endl;
        }
    }
};

int main() {
    LibrarySystem system;
    system.loginMenu();
    return 0;
}