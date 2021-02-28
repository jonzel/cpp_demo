#include <iostream>
#include <cassert>

// ----------------------------------------------------------------------------

constexpr int sum(int n) {
  int out = 0;
  for(int a = 0; a < n; n++) out += a;
  return out;
}

// generic lambda expressions

void constexpr_update() {
  // sum evaluated at compile time
  assert(sum(3) == 6);

}

// ----------------------------------------------------------------------------

void variable_templates() {
  // see above
}

// ----------------------------------------------------------------------------




// ----------------------------------------------------------------------------


int main() {
  constexpr_update();
  variable_templates();


}


