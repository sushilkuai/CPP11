#include <iostream>
#include <vector>
#include <map>
#include <string>

int heavyComputation() { return 42; }

void RunAutoCheck()
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
   
void forLoop()
{
/*
for (auto x : container)       // COPY — changes to x don't affect container
for (auto& x : container)      // REFERENCE — changes to x modify container
for (const auto& x : container) // CONST REF — read-only, no copy (most efficient for large objects)

*/
    std::vector<int> scores = { 85, 92, 78, 95, 88 };

    // Read-only — use const auto&
    std::cout << "Scores: ";
    for (const auto& s : scores)
        std::cout << s << " ";
    std::cout << "\n";

    // Modify in-place — use auto&
    for (auto& s : scores)
        s += 5;   // apply 5-point bonus

    std::cout << "After bonus: ";
    for (const auto& s : scores)
        std::cout << s << " ";
    std::cout << "\n";

    // --- 2. Works on raw arrays too ---
    int arr[] = { 1, 2, 3, 4, 5 };
    int sum = 0;
    for (const auto& x : arr)
        sum += x;
    std::cout << "Sum: " << sum << "\n";

    // --- 3. Map — element is a pair ---
    std::map<std::string, int> ages;
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages["Carol"] = 35;

    for (const auto& entry : ages) {
        // entry is std::pair<const std::string, int>
        std::cout << entry.first << " is " << entry.second << "\n";
    }

    // --- 4. The copy trap — a common C++98 habit that hurts here ---
    std::vector<std::string> names = { "Alice", "Bob", "Carol" };

    for (auto name : names)       // BAD — copies each string
        name += "!";              // modifies the copy, not the original
    std::cout << names[0] << "\n";   // still "Alice"

    for (auto& name : names)      // GOOD — modifies in place
        name += "!";
    std::cout << names[0] << "\n";   // now "Alice!"


}