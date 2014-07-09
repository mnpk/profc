#profc

`profc` is a simple, light weight performance profiler written in c++.

##Example

```c++
#include "profc.h"

void Foo() {
  PROFC_NODE("Foo");
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

int main() {
  Foo();
  Foo();
  return 0;
}
```

`PROFC_NODE` macro will automatically start to measure and ends with the scope.

results will be print out to stdout after your program running.

```
$ ./a.out
--------------------------------------------------------------
name                           count      elapsed      us/call
Foo                                2          2ms       1102us
```

