# Make 的用法

```bash
make        # 依預設規則建置（同 'make all'）
make run    # 建置後直接執行 build/app
make debug  # 用 debug 旗標重新建置（-O0 -g3）
make release# 用 release 旗標重新建置（-O3 -DNDEBUG）
make clean  # 刪除 build/ 重新來
```

# Note

```cpp
// 最安全init的方法
typename variable_name {initializer_value};
```

> [!Info] 3 種 initialization
>
> * **Braced initialization**（花括號、列表初始化）：`int x{10};`
> * **Functional initialization**（括號初始化）：`int x(10);`
> * **Assignment initialization**（指派初始化）：`int x = 10;`

---

## 1) Braced initialization：`T var{...};`

又稱 **list-initialization / uniform initialization**。
特色與好處：

* **禁止 narrowing conversion**（縮窄轉換）：例如 `int x{2.9};` 編譯期就報錯，較安全。
* 對於**基本型別**，`int x{};` 會做 **value-initialization ⇒ 變成 0**。
* 對於容器/聚合型別，可用 `{}` 列表初始化多個成員。

```cpp
int a{};       // a == 0（value-init）
int b{10};     // b == 10
// int c{2.9}; // ❌ 編譯期錯誤：narrowing
```

**何時用？**
預設優先考慮。需要避免縮窄轉換、或要直觀的零初始化時最適合。

---

## 2) Functional initialization：`T var(...);`

又稱 **direct-initialization（括號）**。

特色：

* 允許縮窄轉換（不會在編譯期擋下來），例如 `int x(2.9);` 會被截斷成 `2`。
* 對某些型別可能呼叫對應建構子（如 `std::string s(5, 'a')`）。
* 小心 **most vexing parse** 情況（在宣告語境下可能被解析成函式宣告；基本型別較少遇到）。

```cpp
int d(10);   // d == 10
int e(2.9);  // e == 2（允許縮窄）
```

**何時用？**
需要明確調用建構子語義、或已有舊碼風格時可用。否則建議優先 `{}`。

---

## 3) Assignment initialization：`T var = ...;`

又稱 **copy-initialization（指派）**（現代編譯器常做 copy elision/省略拷貝）。

特色：

* 語意直觀，但**同樣允許縮窄轉換**：`int x = 2.9; // 變成 2`
* 搭配 `auto` 時，型別由右側推導（與 `auto x{...}` 的推導規則不同，要特別留意）。

```cpp
int f = 10;     // f == 10
int g = 2.9;    // g == 2（允許縮窄）
```

**何時用？**
可讀性簡單，但若在意型別安全，仍建議 `{}`。

---

## 常見易錯觀念與細節

> [!Warning] 請不要讀取未初始化的變數
> 宣告 `int x;` **未初始化**，其內容**不定且讀取屬於未定義行為（UB）**。請勿印出「垃圾值」。

> [!Warning] `int` 不是「4 bits」！
> `sizeof(int)` 回傳的是 **位元組（bytes）**，而非 bits。
> 在多數 64 位 Linux/Windows 編譯器上，`int` 通常是 **4 bytes = 32 bits**，但 **標準不保證固定大小**。請用 `sizeof(int)` 在你的環境中確認。

---

## `sizeof` 的正確用法

> [!Info] `sizeof(...)` 回傳型別 `size_t`，單位是 **bytes**，在**編譯期**可得（對不求值運算元而言）。

### 兩種寫法

* **型別**：`sizeof(int)`、`sizeof(std::string)`
* **運算元/變數**：`sizeof(x)`、`sizeof(expr)`

### 常用技巧

1. **計算陣列的元素個數**

```cpp
int arr[5]{};
std::size_t n = sizeof(arr) / sizeof(arr[0]); // n == 5
```

2. **字串常值會含終止字元**

```cpp
static_assert(sizeof("abc") == 4, "包含 '\\0'");
```

