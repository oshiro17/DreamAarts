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
		echo "➡️  Running $$file"; \
		./$(TARGET) < $$file; \
		echo "----------------------------------------"; \
	done

# 完全一致テスト（expected_*と比較）
test-expected: $(TARGET)
	@echo "🔍 Running full diff test against expected outputs..."
	@for file in $(TEST_CASES); do \
		name=$$(basename $$file); \
		echo "➡️  Testing $$name"; \
		./$(TARGET) < $$file > actual.out 2> actual.err; \
		if [ -f $(EXPECTED_OUT)/$$name ]; then \
			if diff -B -w -q actual.out $(EXPECTED_OUT)/$$name > /dev/null; then \
				echo "✅ OK (stdout)"; \
			else \
				echo "❌ NG (stdout)"; diff -B -w actual.out $(EXPECTED_OUT)/$$name; \
			fi; \
		fi; \
		if [ -f $(EXPECTED_ERR)/$$name ]; then \
			if diff -B -w -q actual.err $(EXPECTED_ERR)/$$name > /dev/null; then \
				echo "✅ OK (stderr)"; \
			else \
				echo "❌ NG (stderr)"; diff -B -w actual.err $(EXPECTED_ERR)/$$name; \
			fi; \
		fi; \
		echo "----------------------------------------"; \
	done
	@rm -f actual.out actual.err

# ビルド成果物を削除
clean:
	rm -f $(TARGET) actual.out actual.err