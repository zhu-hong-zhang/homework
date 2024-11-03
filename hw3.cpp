#include <iostream>
#include <list>
#include <forward_list>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Book {
    string title;
    string author;
    string bookID;

    // 驗證圖書編號是否符合格式
    static bool isValidBookID(const string& id) {
        return id.size() == 5 && isalpha(id[0]) && isdigit(id[1]) && isdigit(id[2]) && isdigit(id[3]) && isdigit(id[4]);
    }
};

struct Borrower {
    string name;
    vector<string> borrowedBooks;  // 借閱的圖書編號列表
};

// 用來管理圖書的雙向鏈結串列
list<Book> books = {
    {"紅樓夢", "曹雪芹", "A1234"},
    {"西遊記", "吳承恩", "B2345"},
    {"水滸傳", "施耐庵", "B3456"},
    {"三國演義", "羅貫中", "C4567"},
    {"金瓶梅", "蘭陵笑笑生", "C5678"},
    {"聊齋志異", "蒲松齡", "D6789"},
    {"儒林外史", "吳敬梓", "D7890"},
    {"封神演義", "許仲琳", "E8901"},
    {"鏡花緣", "李汝珍", "E9012"},
    {"老殘遊記", "劉鶚", "F0123"}
};

// 用來管理借閱者的單向鏈結串列
forward_list<Borrower> borrowers = {
    {"小明", {"A1234", "B2345"}},
    {"小華", {"C4567"}},
    {"小美", {"D6789", "E8901"}},
    {"小強", {"F0123"}},
    {"小麗", {"B3456", "C5678"}}
};

// 添加新圖書
void addBook() {
    Book newBook;
    cout << "輸入書名: ";
    cin.ignore();
    getline(cin, newBook.title);
    cout << "輸入作者: ";
    getline(cin, newBook.author);

    do {
        cout << "輸入圖書編號 (格式如 A1234): ";
        cin >> newBook.bookID;
        if (!Book::isValidBookID(newBook.bookID)) {
            cout << "圖書編號格式錯誤，請重新輸入。" << endl;
        }
    } while (!Book::isValidBookID(newBook.bookID));

    books.push_back(newBook);
    cout << "新圖書已添加成功。" << endl;
}

// 刪除指定圖書編號的圖書
void deleteBook() {
    string bookID;
    cout << "輸入要刪除的圖書編號: ";
    cin >> bookID;

    auto it = find_if(books.begin(), books.end(), [&](const Book& book) {
        return book.bookID == bookID;
    });

    if (it != books.end()) {
        books.erase(it);
        cout << "圖書已刪除。" << endl;
    } else {
        cout << "未找到該圖書。" << endl;
    }
}

// 搜索指定圖書編號的圖書
void searchBook() {
    string bookID;
    cout << "輸入要查找的圖書編號: ";
    cin >> bookID;

    auto it = find_if(books.begin(), books.end(), [&](const Book& book) {
        return book.bookID == bookID;
    });

    if (it != books.end()) {
        cout << "找到圖書：" << endl;
        cout << "書名: " << it->title << ", 作者: " << it->author << ", 編號: " << it->bookID << endl;
    } else {
        cout << "未找到該圖書。" << endl;
    }
}

// 依照圖書編號排序列出所有圖書
void listBooks() {
    books.sort([](const Book& a, const Book& b) {
        return a.bookID < b.bookID;
    });

    cout << "圖書列表：" << endl;
    for (const auto& book : books) {
        cout << "書名: " << book.title << ", 作者: " << book.author << ", 編號: " << book.bookID << endl;
    }
}

// 添加新借閱者
void addBorrower() {
    Borrower newBorrower;
    cout << "輸入借閱者姓名: ";
    cin.ignore();
    getline(cin, newBorrower.name);

    int num;
    cout << "輸入借閱圖書的數量: ";
    cin >> num;
    for (int i = 0; i < num; ++i) {
        string bookID;
        cout << "輸入圖書編號 (格式如 A1234): ";
        cin >> bookID;
        if (Book::isValidBookID(bookID)) {
            newBorrower.borrowedBooks.push_back(bookID);
        } else {
            cout << "圖書編號格式錯誤，跳過該圖書。" << endl;
        }
    }

    borrowers.push_front(newBorrower);
    cout << "新借閱者已添加成功。" << endl;
}

// 刪除指定姓名的借閱者
void deleteBorrower() {
    string name;
    cout << "輸入要刪除的借閱者姓名: ";
    cin.ignore();
    getline(cin, name);

    borrowers.remove_if([&](const Borrower& borrower) {
        return borrower.name == name;
    });

    cout << "已刪除借閱者（如果存在）。" << endl;
}

// 搜索指定姓名的借閱者
void searchBorrower() {
    string name;
    cout << "輸入要查找的借閱者姓名: ";
    cin.ignore();
    getline(cin, name);

    auto it = find_if(borrowers.begin(), borrowers.end(), [&](const Borrower& borrower) {
        return borrower.name == name;
    });

    if (it != borrowers.end()) {
        cout << "找到借閱者：" << endl;
        cout << "姓名: " << it->name << ", 借閱圖書編號：";
        for (const string& bookID : it->borrowedBooks) {
            cout << bookID << " ";
        }
        cout << endl;
    } else {
        cout << "未找到該借閱者。" << endl;
    }
}

// 列出所有借閱者及其借閱的圖書
void listBorrowers() {
    cout << "借閱者列表：" << endl;
    for (const auto& borrower : borrowers) {
        cout << "姓名: " << borrower.name << ", 借閱圖書編號：";
        for (const string& bookID : borrower.borrowedBooks) {
            cout << bookID << " ";
        }
        cout << endl;
    }
}

// 顯示選單
void displayMenu() {
    cout << "\n====== 圖書館管理系統選單 ======" << endl;
    cout << "1. 添加新圖書" << endl;
    cout << "2. 刪除圖書" << endl;
    cout << "3. 搜索圖書" << endl;
    cout << "4. 列出所有圖書" << endl;
    cout << "5. 添加新借閱者" << endl;
    cout << "6. 刪除借閱者" << endl;
    cout << "7. 搜索借閱者" << endl;
    cout << "8. 列出所有借閱者" << endl;
    cout << "0. 退出系統" << endl;
    cout << "=================================" << endl;
}

int main() {
    int choice;

    do {
        displayMenu();
        cout << "請輸入選項: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: deleteBook(); break;
            case 3: searchBook(); break;
            case 4: listBooks(); break;
            case 5: addBorrower(); break;
            case 6: deleteBorrower(); break;
            case 7: searchBorrower(); break;
            case 8: listBorrowers(); break;
            case 0: cout << "退出系統。" << endl; break;
            default: cout << "無效的選項，請重新輸入。" << endl;
        }
    } while (choice != 0);

    return 0;
}
