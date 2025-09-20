#include "BST/AVLTree.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

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

void AVLTree::BalanceAVLTree(std::stack<TreeNode **> &nodes) {
  while (!nodes.empty()) {
    TreeNode **p = nodes.top();
    TreeNode *node = *p;

    updateHeight(node);
    int factor = getBalanceFactor(node);

    if (factor > 1) { // 左子树过高
      // LL or LR
      if (getBalanceFactor(node->left) >= 0) {
        // LL
        rotateRight(*p);
      } else {
        // LR: 先对左子树左旋，再对当前右旋
        rotateLeft(node->left);
        rotateRight(*p);
      }
      // 更新旋转后子树根的高度
      if (*p)
        updateHeight(*p);
    } else if (factor < -1) { // 右子树过高
      // RR or RL
      if (getBalanceFactor(node->right) <= 0) {
        // RR
        rotateLeft(*p);
      } else {
        // RL: 先对右子树右旋，再对当前左旋
        rotateRight(node->right);
        rotateLeft(*p);
      }
      if (*p)
        updateHeight(*p);
    }

    nodes.pop();
  }
}

void AVLTree::insertHelp(TreeNode *&root, int val,
                         std::stack<TreeNode **> &nodes) {
  if (root == nullptr) {
    root = new TreeNode(val);
    BalanceAVLTree(nodes);
    return;
  }
  if (val < root->val) {
    nodes.push(&root);
    insertHelp(root->left, val, nodes);
  } else if (val > root->val) {
    nodes.push(&root);
    insertHelp(root->right, val, nodes);
  } else {
    return;
  }
}

void AVLTree::insert(int val) {
  std::stack<TreeNode **> nodes;
  insertHelp(this->root, val, nodes);
}

void AVLTree::findPrevNode(TreeNode *root, TreeNode *&prevNode) {
  if (root->right == nullptr) {
    prevNode = root;
    return;
  }
  findPrevNode(root->right, prevNode);
}

void AVLTree::eraseHelp(TreeNode *&root, int val,
                        std::stack<TreeNode **> &nodes) {
  if (root == nullptr) {
    return;
  }
  if (val < root->val) {
    nodes.push(&root);
    eraseHelp(root->left, val, nodes);
  } else if (val > root->val) {
    nodes.push(&root);
    eraseHelp(root->right, val, nodes);
  } else {
    // 进行四种情况的判断
    if (root->left == nullptr && root->right == nullptr) {
      delete root;
      root = nullptr;
    } else if (root->right == nullptr) {
      TreeNode *temp = root;
      root = root->left;
      delete temp;
    } else if (root->left == nullptr && root->right == nullptr) {
      TreeNode *temp = root;
      root = root->right;
      delete temp;
    } else {
      TreeNode *prevNode = nullptr;
      findPrevNode(root->left, prevNode);
      root->val = prevNode->val;
      eraseHelp(root->left, prevNode->val, nodes);
    }
    BalanceAVLTree(nodes);
  }
}

void AVLTree::erase(int val) {
  std::stack<TreeNode **> nodes;
  eraseHelp(root, val, nodes);
}

void AVLTree::printOrdTreeHelp(TreeNode *root, std::vector<int> &vec) {
  if (root == nullptr) {
    return;
  }
  printOrdTreeHelp(root->left, vec);
  std::cout << root->val << std::endl;
  vec.push_back(root->val);
  printOrdTreeHelp(root->right, vec);
}

void AVLTree::printOrdTree(std::vector<int> &vec) {
  printOrdTreeHelp(this->root, vec);
}