#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

//Whenever you needed a custom comparison or a small operation to pass to an STL algorithm, 
// you had to write a separate functor class — far away from where it was used :
// C++98 — functor defined far from use
struct ScoreCompare {
    bool operator()(const std::pair<std::string, int>& a,
        const std::pair<std::string, int>& b) {
        return a.second > b.second;  // sort by score descending
    }
};

// ...50 lines later...
//std::sort(players.begin(), players.end(), ScoreCompare());

//But C++11 we can use like below
//std::sort(players.begin(), players.end(),
//    [](const auto& a, const auto& b) { return a.second > b.second; });


// ------------SYNTAX------------
// [capture](parameters) -> return_type { body }
// Part	Meaning
//[capture]	Which outer variables the lambda can see — the key concept
//(parameters)Just like a regular function's parameters
//->return_type	Optional — compiler deduces it if omitted
// {    body }	The function body
//-------------------------------

static void capture()
{
    int threshold = 50;

    // [] capture nothing from outer world
    auto above = [](int x) {return x > 50; };

    // [=]  capture threshold BY VALUE (copy)
    auto above1 = [=](int x) { return x > threshold; };

    // [&]  capture threshold BY REFERENCE
    auto above2 = [&](int x) { return x > threshold; };

    // [threshold]  capture only threshold, by value
    auto above3 = [threshold](int x) { return x > threshold; };

    // [=, &a] cupure all by copy except a by reference
    auto above4 = [=, &threshold](int x) {return x > threshold; };

}
static void LambdaCheck()
{
    // 1. Basic Lambda
    auto greet = [](const std::string& name)
        {
            std::cout << "\nHello " << name << "!\n";
        };

    greet("Bob");

    // 2. STL Sorting
    std::vector<int> v = { 5, 2, 8, 1, 9, 3 };

    // This sort will do soring ascending
    std::sort(v.begin(), v.end());

    //this will do descending
    std::sort(v.begin(), v.end(), [](int a, int b) {return a > b; });

    for (const auto& x : v)
        std::cout << x << " ";
    std::cout << "\n";

    // 3. Capture by value [=] ---
    int threshold = 5;
    auto above_threshold = [=](int x) { return x > threshold; };

    // count_if — count elements matching a condition
    int count = std::count_if(v.begin(), v.end(), above_threshold);
    std::cout << "Above " << threshold << ": " << count << "\n";

    // 4. Capture by reference [&] ---
    int total = 0;
    std::for_each(v.begin(), v.end(),
        [&](int x) { total += x; });   // modifies total in outer scope
    std::cout << "Total: " << total << "\n";

    // 5. find_if — first element matching condition ---
    auto it = std::find_if(v.begin(), v.end(),
        [](int x) { return x % 2 == 0; });   // first even number

    if (it != v.end())
        std::cout << "First even: " << *it << "\n";


    // 6. Mutable lambda — modify captured copy ---
    // When you capture by value [x], the lambda gets its own copy of x. 
    // But by default, that copy is const inside the lambda — you cannot modify it.
    //  mutable removes that const, letting you modify the internal copy.
    // The internal copy is not reset between calls.

    // it is helpfull in Stateful callbacks — a lambda that remembers something between calls:
    int counter = 0;
    auto increment = [counter]() mutable {
        ++counter;              // modifies the COPY, not the original
        std::cout << "Inside: " << counter << "\n";
        };
    increment();   // Inside: 1
    increment();   // Inside: 2
    std::cout << "Outside: " << counter << "\n";  // still 0!

}