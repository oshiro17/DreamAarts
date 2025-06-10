#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <regex>
#include <set>

// --- グラフ構造 ---
std::unordered_map<int, std::vector<std::pair<int, double>>> graph;

// --- 最長経路情報 ---
std::vector<int> longest_path;
double max_distance = 0.0;

// --- DFS探索 ---
void dfs(int current,
         std::set<int>& visited,
         std::vector<int>& path,
         double distance) {
    visited.insert(current);
    path.push_back(current);

    if (distance > max_distance) {
        max_distance = distance;
        longest_path = path;
    }

    auto it = graph.find(current);
    if (it != graph.end()) {
        for (const auto& edge : it->second) {
            int next = edge.first;
            double cost = edge.second;

            if (visited.count(next) == 0) {
                dfs(next, visited, path, distance + cost);
            }
        }
    }

    visited.erase(current);
    path.pop_back();
}

int main() {
    std::string line;
    std::regex line_regex(R"(\s*(\d+)\s*,\s*(\d+)\s*,\s*([0-9]*\.?[0-9]+)\s*)");

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::smatch match;
        if (std::regex_match(line, match, line_regex) && match.size() == 4) {
            try {
                int from = std::stoi(match[1].str());
                int to = std::stoi(match[2].str());
                double distance = std::stod(match[3].str());

                if (distance < 0.0) {
                    std::cerr << "無効な距離（負の値）: " << distance << " → 入力行: " << line << std::endl;
                    continue;
                }

                graph[from].emplace_back(to, distance);
            } catch (const std::exception& e) {
                std::cerr << "数値変換エラー: " << e.what()
                          << " → 入力行: " << line << std::endl;
            }
        } else {
            std::cerr << "不正な入力形式: " << line << std::endl;
        }
    }

    // --- DFS実行 ---
    for (const auto& node : graph) {
        int start = node.first;
        std::set<int> visited;
        std::vector<int> path;
        dfs(start, visited, path, 0.0);
    }

    // --- 結果出力 ---
    if (longest_path.empty()) {
        std::cerr << "有効な経路が見つかりませんでした。" << std::endl;
        return 1;
    }

    for (int station : longest_path) {
        std::cout << station << "\r\n";
    }

    return 0;
}