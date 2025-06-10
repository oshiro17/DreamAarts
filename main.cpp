#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <regex>

int main() {
    std::unordered_map<int, std::vector<std::pair<int, double>>> graph;

    std::string line;
    // カンマ区切りで空白が混じってもOKな正規表現
    std::regex line_regex(R"(\s*(\d+)\s*,\s*(\d+)\s*,\s*([0-9]*\.?[0-9]+)\s*)");

    while (std::getline(std::cin, line)) {
        // 空行をスキップ
        if (line.empty()) continue;

        std::smatch match;
        // 入力形式チェック & 正規表現マッチの数チェック
        if (std::regex_match(line, match, line_regex) && match.size() == 4) {
            try {
                int from = std::stoi(match[1].str());
                int to = std::stoi(match[2].str());
                double distance = std::stod(match[3].str());

                graph[from].emplace_back(to, distance);
            } catch (const std::exception& e) {
                std::cerr << "数値変換エラー: " << e.what()
                          << " → 入力行: " << line << std::endl;
            }
        } else {
            std::cerr << "不正な入力形式: " << line << std::endl;
        }
    }

    // --- 確認出力 ---
    for (const auto& node : graph) {
        int from = node.first;
        const auto& edges = node.second;
        for (const auto& edge : edges) {
            int to = edge.first;
            double dist = edge.second;
            std::cout << "from " << from << " to " << to << " dist " << dist << std::endl;
        }
    }

    return 0;
}