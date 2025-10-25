# Make 的用法

```bash
make        # 依預設規則建置（同 'make all'）
make run    # 建置後直接執行 build/app
make debug  # 用 debug 旗標重新建置（-O0 -g3）
make release# 用 release 旗標重新建置（-O3 -DNDEBUG）
make clean  # 刪除 build/ 重新來
```

# Note