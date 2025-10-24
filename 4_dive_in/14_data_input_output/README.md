# Make 的用法

```bash
make        # 依預設規則建置（同 'make all'）
make run    # 建置後直接執行 build/app
make debug  # 用 debug 旗標重新建置（-O0 -g3）
make release# 用 release 旗標重新建置（-O3 -DNDEBUG）
make clean  # 刪除 build/ 重新來
```

# Note

## 1) 快速總表（Streams 一覽）

| Stream      | Purpose                                        |
| ----------- | ---------------------------------------------- |
| `std::cout` | 印出一般訊息到終端機（**緩衝**，常與 `cin` 綁在一起；遇到輸入會自動 flush） |
| `std::cin`  | 從終端機讀取輸入                                       |
| `std::cerr` | 印出錯誤訊息（**預設不緩衝**；馬上輸出）                         |
| `std::clog` | 印出日誌訊息（**緩衝**；適合大量、非即時的 log）                   |

> 小知識：`std::endl` 會 **輸出換行並強制 flush**；而 `"\n"` 只是換行不會強刷。如果你不是立刻要看到輸出，偏好 `"\n"` 以提升效能。

---

## 2) `cout` 的換行與刷新（flush）

* `std::cout << "hi" << std::endl;`  // 換行 + **flush**
* `std::cout << "hi\n";`             // 只換行，不 flush
* `std::cout << std::flush;`           // 只 flush，不輸出內容
* `std::cout << std::unitbuf;`         // 之後每次輸出都自動 flush（再次用 `std::nounitbuf` 取消）

> 一般互動式程式建議：**除非需要立即看到輸出**，否則用 `"\n"`；避免過度 flush 造成效能下降。

---

## 3) 兩種輸入方式：`>>` 與 `getline`

* `operator>>`（例如 `std::cin >> x;`）會：

  * 自動略過前導空白（space / tab / `\n`）。
  * 只讀到下一個空白為止（因此**無法**直接讀到含空格的姓名）。
* `std::getline(std::cin, line)` 會：

  * 讀取「整行直到 `\n`」為止（**包含空白**）。
  * 會把結尾的 `\n` **一起吃掉**（因此下一次 `>>` 不需要再清換行）。

---

## 4) 為什麼常常「讀不到」？——殘留換行 & 失敗旗標

### 4.1 殘留換行（上一筆 `>>` 留下的 `\n`）

* 典型情境：

  1. 你先 `std::cin >> name;`（按 Enter）
  2. 接著你馬上 `std::getline(std::cin, fullName);`
  3. `getline` 讀到的會是 **空行**（因為把殘留的 `\n` 當成一行）
* 解法：在 **從 `>>` 轉換到 `getline` 前** 先丟棄該行殘留：

  ```cpp
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  ```

### 4.2 讀取失敗（`failbit`）

* 例如把字母讀到 `int`：`std::cin >> age;`，使用者輸入 `abc`。
* `cin` 會設置 `failbit`，導致之後的讀取一律失敗。
* 解法：

  ```cpp
  std::cin.clear();  // 清除 failbit / badbit
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 丟掉整行殘渣
  ```

> **結論**：`ignore()` 用在「`>> → getline`」或「讀取失敗後」；**不要**在 `getline → >>` 之間再 `ignore()`，否則會把下一行真正要讀的內容沖掉。

---

## 5) 小工具函式（建議直接帶著用）

```cpp
#include <limits>

// 丟掉直到下一個 '\n'（或 EOF）的所有字元
inline void discard_line() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 若發生讀取失敗，清錯誤旗標並丟掉殘留
inline void flush_input_after_failure() {
    if (!std::cin) {
        std::cin.clear();
    }
    discard_line();
}
```

**何時用？**

* `>>` 之後要改用 `getline`：用 `discard_line()`。
* 讀取失敗（如把字串讀給 `int`）：用 `flush_input_after_failure()`。
* `getline` 後接 `>>`：**不要** `discard_line()`（`getline` 已吃掉那行 `\n`）。

---

## 6) 教學範例（你的練習程式，附註解）

```cpp
// Copyright 2025 rutura
#include <iostream>
#include <string>
#include <limits>

void discard_line() {
    // 從輸入緩衝區中丟棄（忽略）直到遇到 \n 為止的所有字元
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void flush_input_after_failure() {
    if (!std::cin) {       // 讀取失敗 (failbit/badbit)
        std::cin.clear();  // 先清錯誤旗標
    }
    discard_line();
}

int main() {
    std::cout << "This is cout" << std::endl;

    int age = {21};
    std::cout << "The age is: " << age << std::endl;

    std::clog << "This is clog" << std::endl;
    std::cerr << "THis is cerr" << std::endl;

    // cin 基礎練習
    std::cout << "=====" << "cint practice" << "=====" << std::endl;

    std::string my_name;
    int my_age;

    // 情境一：用 >> 分開讀（名字不含空格）
    std::cout << "Please enter your name:" << std::endl;
    std::cin >> my_name;   // 讀一個 token（遇空白就停）

    std::cout << "Please enter your age:" << std::endl;
    std::cin >> my_age;    // 若輸入非數字，會 fail
    if (!std::cin) {
        flush_input_after_failure();
        std::cerr << "Age must be a number.\n";
        return 1;
    }

    std::cout << "Your name is: " << my_name
              << ", your age is: " << my_age << std::endl;

    // 情境二：一次 >> 讀兩個（名字不含空格）
    std::cout << "Please enter your name and age:" << std::endl;
    std::cin >> my_name >> my_age;
    if (!std::cin) {
        flush_input_after_failure();
        std::cerr << "Age must be a number.\n";
        return 1;
    }

    std::cout << "Your name is: " << my_name
              << ", your age is: " << my_age << std::endl;

    // 情境三：用 getline 讀「完整姓名」（可能含空白），再讀年齡
    std::cout << "Please enter your FULL name (can contain spaces), then age:" << std::endl;

    // 前一步是 >>，所以先清一次殘留換行
    discard_line();

    std::getline(std::cin, my_name);  // 這裡會把該行的 \n 也吃掉

    std::cin >> my_age;               // 直接讀年齡；若失敗再清
    if (!std::cin) {
        flush_input_after_failure();
        std::cerr << "Age must be a number.\n";
        return 1;
    }

    std::cout << "Your name is: " << my_name
              << ", your age is: " << my_age << std::endl;

    return 0;
}
```

