#include "BST/AVLTree.h"
#include <algorithm>
#include <iostream>
#include <stack>

int AVLTree::getHeight(TreeNode *node) { return node ? node->height : -1; }
int AVLTree::getBalanceFactor(TreeNode *node) {
  if (!node)
    return 0;
  return getHeight(node->left) - getHeight(node->right);
}

void AVLTree::updateHeight(TreeNode *node) {
  if (node) {
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
  }
}

void AVLTree::rotateLeft(TreeNode *&y) {
  TreeNode *temp = y;
  y = temp->right;
  temp->right = y->left;
  y->left = temp;
  updateHeight(temp);
  updateHeight(y);
}

void AVLTree::rotateRight(TreeNode *&y) {
  TreeNode *temp = y;
  y = temp->left;
  temp->left = y->right;
  y->right = temp;
  updateHeight(temp);
  updateHeight(y);
}

void AVLTree::BalanceAVLTree(std::stack<TreeNode *> &nodes) {
  while (!nodes.empty()) {
    TreeNode *node = nodes.top();
    int factor = getBalanceFactor(node);
    if (factor < -1 || factor > 1) {
      // 不平衡
      if (getHeight(node->left) > getHeight(node->right)) {
        rotateRight(node);
      } else {
        rotateLeft(node);
      }
    }
    nodes.pop();
  }
}
void AVLTree::insertHelp(TreeNode *&root, int val,
                         std::stack<TreeNode *> &nodes) {
  if (root == nullptr) {
    root = new TreeNode(val);
    BalanceAVLTree(nodes);
    return;
  }
  if (val < root->val) {
    nodes.push(root);
    insertHelp(root->left, val, nodes);
  } else if (val > root->val) {
    nodes.push(root);
    insertHelp(root->right, val, nodes);
  } else {
    return;
  }
}

void AVLTree::insert(int val) {
  std::stack<TreeNode *> nodes;
  insertHelp(this->root, val, nodes);
}

void AVLTree::printOrdTreeHelp(TreeNode *root) {
  if (root == nullptr) {
    return;
  }
  printOrdTreeHelp(root->left);
  std::cout << root->val << std::endl;
  printOrdTreeHelp(root->right);
}

void AVLTree::printOrdTree() { printOrdTreeHelp(this->root); }