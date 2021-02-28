#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <stack>
#include <queue>
#include <algorithm>
#include <random>
#include <utility>
#include <tuple>
#include <exception>

// ----------------------------------------------------------------------------
// Input/Output
// ----------------------------------------------------------------------------

void io_streams() {
	std::cout << "This is output!" << std::endl;
	std::cerr << "This is error stream!" << std::endl;
	std::clog << "This too, but buffered!" << std::endl;	
}

// ----------------------------------------------------------------------------
// Container Classes
// ----------------------------------------------------------------------------

void containers() {
  // sequential
  int arr[3] = {1,2,3};
	std::vector<double> a(2,3); // a = {3,3}
	assert(a[1] == 3);
	
	std::vector<int>  b{1,2};
	assert(b[1] == 2);
	
	std::list<double> c = {7, 8, 9}; 
	c.push_front(1);  // or push_back
	assert(c.front() == 1); // or back()
	
	// container adaptors
	std::stack<int> d;
	std::queue<int> e;
	d.push(2);
	e.push(2);
	d.push(8);
	e.push(8);
	assert(d.top() == 8);
	assert(e.front() == 2);

}

// ----------------------------------------------------------------------------
// Iterators
// ----------------------------------------------------------------------------

void iterators() {
  std::vector<int> vec { 1, 8, 2, 8, 4 ,6, 13 };
	std::vector<int>::iterator iter; 
	
	for(iter = vec.begin(); iter != vec.end(); iter++) {
		std::cout << *iter << std::endl;
	}
	
	// reverse iteration
	std::vector<int>::reverse_iterator riter; // uses different iterator
	for(riter = vec.rbegin(); riter != vec.rend(); riter++) {
		std::cout << *riter << std::endl;
	}

}

// ----------------------------------------------------------------------------
// Algorithms
// ----------------------------------------------------------------------------
void print(const int& i)
  {std::cout << i << std::endl; }

void increment(int& i) 
	{ ++i; }
	
bool is_nine(const int& i)
  { return (i == 9); }
	
// functor for shuffle
bool less (const int& left, const int& right) {
		return left < right;
}

void algorithms() {
  std::vector<int> vec { 1, 8, 2, 8, 4 ,6, 13 };
  
  // for each element do increment()
  std::for_each(vec.begin(), vec.end(), increment);
	
	// count all elements == 9 (compare mode)
	int target = 9;
	assert(std::count(vec.begin(), vec.end(), target) == 2);
	// count all elements == 9 (predicate mode)
  assert(std::count_if(vec.begin(), vec.end(), is_nine) == 2);


  // find first element that is even using lambda
  auto ptr = std::find_if(vec.begin(), vec.end(), [] (int i){return i % 2;});
  assert( *ptr == 2); // ptr is type std::vector<int>::iterator
  
  
	std::random_device rd; // for random order
  std::mt19937 g(rd());  // for random order
  std::shuffle(vec.begin(), vec.end(), g);
  std::for_each(vec.begin(), vec.end(), print);

	std::sort(vec.begin(), vec.end());
	std::for_each(vec.begin(), vec.end(), print);
	
	// with functor
	std::shuffle(vec.begin(), vec.end(), g);
	std::sort(vec.begin(), vec.end(), less);
	std::for_each(vec.begin(), vec.end(), print);
}

// ----------------------------------------------------------------------------
// Companion Classes
// ----------------------------------------------------------------------------

void companion_classes() {
  std::pair<int, std::string> number;
  number.first = 1;
  number.second = "one";

  std::tuple<int,int,int> tuple {1, 2, 3};
  assert(std::get<1>(tuple) == 2); // [x,y,z] = tuple in cpp17
}

// ----------------------------------------------------------------------------
// Exceptions
// ----------------------------------------------------------------------------

void exceptions() {
  assert(2 == 1+1);
  static_assert(2 == 1+1);
  
  bool err;
  
  try {
    std::vector<int> vec {1,2,3};
    vec.at(10) = 0;
  } catch(std::exception e) {
    // would also work with less general std::out_of_range
    err = true;
  }
  assert(err);
}

// ----------------------------------------------------------------------------

int main() {
	io_streams();
	containers();
	iterators();
	companion_classes();
    exceptions();
}
























