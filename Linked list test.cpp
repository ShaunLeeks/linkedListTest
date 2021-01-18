#include <memory>
#include <iostream>
#include <cassert>


// Basic linked list
// Does not provide iterators, keeps internal position state
template <typename T>
class LinkedList {
public:
    typedef T item_t;
    typedef typename std::shared_ptr<item_t> ptr_t;

    static const ptr_t empty_ptr() {
        static ptr_t empty{};
        return empty;
    }

private:
    typedef struct node {
        ptr_t data;
        node* nextNode{ nullptr };
        node* prevNode{ nullptr };
    }*nodePtr;

    node* currentNode{ nullptr };
    node* firstNode{ nullptr };
    node* lastNode{ nullptr };
    size_t length{ 0 };

public:
    // Function to check if a list is empty
    bool empty() const
    {
        return length == 0;
    }

    // Return number of elements in the list
    size_t size() const
    {
        return length;
    }

    // Clear all elements from list
    void clear()
    {
        nodePtr current = firstNode;
        nodePtr nextNode;
        while (current){ // This is not O(1) time complexity
            nextNode = current->nextNode;
            delete(current);
            current = nextNode;
        }
        currentNode = nullptr;
        firstNode = nullptr;
        lastNode = nullptr;
        length = 0;
    }

    // Get reference to the current item in the list
    // Behavior is undefined for empty list
    item_t& current()
    {
        return *currentNode->data;
    }

    // Returns pointer to first item in list
    //     or empty_ptr
    // Relocates current to beginning of list
    const ptr_t first()
    {
        currentNode = firstNode;
        return firstNode->data;
    }

    // Returns pointer to last item in list
    //     or empty_ptr
    // Relocates current to end of list
    const ptr_t last()
    {
        currentNode = lastNode;
        return lastNode->data;
    }

    // Get the next / previous item from the list and set as current
    // Returns pointer to current item
    //   or empty_ptr if operation be invalid
    const ptr_t next()
    {
        if (currentNode->nextNode) {
            currentNode = currentNode->nextNode;
            return currentNode->data;
        }
        else
        {
            return empty_ptr();
        }
    }

    const ptr_t prev()
    {
        if (currentNode->prevNode) {
            currentNode = currentNode->prevNode;
            return currentNode->data;
        }
        else
        {
            return empty_ptr();
        }
    }

    // Append an item to the end of the list
    // Does not modify current position
    void push_back(const item_t& item)
    {
        length++;
        nodePtr newNode{ new node };
        newNode->data = std::make_shared<T>(item); // check this

        if (!currentNode) {
            currentNode = newNode;
        }
        if (lastNode)
        {
            lastNode->nextNode = newNode;
            newNode->prevNode = lastNode;
        }
        else if (!firstNode)
        {
            firstNode = newNode;
        }
        lastNode = newNode;
    }

    // Prepend an item to the beginning of the list
    // Does not modify current position
    void push_front(const item_t& item)
    {
        length++;
        nodePtr newNode{ new node };
        newNode->data = std::make_shared<T>(item); // check this

        if (!currentNode) {
            currentNode = newNode;
        }
        if (firstNode)
        {
            firstNode->prevNode = newNode;
            newNode->nextNode = firstNode;
        }
        else if (!lastNode) {
            lastNode = newNode;
        }
        firstNode = newNode;
    }

    // Insert item before current item and set to current
    // Returns pointer to previous current item
    //     or empty_ptr if empty list
    const ptr_t insert(const item_t& item)
    {
        if (size() == 0)
        {
            push_front(item);
            return empty_ptr();
        }
        length++;
        nodePtr newNode{ new node };
        newNode->data = std::make_shared<T>(item); // check this

        if (currentNode != firstNode)
        {
            nodePtr beforeCurrent = currentNode->prevNode;

            beforeCurrent->nextNode = newNode;
            newNode->prevNode = beforeCurrent;
        }

        currentNode->prevNode = newNode;
        newNode->nextNode = currentNode;

        currentNode = newNode;

        return currentNode->nextNode->data;
    }

    // Append item after current item and set to current
    // Returns pointer to previous current item
    //     or empty_ptr if empty list
    const ptr_t append(const item_t& item)
    {
        if (size() == 0)
        {
            push_front(item);
            return empty_ptr();
        }
        length++;
        nodePtr newNode{ new node };
        newNode->data = std::make_shared<T>(item); // check this

        if (currentNode != lastNode)
        {
            nodePtr afterCurrent = currentNode->nextNode;

            afterCurrent->prevNode = newNode;
            newNode->nextNode = afterCurrent;
        }

        currentNode->nextNode = newNode;
        newNode->prevNode = currentNode;

        currentNode = newNode;

        return currentNode->prevNode->data;
    }

