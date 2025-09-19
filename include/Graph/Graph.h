#include <algorithm>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Node {
  int val;
  int id;
};
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
};

bool DFS(TreeNode *root, int target) {
  if (root == nullptr) {
    return false;
  }
  if (root->val == target) {
    return true;
  }
  return DFS(root->left, target) || DFS(root->right, target);
}

bool BFS(TreeNode *root, int target) {
  if (root == nullptr) {
    return false;
  }
  std::queue<TreeNode *> queue;
  queue.push(root);
  while (!queue.empty()) {
    TreeNode *node = queue.front();
    queue.pop();
    if (node->val == target) {
      return true;
    }
    if (node->left) {
      queue.push(node->left);
    }
    if (node->right)
      queue.push(node->right);
  }
  return false;
}

bool DFS(std::map<int, std::vector<int>> &graph, std::vector<int> &visted,
         int cur, int target) {
  if (std::find(visted.begin(), visted.end(), cur) != visted.end()) {
    return false;
  }
  if (cur == target) {
    return true;
    // base case
  }
  visted.push_back(cur);
  for (const int &node : graph[cur]) {
    if (DFS(graph, visted, node, target)) {
      return true;
    }
  }
  return false;
}

bool BFS(std::map<int, std::vector<int>> &graph, std::vector<int> &visted,
         int start, int target) {
  if (start == target) {
    return true;
  }
  std::queue<int> queue;
  queue.push(start);
  visted.push_back(start);
  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop();
    if (node == target) {
      return true;
    }
    for (int neither : graph[node]) {
      if (std::find(visted.begin(), visted.end(), neither) == visted.end()) {
        queue.push(neither);
        visted.push_back(neither);
      }
    }
  }
  return false;
}

std::vector<int> BFSPath(std::map<int, std::vector<int>> &graph, int start,
                         int target) {
  if (start == target) {
    return {start};
  }
  std::queue<int> queue;
  std::unordered_set<int> visited;
  std::unordered_map<int, int> predecessor;
  queue.push(start);
  visited.insert(start);
  while (!queue.empty()) {
    int node = queue.front();
    queue.pop();
    for (int neighbor : graph[node]) {
      if (visited.find(neighbor) == visited.end()) {
        queue.push(neighbor);
        visited.insert(neighbor);
        predecessor[neighbor] = node;
        // 构建路径
        if (neighbor == target) {
          // 找到了
          std::vector<int> path;
          for (int cur = target; cur != start; cur = predecessor[cur]) {
            path.push_back(cur);
          }
          path.push_back(start);
          std::reverse(path.begin(), path.end());
          return path;
        }
      }
    }
  }
  return {};
}

std::vector<std::vector<int>> BFSPath1(std::map<int, std::vector<int>> &graph,
                                       int start, int target) {
  if (start == target) {
    return {{start}};
  }
  std::queue<int> queue;
  std::unordered_set<int> visited;
  std::vector<std::vector<int>> paths;
  queue.push(start);
  visited.insert(start);
  while (!queue.empty()) {
    std::vector<int> path;
    int size = queue.size();
    for (int i = 0; i < size; i++) {
      int node = queue.front();
      queue.pop();
      path.push_back(node);
      for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
          queue.push(neighbor);
          visited.insert(neighbor);
        }
      }
    }
  }
  return paths;
}

int main() {

}
