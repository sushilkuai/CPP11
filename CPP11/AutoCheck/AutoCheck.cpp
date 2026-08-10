#include <iostream>
#include <vector>
#include <map>
#include <string>

int heavyComputation() { return 42; }

static void RunAutoCheck()
{
    std::cout << "hello Auto Check";
    // 1. Basic deduction
    auto i = 10;          // int
    auto d = 10.5;        // double
    auto b = true;        // bool

    // 2. With containers — the real win
    std::map<std::string, std::vector<int>> scores;
    scores["Alice"] = { 90, 85, 92 };
    scores["Bob"] = { 78, 88, 95 };

    // C++98 way (comment this back in to feel the pain):
    // for (std::map<std::string,std::vector<int>>::iterator it = scores.begin(); ...

    // C++11 way:
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        std::cout << it->first << ": ";
        for (auto score : it->second)   // range-for is also C++11 — preview!
            std::cout << score << " ";
        std::cout << "\n";
    }

    // 3. auto with function return values
    auto result = heavyComputation();  // type changes? auto adapts.

    // 4. auto& — deduce reference (avoids copy)
    std::vector<std::string> names = { "Alice", "Bob", "Charlie" };
    for (auto& name : names)      // auto& = reference, no copy
        name += "!";              // modifies in place

    for (const auto& name : names)   // const auto& = read-only, no copy
        std::cout << name << "\n";
    //--------------------------
    //5. auto deduces the value type, stripping refs and const:
    //const int ci = 42;
    //auto x = ci;   // x is int, NOT const int — cv-qualifiers stripped!
    //auto& y = ci;  // y is const int& — ref preserves constness

    // Auto strips const
    // The rule is: auto deduces the value type, stripping top-level const and references.
    const int cj = 42;
    auto x = cj;   // x is int — a fresh copy, free to modify
    x = 100;       // perfectly fine

    //auto& y = ci; is a reference — it's an alias to ci itself, not a copy. Since ci is const int, 
    // you cannot bind a non-const reference to it. The compiler is forced to deduce const int&.
    auto& y = cj;
    //y = 100; in correct

    return;
}
   
static void forLoop()
{


}