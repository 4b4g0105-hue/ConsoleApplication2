# 📚 OOP 期末專題：C++終端機圖書管理系統 (Library Management System)

[![Language](https://img.shields.io/badge/Language-C%2B%2B14%2F17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

本專案為大學物件導向程式設計（Object-Oriented Programming）的期末專題。系統以 C++ 實作，整合了**類別繼承、STL 容器、檔案序列化 I/O** 以及具備**輸入防呆機制的終端機 UI 互動介面**。

---

## 🚀 核心功能與技術特點

### 1. 物件導向：類別繼承與執行期多型
* **基底類別 `Item`**：定義所有館藏的基礎屬性（如 `id`、`name`），並宣告 `virtual` 虛擬解構子，防止衍生類別發生記憶體洩漏（Memory Leak）。
* **衍生類別 `Book`**：繼承自 `Item`，擴充專屬欄位（`author`、`isbn`），並覆寫（Override）虛擬函式 `display()`。透過基底類別指標呼叫時，可在執行期觸發**動態綁定（Dynamic Binding）**實現多型。

### 2. STL 類別庫與智慧指標應用
* 使用 **`std::vector`** 作為動態資料結構，擺脫傳統陣列大小固定的限制。
* 容器內部結合 **`std::unique_ptr`** 智慧指標（`std::vector<std::unique_ptr<Item>>`），在維持多型特性的同時，落實 RAII 機制，由系統自動管理記憶體生命週期。

### 3. 資料持久化（檔案讀寫與例外處理）
* 系統啟動時，會自動透過 `std::ifstream` 載入 `books.txt` 歷史資料。若為首次執行（檔案不存在），系統會觸發 `try-catch` **例外處理機制**，提示使用者並建立新資料庫，避免程式崩潰。
* 系統關閉時，利用 `dynamic_cast` 安全轉型，透過 `std::ofstream` 將最新的館藏資料序列化為以逗號 `,` 分隔的結構化文字檔案。

### 4. 終端機 UI 互動與輸入防呆
* 設計美觀的命令列選單，透過 `while` 迴圈與 `switch-case` 進行狀態控制。
* 內建**強健的輸入防呆機制**：當使用者在選擇選單時誤輸入文字字串，系統會透過 `std::cin.fail()`、`cin.clear()` 與 `cin.ignore()` 清除緩衝區，引導使用者重新輸入，大幅提升 UX（使用者體驗）。

---

## 🛠️ 開發迭代流程 (Development Iteration)

本專案採敏捷開發思維，共分為三個主要迭代版本：

* **Milestone v0.1 (核心架構奠定)**：
  設計 `Item` 與 `Book` 繼承架構，實作封裝與物件初始化。
* **Milestone v0.2 (數據持久化與容器)**：
  導入 `std::vector` 與智慧指標進行動態記憶體管理，並成功開發 `saveToFile()` 與 `loadFromFile()` 串流讀寫功能。
* **Milestone v0.3 (UI 整合與防呆 - 最終發布版)**：
  建構終端機使用者操作選單，加入完整的輸入驗證機制與例外處理，完成系統全面整合測試。

---

## 📂 檔案結構

```text
.
├── main.cpp         # 包含類別定義、I/O 邏輯與 UI 選單的核心主程式
├── books.txt        # 系統自動生成的資料庫文本檔 (以逗號分隔欄位)
└── README.md        # 專案說明文件
