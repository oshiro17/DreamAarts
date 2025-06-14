# コンパイラとオプション
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# ソースとバイナリ
TARGET = longest_path
SRC = main.cpp

# テストディレクトリ
TEST_DIR = tests
EXPECTED_OUT = expected_output
EXPECTED_ERR = expected_error
TEST_CASES = $(wildcard $(TEST_DIR)/*.txt)

# デフォルトターゲット
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 個別実行：例 → make run FILE=tests/case01_simple.txt
run:
	@echo "Usage: make run FILE=tests/yourfile.txt"
	@./$(TARGET) < $(FILE)

# 全テストケースを実行（目視確認）
test-all: $(TARGET)
	@echo "🔎 Running all test cases..."
	@for file in $(TEST_CASES); do \
		echo "----------------------------------------"; \
		echo "➡️  Test $$file"; \
		echo "--- Input -----------------------------"; \
		cat $$file; \
		echo "--- Output ----------------------------"; \
		./$(TARGET) < $$file; \
		echo "----------------------------------------"; \
		echo ; \
	done


# ビルド成果物を削除
clean:
	rm -f $(TARGET) actual.out actual.err