fabricator
==========

I wanted a C++ build system that doesn't suck and doesn't require eons of configuration time. To that end, I decided to prefer convention over configuration. This means that fab-compliant projects have a specific directory structure and work with specific compilers. This is by design - if you really need more power, you need a different build system.

The intent is that you ought to be able to produce a file, Fab.json, with almost nothing in it, and the fabricator tool will just "do the right thing."

Here's a simple Fab.json file:

  { "target" : "demo.exe" }

If you place this in the root of your project, running "fab" in that directory will cause fab to search "src/main" for any file ending in .cpp, and compile them. It will then attempt to link those files into "demo.exe".

You don't need to do anything else.  No compiler flags, nothing.  You *can* specify other stuff, but you don't need to.

#### boost

We use boost extensively inside fab, so as the tool matures, our support for boost will likely mature along with it. Right now it's pretty hoaky, so please bare with us.

#### automated testing

We do automated testing quite a bit, so expect fabricator to support it. We wanted to make it as simple as possible to create unit tests, without requiring you to write a bunch of support code for your tests. To provide that support, we implement this convention:

A test is a function with this signature in any .cpp module residing in src/test:

  <code>void test'anything'();</code>

A test passes if it completes without throwing any exceptions, and fails if it throws an exception.

Here is a complete example:

  <pre><code>
  #include "something.h" // thing to test
  #include &lt;stdexcept&gt;

  void test_something()
  {
    if (!something())
      throw std::runtime_error("Expected something() to return true.");
  }
  </code></pre>
