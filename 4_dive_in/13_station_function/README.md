# Make 的用法

```bash
make        # 依預設規則建置（同 'make all'）
make run    # 建置後直接執行 build/app
make debug  # 用 debug 旗標重新建置（-O0 -g3）
make release# 用 release 旗標重新建置（-O3 -DNDEBUG）
make clean  # 刪除 build/ 重新來
```

# Note：Statement、Function 與跨資料夾的程式結構

這篇筆記會帶你了解三個重點：

1. **什麼是 statement（陳述句）與 function（函式）**
2. **如何把程式分開寫在不同檔案與資料夾中**
3. **為什麼不要在 `namespace` 裡使用 `using namespace std;`**

---

## 📂 專案結構

檔案結構如下：

```
src/
├── number/
│   ├── number.cpp
│   └── number.hpp
└── main.cpp
```
---

## 🧩 一、什麼是 statement（陳述句）

在 C++ 中，**statement 是程式中最小的執行單位**。
例如：

```cpp
int a = 3;               // 宣告 + 指派 statement
std::cout << a << "\n";  // 輸出 statement
a += 1;                  // 運算 statement
return 0;                // return statement
```

在 `main()` 中：

```cpp
int first_num = {13};
int second_num = {7};

number::println("first_num", first_num);
```

這三行就是三個 **statements**。

> 👉 每一個 statement 都在控制程式執行的「步驟」。

---

## 🧠 二、什麼是 function（函式）

**Function 是可以重複使用的程式區塊。**
你定義好名稱與參數後，就能在其他地方呼叫它。

以你的程式為例：

### 宣告（在 `.hpp`）

```cpp
namespace number {
int sum(int a, int b);
void println(const std::string& arg, int n);
}
```

### 實作（在 `.cpp`）

```cpp
namespace number {
int sum(int a, int b) {
    return a + b;
}

void println(const std::string& arg, int n) {
    std::cout << arg << ": " << n << std::endl;
}
}
```

### 呼叫（在 `main.cpp`）

```cpp
int sum = number::sum(first_num, second_num);
number::println("sum", sum);
```

> ✅ **觀念**：
>
> * 宣告（declaration）告訴編譯器這個函式存在。
> * 實作（definition）提供實際程式內容。
> * 呼叫（call）是執行它。

---

## 📦 三、如何將函式放在不同資料夾中使用

### 步驟 1：建立 `number.hpp`（標頭檔）

```cpp
// number.hpp
#pragma once
#include <string>

namespace number {
int sum(int a, int b);
void println(const std::string& arg, int n);
}
```

* `#pragma once` 防止重複 include。
* `namespace number` 用來包起來，避免名稱衝突。

---

### 步驟 2：建立 `number.cpp`（實作檔）

```cpp
#include "src/number/number.hpp"
#include <iostream>
#include <string>

namespace number {
int sum(int a, int b) {
    return a + b;
}

void println(const std::string& arg, int n) {
    std::cout << arg << ": " << n << std::endl;
}
}
```

---

### 步驟 3：在主程式中引入

```cpp
#include <iostream>
#include "src/number/number.hpp"

int main() {
    int first_num = {13};
    int second_num = {7};

    number::println("first_num", first_num);
    number::println("second_num", second_num);

    int sum = number::sum(first_num, second_num);
    number::println("sum", sum);
    return 0;
}
```

### 步驟 4：編譯

```bash
clang++ -std=c++20 src/main.cpp src/number/number.cpp -o build/app
```

> 🧠 提示：如果使用 `g++` 或 `clang++`，記得把所有 `.cpp` 一起編譯，因為標頭檔只宣告不包含實作。

---

## ⚙️ 四、為什麼不要在 `namespace` 中用 `using namespace std;`

在程式中：

```cpp
namespace number {
int sum(int a, int b) {
    return a + b;
}

void println(const std::string& arg, int n) {
    std::cout << arg << ": " << n << std::endl;
}
}
```

這裡使用完整寫法 `std::cout`、`std::string` 是**好習慣**。
原因如下：

| 不建議寫法                   | 問題                                                           |
| ----------------------- | ------------------------------------------------------------ |
| `using namespace std;`  | 把整個標準命名空間攤開，可能造成名稱衝突。例如：你自己定義了 `vector`，會和 `std::vector` 混淆。 |
| 放在 `namespace number` 裡 | 所有 `number` 裡的符號都會被污染，讓維護變困難。                                |
| 放在 `.hpp` 檔             | 所有 `#include` 這個標頭的檔案都會被影響（非常危險）。                            |

> ✅ 建議：
>
> * 在 `.cpp` 中若真的需要簡化，可以寫：
>
>   ```cpp
>   using std::cout;
>   using std::endl;
>   ```
>
>   限定使用少數幾個名稱，不污染整個空間。

---

## 🧷 五、`const std::string&` 為什麼要這樣寫？

在 `println` 中：

```cpp
void println(const std::string& arg, int n);
```

這樣寫有三個目的：

| 關鍵字           | 意義                               |
| ------------- | -------------------------------- |
| `const`       | 函式內**不能修改**這個參數的值。               |
| `&`           | 代表「以參考傳遞（by reference）」而非複製整個字串。 |
| `std::string` | 表示字串型別。                          |

> ✅ 結論：
> `const std::string&` 讓程式**更快（不複製記憶體）**又**更安全（不會被改）**。

範例：

```cpp
void println(const std::string& arg, int n) {
    std::cout << arg << ": " << n << "\n";
}
```

呼叫時：

```cpp
number::println("sum", 10);
```

即使傳入 `"sum"` 是臨時字串，仍能安全運作。

---

## 🎯 總結

| 主題                     | 重點                                   |
| ---------------------- | ------------------------------------ |
| **Statement**          | 程式中最小的執行單位（如 `int a=3;`）。            |
| **Function**           | 可重複呼叫的程式區塊，有「宣告、實作、呼叫」。              |
| **多檔案結構**              | `.hpp` 放宣告，`.cpp` 放實作，`main.cpp` 呼叫。 |
| **Namespace**          | 避免名稱衝突，不要用 `using namespace std;`。   |
| **const std::string&** | 高效又安全的傳參寫法。                          |

---

💡 **練習建議：**

1. 在 `number` namespace 新增一個 `multiply()` 函式。
2. 嘗試讓 `println()` 接受 `double` 或 `std::string` 型別。
3. 修改 `main.cpp` 測試新功能。

