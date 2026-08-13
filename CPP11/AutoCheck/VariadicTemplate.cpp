
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

//Before variadic templates, if you wanted a function that accepted any number of arguments, 
// you either used unsafe C-style va_args or wrote a separate overload for each count:
// C++98 — had to write every combination manually
template<typename T1>
void print(T1 a) { std::cout << a << "\n"; }

template<typename T1, typename T2>
void print(T1 a, T2 b) { std::cout << a << " " << b << "\n"; }

template<typename T1, typename T2, typename T3>
void print(T1 a, T2 b, T3 c) { std::cout << a << " " << b << " " << c << "\n"; }
// ... and so on

// Or unsafe C-style varargs — no type safety at all
void unsafePrint(const char* fmt, ...) { /* va_list, va_arg, va_end */ }

//-------------------------

// 
// typename... Args declares a template parameter pack — it holds zero or more types.Args... expands that pack :

template<typename... Args>   // Args = pack of zero or more types
void print1(Args... args);    // args = pack of zero or more values

//C++11 has recursive unpacking
// Base case — called when pack is empty
void print1() {
    std::cout << "\n";
}

// Recursive case — peel off first, recurse with rest
template <typename T, typename ... Rest>
void print1(T first, Rest ... rest)
{
    std::cout << first;
    if (sizeof... (rest) >= 0)   // sizeof... counts elements in pack
    {
        std::cout << " ";
    }
    print1(rest ...);   // expand rest and recurse
  
}

// -----------------------------------------------
// Example 1: type-safe print — any types, any count
// -----------------------------------------------
void print() { std::cout << "\n"; }

template<typename T, typename... Rest>
void print(T first, Rest... rest) {
    std::cout << first;
    if (sizeof...(rest) > 0) std::cout << " ";
    print(rest...);
}

// -----------------------------------------------
// Example 2: sum — any number of numeric arguments
// -----------------------------------------------
template<typename T>
T sum(T val) { return val; }   // base case

template<typename T, typename... Rest>
T sum(T first, Rest... rest) {
    return first + sum(rest...);
}

// -----------------------------------------------
// Example 3: max of any number of values
// -----------------------------------------------
template<typename T>
T myMax(T val) { return val; }

template<typename T, typename... Rest>
T myMax(T first, Rest... rest) {
    T restMax = myMax(rest...);
    return first > restMax ? first : restMax;
}

// -----------------------------------------------
// Example 4: type-safe string builder
// -----------------------------------------------
/*
void buildString(std::ostringstream&) {}   // base case

template<typename T, typename... Rest>
void buildString(std::ostringstream& oss, T first, Rest... rest) {
    oss << first;
    buildString(oss, rest...);
}

template<typename... Args>
std::string concat(Args... args) {
    std::ostringstream oss;
    buildString(oss, args...);
    return oss.str();
}
*/
// -----------------------------------------------
// Example 5: sizeof... — count pack elements
// -----------------------------------------------
template<typename... Args>
void printInfo(Args... args) {
    std::cout << "Argument count: " << sizeof...(args) << "\n";
    std::cout << "Type count:     " << sizeof...(Args) << "\n";
    print(args...);
}

// -----------------------------------------------
// Example 6: factory function — construct any type
//            with any constructor arguments
// -----------------------------------------------
template<typename T, typename... Args>
T* create(Args... args) {
    return new T(args...);
}

struct Config {
    std::string host;
    int port;
    bool secure;
    Config(const std::string& h, int p, bool s)
        : host(h), port(p), secure(s) {
    }
};

void checkVarTemp()
{
    print("H", 3, 3.4, "test");
    print();


    std::cout << "=== Type-safe print ===\n";
    print(1, 2.5, "hello", true);
    print("mixed:", 42, 3.14, 'A');
    print();   // base case — just newline

    std::cout << "\n=== Sum ===\n";
    std::cout << sum(1, 2, 3, 4, 5) << "\n";           // 15
    std::cout << sum(1.1, 2.2, 3.3) << "\n";           // 6.6
    std::cout << sum(10) << "\n";                       // 10

    std::cout << "\n=== Max ===\n";
    std::cout << myMax(3, 1, 4, 1, 5, 9, 2, 6) << "\n";  // 9
    std::cout << myMax(3.14, 2.71, 1.41) << "\n";         // 3.14

    //std::cout << "\n=== String concat ===\n";
    //std::string s = concat("Hello", ", ", "World", "! ", 42);
    //std::cout << s << "\n";

    std::cout << "\n=== sizeof... ===\n";
    printInfo(10, 3.14, "test");

    std::cout << "\n=== Factory ===\n";
    Config* cfg = create<Config>("localhost", 8080, true);
    std::cout << cfg->host << ":" << cfg->port
        << (cfg->secure ? " (secure)" : "") << "\n";
    delete cfg;

}