#profc

**profc** is a simple, light weight performance profiler written in c++.

##Example

Simply add `PROFC_NODE` macro into your code.

```c++
#include <thread>
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

The macro will automatically start to measure and ends with the scope.

Results will be print out to stdout after your program running.

```
$ ./a.out
--------------------------------------------------------------
name                           count      elapsed      us/call
Foo                                2          2ms       1102us

$
```

