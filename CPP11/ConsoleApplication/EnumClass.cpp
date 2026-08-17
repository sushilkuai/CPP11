#include <cstdint>
#include <cassert>
#include <type_traits>
#include <string>



// C++98
enum Direction { North, South, East, West };
enum Color { Red, Green, Blue };

//int North = 5;   // ERROR — North already defined by the enum!

//Every enumerator name pollutes the surrounding namespace — you can't have two enums with the same value name.
enum Status { /*Red,*/ OK };   // ERROR — Red already defined by Color!

//implicit conversion 
Direction d = North;
Color c = Red;

//if (d == c) {}          // compiles fine — comparing unrelated enums!
int xK = North;           // compiles fine — silent conversion to int
 

/////////////// C++11


enum class Direction1 { North, South, East, West };
enum class Color1 { Red, Green, Blue };

Direction1 d1 = Direction1::North;   // must qualify — no name leaking
Color1     c1 = Color1::Red;

// if (d == c) { }    // ERROR — different types, won't compile
// int x = d;         // ERROR — no implicit conversion to int
// Direction bad = 2; // ERROR — no implicit conversion from int


//we can control the storage type of an enum — useful for serialization, bitfields, and forward declarations:
enum class Status2 : uint8_t { OK = 0, Error = 1, Timeout = 2 };
enum class Flags : uint32_t { None = 0, Read = 1, Write = 2, Execute = 4 };

// Forward declaration — impossible with C++98 plain enums
enum class Direction2 : int;   // declare here, define elsewhere





//==================================== STATIC asserts ============================

//C++98 assert() only fires at runtime.
// Template errors produced pages of unreadable error messages with no clear indication of what went wrong :
// C++98
template<typename T>
void serialize(T value) {
    // Intended only for numeric types, but nothing stops you passing a string
    // If you pass std::string — compiles fine, crashes or corrupts at runtime
}

template<typename T>
T add(T a, T b) {
    static_assert(std::is_arithmetic<T>::value,
        "add() only works with numeric types (int, float, double, etc.)");
    return a + b;
};

int add(int a, int b) { return a + b; }
std::string add(std::string a, std::string b) {return a ; }

void C11Asserts()
{
    //static_assert(condition, message) fires at compile time — zero runtime cost, immediate clear error message :
    //If the condition is false, the build fails with your message — no runtime crash, no guessing.
    static_assert(sizeof(int) == 4, "This code requires 32-bit int");
    //static_assert(sizeof(int) == 5, "This code requires 32-bit int"); this will fail at compile time
    static_assert(sizeof(void*) == 8, "This code requires 64-bit platform");
    //this will fail incompilation
    //static_assert(sizeof(void*) == 9, "This code requires 64-bit platform");
    


    //there are various trype_traits to check.

    std::is_integral<int>::value;
    std::is_floating_point<float>::value;
    std::is_reference<int&>::value;
    std::is_const<const int>::value;
    //std::is_same<T4, int>::value;


}













void checkEnum_Asserts()
{
    // Runtime assert — too late
    assert(sizeof(int) == 4);   // crashes the program, not caught at build time

    C11Asserts();
}

