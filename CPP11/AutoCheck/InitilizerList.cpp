#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>




void TestInitilizeList()
{
	// C++98 examples
	// C++98 — four different styles, nothing consistent
	//C++98 had multiple different syntaxes depending on what you were initializing, 
	// and containers couldn't be initialized inline at all:
	int xx = 5;
	int arr[] = { 1, 2, 3 };
	//Point p(1, 2);           // constructor-style
	std::string s("hello");

	// Containers — had to push_back one by one
	std::vector<int> v1;
	v1.push_back(10);

	//Widget w();    // looks like object creation, actually declares a function!

	//---------------------
	// C++11 : Braces {} work uniformly across all types:
	int x2{ 5 };
	int arr2[]{ 1, 2, 3 };
	//Point p{ 1, 2 };
	std::string s2{ "hello" };
	std::vector<int> v3{ 10, 20, 30 };   // inline initialization!
	//Widget w{};    // unambiguous — creates object, never a function






	//Benefits
	//1 . inilization is done for exact type
	int c = 1000;
	char d = c;    // C++98 — silently truncates, no warning
	//char e{ c };     // C++11 — ERROR: narrowing conversion

	//gotcha
	std::vector<int> a1(5);    // 5 elements, all zero:    [0, 0, 0, 0, 0]
	std::vector<int> b1{ 5 };    // 1 element, value is 5:   [5]

	std::vector<int> c1(5, 1); // 5 elements, all one:     [1, 1, 1, 1, 1]
	std::vector<int> d1{ 5, 1 }; // 2 elements, values 5,1:  [5, 1]

}

//constexpr functions
constexpr int square(int x) { return x * x; }

//C++11 restriction: constexpr functions must be a single return statement — no if, no loops, no local variables.
constexpr int factorial(int n) {
	return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int power(int base, int exp) {
	return exp == 0 ? 1 : base * power(base, exp - 1);
}
void TestConstExpr()
{
	//constexpr tells the compiler: evaluate this at compile time. 
	// The result is embedded directly in the binary — no runtime cost at all.
	constexpr int MAX_SIZE = 100;    // guaranteed compile-time
	constexpr double PI = 3.14159;   // typed, scoped, debuggable — unlike #define

	// old methods like
	// have no type #define PI 3.14
	// might be compile time : const int size = getSize(); // run time
	// 
	// 

	
	

	constexpr int a = square(5);   // compile time — a = 25, baked into binary
	int n = 7;
	int b = square(n);  //run time — n not known at compile time

	// --- constexpr class ---
	class Circle {
	public:
		double radius;
		constexpr Circle(double r) : radius(r) {}
		constexpr double area()        const { return 3.14159 * radius * radius; }
		constexpr double circumference() const { return 2 * 3.14159 * radius; }
	};
	constexpr Circle unit{ 1.0 };
	constexpr double area = unit.area();        // computed at compile time
}
