# --- Toolchain ---
CXX ?= clang++           # 指定 C++ 編譯器，預設用 clang++；若外部已用 CXX 指定（如 CXX=g++ make），就用外部值
CXXSTD ?= c++23          # 指定 C++ 標準版本，預設 c++23，同樣可被外部覆蓋
CXXFLAGS ?= -std=$(CXXSTD) -O2 -g -Wall -Wextra -Wpedantic -MMD -MP
# CXXFLAGS：編譯旗標
#  -std=$(CXXSTD)   使用上面設定的 C++ 標準
#  -O2              一般最佳化（release/日常開發常用）
#  -g               產生除錯符號（方便 gdb / lldb）
#  -Wall -Wextra    打開常見與額外警告
#  -Wpedantic       更嚴謹的標準相容性警告
#  -MMD -MP         產生 .d 依賴檔（header 相依），MP 避免缺少目標導致的假警告

# 如果你要加 sanitizer（偵錯時好用），打開下面兩行：
CXXFLAGS += -fsanitize=address,undefined   # 啟用 AddressSanitizer 與 UndefinedBehaviorSanitizer（編譯期）
LDFLAGS  += -fsanitize=address,undefined   # 連結時也要加，確保 sanitizer runtime 正確連結

# --- Project layout ---
APP := build/app        # 最終輸出可執行檔位置（在 build/app）

# 找出所有 .cpp（排除 build/）
SRCS := $(shell find . -type f -name '*.cpp' ! -path './build/*')
# ↑ 用 shell 的 find 掃專案下所有 .cpp（排除 build/），產生來源檔清單

# 把 ./foo/bar.cpp 轉成 build/foo/bar.o
OBJS := $(patsubst ./%.cpp,build/%.o,$(SRCS))
# ↑ 將來源檔路徑對應到目標物件檔路徑（把前綴 ./ 與副檔名 .cpp 換成 build/… .o）

DEPS := $(OBJS:.o=.d)
# ↑ 對每個 .o 對應一個 .d（相依檔），由 -MMD 自動生成，讓 header 變更時能自動重編

# include 路徑（專案根 + 子資料夾都吃得到）
INCLUDES := -I .
# ↑ 把專案根目錄加進 include path，像 #include "math/math.hpp" 才找得到

.PHONY: all run clean debug release
# ↑ 宣告這些目標是「虛擬目標」（不是實體檔案名），避免同名檔案干擾規則

all: $(APP)
# ↑ 預設目標：執行 `make` 就會建出 $(APP)

# 連結
$(APP): $(OBJS)          # 規則：app 依賴所有 .o，任何 .o 改變都會觸發重連結
	@mkdir -p $(dir $@)  # 建立輸出目錄（build/），@ 表示不顯示這行命令
	$(CXX) $(OBJS) $(LDFLAGS) -o $@
	# ↑ 用編譯器（其實是連結器）把所有 .o 連成一個可執行檔 $@（也就是 $(APP)）

# 編譯（自動建立對應目錄 + 生成 .d 相依檔）
build/%.o: ./%.cpp       # 模式規則：任何 ./X.cpp 會對應生出 build/X.o
	@mkdir -p $(dir $@)  # 確保 build/X/ 子目錄存在
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	# $< 是第一個相依（來源 .cpp），$@ 是目標（.o）
	# -c 表示只編譯成物件檔，不連結

# 執行
run: $(APP)              # 先確保 app 存在
	@./$(APP)           # 執行產出的可執行檔

# 偵錯/釋出快捷旗標（可選）
debug:
	@$(MAKE) CXXFLAGS="$(CXXFLAGS) -O0 -g3" all
	# ↑ 重新以 debug 旗標（關閉最佳化 O0、更多除錯資訊 g3）呼叫 make all

release:
	@$(MAKE) CXXFLAGS="$(CXXFLAGS) -O3 -DNDEBUG" all
	# ↑ 重新以 release 旗標（高最佳化 O3、關掉 assert 的 NDEBUG）呼叫 make all

clean:
	@rm -rf build        # 乾淨重來：刪掉整個 build/ 目錄

# 讓 header 更新時自動重編
-include $(DEPS)
# ↑ 嘗試 include 所有 .d 相依檔（前面有 - 表示「檔案不存在也不報錯」）
#    這讓 make 知道：若某 .o 依賴的 .hpp 有變更，就自動重編該 .o
