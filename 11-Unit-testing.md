# Unit Testing

## Motivation

You're the quality control manager for the Confabulator, a hot new product in development by Acme Inc. LLC.
Every day, engineers give you new prototype confabulators to test and make sure they meet the ill--defined
and entirely made--up requirements upper management has set for the product.
You put each confabulator through its paces --- to the best of your memory of what those paces are ---
and make a report for the designers telling them what works and what doesn't.

Usually they give you the same prototype back with some fixes for the problems you reported the previous day.
Testing confabulators takes up time you could otherwise use productively, maybe for complaining about life to your coworkers or taking extended lunch breaks.
So you don't always put the fixed-up prototype through the full battery of tests --- you just test the stuff that got fixed.
Unfortunately, sometimes (as engineers are wont to do) a fix gets added that affects something else in an entirely different spot in the system!
You'll never forget the endless meetings after it took you a week to discover that a fix to the frobnicator interfered with the rotagration arm.
Those engineers were *mad*. How could you tell them everything is fine when something was broken?

Even worse, it's hard to remember exactly what tests you're supposed to do each time, and management seems to keep changing their minds
on exactly what features a confabulator is supposed to have.
Last month it was just supposed to be for annularity congruification, but then a contract with Statorfile Exceed GmbH. came through
and now it also has to calabricate the vibrosity of splinal conformities.
The number of things you have to check for just seems to get bigger and bigger, and of course once you add on a vibrous harmonicator,
you have to check that it works regardless of whether the radiometer intensimission is engaged or disengaged.
It seems like every new whizbang adds a half a dozen whatsits and thus a gross more tests[^gross] for you.

But the worst thing of all is that you have to do this all by hand, day in and day out.
Nothing rots the brain faster than the dull monotony of doing something you're ambivalent about.
Heck, if you're going to be experiencing dull monotony either way, the least your boss could do is let you watch some reality TV.
But nooooo, apparently all the monotony must be job--inflicted.

Fortunately, computers have solved this problem!
Instead of manually testing your program, you can write *unit tests* that test each piece for you.
Once you add a new feature and write tests for it, you can run your program through a full test suite to make sure everything works as intended.
Rather than mind-numbingly checking everything by hand, you can experience the monotony of writing a test once and then forget about it forever
(or at least until it fails and you have to figure out what you broke).

Unit testing is widely used in industry because it is quite effective at keeping bugs out of code.[^pedantry]
You can even measure how much of your code is tested by unit tests --- 100% code coverage means that you've found
at least most of the obvious bugs!

