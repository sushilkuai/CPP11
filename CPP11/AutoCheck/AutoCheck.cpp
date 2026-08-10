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

    return ;
}