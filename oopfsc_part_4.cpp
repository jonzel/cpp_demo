#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <thread>
#include <chrono>
#include <mutex>

// compiles with g++ oopfsc_part_4.cpp -pthread
// concurrency part uncommented in main for quicker execution

// ----------------------------------------------------------------------------
// Automatic type deduction & trailing return types
// ----------------------------------------------------------------------------

// decltype from type_traits
auto adder(double a, int b) -> decltype(a + b) {
  return a + b;
}

void automatic_type_deduction() {
  // autom. type deduction:
  auto a = 1;
  auto b = 1.5;
  auto c = 2.1f;
  const auto& d = true;
  
  auto e = adder(a, 3);
}

// ----------------------------------------------------------------------------
// Compile-time evaluation
// ----------------------------------------------------------------------------
// using const expressions we can have compile time evaluation without
// template metaprogramming

constexpr int sum(int n) {
  return (n > 0) ? (sum(n-1) + n) : 0;
}

void compile_time_eval() {
  static_assert(sum(5) == 15);
}

// ----------------------------------------------------------------------------
// Move Semantics
// ----------------------------------------------------------------------------

// rule of five: copy/move/dfault constructor, move/default copy, destructor
struct MoveDemo {
  MoveDemo(int a) : a(a) {};
  ~MoveDemo() {};
  MoveDemo(MoveDemo& other)  { a = other.a; }
  MoveDemo(MoveDemo&& other) { a = std::move(other.a); };
  
  MoveDemo& operator=(MoveDemo& other) {
    return *this = MoveDemo(other);
  };
  MoveDemo& operator=(MoveDemo&& other) {
    a   = std::move(other.a);
    return *this;
  };
  
  int a;
};

void move_semantic() {
  int a = 3;
  MoveDemo A = MoveDemo(a);
  //MoveDemo B = std::move(A);
  
  // perfect forwarding (accepts both lvalue rvalue ref)
  std::vector<int> vec = {1, 43, 12, 675, 13, 7};
  for(auto&& i : vec) {
    if(i % 2 == 0) i = std::move(i + 2);
    else           i = a;
  }
}

// ----------------------------------------------------------------------------

void smart_pointers() {
// unique ptr demo
  std::unique_ptr<int> ptr_a(new int);
  *ptr_a = 5;
  
  // try to copy; does not work because pointer is unique
  std::unique_ptr<int> ptr_b;
  // ptr_b = ptr_a;
  
  ptr_b = std::move(ptr_a); // works as ptr is left unvalid

  // shared ptr demo  
  std::shared_ptr<int> ptr_c(new int);
  std::shared_ptr<int> ptr_d;
  *ptr_c = 2;
  // try to copy; works because both ptr_c is a shared pointer!
  ptr_d = ptr_c; 
  
  // weak pointers allow access but have no ability to
  // increase the pointer's life span (=they are non-owning)
  std::shared_ptr<int> ptr_shared(new int);
  *ptr_shared = 9;
  std::weak_ptr<int> ptr_weak   = ptr_shared;
  // get value via lock
  assert(*ptr_weak.lock() == 9);
  
  // reset shared pointer (ptr counter is zero since weak ptr don't count)
  ptr_shared.reset(new int);
  *ptr_shared = 4;
  assert(ptr_weak.expired()); // lock() is also false
  // all shared pointers gone => desctructor called
  // all shared & weak gone => memory becomes free
  
  // same declaration as above but used less allocs
  auto ptr_e = std::make_shared<int>(6);
  auto ptr_f = std::make_unique<int>(1);
  // also: you don't have to delete any smart pointers
  
  // remember pit falls in case of cyclic dependency
}

// ----------------------------------------------------------------------------

class lambda_functor {
public:
  double operator()(int a) {double b = a; return b += .5;}
};


void lambda_expressions() {
  // [capture list] (input par) -> return type {body};
  auto f = [] (int a) -> double { double b = a; return b += .5; };
  int a = 1;
  assert(f(a) == 1.5);
  
  //compared to functor:
  lambda_functor f_func;
  assert(f_func(a) == 1.5);
  
  // we can capture values from outside
  a = 0;
  int b = 0;
  
  auto g = [a, &b] () -> void {int c = a; ++b;};
  g();
  assert(b == 1); // was passed by reference

  int m = 1, n = 2, o = 3;
  // captures b by-reference, a and c by-value (if actually used)
  auto lamdba_a = [=,&n] () -> void {};
  // captures a by-value, b and c by-reference (if actually used)
  auto lambda_b = [&,m]  () -> void {};

}

// ----------------------------------------------------------------------------

// using the recursive approach we need a final overload
template<typename T>
int print(T head) { return head; }

// recursive main part
template<typename T, typename... Ts>
int sum(T head, Ts... tail) {
  return head + sum(tail...);
}

void variadic_templates() {
  int sum_a = sum(1, 2);
  int sum_b = sum(1, 2, 3, 4, 5, 6, 7, 8, 9);
  // recursive algorithm not working correctly yet
}

// ----------------------------------------------------------------------------

// compile this with flag -pthread
void concurrency() {
  int time_start, time_end;
  time_start = std::clock();
  std::thread thread_a( [](){
          std::this_thread::sleep_for(std::chrono::seconds(5));
        });
  std::thread thread_b( [](){
          std::this_thread::sleep_for(std::chrono::seconds(5));
        });

  thread_a.join();
  thread_b.join();
  time_end = std::clock();
  std::cout << (time_end - time_start)/double(CLOCKS_PER_SEC) << std::endl;
  
  std::mutex stop;
  
  time_start = std::clock();
  std::thread thread_c( [&stop](){
          std::lock_guard<std::mutex> lock(stop); //unsafe: stop.lock();
          std::this_thread::sleep_for(std::chrono::seconds(5));
        });
  std::thread thread_d( [&stop](){
          std::lock_guard<std::mutex> lock(stop); //unsafe: stop.lock();
          std::this_thread::sleep_for(std::chrono::seconds(5));
        });

  thread_c.join();
  thread_d.join();
  time_end = std::clock();
  std::cout << (time_end - time_start)/double(CLOCKS_PER_SEC) << std::endl;
}

// ----------------------------------------------------------------------------

class Dummy{};
enum class Planet {Earth, Mars, Saturn};


void divide_by_zero(int a) noexcept {1/a;}

void new_cpp11_features() {
  // now possible to instatiate using brackets:
  Dummy A{};
  std::vector<int> vec = {1, 2, 3, 4};
  
  // this possible with custom class but needs iterator, end(), begin()
  // range loops using :
  for(auto i : vec) {}
  for(auto&& i : vec) {} // forwarding reference, allows rvalues & lvalues
  
  //template<typename T>
  //using Vector3D = std::vector<T,3>; 
  //Vector3D<int> my_vec;
  
  // scoped enums
  Planet home = Planet::Earth;
  
  // noexcept
  divide_by_zero(0);

  // custom literals, random generator, time, regex & raw literals
}

// ----------------------------------------------------------------------------

int main() {
  automatic_type_deduction();
  compile_time_eval();
  move_semantic();
  smart_pointers();
  lambda_expressions();
  variadic_templates();
  //concurrency();  
  new_cpp11_features(); 
    
  return 0;
}


