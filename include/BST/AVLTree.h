#pragma once
#include <stack>
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  int height;
  TreeNode(int val) : val(val), left(nullptr), right(nullptr), height(0) {}
};

class AVLTree {
private:
  TreeNode *root;

  /**
   * @brief 得到当前节点的高度
   *
   * @param node
   * @return int
   */
  int getHeight(TreeNode *node);
  /**
   * @brief 更新节点的高度
   *
   * @param node
   */
  void updateHeight(TreeNode *node);

  /**
   * @brief 获取节点的平衡因子
   *
   * @param node
   * @return int
   */
  int getBalanceFactor(TreeNode *node);
  /**
   * @brief 左旋
   *
   * @param y
   */
  void rotateLeft(TreeNode *&y);
  /**
   * @brief 右旋
   *
   * @param y
   */
  void rotateRight(TreeNode *&y);

  /**
   * @brief 插入数据
   *
   * @param val
   */
  void insertHelp(TreeNode *&root, int val, std::stack<TreeNode *> &nodes);

  void BalanceAVLTree(std::stack<TreeNode *> &nodes);
  void printOrdTreeHelp(TreeNode* root);
public:
  AVLTree() : root(nullptr) {}
  /**
   * @brief 插入数据
   *
   * @param val
   */
  void insert(int val);

  /**
   * @brief 遍历AVL树测试用
   *
   */
  void printOrdTree();
};