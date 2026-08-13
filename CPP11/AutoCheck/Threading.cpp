#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <mutex>
#include <future>

void printNumbers(int start, int end, const std::string& label)
{
	for (int i = start; i <= end; i++)
	{
		std::cout << label << " : " << i<<"\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void threadBasics()
{
	//launch thread with regular function
	std::thread t1(printNumbers, 1, 5, "T-1");

	//launch thread with lambda function
	std::thread t2([] {
		for (int i = 0; i < 5; i++)
		{
			std::cout << "Lambda Thread :" << i << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		});

	//Rule: Every std::thread must be either join()ed or detach()ed before it goes out of scope,
	// otherwise the program calls std::terminate().
	t1.join();
	t2.join();
	std::cout << "both threads end";
}

//-------------------------------------------------------------
// Race condition : Mutex

int counter = 0;
std::mutex mtx;

void incrementUnsafe()
{
	for (int i = 0; i < 100000; i++)
		counter++;
}

void incrementSafe()
{
	for (int i = 0; i < 100000; i++)
	{
		std::lock_guard<std::mutex> lock(mtx); // locks on construction
		counter++;  // only one thread here at a time
		// lock releases automatically when lock goes out of scope
		//std::lock_guard is RAII for mutexes — locks on construction, unlocks on destruction. Never forget to unlock.
		
	}
}

void RaceConditionDemo()
{
	//unsafe
	counter = 0;
	std::thread t1(incrementUnsafe);
	std::thread t2(incrementUnsafe);

	t1.join();
	t2.join();
	std::cout << "Unsafe result:  " << counter << " (expected 200000)\n";
	// result will be less than 200000

	counter = 0;
	std::thread t3(incrementSafe);
	std::thread t4(incrementSafe);
	t3.join();
	t4.join();
	std::cout << "Safe result:    " << counter << " (expected 200000)\n";
	// Always exactly 200000
}

//-------------------------------------------------------------
// Race condition : Atomic
//For simple types like counters, std::atomic is faster than a mutex — no locking overhead:
//Use std::atomic for counters, flags, simple values.Use std::mutex when protecting a block of code or complex data structures.

std::atomic<int> atomicCounter{ 0 };

void incrementAtomic() {
	for (int i = 0; i < 100000; i++)
		atomicCounter++;    // atomic — guaranteed correct, no mutex needed
}
void atomicDemo() {
	std::thread t1(incrementAtomic);
	std::thread t2(incrementAtomic);
	t1.join();
	t2.join();
	std::cout << "Atomic result: " << atomicCounter << "\n";  // always 200000
}

//----------------------------------------
// Async and future
// std::async launches a task and gives you a std::future to retrieve the result later .
// cleaner than raw threads for parallel computations:

long long sumRange(int start, int end) {
	long long s = 0;
	for (int i = start; i <= end; i++) s += i;
	return s;
}

void asyncDemo()
{
	//std::launch::async forces a new thread. Without it, the implementation may defer execution.
	std::future<long long> f1 = std::async(std::launch::async, sumRange, 1, 500000);
	std::future<long long> f2 = std::async(std::launch::async, sumRange, 500001, 1000000);

	// Both running in parallel while main thread continues
	std::cout << "Computing in parallel...\n";

	// get() blocks until result is ready
	long long result1 = f1.get();
	long long result2 = f2.get();
	std::cout << "Sum 1-500000:      " << result1 << "\n";
	std::cout << "Sum 500001-1000000:" << result2 << "\n";
	std::cout << "Total:             " << result1 + result2 << "\n";
}



// test all 
void CheckThreads()
{
	//threadBasics();

	RaceConditionDemo();
	atomicDemo();
	asyncDemo();
}