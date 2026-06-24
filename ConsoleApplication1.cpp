#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <limits> // 用於輸入防呆

// ==================== 類別定義區 ====================

class Item {
protected:
    std::string id;
    std::string name;
public:
    Item(std::string id, std::string name) : id(id), name(name) {}
    virtual ~Item() {}
    virtual void display() const {
        std::cout << "ID: " << id << " | 書名: " << name;
    }
    std::string getId() const { return id; }
    std::string getName() const { return name; }
};

class Book : public Item {
private:
    std::string author;
    std::string isbn;
public:
    Book(std::string id, std::string name, std::string author, std::string isbn)
        : Item(id, name), author(author), isbn(isbn) {
    }
    virtual ~Book() override {}

    virtual void display() const override {
        Item::display();
        std::cout << " | 作者: " << author << " | ISBN: " << isbn;
    }
    // 供寫檔使用的 Getter
    std::string getAuthor() const { return author; }
    std::string getIsbn() const { return isbn; }
};

// ==================== 檔案 I/O 功能區 ====================

void saveToFile(const std::string& filename, const std::vector<std::unique_ptr<Item>>& library) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "❌ 錯誤：無法開啟檔案進行寫入！\n";
        return;
    }
    for (const auto& item_ptr : library) {
        Book* book_ptr = dynamic_cast<Book*>(item_ptr.get());
        if (book_ptr) {
            outFile << book_ptr->getId() << ","
                << book_ptr->getName() << ","
                << book_ptr->getAuthor() << ","
                << book_ptr->getIsbn() << "\n";
        }
    }
    outFile.close();
    std::cout << "💾 資料已成功儲存至 " << filename << "\n";
}

void loadFromFile(const std::string& filename, std::vector<std::unique_ptr<Item>>& library) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cout << "⚠️ 提示：找不到舊資料檔，將在首次儲存時建立新檔案。\n";
        return;
    }
    library.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, name, author, isbn;
        if (std::getline(ss, id, ',') && std::getline(ss, name, ',') &&
            std::getline(ss, author, ',') && std::getline(ss, isbn, ',')) {
            library.push_back(std::make_unique<Book>(id, name, author, isbn));
        }
    }
    inFile.close();
    std::cout << "📂 成功載入歷史書籍資料！\n";
}

// ==================== 主程式 UI 互動介面 ====================

int main() {
    std::vector<std::unique_ptr<Item>> library;
    const std::string DATA_FILE = "books.txt";

    // 系統啟動時先讀檔
    std::cout << "=======================================\n";
    std::cout << "        歡迎使用圖書管理系統 V1.0        \n";
    std::cout << "=======================================\n";
    loadFromFile(DATA_FILE, library);

    int choice = 0;
    bool isRunning = true;

    while (isRunning) {
        // 印出整齊的選單
        std::cout << "\n=======================================\n";
        std::cout << "             主 功 能 選 單            \n";
        std::cout << "---------------------------------------\n";
        std::cout << "  1. 顯示所有書籍\n";
        std::cout << "  2. 新增書籍資料\n";
        std::cout << "  3. 儲存並離開系統\n";
        std::cout << "=======================================\n";
        std::cout << "請輸入選項 (1-3): ";

        std::cin >> choice;

        // 【關鍵防呆】處理使用者亂輸入文字導致 cin 崩潰的問題
        if (std::cin.fail()) {
            std::cin.clear(); // 清除錯誤旗標
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除緩衝區
            std::cout << "❌ 輸入錯誤！請輸入 1 到 3 的數字。\n";
            continue;
        }

        std::cin.ignore(); // 清除輸入數字後的換行符，避免影響後續 getline

        switch (choice) {
        case 1: { // 顯示所有書籍
            std::cout << "\n--- [目前館藏清單] ---\n";
            if (library.empty()) {
                std::cout << "ℹ️ 目前系統中沒有任何書籍資料。\n";
            }
            else {
                int index = 1;
                for (const auto& item_ptr : library) {
                    std::cout << index++ << ". ";
                    item_ptr->display();
                    std::cout << "\n";
                }
            }
            std::cout << "----------------------\n";
            break;
        }
        case 2: { // 新增書籍資料
            std::string id, name, author, isbn;
            std::cout << "\n--- [開始輸入新書資料] ---\n";
            std::cout << "請輸入書籍 ID: ";   std::getline(std::cin, id);
            std::cout << "請輸入書籍名稱: "; std::getline(std::cin, name);
            std::cout << "請輸入書籍作者: "; std::getline(std::cin, author);
            std::cout << "請輸入書籍 ISBN: "; std::getline(std::cin, isbn);

            // 檢查是否輸入空值
            if (id.empty() || name.empty()) {
                std::cout << "❌ 錯誤：ID 與書名不能為空！新增失敗。\n";
            }
            else {
                library.push_back(std::make_unique<Book>(id, name, author, isbn));
                std::cout << "✅ 書籍《" << name << "》已成功加入暫存區。\n";
            }
            std::cout << "--------------------------\n";
            break;
        }
        case 3: { // 儲存並離開系統
            std::cout << "\n正在儲存資料並關閉系統...\n";
            saveToFile(DATA_FILE, library);
            std::cout << "謝謝使用，再見！\n";
            std::cout << "=======================================\n";
            isRunning = false;
            break;
        }
        default:
            std::cout << "❌ 無此選項！請輸入 1, 2 或 3。\n";
            break;
        }
    }

    return 0;
}