    // Remove the current item from the list
    //    and move current item to next after removed item
    // no-op if empty list
    // Returns pointer to removed (formerly current) item
    //     or empty_ptr if empty list
    const ptr_t removeCurrent()
    {
        if (size() == 0)
        {
            return empty_ptr();
        }
        else {
            nodePtr tempNode = currentNode;
            if (currentNode == firstNode) {
                firstNode = firstNode->nextNode;
                currentNode = firstNode;
            }
            else if (currentNode == lastNode) {
                lastNode = lastNode->prevNode;
                currentNode = lastNode;
            }
            else {
                nodePtr beforeCurrent = currentNode->prevNode;
                nodePtr afterCurrent = currentNode->nextNode;

                beforeCurrent->nextNode = afterCurrent;
                afterCurrent->prevNode = beforeCurrent;
                currentNode = afterCurrent;
            }
            delete(tempNode);
            return currentNode->data;
        }
    }

};
using linked_list_t = LinkedList<int>;
using std::cout;
using std::cerr;

// NOTE - is_equal() relocates current() to end of list
bool is_equal(linked_list_t& ll, const std::initializer_list<int>& vals)
{
    if (ll.size() != vals.size())
        return false;

    ll.first();
    for (auto iter : vals)
    {
        auto temp = ll.current();
        if (temp != iter)
            return false;
        cout << ll.current() << ' ';
        ll.next();
    }

    cout << '\n';
    return true;
}

int main(int argc, char** argv)
{
    linked_list_t ll;
    cerr << "Test empty...";
    assert(ll.empty());
    cerr << "done\n";

    cerr << "Test push_back...";
    ll.push_back(1);
    assert(!ll.empty());
    assert(ll.size() == 1);
    assert(ll.current() == 1);
    ll.push_back(2);
    assert(ll.size() == 2);
    assert(ll.current() == 1);
    assert(is_equal(ll, { 1, 2 }));
    cerr << "done\n";

    cerr << "Test push_front...";
    ll.push_front(0);
    assert(ll.size() == 3);
    assert(is_equal(ll, { 0, 1, 2 }));
    cerr << "done\n";

    cerr << "Test clear...";
    ll.clear();
    assert(ll.size() == 0);
    cerr << "done\n";

    cerr << "Test removeCurrent() on empty list...";
    auto item = ll.removeCurrent();
    assert(item == linked_list_t::empty_ptr());
    cerr << "done\n";


    cerr << "Test insert on empty list...";
    item = ll.insert(100);
    assert(item == linked_list_t::empty_ptr());
    assert(ll.size() == 1);
    assert(ll.current() == 100);
    cerr << "done\n";


    cerr << "Test append on empty list...";
    ll.clear();
    assert(ll.size() == 0);
    item = ll.append(0);
    assert(item == linked_list_t::empty_ptr());
    assert(ll.size() == 1);
    assert(ll.current() == 0);
    cerr << "done\n";

    for (int i = 1; i < 10; ++i)
        ll.push_back(i);

    assert(ll.size() == 10);
    assert(ll.current() == 0);

    cerr << "Test first and next...";
    item = ll.first();
    for (int i = 0; i < 5; ++i) {
        item = ll.next();
    }

    assert((*item) == 5);
    cerr << "done\n";

    cerr << "Test insert on non-empty list...";
    item = ll.insert(40);

    assert(ll.size() == 11);
    assert(ll.current() == 40);
    assert(is_equal(ll, { 0, 1, 2, 3, 4, 40, 5, 6, 7, 8, 9 }));
    cerr << "done\n";

    item = ll.first();
    for (int i = 0; i < 5; ++i) {
        item = ll.next();
    }

    cerr << "Test removeCurrent...";
    item = ll.removeCurrent();
    assert(*item == 40);
    assert(ll.size() == 10);
    assert(ll.current() == 5);
    assert(is_equal(ll, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
    cerr << "done\n";

    item = ll.first();
    for (int i = 0; i < 5; ++i) {
        item = ll.next();
    }

    cerr << "Test append...";
    item = ll.append(50);
    assert(*item == 5);
    assert(ll.size() == 11);
    assert(ll.current() == 50);
    assert(is_equal(ll, { 0, 1, 2, 3, 4, 5, 50, 6, 7, 8, 9 }));
    cerr << "done\n";

    item = ll.first();
    for (int i = 0; i < 6; ++i) {
        item = ll.next();
    }
    item = ll.removeCurrent();
    assert(*item == 50);
    assert(ll.size() == 10);
    assert(ll.current() == 5);

    cerr << "Test last and prev...";
    item = ll.last();
    assert(*item == 9);

    item = ll.prev();
    assert(*item == 8);

    cerr << "done\n";
    return 0;
}