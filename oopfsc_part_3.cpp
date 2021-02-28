#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

// ----------------------------------------------------------------------------
// Template Specialization
// ----------------------------------------------------------------------------

// 1. base
template<typename T, typename U>
struct Numbers {
  Numbers(T a, U b) : a(-1), b(b) {};
  T a;
  U b;
};

// 2. partial specialization
template<typename T>
struct Numbers<T,int> {
  Numbers(T a, int b) : a(-2), b(b) {};
  T a;
  int b;
};

// 3. full specialization
template<>
struct Numbers<long, long> {
  Numbers(long a, long b) : a(-3), b(b) {};
  long a;
  long b;
};

// function template
template<typename T>
T increment(T a) {return ++a;}

// full specialization
template<>
int increment<int>(int a) {return a+=2;}

void template_specialization() {
  Numbers<float,unsigned int> a(1.1f, 2);
  Numbers<double,int> b(2.2, 3);
  Numbers<long,long> c(1LL, 2LL);
  
  assert(a.a == -1);
  assert(b.a == -2);
  assert(c.a == -3);
  
  // works the same for function template specialization, but
  // partial specialization is not allowed
  assert(3.f == increment(2.f));
  assert(3 == increment(1));
  
  // function overloads are resolved by: 
  // base templates > normal functions > specializations
}

// ----------------------------------------------------------------------------
// Templates and inheritance
// ----------------------------------------------------------------------------

// independent (= no template parameters)
struct IBase {typedef int T;};

// derived template class
template<typename T>
struct TDerived : IBase { T value; };

void template_inheritance() {
  TDerived<unsigned int> a;
  a.value = -1;
  assert(a.value == -1);
  
  // loop up for names:
  // in class > in independent base class > template arguments
  
  // when inheriting from dependent base classes use this-> or
  // Base<T>:: to avoid ambiguity
}

// ----------------------------------------------------------------------------
// Resource aquisition is initialization
// ----------------------------------------------------------------------------

template<typename T>
class Ptr {
public:
	Ptr() : ptr(new T) {}
	~Ptr() { delete ptr; }
	T* operator->() { return ptr; }
private:
	T* ptr;
}; // see smart pointers for standard library pendant

struct RAII_Demo {
	void do_something() { }
	Ptr<int> pointer;
};

void raii() {
  // Idea: Free resources in destructor = heap resources have handle on stack
  
  try {
    RAII_Demo A; // object stored on stack
    A.do_something();
  } // leaving try-block triggers destructor
	catch (std::bad_alloc) { }
}


// ----------------------------------------------------------------------------
// Template Metaprogramming
// ----------------------------------------------------------------------------

// recursive part
template<int N>
struct Factorial{enum {value = Factorial<N-1>::value * N};};

// recursive stop part
template<>
struct Factorial<0>{enum {value = 1};};

void template_metaprogramming() {
  // at compile time
  static_assert(Factorial<4>::value == 24);
}

// ----------------------------------------------------------------------------
// Dynamic Polymorphism (at runtime/late binding)
// ----------------------------------------------------------------------------


struct PBase {
  virtual int increase(int i) {return i;};
  ~PBase() {}; 
  int a = 0;
};

struct ChildA : PBase {
  int increase(int i) override {return ++i;};
  ~ChildA() {};
};


void dynamic_poymorphism() {
  int i = 0;
  ChildA a;
  PBase& b = a; 
	assert(b.increase(i) == 1);
	
	ChildA* a_ptr = new ChildA;
	PBase*  b_ptr = a_ptr;
	delete a_ptr; // call child destructor
	
	// we also have abstract base classes (pure virtual)
}

// ----------------------------------------------------------------------------
// Static polymorphism (at compile time/early binding)
// ----------------------------------------------------------------------------

// curiously recurring template pattern
template<typename T> 
struct BaseClass {
	int increase(int i) {return static_cast<T*>(this)->increase(i);};
};

// we want to be able to swap the derived class
struct DerivA : BaseClass<DerivA> {
	int increase(int i) { return i+=2; };
};

struct DerivB : BaseClass<DerivB> {
	int increase(int i) { return i+=4; };
};

void static_polymorphism() {
  BaseClass<DerivA> A;
  BaseClass<DerivB> B;
  
  assert(A.increase(0) == 2);
  assert(B.increase(0) == 4);
}

// ----------------------------------------------------------------------------
// SFINAE (todo)
// ----------------------------------------------------------------------------

int main() {
  template_specialization();
	template_inheritance();
	template_metaprogramming();
	dynamic_poymorphism();
	static_polymorphism();
	
	
	/* types of polymorphism: +++++++++++++++++++++++++++++++++++++++++++++++++
		
		dynamic: (late binding, functions are chosen at runtime)
	*/
	
	// slicing
	
	
	// dynamic polymorphism
	//Interface *inter = new Implementation();
	//std::cout << inter->increment(1) << std::endl;
	
	// static polymorphism (at complile time!)
	// Curiously Recurring Template Pattern:
	//BaseClass<Deriv> crtp_example;
	//crtp_example.say_hello(); (not figured out yet)
	
	// SFINAE
	// todo

  std::cout << "Prgram end has been reached.";
	return 0;
}