### 互動示例（輸入/輸出）

```
Please enter your name:
aaa
Please enter your age:
123
Your name is: aaa, your age is: 123

Please enter your name and age:
aaa 456
Your name is: aaa, your age is: 456

Please enter your FULL name (can contain spaces), then age:
aaa bbb
5656
Your name is: aaa bbb, your age is: 5656
```

> 注意：在 **getline 之後** 直接 `>>` 讀年齡時，不要再 `discard_line()`，否則像上例中的 `5656` 會被「沖掉」。

---

## 7) 更穩健的輸入樣式（兩種實用模板）

### 7.1 先整行接收，再拆出最後一個「年齡」

> 適合輸入如：`John Ronald Reuel Tolkien 81`

```cpp
std::string line;
std::cout << "Enter FULL name then age (same line):\n";

// 若前一步用過 >>，先清一次換行
discard_line();

std::getline(std::cin, line);

auto pos = line.find_last_of(' ');
if (pos == std::string::npos) {
    std::cerr << "Please provide both name and age.\n";
} else {
    std::string name = line.substr(0, pos);
    std::string age_token = line.substr(pos + 1);
    int age = 0;
    try {
        size_t idx = 0;
        age = std::stoi(age_token, &idx);
        if (idx != age_token.size()) throw std::invalid_argument("junk");
        std::cout << "Your name is: " << name
                  << ", your age is: " << age << '\n';
    } catch (...) {
        std::cerr << "Age must be a valid integer at the end.\n";
    }
}
```

### 7.2 安全讀整數（直到輸入合法為止）

```cpp
int read_int(const char* prompt) {
    int value{};
    for (;;) {
        std::cout << prompt;
        if (std::cin >> value) return value;   // OK
        // 失敗：清錯誤 + 丟當前行
        flush_input_after_failure();
        std::cerr << "Please enter a valid integer.\n";
    }
}
```

---

## 8) 常見陷阱對照表（Cheat Sheet）

| 情境                | 會發生什麼                   | 解法                                                           |
| ----------------- | ----------------------- | ------------------------------------------------------------ |
| `>>` 後接 `getline` | `getline` 讀到空行（殘留 `\n`） | 先 `discard_line()` 再 `getline`                               |
| 把字母讀進 `int`       | 之後所有讀取都失敗               | `std::cin.clear();` + `discard_line()`                       |
| `getline` 後接 `>>` | 不會有殘留 `\n`              | **不要** `discard_line()`                                      |
| 想「清輸入快取」          | `fflush(stdin)`？        | **未定義行為**，不要用；用 `ignore()`                                   |
| 想跳過前導空白           | `getline` 會讀到空白開頭       | 可用 `std::ws` 吃前導空白：`std::getline(std::cin >> std::ws, line)` |

---

## 9) 進階備註（效能與同步）

* `std::ios::sync_with_stdio(false);` 可關閉與 C I/O 的同步，**可能提速**，但需確保不要混用 `printf/scanf` 與 `iostream`。
* `std::cin.tie(nullptr);` 解除 `cin` 與 `cout` 的綁定，避免每次輸入前都自動 flush `cout`，對大量 I/O 可能有幫助。
* `std::cin.sync()` 並非標準化行為，不能指望它清掉緩衝區內容；**不要**拿它取代 `ignore()`。

---

## 10) 小練習（動手做）

1. 讓使用者輸入多行「姓名 + 年齡」，直到輸入空行才結束；輸出年齡平均值與最年長者姓名。
2. 寫一個 `read_int_in_range(min, max)`，持續詢問直到輸入介於 `[min, max]` 的整數為止。
3. 寫一個小工具，能把 `YYYY MM DD` 三個數字用 `>>` 讀入，再用 `getline` 讀完整的備註（備註可能含空白）。

---

### 附：本講義示例來源程式（你提供的版本，已加入註解與安全檢查）

> 已在 §6 中整合展示；你可直接複製該段程式到專案中試跑。

---

**恭喜！** 你已經掌握 iostream 常見坑點與最佳實務。下一步建議：把 §7 的兩個「穩健模板」整合到你的工具庫，之後寫互動式程式會穩很多。