3. **參考（reference）** 的 `sizeof` 是被參考之**實際型別大小**

```cpp
int x{};
int& rx = x;
static_assert(sizeof(rx) == sizeof(int));
```

4. **結構的 padding**
   `sizeof(struct)` 可能大於成員尺寸總和，因為對齊（alignment）與填充（padding）。

5. **靜態檢查**

```cpp
static_assert(sizeof(int) >= 2, "int 至少 16 bits，但通常是 32 bits");
```

---

## 你的範例程式：重點與安全修正

你原始範例展示非常好（比較三種初始化＋`sizeof`），以下是我做的**安全版**（移除未定義行為、補註解、加入更多 `sizeof` 示範）：

```cpp
// Copyright rutura 2025
#include <iostream>
#include <string>
#include <type_traits>

int main() {
    // 顯示 sizeof(int)（單位：bytes，而非 bits）
    std::cout << "sizeof(int): " << sizeof(int) << " bytes\n";

    // ===== Braced initialization（安全：禁止縮窄） =====
    int zero_int{};         // value-init ⇒ 0
    int dog_int{10};
    int cat_int{15};
    int animal_int{dog_int + cat_int}; // 25
    std::cout << "zero_int: "   << zero_int   << '\n';
    std::cout << "animal_int: " << animal_int << '\n';

    // ===== Functional initialization（允許縮窄） =====
    int func_narrow_int(2.9); // 變 2
    std::cout << "func_narrow_int: " << func_narrow_int << '\n';

    // ===== Assignment initialization（允許縮窄） =====
    int assign_narrow_int = 2.9; // 變 2
    std::cout << "assign_narrow_int: " << assign_narrow_int << '\n';

    // ===== sizeof 示範 =====
    // 對型別
    std::cout << "sizeof(double): " << sizeof(double) << " bytes\n";

    // 對變數/運算元
    std::cout << "sizeof(assign_narrow_int): " << sizeof(assign_narrow_int) << " bytes\n";

    // 陣列元素個數
    int arr[5]{};
    std::cout << "sizeof(arr): " << sizeof(arr) << " bytes\n";
    std::cout << "element count of arr: " << (sizeof(arr) / sizeof(arr[0])) << '\n';

    // 字串常值（包含終止字元 '\0'）
    std::cout << "sizeof(\"abc\"): " << sizeof("abc") << " bytes (包含終止字元) \n";

    // 參考（reference）與原型別尺寸相同
    int& ref = dog_int;
    std::cout << "sizeof(ref): " << sizeof(ref) << " bytes (等於 sizeof(int))\n";

    return 0;
}
```

---

## 快速比較表（什麼時候用哪一個？）

| 需求/情境                       | 建議寫法                            | 原因           |
| --------------------------- | ------------------------------- | ------------ |
| 需要**零初始化**基本型別              | `int x{};`                      | 明確、可讀、保證為 0  |
| 嚴格避免**縮窄轉換**                | `int x{value};`                 | 編譯期擋下不安全的轉換  |
| 舊碼／建構子語義（如 `string(5,'a')`） | `T x(...);`                     | 直觀呼叫建構子      |
| 直觀賦值風格                      | `T x = value;`                  | 可讀，但要留意縮窄    |
| 計算陣列長度                      | `sizeof(arr)/sizeof(arr[0])`    | 編譯期、零成本      |
| 檢查尺寸假設                      | `static_assert(sizeof(T) == N)` | 早期失敗、避免執行期踩雷 |

---

## 補充：你在 code 中的兩個提醒，改成更精準的標示

> [!Info] sizeof()
> `sizeof(int)` 或 `sizeof(var)` 能得知**佔用的 bytes（位元組）**，而不是 bits。結果型別為 `size_t`。

> [!Warning] 關於「int 是 4 bits!」
> 應改為：「**多數**平台上 `int` 為 **4 bytes = 32 bits**，**但標準未保證固定大小**。請以 `sizeof(int)` 為準。」
