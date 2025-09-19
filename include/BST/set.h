struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int val) : val(val), left(nullptr), right(nullptr) {}
};

class MySet {
private:
  TreeNode *root;

  void insertHelp(TreeNode *&root, int val);
  void printHelp(TreeNode *root);
  bool findHelp(TreeNode *root, int val);
  void eraseHelp(TreeNode *root, int val);
  void FindPredecessor(TreeNode *node, TreeNode *&prev);
  int highHelp(TreeNode *root);

public:
  MySet() = default;
  void insert(int val);
  void erase(int val);
  bool find(int val);
  void print();
  int high();
  void RotateRight(TreeNode *&node);
  void RotateLeft(TreeNode *&node);
};