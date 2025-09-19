#include "BST/set.h"
#include <iostream>

void MySet::insertHelp(TreeNode *&root, int val) {
  if (root == nullptr) {
    root = new TreeNode(val);
    return;
  }
  if (val < root->val) {
    insertHelp(root->left, val);
  } else if (val > root->val) {
    insertHelp(root->right, val);
  } else {
    return;
  }
}

void MySet::insert(int val) { insertHelp(this->root, val); }

void MySet::printHelp(TreeNode *root) {
  if (root == nullptr) {
    return;
  }
  std::cout << root->val << std::endl;
  printHelp(root->left);
  printHelp(root->right);
}

void MySet::print() { printHelp(root); }
bool MySet::findHelp(TreeNode *root, int val) {
  if (root == nullptr) {
    return false;
  }
  if (val == root->val) {
    return true;
  } else if (val < root->val) {
    return findHelp(root->left, val);
  } else {
    return findHelp(root->right, val);
  }
}

bool MySet::find(int val) { return findHelp(this->root, val); }

void MySet::FindPredecessor(TreeNode *node, TreeNode *&prev) {
  if (node->right == nullptr) {
    prev = node;
    return;
  }
  FindPredecessor(node->right, prev);
}

void MySet::eraseHelp(TreeNode *root, int val) {
  if (root == nullptr) {
    return;
  }
  if (val < root->val) {
    eraseHelp(root->left, val);
  } else if (val > root->val) {
    eraseHelp(root->right, val);
  } else {
    if (root->left == nullptr) {
      TreeNode *node = root;
      root = root->right;
      delete node;
    } else if (root->right == nullptr) {
      TreeNode *node = root;
      root = root->left;
      delete node;
    } else {
      // 如果两个节点都存在 找到该节点左子树的最大节点和右子树的最小节点
      // 然后将这个左子树最大节点(or 右子树最大节点)
      TreeNode *prev = nullptr;
      FindPredecessor(root->left, prev);
      root->val = prev->val;
      eraseHelp(root->left, prev->val);
    }
  }
}

void MySet::erase(int val) { eraseHelp(this->root, val); }

int MySet::highHelp(TreeNode *root) {
  if (root == 0) {
    return 0;
  }
  int leftDepth = 1;
  int rightDepth = 1;
  leftDepth = highHelp(root->left);
  rightDepth = highHelp(root->right);
  return std::max(leftDepth, rightDepth);
}
int MySet::high() { return highHelp(root); }

void MySet::RotateLeft(TreeNode *&node) {
  TreeNode *temp = node;
  node = temp->left;
  temp->left = node->right;
  node->right = temp;
}

void MySet::RotateRight(TreeNode *&node) {
  TreeNode *temp = node;
  node = temp->right;
  temp->right = node->left;
  node->left = temp;
}