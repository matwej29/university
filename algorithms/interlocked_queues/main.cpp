#include <climits>
#include <cstddef>
#include <iostream>
#include <sstream>

int FOREST_CAPACITY = 1;

using namespace std;

struct Node {
  Node *left = nullptr;
  Node *middle = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;
  int L = INT_MIN; // тут значение, если вершина - лист
  int M = INT_MIN;

  Node *find(int v, Node *root);
  Node *insert(int v);
  void split_node(Node *node_root, Node *new_node, int v);
  void attach_to_parent(Node *current, Node *splitted);

  bool is_leaf() const {
    return this->left == nullptr and this->middle == nullptr and
           this->right == nullptr;
  }

  void sort_children() {
    if (this->right != nullptr and this->right->L < this->middle->L) {
      swap(this->right, this->middle);
    }
    if (this->middle->L < this->left->L) {
      swap(this->middle, this->left);
    }
  }

  void correct_marks(Node *start);
  int node_max();
};

// int Node::node_max() {
//   if (is_leaf())
//     return L;

//   int lMax = (left) ? max(left->L, left->M) : INT_MIN;
//   int mMax = (middle) ? max(middle->L, middle->M) : INT_MIN;
//   int rMax = (right) ? max(right->L, right->M) : INT_MIN;
//   return max({lMax, mMax, rMax});
// }

int Node::node_max() {
  if (is_leaf()) {
    return L;
  }
  if (left->is_leaf()) {
    if (right) {
      return max({left->L, middle->M, right->L});
    }

    return max(left->L, middle->L);
  }

  if (right) {
    return right->node_max();
  }
  return max(left->node_max(), middle->node_max());
}

void Node::correct_marks(Node *start) {
  Node *current = start;

  while (current != nullptr) {
    const int oldL = current->L;
    const int oldM = current->M;

    if (current->left) {
      current->L = current->left->node_max();
    }

    if (current->middle) {
      current->M = current->middle->node_max();
    }

    if (current->L == oldL and current->M == oldM) {
      return;
    }

    current = current->parent;
  }
}

Node *Node::find(int v, Node *root) {
  if (root->is_leaf()) {
    return root;
  }

  if (v <= root->L) {
    return find(v, root->left);
  } else if (v <= root->M or root->right == nullptr) {
    return find(v, root->middle);
  } else if (root->right != nullptr) {
    return find(v, root->right);
  } else {
    return root;
  }
}

Node *Node::insert(int v) {
  Node *root = this;
  // случай, когда дерево содержит только один элемент
  if (root->is_leaf() and this->parent == nullptr) {
    Node *new_root = new Node{};
    new_root->left = this;
    new_root->middle = new Node{.L = v};
    this->parent = new_root;
    new_root->middle->parent = new_root;
    if (new_root->left->L > new_root->middle->L) {
      swap(new_root->left, new_root->middle);
    }
    new_root->correct_marks(new_root);
    return new_root;
  }

  Node *node_root = this->find(v, this);
  if (node_root->is_leaf()) {
    if (node_root->L == v) {
      printf("Вершина %d уже существует\n", node_root->L);
      return root;
    }
    node_root = node_root->parent;
  }

  Node *new_node = new Node{.parent = node_root, .L = v};

  if (node_root->right == nullptr) {
    node_root->right = new_node;
    node_root->sort_children();
    node_root->correct_marks(node_root);
    return root;
  }

  this->split_node(node_root, new_node, v);

  Node *current = node_root;
  while (current->parent) {
    current = current->parent;
  }
  return current;
}

void Node::attach_to_parent(Node *current, Node *splitted) {
  Node *parent = current->parent;
  if (!parent) {
    Node *new_root = new Node{};
    new_root->left = current;
    new_root->middle = splitted;
    current->parent = new_root;
    splitted->parent = new_root;
    new_root->correct_marks(new_root);
    return;
  }

  if (parent->right == nullptr) {
    parent->right = splitted;
    splitted->parent = parent;
    parent->correct_marks(parent);
    return;
  }

  this->split_node(parent, splitted, splitted->L);
}

