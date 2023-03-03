#include <iostream>
#include <vector>

using namespace std;

struct Fibonacci_heap {
    struct node {
        int key;
        node *parent; // указатель на родителя
        node *child;  // указатель на сына
        node *left = nullptr;   // указатель на левый узел того же предка
        node *right = nullptr;  // указатель на правый узел того же предка
        int degree = 0;   // степень вершины
        bool mark = false;    // был ли удален в процессе изменения ключа сын этой вершины

        explicit node(int x): key(x){
            parent = this;
            child = this;
        }
    };

    int size = 0;  // текущее число узлов
    node *min = nullptr; // указатель на корень дерева с минимальным ключом

    // вставка элемента в список корней правее минимального элемента
    void insert(int weight, u_int vertex_index) {
        node new_node(weight);

        if (size == 0) {
            min = &new_node;
            min->left = &new_node;
            min->right = &new_node;
        } else {
            node *prev_right = min->right;

            min->right = &new_node;
            new_node.left = min;
            new_node.right = prev_right;
            prev_right->left = &new_node;
        }

        if (new_node.key < min->key)
            min = &new_node;


        size++;
    }

//    node *get_min() {
//        return min;
//    }

};

// prima algorithm
void minimum_spanning_tree(vector<vector<u_int>> adjacency_list) {

}

int main() {
    /* На вход подается количество вершин и списки смежности вида:
     * 2 3 5 // для вершины 1
     * 1 6   // для вершины 2
     * 1     // для вершины 3
     * */

    size_t n;
    cin >> n;
    vector<vector<u_int>> adjacency_list;

    for (size_t i = 0; i < n; i++) {
        u_int temp;
        while (cin >> temp) {
            adjacency_list[i].emplace_back(temp);
        }
    }

    return 0;
}
