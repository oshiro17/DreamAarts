# コンパイラとフラグ
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# 実行ファイル名
TARGET = longest_path
SRC = main.cpp

# テストファイル関連
TEST_DIR = tests
TEST_CASES = $(wildcard $(TEST_DIR)/*.txt)

# デフォルトターゲット
all: $(TARGET)

# 実行ファイルのビルド
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 単一テスト実行（1ファイルだけ手動で試したいとき）
run:
	@echo "Usage: make run FILE=tests/case01_simple.txt"
	@./$(TARGET) < $(FILE)

# 全テスト実行
test-all: $(TARGET)
	@echo "🔎 Running all test cases..."
	@for file in $(TEST_CASES); do \
		echo "➡️  Running $$file"; \
		./$(TARGET) < $$file; \
		echo "----------------------------------------"; \
	done

# 実行ファイル削除
clean:
	rm -f $(TARGET)