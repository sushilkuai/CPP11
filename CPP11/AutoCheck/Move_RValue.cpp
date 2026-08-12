#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

// C++98 — Buffer manages a heap array
class Buffer {
public:
    int* data;
    int  size;

    Buffer(int n) : size(n), data(new int[n]) {}
    ~Buffer() { delete[] data; }

    // Copy constructor — deep copy
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        for (int i = 0; i < size; i++) data[i] = other.data[i];
        std::cout << "COPY — allocated " << size << " ints\n";
    }
    //
    // Move constructor
    Buffer(Buffer&& other) : size(other.size), data(other.data) {
        other.data = nullptr;   // source is now empty — it owns nothing
        //The source is a temporary that will be destroyed anyway — so it doesn't matter that we emptied it.
        other.size = 0;
        std::cout << "MOVE — no allocation\n";
    }

    // Move assignment — steal pointer O(1)
    Buffer& operator=(Buffer&& other) {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        std::cout << "  [MOVE=]     size=" << size << "\n";
        return *this;
    }
};

Buffer createBuffer() {
    Buffer b(1000000);
    return b;             // copies 1 million ints — even though b is about to die
}

// R Values
//C++11 adds a new kind of reference that binds only to rvalues (temporaries):
//This lets you write a move constructor — 
// one that is called only for temporaries, allowing you to steal the resource instead of copying it.
int x = 42;
int& lref = x;    // lvalue reference — C++98
int&& rref = 42;   // rvalue reference — C++11, binds to temporary

//
//Buffer(Buffer&& other)            // move constructor — called for temporaries
//Buffer& operator=(Buffer&& other) // move assignment





void checkMoveRValue()
{
    Buffer a = createBuffer();   // another copy — 2 million int copies total
}