#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

// C++98 — three ways this leaks or crashes
// Smart pointers solve all three by tying the object's lifetime to a stack variable. 
// When the smart pointer goes out of scope, it automatically calls delete.

class MyClass{
public:
    void doWork() { ; }
};
// Problem 1: Forgot to delete
void process1() {
    MyClass* obj = new MyClass();
    obj->doWork();
    // forgot delete obj; → memory leak
}

// Problem 2: Exception skips delete
void process2() {
    MyClass* obj = new MyClass();
    obj->doWork();   // throws exception → delete never reached → leak
    delete obj;
}

// Problem 3: Double delete
void doubleDelete()
{
    
    MyClass* a = new MyClass();
    MyClass* b = a;
    delete a;
    delete b;   // crash — deleted twice
}

class Player {
public:
    std::string name;
    int health;

    Player(const std::string& n, int h) : name(n), health(h) {
        std::cout << "Player created: " << name << "\n";
    }
    ~Player() {
        std::cout << "Player destroyed: " << name << "\n";
    }
    void takeDamage(int d) { health -= d; }
};

// -----------------Part A : Unique pointers ---------------------
// unique_ptr says: exactly one owner. When it goes out of scope, the object is deleted. It cannot be copied — only moved.

void UniquePtrDemo()
{
    // create it, no need to call delete for this.

    std::unique_ptr<Player> p1(new Player("Boby", 100));

    //use it like a pointer
    p1->takeDamage(10);
    std::cout << p1->name << " health = " << p1->health << "\n";


    // Cannot copy — ownership is exclusive
    // std::unique_ptr<Player> p2 = p1;  // ERROR — deleted copy constructor

    // Can MOVE — transfers ownership
    std::unique_ptr<Player> p2 = std::move(p1);
    // p1 is now empty (nullptr)
    std::cout << "p1 is empty: " << (p1 == nullptr ? "yes" : "no") << "\n";
    std::cout << "p2 has: " << p2->name << "\n";

    // Container items are destroyed when the container goes out of scope
    {
        std::vector<std::unique_ptr<Player>> team;
        team.push_back(std::unique_ptr<Player>(new Player("Bob", 100)));
        team.push_back(std::unique_ptr<Player>(new Player("Carol", 90)));
        team.push_back(std::unique_ptr<Player>(new Player("Dave", 80)));

        for (const auto& p : team)
            std::cout << p->name << ": " << p->health << "\n";

        // All Players automatically deleted when team goes out of scope
    }
}


// -----------------Part B : Shared pointers ---------------------
// shared_ptr says: multiple owners. It keeps a reference count — when the last owner goes away, the object is deleted.

void sharedPtrDemo() {
    std::shared_ptr<Player> p1(new Player("Alice", 100));

    // better to use make_share
    auto p2 = std::make_shared<Player>("B", 300);
    std::cout << "Count: " << p1.use_count() << "\n";  // 1

    {
        std::shared_ptr<Player> p2 = p1;   // copy is allowed — both own it
        std::cout << "Count: " << p1.use_count() << "\n";  // 2

        p2->takeDamage(30);
        std::cout << p1->health << "\n";   // 70 — same object!
    }
    // p2 destroyed here — count drops to 1

    std::cout << "Count: " << p1.use_count() << "\n";  // 1
    // p1 destroyed at end of function — count → 0 → Player deleted
}

// -----------------Part C : Weak pointers ---------------------
// weak_ptr observes a shared_ptr without affecting the reference count. 
// Its main job is breaking circular references which would otherwise cause leaks:

// Class with shared pointer
class Node {
public: 
    std::string name;
    std::shared_ptr<Node> next;

    Node( const std::string& n) : name(n){ std::cout << "Node created: " << name << "\n"; }
    ~Node() { std::cout << "Node destroyed: " << name << "\n"; }

};

void CirculerDomo()
{
    std::shared_ptr<Node> a (new Node("a"));
    std::shared_ptr<Node> b (new Node("b"));


    a->next = b; //A owns B. B.use_count will be 2 now
    b->next = a;  // B owns A, crculer depandancy, no one will be deleted. A.use_count will be 2 now


}

//class with weak pointer
class SafeNode {
public:
    std::string name;
    std::weak_ptr<SafeNode> next;

    SafeNode(const std::string& n) : name(n) { std::cout << "SafeNode created: " << name << "\n"; }
    ~SafeNode() { std::cout << "SafeNode destroyed: " << name << "\n"; }

};

void safeCircularDemo() {
    std::shared_ptr<SafeNode> a(new SafeNode("A"));
    std::shared_ptr<SafeNode> b(new SafeNode("B"));

    a->next = b;   // weak — doesn't increase count
    b->next = a;   // weak — doesn't increase count

    // To use a weak_ptr, lock it to get a temporary shared_ptr
    if (auto locked = a->next.lock()) {
        std::cout << "A's next is: " << locked->name << "\n";
    }
    // Both safely destroyed at end of scope
}

// When to use
//Smart pointer	        Use when
//unique_ptr	        One clear owner — default choice for heap objects
//shared_ptr	        Genuinely shared ownership — caches, shared resources
//weak_ptr	            Observer that must not keep the object alive — callbacks, caches, breaking cycles
//Raw pointer*          Non - owning reference only — never use for ownership