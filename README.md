#prof

`prof` is a simple in-house profiler, written in header only c++.

##How To Use

Include the header,

```
#include "profiler.h"
```

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

results will be print out to stdout after running you program

```
$ ./a.out
-----------------------------------------------------------------------
name                            rate       total     count     us/call
Logger/format_string             41%       863ms   1000001     0.863us
Logger/push                      30%       638ms   1000001     0.638us
Logger/logging                   27%       561ms   1000001     0.561us
Logger/swap                       0%         0ms        48     2.916us

$
```

if you set `RUNTIME_PROFILING` environment variable while running your program, you can get the result every seconds.

```
$ export RUNTIME_PROFILING=true
$ ./a.out
-----------------------------------------------------------------------
name                            rate       total     count     us/call
Logger/format_string             43%       167ms    193226     0.868us
Logger/push                      32%       125ms    193225     0.648us
Logger/logging                   24%        94ms    170489     0.555us
Logger/swap                       0%         0ms         8         2us
-----------------------------------------------------------------------
name                            rate       total     count     us/call
Logger/format_string             42%       337ms    391784      0.86us
Logger/push                      31%       250ms    391783      0.64us
Logger/logging                   26%       210ms    379756     0.554us
Logger/swap                       0%         0ms        16       2.5us
-----------------------------------------------------------------------
name                            rate       total     count     us/call
Logger/format_string             42%       506ms    587611     0.861us
Logger/push                      31%       375ms    587611     0.639us
Logger/logging                   26%       314ms    565061     0.556us
Logger/swap                       0%         0ms        24     2.625us

...
```


