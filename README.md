fabricator
==========

I wanted a C++ build system that doesn't suck and doesn't require eons of configuration time. To that end, I decided to prefer convention over configuration. This means that fab-compliant projects have a specific directory structure and work with specific compilers. This is by design - if you really need more power, you need a different build system.

The intent is that you ought to be able to produce a file, Fab.json, with almost nothing in it, and the fabricator tool will just "do the right thing."

Here's a simple Fab.json file:

```json
{ "target" : "demo.exe" }
```

If you place this in the root of your project, running "fab" in that directory will cause fab to search "src/main" for any file ending in .cpp, and compile them. It will then attempt to link those files into "demo.exe".

You don't need to do anything else.  No compiler flags, nothing.  You *can* specify other stuff, but you don't need to.

#### boost

We use boost extensively inside fab, so as the tool matures, our support for boost will likely mature along with it. Right now it's pretty hokey, so please bare with us.  See the source code to get an idea of what we mean by hokey.

#### automated testing

We do automated testing quite a bit, so expect fabricator to support it. We wanted to make it as simple as possible to create unit tests, without requiring you to write a bunch of support code for your tests. To provide that support, we implement this convention:

A test is a function with this signature in any .cpp module residing in ```src/test```:

```c++
void test/*anything*/();
```

A test passes if it completes without throwing any exceptions, and fails if it throws an exception.

Here is a complete example:

```c++
#include "something.h" // thing to test
#include <stdexcept> // for std::runtime_error

void test_something()
  {
    if (!something())
      throw std::runtime_error("Expected something() to return true.");
  }
```

#### Some notes

Fabricator is currently hard-coded to work w/ g++-4.9 as installed by brew on OS/X. The plan is to modify it to handle other compilers at some point, but we only intend to support c++-11 at this time.

Likewise, it also requires gobjdump - install binutils from brew for that guy. We use gobjdump to find out after compiling an object (.o) file which functions the original .cpp file had.  This is way easier than trying to parse the c++ to find out. 

The gobjdump trick is how we automatically determine which tests are present in src/test - just dump the list of free functions whose names start with "test" and whose argument list is "()".

The convention for fabricator is that source code goes in src/main (or subdirectories thereof, organized to your liking), and test code goes in src/test.  The only function you cannot write unit tests for, of course, is main(), because that function ends up being reserved for the test harness.

Speaking of test harnesses, fabricator will generate a file in the root of your project called .test_harness.cpp, and then compile & link that to .test_harness.exe, and finally run that program, which will run all of your unit tests.