void Node::split_node(Node *node_root, Node *new_node, int v) {
  Node *children[4] = {node_root->left, node_root->middle, node_root->right,
                       new_node};

  for (int i = 3; i > 1; --i) {
    if (children[i]->L < children[i - 1]->L) {
      swap(children[i], children[i - 1]);
    } else {
      break;
    }
  }

  node_root->left = children[0];
  node_root->middle = children[1];
  node_root->right = nullptr;
  node_root->left->parent = node_root;
  node_root->middle->parent = node_root;

  Node *splitted = new Node{};
  splitted->left = children[2];
  splitted->middle = children[3];
  splitted->right = nullptr;
  splitted->left->parent = splitted;
  splitted->middle->parent = splitted;

  node_root->correct_marks(node_root);
  splitted->correct_marks(splitted);

  attach_to_parent(node_root, splitted);
}

int get_height(const Node *root) {
  if (!root || root->is_leaf()) {
    return 0;
  }

  return 1 + get_height(root->left);
}

Node *find_node_at_height(Node *root, int targetHeight, bool goRightSide) {
  if (!root) {
    printf("find_node_at_height: root is nullptr\n");
    return nullptr;
  }
  if (get_height(root) == targetHeight) {
    return root;
  }
  if (goRightSide == false) {
    if (root->left) {
      Node *next = find_node_at_height(root->left, targetHeight, goRightSide);
      if (next) {
        return next;
      }
    }
  }
  if (root->right) {
    Node *next = find_node_at_height(root->right, targetHeight, goRightSide);
    if (next) {
      return next;
    }
  } else {
    Node *next = find_node_at_height(root->middle, targetHeight, goRightSide);
    if (next) {
      return next;
    }
  }

  return nullptr;
}

Node *couple_trees(Node *leftRoot, Node *rightRoot) {
  int hLeft = get_height(leftRoot);
  int hRight = get_height(rightRoot);

  if (hLeft == hRight) {
    Node *newRoot = new Node{};
    newRoot->left = leftRoot;
    newRoot->middle = rightRoot;
    leftRoot->parent = newRoot;
    rightRoot->parent = newRoot;
    newRoot->correct_marks(newRoot);
    return newRoot;
  }

  if (hLeft > hRight) {
    Node *targetNewRoot = find_node_at_height(leftRoot, hRight + 1, true);
    if (!targetNewRoot) {
      printf("targetNewRoot is nullptr\n");
      return nullptr;
    }
    if (!targetNewRoot->right) {
      targetNewRoot->right = rightRoot;
      targetNewRoot->sort_children();
    } else {
      targetNewRoot->split_node(targetNewRoot, rightRoot, rightRoot->L);
    }
    rightRoot->parent = targetNewRoot;
    targetNewRoot->correct_marks(targetNewRoot);

    while (leftRoot->parent) {
      leftRoot = leftRoot->parent;
    }
    return leftRoot;
  }

  Node *targetNewRoot = find_node_at_height(rightRoot, hLeft + 1, false);
  if (!targetNewRoot) {
    printf("targetNewRoot is nullptr\n");
    return nullptr;
  }
  if (!targetNewRoot->right) {
    targetNewRoot->right = leftRoot;
  } else {
    targetNewRoot->split_node(targetNewRoot, leftRoot, leftRoot->L);
  }
  leftRoot->parent = targetNewRoot;
  targetNewRoot->correct_marks(targetNewRoot);

  while (rightRoot->parent) {
    rightRoot = rightRoot->parent;
  }
  return rightRoot;
}

int build_path(Node *leaf, Node *pathArray[], const int capacity) {
  int count = 0;
  Node *cur = leaf;
  while (cur && count < capacity) {
    pathArray[count++] = cur;
    cur = cur->parent;
  }
  return count;
}

Node *couple_rightwise(Node *forest[], int &count) {
  if (count == 0) {
    return nullptr;
  }
  while (count > 1) {
    Node *t1 = forest[count - 1];
    Node *t2 = forest[count - 2];
    Node *c = couple_trees(t2, t1);
    if (count < FOREST_CAPACITY) {
      forest[count - 2] = c;
      count--;
    }
  }
  return forest[0];
}

Node *couple_leftwise(Node *forest[], int &count) {
  if (count == 0) {
    return nullptr;
  }
  int start_ind = 0;
  while (count > 1) {
    Node *t1 = forest[start_ind];
    Node *t2 = forest[start_ind + 1];
    Node *c = couple_trees(t1, t2);
    if (count < FOREST_CAPACITY) {
      start_ind++;
      forest[start_ind] = c;
      count--;
    }
  }
  return forest[start_ind];
}

