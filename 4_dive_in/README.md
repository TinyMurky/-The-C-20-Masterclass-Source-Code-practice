# Make 的用法

```bash
make        # 依預設規則建置（同 'make all'）
make run    # 建置後直接執行 build/app
make debug  # 用 debug 旗標重新建置（-O0 -g3）
make release# 用 release 旗標重新建置（-O3 -DNDEBUG）
make clean  # 刪除 build/ 重新來
```

# Note

## 1. First C++：`constexpr` 與 `consteval`（含 `namespace`／`using` 小重點）

下面是一段示範程式碼，展示 **在編譯期就能算出數值** 的兩種做法：`constexpr` 與 `consteval`。

```cpp
// Copyright rutura/The-C-20-Masterclass-Source-Code 2025
#include <iostream>

// What is constexpr: https://tjsw.medium.com/%E6%BD%AE-c-constexpr-ac1bb2bdc5e2
// What is consteval: https://tjsw.medium.com/%E6%BD%AE-c-20-consteval-constexpr-%E7%9A%84%E5%A5%BD%E5%85%84%E5%BC%9F-bfbcfdd4c763
consteval int get_value() {
    return 3;
}

using std::cout;
using std::endl;

int main() {
    // 注意是 expr
    constexpr int value = get_value();
    cout << "Value: " << value << endl;
    return 0;
}
```

---

### 這段程式在做什麼？

* `consteval int get_value()`
  宣告一個 **立即常數求值** 的函式：**只允許在編譯期被呼叫**，呼叫時就必須馬上得出常數結果。這裡回傳固定的 `3`。

* `constexpr int value = get_value();`
  建立一個 **編譯期常數** 變數 `value`，並以 `get_value()` 的結果初始化。因為 `get_value()` 是 `consteval`，保證在編譯期就有值，因此 `value` 也會是編譯期常數。

* `using std::cout; using std::endl;`
  把 `std::cout` 和 `std::endl` 匯入目前作用域，之後就能直接寫 `cout`、`endl`。

* 輸出 `Value: 3`。

---

### 小試身手：如何編譯與執行

```bash
# clang++
clang++ -std=c++20 -O2 -Wall -Wextra -pedantic main.cpp -o main
./main

# g++
g++ -std=gnu++20 -O2 -Wall -Wextra -pedantic main.cpp -o main
./main
```

---

### 逐行重點與常見迷思

1. `consteval` 與 `constexpr` 都和「編譯期求值」有關，但**強度不同**：

   * `consteval`：呼叫**一定**在編譯期發生，否則**編譯錯誤**。
   * `constexpr`：**可以**在編譯期，也**可以**在執行期；視呼叫情境而定。

2. `constexpr int value = get_value();`
   因為 `get_value()` 是 `consteval`，所以 `value` 一定是編譯期常數，能安全用在需要常數表達式（constant **expr**）的地方，例如陣列大小、`switch` 標籤等。

3. 註解裡的「注意是 expr」指的是 **constant expression（常數表達式）** 的「expr」。

---

## 進一步範例：`consteval` 與 `constexpr` 的差別

### A. 把 `consteval` 換成 `constexpr`（**允許**執行期呼叫）

```cpp
constexpr int get_value_ce() { return 3; }

int runtime_n();
int main() {
    // 若右邊是編譯期可得的值，value 會成為編譯期常數
    constexpr int value = get_value_ce(); // OK

    // 但你也可以在執行期用它：
    int x = runtime_n();          // 假設執行期才知道
    int y = get_value_ce() + x;   // OK：這次在執行期呼叫
}
```

### B. 保持 `consteval`，但企圖在執行期呼叫（**錯誤**）

```cpp
consteval int get_value_cv() { return 3; }

int runtime_n();
int main() {
    int x = runtime_n();
    // ❌ 錯：get_value_cv 是 consteval，只能在編譯期呼叫
    int y = get_value_cv() + x;   // 編譯失敗
}
```

> 心法：
>
> * **一定要編譯期** → 用 `consteval`。
> * **優先在編譯期；必要時可在執行期** → 用 `constexpr`。

---

### 常見問題（FAQ）

#### Q1：`const` vs `constexpr` 有什麼不同？

* `const`：值**不能被修改**，但不保證是編譯期常數（可能要到執行期才知道初值）。
* `constexpr`：表示**常數表達式資格**；如果初始化式是編譯期可計算的，那它就是編譯期常數。

#### Q2：什麼時候需要編譯期常數？

* 陣列長度（舊式 C 陣列）、`switch` 的 `case` 標籤、模板參數、`std::array<N>` 的 `N`、`static_assert` 等。

---

### `namespace`（命名空間）

* 用來**避免名稱衝突**，把相關的型別與函式放在同一個邏輯空間。
* 標準函式庫都放在 `std` 命名空間中，所以你會看到 `std::cout`、`std::vector` 等寫法。
* 自訂命名空間：

  ```cpp
  namespace math {
      int add(int a, int b) { return a + b; }
  }
  int main() {
      int s = math::add(1, 2);
  }
  ```

### 使用 `using` 與直接寫 `std::`

* **直接寫 `std::cout`（推薦習慣）**

  * 優點：**清楚**、不易污染名稱、可讀性高（來源一目了然）。
  * 缺點：稍微冗長。
* **`using std::cout; using std::endl;`（局部引入）**

  * 優點：在**限定作用域**內簡潔地使用特定名稱。
  * 缺點：仍有**名稱衝突**風險（但比 `using namespace std;` 小很多）。
* **`using namespace std;`（不建議在標頭或大範圍）**

  * 可能把大量名稱通通帶進來，造成衝突與可讀性下降。
* **實務建議**：

  * 在 `.cpp` 的**小範圍**內，針對少數標準名稱用 `using std::xxx;` 尚可。
  * 在標頭檔或大型專案中，**維持 `std::` 明確前綴**最安全。

### `constexpr` 與 `consteval` 的精準比較

| 特性   | `constexpr`                 | `consteval`            |
| ---- | --------------------------- | ---------------------- |
| 引入版本 | C++11（C++14/17/20 逐步增強）     | C++20                  |
| 求值時機 | **可**在編譯期，也**可**在執行期        | **必須**在編譯期             |
| 適用對象 | 變數、函式、建構函式、成員函式等            | 函式                     |
| 失敗時機 | 在需要常數表達式而做不到時，改為執行期（若上下文允許） | 一旦出現在執行期上下文 → **編譯錯誤** |
| 典型用途 | 提供「**能**編譯期就先算」的彈性          | 宣告「**一定要**編譯期就算完」的保證   |

> 簡記：
>
> * 想給呼叫端**彈性** → `constexpr`。
> * 想對呼叫端**施加保證**（一定編譯期）→ `consteval`。

---

### 總結

* `consteval` 函式**只能**在編譯期呼叫，強制產生常數結果。
* `constexpr` 變數／函式**能**在編譯期運作，但也可在執行期使用，視情境而定。
* 在名稱使用上，**清楚勝於省字**：大型專案建議保留 `std::`，或只在小範圍精準 `using` 個別名稱。
* 熟悉 `namespace` 能讓你的程式碼更有結構、避免撞名。

> 小目標：嘗試把 `get_value()` 改寫為 `constexpr` 版，並各自測試「編譯期使用」與「執行期使用」兩種情境，體會差異！

## 2. Comment