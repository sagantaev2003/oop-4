#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

// узел бинарного дерева
struct Node {
    string data;
    Node* left;
    Node* right;
    Node(string val) : data(val), left(nullptr), right(nullptr) {}
};

// функция сравнения чисел и строк
bool compare(const string& a, const string& b) {
    bool a_is_num = all_of(a.begin(), a.end(), ::isdigit);
    bool b_is_num = all_of(b.begin(), b.end(), ::isdigit);

    if (a_is_num && b_is_num)
        return stoi(a) < stoi(b); 
    if (a_is_num && !b_is_num)
        return true;  
    if (!a_is_num && b_is_num)
        return false; 
    return a < b;     
}

// вставка узла в дерево
Node* insert(Node* root, const string& value) {
    if (!root) return new Node(value);
    if (compare(value, root->data)) root->left = insert(root->left, value);
    else root->right = insert(root->right, value);
    return root;
}

// обходы
void inorder(Node* root, ostream& out1, ostream& out2) {
    if (root) {
        inorder(root->left, out1, out2);
        out1 << root->data << " ";
        out2 << root->data << " ";
        inorder(root->right, out1, out2);
    }
}

void preorder(Node* root, ostream& out1, ostream& out2) {
    if (root) {
        out1 << root->data << " ";
        out2 << root->data << " ";
        preorder(root->left, out1, out2);
        preorder(root->right, out1, out2);
    }
}

void postorder(Node* root, ostream& out1, ostream& out2) {
    if (root) {
        postorder(root->left, out1, out2);
        postorder(root->right, out1, out2);
        out1 << root->data << " ";
        out2 << root->data << " ";
    }
}

// вывод дерева по уровням
void printByLevels(Node* root, ostream& out1, ostream& out2) {
    if (!root) return;
    queue<Node*> q;
    q.push(root);
    int level = 1;

    while (!q.empty()) {
        int sz = q.size();
        out1 << "Уровень " << level << ": ";
        out2 << "Уровень " << level << ": ";
        for (int i = 0; i < sz; ++i) {
            Node* node = q.front(); q.pop();
            out1 << node->data << " ";
            out2 << node->data << " ";
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        out1 << "\n";
        out2 << "\n";
        ++level;
    }
}

// построение дерева из файла
Node* buildTreeFromFile(const string& filename, vector<string>& dataList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return nullptr;
    }
    Node* root = nullptr;
    string token;
    while (file >> token) {
        dataList.push_back(token);
        root = insert(root, token);
    }
    return root;
}

// подсчёт высоты дерева
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

// освобождение памяти
void freeTree(Node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        delete root;
    }
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "ru");

    string inputFile = "data.txt";
    string outputFile = "output.txt";

    vector<string> dataList;
    Node* root = buildTreeFromFile(inputFile, dataList);
    if (!root) {
        cerr << "Ошибка: дерево не построено." << endl;
        return 1;
    }

    ofstream out(outputFile);
    if (!out.is_open()) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        freeTree(root);
        return 1;
    }

    // вывод исходных данных
    cout << "Исходные данные:\n";
    out << "Исходные данные:\n";
    for (const auto& s : dataList) {
        cout << s << " ";
        out << s << " ";
    }
    cout << "\n\n";
    out << "\n\n";

    // сортировка (In-order)
    cout << "In-order (сортированный):\n";
    out << "In-order (сортированный):\n";
    inorder(root, cout, out);
    cout << "\n\n"; out << "\n\n";

    // Pre-order
    cout << "Pre-order (корень-лево-право):\n";
    out << "Pre-order (корень-лево-право):\n";
    preorder(root, cout, out);
    cout << "\n\n"; out << "\n\n";

    // Post-order
    cout << "Post-order (лево-право-корень):\n";
    out << "Post-order (лево-право-корень):\n";
    postorder(root, cout, out);
    cout << "\n\n"; out << "\n\n";

    // Level-order
    cout << "Дерево по уровням:\n";
    out << "Дерево по уровням:\n";
    printByLevels(root, cout, out);
    cout << "\n"; out << "\n";

    // высота дерева
    int h = height(root);
    cout << "Высота бинарного дерева: " << h << endl;
    out << "Высота бинарного дерева: " << h << endl;

    out.close();
    freeTree(root);

    return 0;
}
