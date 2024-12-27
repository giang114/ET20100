
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits>
using namespace std;

// Class đại diện cho một Node trong cây (thư mục hoặc tệp)
class Node {
public:
    string name;
    bool isFile;
    vector<shared_ptr<Node>> children;

    Node(string name, bool isFile) : name(name), isFile(isFile) {}
};

// Kiểm tra tên đã tồn tại trong danh sách con của một nút
bool nameExists(shared_ptr<Node> root, const string& newName) {
    for (const auto& child : root->children) {
        if (child->name == newName) {
            return true;
        }
    }
    return false;
}

// Thêm một thư mục hoặc tệp vào cây
bool addNode(shared_ptr<Node> root, const string& parentName, const string& newName, bool isFile) {
    if (root->name == parentName) {
        if (nameExists(root, newName)) {
            cout << "\nLỗi: Tên đã tồn tại trong thư mục cha.\n";
            return false;
        }
        root->children.push_back(make_shared<Node>(newName, isFile));
        return true;
    }

    for (auto child : root->children) {
        if (!child->isFile) {
            if (addNode(child, parentName, newName, isFile)) {
                return true;
            }
        }
    }
    return false;
}

// Xóa một thư mục hoặc tệp khỏi cây
bool deleteNode(shared_ptr<Node> root, const string& targetName) {
    for (auto it = root->children.begin(); it != root->children.end(); ++it) {
        if ((*it)->name == targetName) {
            root->children.erase(it);
            return true;
        }

        if (!(*it)->isFile) {
            if (deleteNode(*it, targetName)) {
                return true;
            }
        }
    }
    return false;
}

// Đổi tên một thư mục hoặc tệp trong cây
bool renameNode(shared_ptr<Node> root, const string& currentName, const string& newName) {
    // Nếu tên mới đã tồn tại trong danh sách con của node cha
    if (nameExists(root, newName)) {
        cout << "\nLỗi: Tên mới đã tồn tại trong cùng thư mục.\n";
        return false;
    }

    // Duyệt danh sách con để tìm và đổi tên
    for (auto& child : root->children) {
        if (child->name == currentName) {
            child->name = newName;
            return true;
        }

        // Đệ quy để tiếp tục tìm trong cây con
        if (!child->isFile) {
            if (renameNode(child, currentName, newName)) {
                return true;
            }
        }
    }
    return false; // Nếu không tìm thấy node cần đổi tên
}

// Hiển thị cấu trúc cây theo DFS, hỗ trợ phân trang
void displayTree(shared_ptr<Node> root, const string& indent = "", int level = 0, int maxLevel = -1) {
    if (maxLevel != -1 && level > maxLevel) return;
    cout << indent << (root->isFile ? "[File] " : "[Folder] ") << root->name << endl;
    for (auto child : root->children) {
        displayTree(child, indent + "  ", level + 1, maxLevel);
    }
}

// Kiểm tra nhập liệu an toàn
int safeInputInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nLỗi: Vui lòng nhập một số nguyên hợp lệ.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

int main() {
    // Tạo thư mục gốc
    shared_ptr<Node> root = make_shared<Node>("root", false);

    // Menu
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Thêm thư mục hoặc tệp tin\n";
        cout << "2. Xóa thư mục hoặc tệp tin\n";
        cout << "3. Hiển thị cấu trúc cây\n";
        cout << "4. Hiển thị cấu trúc cây (giới hạn độ sâu)\n";
        cout << "5. Đổi tên thư mục hoặc tệp tin\n"; 
        cout << "0. Thoát\n";
        choice = safeInputInt("Lựa chọn: ");

        if (choice == 1) {
            string parentName, newName;
            cout << "Nhập tên thư mục cha: ";
            cin >> parentName;
            cout << "Nhập tên mới: ";
            cin >> newName;
            int type = safeInputInt("Loại (0: thư mục, 1: tệp tin): ");
            if (type != 0 && type != 1) {
                cout << "\nLỗi: Chỉ chấp nhận giá trị 0 hoặc 1.\n";
                continue;
            }
            if (!addNode(root, parentName, newName, type == 1)) {
                cout << "\nKhông thể thêm: Thư mục cha không tồn tại hoặc lỗi xảy ra.\n";
            }
        } else if (choice == 2) {
            string targetName;
            cout << "Nhập tên thư mục hoặc tệp tin cần xóa: ";
            cin >> targetName;
            if (deleteNode(root, targetName)) {
                cout << "Xóa thành công!\n";
            } else {
                cout << "\nLỗi: Không tìm thấy thư mục hoặc tệp tin cần xóa.\n";
            }
        } else if (choice == 3) {
            displayTree(root);
        } else if (choice == 4) {
            int depth = safeInputInt("Nhập độ sâu tối đa để hiển thị: ");
            if (depth < 0) {
                cout << "\nLỗi: Độ sâu phải là số không âm.\n";
            } else {
                displayTree(root, "", 0, depth);
            }
        } else if (choice == 5) {
            string currentName, newName;
            cout << "Nhập tên hiện tại: ";
            cin >> currentName;
            cout << "Nhập tên mới: ";
            cin >> newName;
            if (!renameNode(root, currentName, newName)) {
                cout << "\nLỗi: Không thể đổi tên hoặc tên mới đã tồn tại.\n";
            } else {
                cout << "Đổi tên thành công!\n";
            }
        }
    } while (choice != 0);

    return 0;
}