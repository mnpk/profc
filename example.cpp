#include "profc.h"

void Foo() {
  PROFC_NODE("Foo");
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
void Bar() {
  PROFC_NODE("Bar");
  std::this_thread::sleep_for(std::chrono::microseconds(50));
}

int main() {
  Foo();
  Foo();
  Bar();
  return 0;
}

