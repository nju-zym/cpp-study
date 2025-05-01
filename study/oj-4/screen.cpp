#include <iostream>

// 窗口基类
class Window {
   protected:
    int _x, _y, _w, _h;
    friend class Screen;  // Allow Screen to access protected members
};

// 派生类，用链表的形式将窗口组织起来
class WindowInScreen : public Window {
    int id{};
    WindowInScreen* next;

   public:
    friend class Screen;
    WindowInScreen() : next(nullptr) {}

    void display() {
        std::cout << "(id: " << id << ", x: " << _x << ", y: " << _y << ", w: " << _w << ", h: " << _h << ")" << '\n';
    }
};

// 屏幕类
class Screen {
    int _w, _h;
    int count;
    WindowInScreen* root;

    // Helper to resize the partner window when its counterpart is deleted
    void resizePartner(WindowInScreen* partner, WindowInScreen* removed) {
        if (partner == nullptr || removed == nullptr) {
            return;
        }
        // Check how they were split and double the dimension of the partner
        if (partner->_y + partner->_h == removed->_y && partner->_x == removed->_x) {  // removed was below partner
            partner->_h *= 2;
        } else if (partner->_x + partner->_w == removed->_x
                   && partner->_y == removed->_y) {  // removed was right of partner
            partner->_w *= 2;
        }
        // Assuming addWindow logic prevents other cases (e.g., removed above/left partner)
    }

   public:
    Screen(int w = 1920, int h = 1080) : _w(w), _h(h), count(0), root(nullptr) {}
    ~Screen() {
        WindowInScreen* current = root;
        while (current) {
            WindowInScreen* next = current->next;
            delete current;
            current = next;
        }
        root  = nullptr;
        count = 0;
    }

    void addWindow(int id) {
        WindowInScreen* newWindow = new WindowInScreen();
        newWindow->id             = id;
        newWindow->next           = nullptr;

        if (root == nullptr) {  // First window case
            newWindow->_x = 0;
            newWindow->_y = 0;
            newWindow->_w = _w;
            newWindow->_h = _h;
            root          = newWindow;
        } else {  // Not the first window
            WindowInScreen* lastWindow = root;
            while (lastWindow->next) {
                lastWindow = lastWindow->next;
            }

            // Halve the larger dimension of the last window and place new window
            if (lastWindow->_h > lastWindow->_w) {  // Height is larger, split vertically
                lastWindow->_h /= 2;                // Update last window's height
                newWindow->_w   = lastWindow->_w;
                newWindow->_h   = lastWindow->_h;  // New window gets the halved height
                newWindow->_x   = lastWindow->_x;
                newWindow->_y   = lastWindow->_y + lastWindow->_h;  // Place below the modified last window
            } else {                                                // Width is larger or equal, split horizontally
                lastWindow->_w /= 2;                                // Update last window's width
                newWindow->_w   = lastWindow->_w;                   // New window gets the halved width
                newWindow->_h   = lastWindow->_h;
                newWindow->_x   = lastWindow->_x + lastWindow->_w;  // Place right of the modified last window
                newWindow->_y   = lastWindow->_y;
            }
            lastWindow->next = newWindow;  // Link the new window
        }
        count++;  // Increment count
    }

    void delWindow(int id) {
        if (root == nullptr) {
            return;  // Empty screen
        }

        WindowInScreen* current = root;
        WindowInScreen* prev    = nullptr;

        // Find the window to delete
        while (current && current->id != id) {
            prev    = current;
            current = current->next;
        }

        if (current == nullptr) {
            return;  // Window not found
        }

        WindowInScreen* nodeToDelete = nullptr;  // Keep track of the node to delete physically

        if (current->next == nullptr) {  // Case 1: Deleting the last node in the list
            nodeToDelete = current;
            if (prev == nullptr) {  // It was the only node
                root = nullptr;
            } else {                           // More than one node existed
                resizePartner(prev, current);  // Resize the previous node
                prev->next = nullptr;          // Unlink the last node
            }
        } else {  // Case 2: Deleting the root or a middle node
                  // Shift IDs up starting from the deleted position,
                  // then resize the second-to-last node and remove the last one.
            WindowInScreen* p                       = current;
            WindowInScreen* node_before_data_source = current;  // Tracks the node receiving the ID

            while (p->next != nullptr) {
                WindowInScreen* dataSource = p->next;
                p->id                      = dataSource->id;  // Copy ID from the next node

                node_before_data_source = p;           // This node now holds the ID previously in dataSource
                p                       = dataSource;  // Move to the next node (the one we just copied from)
            }
            // After the loop:
            // p points to the original last node in the chain segment.
            // node_before_data_source points to the node just before p.

            nodeToDelete = p;                           // The node to physically delete is the last one (p)
            resizePartner(node_before_data_source, p);  // Resize the partner
            node_before_data_source->next = nullptr;    // Unlink the last node

            // If the root was the target (prev == nullptr), the root node structure
            // remains but now holds the ID of the original second node.
        }

        delete nodeToDelete;  // Delete the identified node
        count--;              // Decrement count
    }

    void display() {
        WindowInScreen* current = root;
        while (current) {
            current->display();
            current = current->next;
        }
    }
};

int main() {
    Screen screen;
    screen.addWindow(1);
    screen.addWindow(2);
    screen.addWindow(3);
    screen.addWindow(4);
    std::cout << "Initial state:\n";
    screen.display();
    screen.delWindow(2);
    std::cout << "\nAfter deleting window 2:\n";
    screen.display();

    // Add more tests if needed
    Screen screen2(100, 100);
    screen2.addWindow(10);
    screen2.addWindow(20);
    screen2.addWindow(30);
    std::cout << "\nScreen 2 Initial state:\n";
    screen2.display();
    screen2.delWindow(10);  // Delete root
    std::cout << "\nScreen 2 After deleting window 10:\n";
    screen2.display();
    screen2.delWindow(30);  // Delete last
    std::cout << "\nScreen 2 After deleting window 30:\n";
    screen2.display();

    return 0;  // Added return 0 for main
}