int forest_push(Node *node, Node *forest[], int count) {
  if (node == nullptr) {
    return count;
  }
  if (count >= FOREST_CAPACITY) {
    printf("forest_push: forest is full\n");
    return count;
  }
  node->parent = nullptr;
  forest[count] = node;
  count++;
  return count;
}

pair<Node *, Node *> uncouple_tree(Node *root, int val) {
  Node *leaf = root->find(val, root);
  if (leaf == nullptr || !leaf->is_leaf() || leaf->L != val) {
    return {nullptr, nullptr};
  }

  Node *pathArray[FOREST_CAPACITY];
  int pathSize = build_path(leaf, pathArray, FOREST_CAPACITY);

  Node *leftForest[FOREST_CAPACITY];
  Node *rightForest[FOREST_CAPACITY];
  int leftCount = 0;
  int rightCount = 0;

  Node *leaf_parent = pathArray[0]->parent;
  if (leaf_parent->right == pathArray[0]) {
    leftCount = forest_push(leaf_parent->left, leftForest, leftCount);
    leftCount = forest_push(leaf_parent->middle, leftForest, leftCount);
    leftCount = forest_push(leaf_parent->right, leftForest, leftCount);
  } else if (leaf_parent->left == pathArray[0]) {
    leftCount = forest_push(leaf_parent->left, leftForest, leftCount);
    rightCount = forest_push(leaf_parent->middle, rightForest, rightCount);
    rightCount = forest_push(leaf_parent->right, rightForest, rightCount);
  } else { // middle
    leftCount = forest_push(leaf_parent->left, leftForest, leftCount);
    leftCount = forest_push(leaf_parent->middle, leftForest, leftCount);
    rightCount = forest_push(leaf_parent->right, rightForest, rightCount);
  }
  for (int i = 0; i < leftCount / 2; i++) {
    swap(leftForest[i], leftForest[leftCount - i - 1]);
  }

  for (int i = 1; i < pathSize; i++) {
    Node *nodeOnPath = pathArray[i];
    Node *parent = nodeOnPath->parent;
    if (!parent) {
      continue;
    }
    if (parent->left == nodeOnPath) {
      rightCount = forest_push(parent->middle, rightForest, rightCount);

      if (parent->right) {
        rightCount = forest_push(parent->right, rightForest, rightCount);
      }
    } else if (parent->middle == nodeOnPath) {
      leftCount = forest_push(parent->left, leftForest, leftCount);

      if (parent->right) {
        rightCount = forest_push(parent->right, rightForest, rightCount);
      }
    } else {
      leftCount = forest_push(parent->left, leftForest, leftCount);
      leftCount = forest_push(parent->middle, leftForest, leftCount);
    }
  }

  for (int i = 1; i < pathSize; i++) {
    delete pathArray[i];
  }

  for (int i = 0; i < leftCount / 2; i++) {
    swap(leftForest[i], leftForest[leftCount - i - 1]);
  }

  Node *leftResult = couple_rightwise(leftForest, leftCount);

  Node *rightResult = couple_leftwise(rightForest, rightCount);

  return {leftResult, rightResult};
}

void print_tree(Node *root, int level = 0) {
  if (!root) {
    return;
  }
  if (level == 0) {
    cout << "Tree:\n";
  }
  if (level > 0) {
    cout << '|';
  }
  for (int i = 0; i < level; ++i) {
    cout << " - ";
  }
  cout << "(L=" << root->L << ", M=" << root->M << ")";
  if (root->is_leaf()) {
    cout << " [лист]";
  }
  cout << "\n";

  print_tree(root->left, level + 1);
  print_tree(root->middle, level + 1);
  print_tree(root->right, level + 1);
}

// int main() {
//   FOREST_CAPACITY = 100;
//   Node *root1 = new Node{.L = 1};
//   root1 = root1->insert(2);
//   root1 = root1->insert(4);
//   root1 = root1->insert(5);

//   Node *root2 = new Node{.L = 6};
//   root2 = root2->insert(7);
//   root2 = root2->insert(9);
//   root2 = root2->insert(11);
//   root2 = root2->insert(12);

//   print_tree(root1);
//   cout << "\n";
//   print_tree(root2);

//   Node *coupledRoot = couple_trees(root1, root2);
//   cout << "\nCoupled tree:\n";
//   print_tree(coupledRoot);
//   cout << "\n";

