#include <functional>
#include <functional>
#include <iostream>
#include <vector>



//C++98 function pointers could only store plain functions — not lambdas, not functors, not member functions without workarounds :
typedef void (*Callback)(int);   // only plain functions fit
void onEvent(int x) {}
Callback cb = onEvent;           // OK
//it is not possible to store functors, lambda etc.

//------------------C++11------------------


//std::function<ReturnType(ArgTypes...)> stores any callable with a matching signature — 
// plain function, lambda, functor, member function, or std::bind result:
std::function<int(int, int)> f;   // can hold anything callable with (int,int)->int


//std::bind creates a new callable by pre-filling some arguments of an existing callable. 
// Unfilled slots use placeholders _1, _2, etc.:
int addd(int a, int b) { return a + b; }
auto add5 = std::bind(addd, 5, std::placeholders::_1);   // pre-fill first arg as 5
int k = add5(4); //calls add(5,4)



// -----------------------------------------------
// Part A: std::function — store any callable
// -----------------------------------------------
int multiply(int a, int b) { return a * b; }

struct Adder {
	int base;
	Adder (int b) : base(b){}
	int operator()(int x) const { return base + x; }
};
void functionBasics()
{
	//store in function
	std::function<int(int, int)> f1 = multiply;

	//store in lambda
	std::function<int(int, int)> f2 = [](int a, int b) {return a + b; };

	//store in functor
	Adder adder10(10);
	std::function<int(int)> f3 = adder10;

	//call them;
	f1(3, 4);
	f2(3, 4);
	f3(20);

	//reassing to same type;
	f1 = [](int a, int b) {return a - b; };

	//check if empty
	std::function<void()> empty;

	empty = [] {std::cout << "now it has some value"; };
	empty();

}

// -----------------------------------------------
// Part B: std::function as callback — practical pattern
// -----------------------------------------------

class Button {
public :
	std::string label;
	std::function<void()> onClick; //to store any callable

	Button(const std::string& lbl) : label(lbl){}

	void click()
	{
		std::cout << "[" << label << "]" << " clicked\n";
		if (onClick)
		{
			onClick();
		}
	}

};

class Counter
{
public:
	int count = 0;
	void increment() { ++count; std::cout << "Count :" << count << "\n"; }
	void reset() { count = 0; std::cout << "RESET \n"; }
};

void CallbackDemo()
{
	std::cout << "\n=== Callback Pattern ===\n";

	Button btn("Submit");
	btn.onClick = [] {std::cout << "Lambda handler : form submitted \n"; };
	btn.click();

	//reassgined to different call back.
	int clicks = 0;
	btn.onClick = [&clicks]() {
		++clicks;
		std::cout << "  Click #" << clicks << "\n";
		};
	btn.click();
	btn.click();

	// store callbacks in vector
	std::vector<std::function<void()> > handlers;
	handlers.push_back([] {std::cout << "  Handler 1\n"; });
	handlers.push_back([] {std::cout << "  Handler 2\n"; });
	handlers.push_back([] {std::cout << "  Handler 3\n"; });

	Button multiBtn("MultiButton");
	multiBtn.onClick = [&handlers] {
		for (auto& h : handlers)
			h();
		};
	multiBtn.click();
}

// ---------------------------------------------- -
// Part C: std::bind — partial application
// -----------------------------------------------

int add_1(int a, int b) { return a + b; }
int subtract_1(int a, int b) { return a - b; }
bool isGreaterThan(int value, int threshold) { return value > threshold; }
void log_1(const std::string& level, const std::string& msg) {
	std::cout << "[" << level << "] " << msg << "\n";
}
void bindDemo() {
	std::cout << "\n=== std::bind ===\n";
	using namespace std::placeholders;
	// Pre-fill first argument
	auto add100 = std::bind(add_1, 10, _1);
	std::cout << "add10(7):      " << add100(7) << "\n";   // 17

	// Reorder arguments — _2 first, _1 second
	auto flipped = std::bind(subtract_1, _2, _1);  // subtract(_2, _1)
	std::cout << "flipped(3,10): " << flipped(3, 10) << "\n"; // 10-3 = 7

	// Pre-fill a string argument — logging with fixed level
	auto ErrorLog = std::bind(log_1, "ERROR", _1);
	auto FatalLog = std::bind(log_1, "FATAL", _1);
	ErrorLog("File not found");
	FatalLog("Server started");

}

void checkFunction_bind()
{
	functionBasics();
	CallbackDemo();

	bindDemo();
}