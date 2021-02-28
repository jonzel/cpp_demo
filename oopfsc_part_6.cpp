#include <iostream>
#include <type_traits>
#include <tuple>
#include <vector>
#include <cassert>


// ----------------------------------------------------------------------------

template<typename T>
bool func(const T& in) {
  if constexpr (std::is_integral<T>::value) {
    return true;
  } else {
    return false;
  }
}

struct Dummy{ int a = 0; };

void compile_time_branches() {
  int a = 0;
  Dummy b;
  assert(func(a) == true);
  assert(func(b) == false);
  
  // also std::is_same_v<T1,T2> etc
}

// ----------------------------------------------------------------------------

auto structure_out() -> std::tuple<int,int,int> {
  std::tuple<int,int,int> a{1,2,3};
  return a;
}

void structured_bindings() {
  auto [x,y,z] = structure_out();
  assert(x == 1);
}


// ----------------------------------------------------------------------------

  void utility_classes() {
  
  }


// ----------------------------------------------------------------------------



int main() {
  compile_time_branches();
  structured_bindings();

  return 0;
}
