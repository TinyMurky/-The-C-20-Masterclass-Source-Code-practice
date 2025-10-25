# Make 的用法

```bash
make        # 依預設規則建置（同 'make all'）
make run    # 建置後直接執行 build/app
make debug  # 用 debug 旗標重新建置（-O0 -g3）
make release# 用 release 旗標重新建置（-O3 -DNDEBUG）
make clean  # 刪除 build/ 重新來
```

# Note

c++ 表示進位的方法：

```cpp
    int number1 = {15};      // Decimal
    int number2 = {017};     // Octal (開頭是0)
    int number3 = {0xF};     // Hexadecimal
    int number4 = {0b1111};  // binary
```

## 備註：

> [!Info]
> int 10 和 int = {10} 的差別

差別在「初始化的語法」與「是否允許窄化 (narrowing) 轉換」。

* `int x = 10;`
  這是**拷貝初始化 (copy initialization)**。對內建型別就是把右邊的值轉成 `int` 後賦給 `x`。允許發生窄化轉換（通常只給警告，仍可編譯）。

* `int x = {10};` 或 `int x{10};`
  這是 **列表初始化 (list initialization / brace-init)**。它會禁止**窄化轉換**；一旦右邊的值可能丟失資訊，就會是**編譯錯誤**，更安全。

對你給的常數 `10`，兩者結果完全一樣；差異會出現在可能窄化的情況，例如把 `double` 指派給 `int`：

```cpp
double d = 3.14;

int a = d;    // 允許（可能有警告）：會被截斷成 3
int b{d};     // 編譯錯誤：列表初始化禁止窄化
int c = {d};  // 編譯錯誤：同上
```

再補幾個常見延伸點（知道即可）：

* `int x(10);` 是**直接初始化 (direct initialization)**，效果與 `int x = 10;` 對內建型別無實質差異。
* 對**類別型別**，花括號會優先匹配 `std::initializer_list` 建構子，行為可能不同於圓括號／等號。
* 與 `auto` 一起用時要小心：`auto x = {10};` 會得到 `std::initializer_list<int>`，而不是 `int`；`auto x{10};` 在現代標準裡通常也推導為 `std::initializer_list<int>`（不是單純的 `int`）。

### 小結

* 內建型別 + 合法常數 → `= 10` 與 `= {10}`/`{10}` 沒差。
* 需要**防止隱性窄化**（更安全）→ 用花括號初始化。
* 需要與 `auto` 或類別型別互動時 → 注意花括號可能導致不同的建構子或型別推導結果。
