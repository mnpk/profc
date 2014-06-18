#prof

`prof` is a simple in-house profiler, written in header only c++.

##How To Use

Include header file,
    #include "profiler.h"

and define profile nodes wherever in your code.

```
void LoggerBase::debug(const std::string& section, const char* format, ...)
{
	DEFINE_PROFILE("LoggerBase::debug");

	...

	return;
}
```
`DEFINE_PROFILE` macro automatically start to measure and ends with the scope link `scoped_ptr`.