//   auto [leftTree, rightTree] = uncouple_tree(coupledRoot, 7);
//   cout << "Uncoupled trees:\n";
//   cout << "Left tree:\n";
//   print_tree(leftTree);
//   cout << "\nRight tree:\n";
//   print_tree(rightTree);

//   // insert 3 into left tree
//   leftTree = leftTree->insert(3);
//   cout << "\nLeft tree after inserting 3:\n";
//   print_tree(leftTree);
//   cout << "\n";

//   // couple left tree with right tree
//   Node *newCoupledRoot = couple_trees(leftTree, rightTree);
//   cout << "New coupled tree:\n";
//   print_tree(newCoupledRoot);
//   cout << "\n";

//   // Node *root1 = new Node{.L = 1};
//   // root1 = root1->insert(2);
//   // root1 = root1->insert(3);
//   // root1 = root1->insert(4);
//   // root1 = root1->insert(5);
//   // print_tree(root1);

//   // auto [leftTree, rightTree] = uncouple_tree(root1, 3);
//   // print_tree(leftTree);
//   // print_tree(rightTree);
//   return 0;
// }

int main() {
  Node *trees[100];
  for (int i = 0; i < 100; i++) {
    trees[i] = nullptr;
  }
  int trees_next = 0;

  cout << "Commands:\n"
       << "  a i t  -> add element i to tree t\n"
       << "  f i t  -> find element i in tree t\n"
       << "  u i t  -> uncouple tree t by element i (creates two new trees)\n"
       << "  c t1 t2-> couple trees t1 and t2 (creates a new tree)\n"
       << "  l      -> list indexes of existing trees\n"
       << "  p t    -> print tree t\n"
       << "Empty line -> exit\n";

  while (true) {
    string line;
    if (!std::getline(cin, line) || line.empty()) {
      break; // Stop on empty line or EOF
    }

    std::stringstream ss(line);
    string cmd;
    ss >> cmd;
    if (cmd == "l") {
      for (int idx = 0; idx < FOREST_CAPACITY; idx++) {
        if (trees[idx] != nullptr) {
          cout << idx << " ";
        }
      }
      cout << "\n";
    } else if (cmd == "p") {
      int t;
      ss >> t;
      if (t >= 0 && t < FOREST_CAPACITY && trees[t]) {
        print_tree(trees[t]);
      } else {
        cout << "No such tree " << t << "\n";
      }
    } else if (cmd == "a") {
      FOREST_CAPACITY++;
      int i, t;
      ss >> i >> t;
      if (t >= 0 && t < FOREST_CAPACITY) {
        if (!trees[t]) {
          trees[t] = new Node{.L = i};
          trees_next = max(trees_next, t) + 1;
        } else {
          trees[t] = trees[t]->insert(i);
        }
      }
    } else if (cmd == "f") {
      int i, t;
      ss >> i >> t;
      if (t >= 0 && t < FOREST_CAPACITY && trees[t]) {
        Node *found = trees[t]->find(i, trees[t]);
        if (found && found->is_leaf() && found->L == i) {
          cout << "да\n";
        } else {
          cout << "нет\n";
        }
      } else {
        cout << "нет\n";
      }
    } else if (cmd == "u") {
      int i, t;
      ss >> i >> t;
      if (t >= 0 && t < FOREST_CAPACITY && trees[t]) {
        auto [leftT, rightT] = uncouple_tree(trees[t], i);
        print_tree(leftT);
        print_tree(rightT);
        if (!leftT && !rightT) {
          cout << "Uncouple failed\n";
        } else {
          int left_ind = trees_next++;
          int right_ind = trees_next++;
          trees[t] = nullptr;
          trees[left_ind] = leftT;
          trees[right_ind] = rightT;
          cout << "Uncoupled into trees " << left_ind << " and " << right_ind
               << "\n";
        }
      }
    } else if (cmd == "c") {
      int t1, t2;
      ss >> t1 >> t2;
      if (t1 >= 0 && t1 < FOREST_CAPACITY && t2 >= 0 && t2 < FOREST_CAPACITY
      &&
          trees[t1] && trees[t2]) {
        Node *result = couple_trees(trees[t1], trees[t2]);
        trees[t1] = result;
        trees[t2] = nullptr;
        cout << "Coupeled at " << t1 << endl;
      } else {
        cout << "Invalid tree indexes\n";
      }
    } else {
      cout << "Unknown command\n";
    }
  }

  return 0;
}