This chapter will focus on the Catch unit testing framework for C++.
There are a number of popular unit testing frameworks; Boost has one,[^sink] Google makes one called `gtest`, etc.
However, Catch is easy to install, easy to write tests in, and downright beautiful compared to Boost's test framework.
(It's also popular, in case you were wondering.)

### Takeaways

- Learn how to write unit tests with Catch
- Organize your code and tests to preserve your sanity
- Measure how much of your code is covered by the tests you've written

## Walkthrough

### Setting up Catch

Catch is distributed as a single `.hpp` file that you can download and include in your project.
Download it from [GitHub](https://github.com/philsquared/Catch) --- the link to the single header is in the `README`.

In *exactly one* `.cpp` file, you must include the following lines:

```c++
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
```

This generates a `main()` function that runs your unit tests.
You will have two programs now --- your actual program, and a program that runs your unit tests.

Every other file you write tests in should include Catch:

```c++
#include "catch.hpp"
```

Later, we'll discuss how best to organize your tests, so don't worry too much about the "right" place to put the main function yet.
For now, just throw it at the top of a `test.cpp` file, and let's write some tests!

### Basic Tests

We are going to test a function that generates Fibonacci numbers (1, 1, 2, 3, 5, 8, ...).
Here's our function:

```{.cpp .numberLines}
/* Generate the Nth Fibonacci number */
int fibonacci(int n)
{
	if(n <= 1)
	{
		return n;
	}
	else
	{
		return fibonacci(n - 1) + fibonacci(n - 2);
	}
}
```

In Catch, every test lives inside a `TEST_CASE` block.
You can make these as fine-grained as you want, but generally you'll find it easy to collect a bunch of related checks
into one `TEST_CASE`.
Each test case has a name and a tag; generally you'll tag all test cases for a function/class with the same tag.
(You can tell Catch to run only tests with specific names or tags if you like.)

Inside a test case, you can put one or more `REQUIRE` or `CHECK` assertions.
A `REQUIRE` statement checks that a certain condition holds; if it does not, it reports a test failure and stops the execution of that test case.
`CHECK` is similar to require, but if the condition does not hold, it reports a test failure and keeps running the test case.
Usually, you use `REQUIRE` when a failure indicates that the code is broken enough that it does not make sense to keep going with the test.

In general, when writing tests, you want to test every path through your code at least once.
Here's a pretty good test for our Fibonacci function:

```{.cpp .numberLines}
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Fibonacci", "[Fibonacci]") // Test name and tag
{
	CHECK(fibonacci(0) == 1);
	CHECK(fibonacci(1) == 1);
	CHECK(fibonacci(2) == 2);
	CHECK(fibonacci(5) == 8);
}
```

If we compile and run this code, we get the following output:

```
fibonacci.cpp:19: FAILED:
  CHECK( fibonacci(0) == 1 )
with expansion:
  0 == 1

fibonacci.cpp:21: FAILED:
  CHECK( fibonacci(2) == 2 )
with expansion:
  1 == 2

fibonacci.cpp:22: FAILED:
  CHECK( fibonacci(5) == 8 )
with expansion:
  5 == 8

===================================
test cases: 1 | 1 failed
assertions: 4 | 1 passed | 3 failed
```

Oh no! We have a bug![^hand]
In fact, it is in the `return n;` statement in our function on line 6 --- it should be `return 1;` instead.
If we fix that and re--run our tests, everything is kosher:

```
All tests passed (4 assertions in 1 test case)
```

Now, you may notice that Catch expands the thing inside the `CHECK` function --- it prints the value that `fibonacci` returns.
It does this by the power of *template magic*.
This magic is only so powerful.[^errors]
So, if you want to write a more complex expression, you'll need to either break it into individual assertions or tell Catch to not attempt to expand it.
For 'and' statements, rather than `CHECK(x && y);`, write `CHECK(x); CHECK(y);`.
For 'or' statements, enclose your expression in an extra pair of parentheses: `CHECK((x || y));`.
(The extra parentheses tell Catch to not attempt to expand the expression; you can do this with 'and' statements as well, but expansion is nice to have.)

There are also matching assertions `REQUIRE_FALSE` and `CHECK_FALSE` that check to make sure a statement is false, rather than true.

### Testing Exceptions

Let's modify our Fibonacci function to throw an exception if the user passes us a number that's not within the range of our function.
```{.cpp .numberLines}
#include<stdexcept> // for domain_error
using namespace std;

/* Generate the Nth Fibonacci number */
int fibonacci(int n)
{
	if(n < 0)
	{
		throw domain_error("Fibonacci not defined for negative indices");
	}
	else if(n <= 1)
	{
		return 1;
	}
	else
	{
		return fibonacci(n - 1) + fibonacci(n - 2);
	}
}
```

Catch provides a number of assertions for testing whether expressions throw exceptions and what kinds of exceptions are thrown.
As before, each assertion comes in a `CHECK` and a `REQUIRE` flavor.

- `CHECK_NOTHROW(expression)`: Asserts the expression does not throw an exception.
- `CHECK_THROWS(expression)`: Asserts the expression throws an exception. Any ol' exception will do; it just has to throw something.
- `CHECK_THROWS_AS(expression, exception_type)`: Asserts the expression throws an exception of a specified type.
- `CHECK_THROWS_WITH(expression, string)`: Asserts that the expression thrown, when converted to a string, matches the specified string.[^matcher]

For example, we can check that our Fibonacci function properly verifies that its input is in the domain by testing when it throws exceptions
and what exceptions it throws:

```{.cpp .numberLines}
TEST_CASE("Fibonacci Domain", "[Fibonacci]")
{
  CHECK_NOTHROW(fibonacci(0));
  CHECK_NOTHROW(fibonacci(10));
  CHECK_THROWS_AS(fibonacci(-1), domain_error);
  CHECK_THROWS_WITH(fibonacci(-1), "Fibonacci not defined for"
      "negative indices");
}
```

### Organizing Your Tests

At this point you know enough to start writing tests for functions.
Before you go too hog--wild, shoving test cases every which where, let's talk about how to organize tests so they're easy to find and use.

First, we can't have our `main()` function and Catch's auto-generated `main()` in the same program.
You'll need to organize your code so that you can compile your test cases without including your `main()` function.
So make your program's `main()` as small as possible and have it call other functions that can be unit tested.

Second, we don't want our test code included in our actual program.
A generally good pattern to follow is to divide your code into multiple files as usual,
then make a separate test file for each implementation file.

For example, if we made `fibonacci.h` and `fibonacci.cpp` files for our function above, we'd also make a `test_fibonacci.cpp` file
that contains our unit tests.

Third, compiling Catch's auto-generated `main()` function takes a while.
This is doubly annoying because it never changes!
Rather than rebuilding it all the time, we can harness the power of makefiles and incremental compilation by making a separate `test_main.cpp` file
that just contains Catch's `main()`.
This file looks exactly like this:

```{.cpp .numberLines}
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
```

Then in `test_fibonacci.cpp`, we just have the following includes:
```{.cpp .numberLines}
#include "fibonacci.h"
#include "catch.hpp"

// insert unit tests here
```

Building this code is done as follows:

```
$ g++ -c test_main.cpp                  # Compile Catch's main()
$ g++ -c test_fibonacci.cpp             # Compile Fibonacci tests
$ g++ test_main.o test_fibonacci.o -o testsuite  # Link testsuite
```

Now you can add new unit tests and just recompile `test_fibonacci.cpp` and re-link the test suite.
Much faster!
(Hint: a Makefile is *very* handy for this process!)

### Testing Classes

Testing classes works more-or-less like testing functions.
You'll still write `TEST_CASE`s with various `CHECK` and `REQUIRE` statements.

However, when testing classes, it's common to need to set up a class instance to run a bunch of tests on.
For example, let's suppose we have a Vector class with the following declaration:

```{.cpp .numberLines}
template<class T>
class Vector
{
  public:
    // Constructor
    Vector();

    // Copy Constructor
    Vector(const Vector<T>& v);

    // Destructor
    ~Vector();

    // Add elements to the vector
    void push_back(T v);

    // Access elements of the vector
    T& operator[](const unsigned int idx);

    // Number of elements in the vector
    unsigned int length() const;

    // Capacity of the underlying array
    unsigned int capacity() const;

  private:
    unsigned int len;
    unsigned int cap;
    T* array;
};
```

To test the `[]` operator or the copy constructor, we need to make a vector that contains elements to access or copy.
You could write a bunch of test cases and duplicate the same test setup code in each, but there is a better option!
Each `TEST_CASE` can be split into multiple `SECTION`s, each of which has a name.
For each section, Catch runs the test case from the beginning but only executes one section each run.

We can use this to set up a test vector once to test the constructor and accessor functions:

```{.cpp .numberLines}
TEST_CASE("Vector Elements", "[vector]")
{
  Vector<int> v; // Re-initialized for each section

  for(int i = 0; i < 5; i++)
  {
    v.push_back(i);
  }

  SECTION("Elements added with push_back are accessible")
  {
    for(int i = 0; i < 5; i++)
    {
      CHECK(v[i] == i);
    }
  }

  Vector<int> copy(v); // Only run before the sections below it

  SECTION("A copied vector is identical")
  {
    for(int i = 0; i < v.length(); i++)
    {
      CHECK(v[i] == copy[i]);
    }
  }

  SECTION("Vector copies are deep copies")
  {
    for(int i = 0; i < v.length(); i++)
    {
      v[i] = -1;
      CHECK(v[i] != copy[i]);
    }
  }
}
```

In this example, Catch runs lines 1--16, then starts over and runs lines 1--8 and 18--26, then lines 1--8, 18, and 28--35.
Since we get a fresh `v` vector for each section, the code inside each section can mutate `v` however it likes without impacting any of the other
sections' tests!
Even better, we can add more setup as we go through the test case; our `copy` vector is only created for the sections that test the copy constructor.

In general, you'll want to group related tests into one `TEST_CASE` with multiple `SECTION`s that provide more fine-grained organization of your
test assertions.


### Advanced Tests

Catch provides some advanced features that come in handy when testing code that uses strings and floating point arithmetic.

When testing code that produces strings, sometimes you do not know what the entire string produced will be,
but you want to check that it contains some particular substring.
Catch offers a pair of assertions, `CHECK_THAT` and `REQUIRE_THAT`, that use a *matcher* to check only parts of strings.
For instance, we can test that a string starts with "Dear Prudence" like so:

```
REQUIRE_THAT(my_string, StartsWith("Dear Prudence"));
```

In addition to the `StartsWith` matcher, there is an `EndsWidth` matcher and a `Contains` matcher.
These matchers can be combined using logical operators; for example:

```
REQUIRE_THAT(my_string, StartsWith("Dear Prudence") &&
   !Contains("Sincerely"));
```

These matchers can also be used in the `THROWS_WITH` assertions!

Testing floating point code presents a challenge because floating point operations may have some round-off that prevents exact equality checks from working.
Catch provides a class named `Approx` that performs approximate equality checks; for instance, `CHECK(PI == Approx(3.14));`.
By default, the comparison can be off by 0.001%, but you can change this!
For a more precise comparison, you can set the `epsilon` to a smaller percentage: `CHECK(PI = Approx(3.1415).epsilon(0.0001));`.

### Code Coverage

Unit tests are most valuable when all your important code is tested.
You can check this by hand, but that's no fun, especially on a big codebase.
Fortunately, there are tools to check for you!
We'll use `gcov` to generate code coverage reports for us.

First, a few words about template classes.
`gcov` only gives meaningful results if each function in the template class is actually generated one place or another.
Fortunately, you can explicitly ask the compiler to instantiate a copy of every template class function.
For example, at the top of our `test_vector.cpp` file, we would put

```c++
template class Vector<int>;
```

so that `gcov` properly reports if we forget to test any member functions of our `Vector` class.

As with Address Sanitizer and `gprof`, `gcov` requires some compile-time instrumentation.
Compile your test files with the `--coverage` flag.

Once you have compiled your tests, execute them as normal.
In addition to running your tests, your executable will also produce a number of files ending in `.gcda` and `.gcno`.
These are data files for `gcov`. They're binary, so opening them in a text editor will not be particularly enlightening.
To get meaningful coverage statistics, you run `gcov` and give it a list of `.cpp` files whose behavior you want to see.
(Generally this will be all your test `.cpp` files.)

There are a couple of flags that you definitely want to use for `gcov`:

- `-m`: De-mangle C`++` names. For whatever reason, C`++` names are mangled by the compiler and look very odd unless you tell programs to demangle them.
- `-r`: Only generate information for files in the current directory and sub-directories. This prevents you from generating coverage information about stuff in the standard library, which I hope you are not attempting to unit test.

So, for our Vector example above, we would do something like the following:

```
$ g++ --coverage -c test_vector.cpp
$ g++ --coverage -c test_main.cpp
$ g++ --coverage test_main.o test_vector.o -o testsuite

$ testsuite
================================================
All tests passed (25 assertions in 2 test cases)

$ gcov -mr test_vector.cpp
File 'test_vector.cpp'
Lines executed:100.00% of 39
Creating 'test_vector.cpp.gcov'

File 'catch.hpp'
Lines executed:62.50% of 64
Creating 'catch.hpp.gcov'

File 'vector.hpp'
Lines executed:100.00% of 34
Creating 'vector.hpp.gcov'
```

(Hint the second: *makefiles are very nice for automating this process*.)

When looking at `gcov`'s output, you are mostly concerned that all the code you set out to test is being executed.
In this case, that means we are looking for `vector.hpp` to be 100% executed, and it is!

If you are curious, you can open `vector.hpp.gcov` and see the number of times each line is executed.
Here's a snippet for `Vector`'s constructor:

```
       16:    3:Vector<T>::Vector()
        -:    4:{
       16:    5:  cap = 4;
       16:    6:  len = 0;
       16:    7:  array = new T[cap];
       16:    8:}
```

The numbers in the left margin are the number of times each line is executed.
If a line isn't executed, you will see `####` in the left column instead.
This makes it easy to spot code that isn't covered by your tests!

### Writing Quality Unit Tests

Alright, now that you know how to write unit tests, let's talk about the philosophy of writing good unit tests.
Unit tests are most useful if you write them as you go, rather than writing a big chunk of code and then writing tests.
Some people prefer to write their tests first, then write the code needed to make the tests pass.
Others write the code first and then the tests.
Either way, testing as you go will help you think through how your program ought to work and help you spot bugs
that come from changing or refactoring your code.

Tests should be small and designed to test the smallest amount of functionality possible --- typically a single function,
or a single feature of a function.
Typically, more code means more bugs; we do not want our unit tests to be complex enough to introduce their own bugs!
Tests should be obviously correct as much as is possible.
Start by testing basic functions, such as accessors and mutators.
Once those have been tested, you can use them in more complex functionality tests;
if one of those tests fails, you know that the bug does lies somewhere other than your basic functions.

If you come across a bug in your program, write a unit test that reproduces it,
then fix your code so that the test passes.
This way, you won't accidentally reintroduce the bug later on!

When writing tests, think about the different ways your code can be executed.
Consider your `if` statements and loops --- how might each be executed or not?
What side effects does your code have? Does it modify member variables? Write to a file?
Thorough tests cover as many of these possible execution paths as is feasible.
Think about your preconditions and postconditions and write tests that verify your interface conforms to its documentation!
Test your edge cases, not just the "happy path" that normal execution would take.

Finally, a word on code coverage.
Like any metric, code coverage is not a perfect measure of your tests' quality.
Practically speaking, 100% code coverage is difficult to achieve; it is better to have 90% coverage and well thought out tests
than 100% coverage with tests that don't reflect how your code will actually be used.

\newpage
## Questions

Name: `______________________________`

1. In your own words, what is the goal of unit testing? How do you know you have written good tests?
\vspace{10em}
2. What is the difference between the `CHECK` and `REQUIRE` test assertions?
\vspace{10em}
3. Write the test assertion you would use if you wanted to assert that a call to `frobnicate()` throws an exception of type `bad_joke`
and to bail out of the test case if it does not.
\newpage

## Quick Reference

### Assertions

Boolean:

- `CHECK`/`REQUIRE`: Assert that an expression is true
- `CHECK_FALSE`/`REQUIRE_FALSE`: Assert that an expression is false

Exceptions:

- `CHECK_NOTHROW`/`REQUIRE_NOTHROW`: Assert that no exception is thrown
- `CHECK_THROWS`/`REQUIRE_THROWS`: Assert that an exception is thrown
- `CHECK_THROWS_AS`/`REQUIRE_THROWS_AS`: Assert that an exception of a specific type is thrown
- `CHECK_THROWS_WITH`/`REQUIRE_THROWS_WITH`: Assert that an exception with a specific error string is thrown

String Matchers:

- `CHECK_THAT`/`REQUIRE_THAT`: Assert that a string satisfies a match expression
- `StartsWith`: Verify that a string starts with a given string
- `EndsWith`: Verify that a string ends with a given string
- `Contains`: Verify that a string contains a given string

Floating Point:

- `Approx`: Perform approximate floating point comparison (by default up to 0.001% error)
- `epsilon`: Set the precision of the comparison

### Coverage

- Compile your tests with the `--coverage` flag
- Run your test suite executable
- Run `gcov -mr [.cpp files]` with all `.cpp` files in your project to compute code coverage

## Further Reading

- [Catch Tutorial](https://github.com/philsquared/Catch/blob/master/docs/tutorial.md)
- [Catch Manual](https://github.com/philsquared/Catch/blob/master/docs/Readme.md)
- [Floating Point Comparisons](https://github.com/philsquared/Catch/blob/master/docs/assertions.md#floating-point-comparisons)
- [Matcher Expressions](https://github.com/philsquared/Catch/blob/master/docs/matchers.md)
- [Catch GitHub Repository](https://github.com/philsquared/Catch)

<!-- -->

- [`gcov` Manual](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)

[^gross]: That is, 144 more disgusting tests.
[^pedantry]: Pedantry: unit tests technically cannot show the absence of all bugs; they can just show that under certain circumstances your program does not have bugs.
In logical terms, unit tests are a bunch of "there exists" statements; whereas a proof of correctness is a "for all" statement.
Unfortunately, proving programs correct is a difficult task and the tools to do so are not exactly ready for widespread use yet.
In the meantime, while we wait for math and logic to catch up to the needs of engineering, we'll have to settle for thorough unit testing.
[^matcher]: You can also use a string matcher in place of the string if you want to match multiple strings; we'll talk about these later in the chapter.
[^hand]: Yes, you with your hand up in the back? You saw the bug before the test failed? Yes, yes, you're very clever.
[^sink]: Of course it would --- Boost even has a kitchen sink library.
[^errors]: It could be made more powerful, but the downside is that the compiler errors from misuse would become even more terrifying.
