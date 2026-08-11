#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

//These three keywords share a common theme: catching bugs at compile time that C++98 silently let through. 
// They are small additions but save real debugging time.


// nullptr
// nullptr is a real keyword with its own type: std::nullptr_t. 
// It converts to any pointer type but never to an integer:

void log(int code) { std::cout << "log(int): " << code << "\n"; }
void log(const char* msg) 
	{ 
		if (msg != nullptr) 
			std::cout << "log(char*): " << msg << "\n"; 
		else
			std::cout << "log(char*): " << "EMPTY message" << "\n";
}

class Node {
public: 
	int value;
	Node* Next;
	Node(int v) :value(v), Next(nullptr) {}
};

bool isValid(Node* n)
{
	return n != nullptr;
}

void checkNull()
{
	log(0);
	log(NULL);
	log(nullptr);

	// Pointer checks — all equivalent but nullptr is clearest
	Node* p = nullptr;
	if (p == nullptr)  std::cout << "null (nullptr style)\n";
	if (!p)            std::cout << "null (bool style)\n";
}

//-------------------
//     Override
// In C++98, if you made a typo or a signature mismatch in a derived class virtual function, 
// the compiler silently created a new function instead of overriding. No error. No warning. 
// Just wrong behavior at runtime:

// C++98 — silent bug
class Base {
public:
	virtual void render(int x) {}
	virtual void draw() const {
		std::cout << "Base::draw\n";
	}
	virtual void Finalfun() {};
};

class Derived : public Base {
public:
	virtual void render(float x) {}  // typo: float vs int
	// Compiler says nothing — this is a NEW function, not an override!
	// Base::render(int) is still called through a Base pointer

	virtual void draw() // this is new function as const is missing
	{
		std::cout << "Derive::draw\n";
	}

	
};

class Derived2 : public Base {
public:
	//override tells the compiler: 
	// "this function MUST override something in the base class — if it doesn't, give me an error."
	//Rule of thumb : Add override to every virtual function in a derived class.It is always safe — it only ever catches bugs.

	// comment the line below to avoid compiler error
	//void render(float x) override {}  // ERROR — no matching virtual in Base
	void render(int x) override {}    // OK — correctly overrides Base::render

	// Below will ask to add const.
	//virtual void draw()  override { std::cout << "Derive::draw\n"; }
};


// -----------------------------
//          final keyword

//final has two uses: 
// prevent a virtual function from being overridden further, or prevent a class from being inherited at all.

class Derived3 : public Base {
public:
	virtual void Finalfun() override final { std::cout << "Derive3::Finalfun\n"; }
};

class Derived4 : public Derived3 {
public:
	// need to comment it as Finalfun is function
	// virtual void Finalfun() override { std::cout << "Derive4::Finalfun\n"; }
};

// Prevent inheritance entirely
class Singleton final {
	// nothing can inherit from Singleton
};

// Final class can not be derived.
// class MySingleton : public Singleton {};  // ERROR — Singleton is final