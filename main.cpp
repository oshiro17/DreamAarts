#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include <exception>
#include <algorithm>

// -------- 型定義 --------
struct Edge { int to; double dist; };
using Graph = std::unordered_map<int, std::vector<Edge>>;

// -------- グローバル --------
Graph            graph;
std::vector<int> bestPath;
double           bestDist     = 0.0;
constexpr size_t RECUR_LIMIT  = 20000;
bool             hasErrorLine = false;          // ★ エラー行があったか

// -------- 文字列トリム -------- ★
void trim(std::string& s) {
    const char* ws = " \t\r\n";
    s.erase(0, s.find_first_not_of(ws));
    s.erase(s.find_last_not_of(ws) + 1);
}

// -------- ユーティリティ --------
void addEdge(int u, int v, double d) {
    graph[u].push_back({v, d});
    graph[v].push_back({u, d});                 // 無向
}

// -------- DFS（root を保持，閉路対応）--------
void dfs(int cur, int root,
         std::set<int>& vis,
         std::vector<int>& path,
         double dist, size_t depth = 0)
{
    if (depth > RECUR_LIMIT) return;

    vis.insert(cur);
    path.push_back(cur);

    if (dist > bestDist) { bestDist = dist; bestPath = path; }

    bool returnedToRoot = false;
    for (const auto& e : graph[cur]) {
        double nd = dist + e.dist;

        // root へ 1 回だけ戻ってサイクルを評価
        if (e.to == root && path.size() > 1 && !returnedToRoot) {
            returnedToRoot = true;
            if (nd > bestDist) {
                auto cyc = path;
                cyc.push_back(root);
                bestDist = nd;
                bestPath = std::move(cyc);
            }
            continue;
        }
        if (!vis.count(e.to))
            dfs(e.to, root, vis, path, nd, depth + 1);
    }

    vis.erase(cur);
    path.pop_back();
}

// -------- メイン --------
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // -? で負値もマッチ
    const std::regex re(R"(\s*(\d+)\s*,\s*(\d+)\s*,\s*(-?[0-9]*\.?[0-9]+)\s*)");
    std::string line;

    // ---- 入力 ----
    while (std::getline(std::cin, line)) {
        trim(line);                               // ★ 空白だけの行を除去
        if (line.empty()) continue;

        std::smatch m;
        if (!std::regex_match(line, m, re) || m.size() != 4) {
            std::cerr << "不正な入力形式: " << line << '\n';
            hasErrorLine = true;                  // ★
            continue;
        }

        try {
            int    u = std::stoi(m[1].str());
            int    v = std::stoi(m[2].str());
            double d = std::stod(m[3].str());
            if (d < 0.0) {
                std::cerr << "無効な距離（負の値）: " << d
                          << " → 入力行: " << line << '\n';
                hasErrorLine = true;              // ★
                continue;
            }
            addEdge(u, v, d);
        }
        catch (const std::exception& ex) {
            std::cerr << "数値変換エラー: " << ex.what()
                      << " → 入力行: " << line << '\n';
            hasErrorLine = true;                  // ★
        }
    }

    // 入力が完全に空 or 有効エッジが 0
    if (graph.empty()) {
        if (!hasErrorLine) std::cerr << "入力なし\n";   // ★ 二重表示を防ぐ
        return 1;
    }

    // ---- 探索 ----
    for (const auto& [start, _] : graph) {
        std::set<int>   vis;
        std::vector<int> path;
        dfs(start, start, vis, path, 0.0);
    }

    // ---- 向きを辞書順で最小化 ----
    if (!bestPath.empty()) {
        auto rev = std::vector<int>(bestPath.rbegin(), bestPath.rend());
        if (bestPath > rev) std::reverse(bestPath.begin(), bestPath.end());
    }

    // ---- 出力 ----
    for (int id : bestPath) std::cout << id << '\n';
    return 0;
}