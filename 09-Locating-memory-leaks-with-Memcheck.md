# Finding Memory Safety Bugs

## Motivation

It's the night before the Data Structures linked list assignment is due, and you are so ready.
Not only do you *understand* linked lists, you *are* a linked list.
You sit down at your terminal,[^term] crack your knuckles, and (digitally) pen a masterpiece.
Never before in the history of computer science has there been such a succinct, elegant linked list implementation.

You compile it and run the test suite, expecting a beautiful `All Tests Passed!`.
Instead, you are greeted with a far less congratulatory `Segmentation Fault (core dumped)`.
I guess that's what you get for expecting a machine to appreciate beauty!

A more pragmatic reason for that segmentation fault is that somewhere your program has accessed memory it didn't have permission to access.
Segmentation faults are but one kind of memory safety bug.
You can also unintentionally overwrite other variables in your program,
or interpret one kind of variable as another (say, treating a class as an `int`)!
These sorts of memory bugs can cause your program to behave unpredictably and thus can be quite difficult to find.
In this chapter we will explore the different types of memory safety bugs you may encounter as well as tools for detecting and analyzing them.

There is another incentive for ruthlessly excising memory safety bugs: every kind of memory safety bug allows an attacker to use it to exploit
your program.
Many of these bugs allow for arbitrary code execution, where the attacker injects their own code into your program to be executed.

The first widespread internet worm, the [Morris worm](https://en.wikipedia.org/wiki/Morris_worm), used a memory safety bug to infect other machines in 1988.
Unfortunately, *thirty years* later, memory safety bugs are still incredibly common in popular software and many viruses continue to exploit them.
For one example, the [WannaCry](https://en.wikipedia.org/wiki/WannaCry_ransomware_attack) and [Petya](https://en.wikipedia.org/wiki/2017_NotPetya_cyberattack)
viruses use a memory safety exploit called [EternalBlue](https://www.rapid7.com/db/modules/exploit/windows/smb/ms17_010_eternalblue) developed
by the NSA[^allegedly] and released by Russian[^allegedly2] hackers early in 2017.

<!--
Image is in the public domain.
source: https://commons.wikimedia.org/wiki/File:Smokey3.jpg
-->

![Smokey Says: "Only You Can Prevent Ransomware"](09/Smokey3.jpg){height=35%}

### Takeaways

- Learn about how memory is managed in programs
- Learn four common memory-related bugs
- Use Valgrind, Address Sanitizer, and GCC to help find and diagnose these bugs

## Walkthrough

### The Stack and The Heap

Your operating system provides two areas where memory can be allocated: the stack and the heap.
Memory on the stack is managed automatically,[^joint] but any allocation only lives as long as the function that makes it is executing.
When a function is called, a *stack frame* is pushed onto the stack.
The stack frame holds variables declared in that function as well as some bookkeeping information.
Crucially, this bookkeeping information includes the memory address of the code to return to once the function completes.
When that function `return`s, its stack frame is popped off the stack and the associated memory is used for the stack frame of the next called function.

Memory allocated on the heap lives as long as you like it to; however, you have to manually allocate and free that memory using `new` and `delete`.[^free]
While the automatic management of the stack is nice, the freedom to be able to make memory allocations that live longer than the function that created them
is essential, especially in large programs.

To help you get the mental picture right,
on modern Intel CPUs, the stack starts at a high memory address and grows downward, while the heap starts at a low memory address and grows upward.
The addresses they start at vary from system to system, and are often randomized to make writing exploits more difficult.

### Uninitialized Values

When you allocate memory, either on the stack or on the heap, the contents of that memory is undefined.
Maybe it's a bunch of zeros; maybe it's some weird looking garbage;
maybe it's a password because the last time that memory was used, a program stored a password there.
You don't know what's there, and you'd be foolhardy to use that value for anything.[^random]
In particular, you *really* don't want to use such a value in a condition for an `if` statement or a loop!
Doing so gives control over your program to anyone can control the values of your uninitialized variables.

Even worse, in C`++`, accessing uninitialized memory is *undefined behavior* and thus if the compiler catches you doing it,[^always]
it can do whatever it wants, including `rm -rf /`, playing mean taunts over your speakers to you, or just removing any code that depends on the value
of an uninitialized variable from your executable.

So, using uninitialized values in your program can introduce weird seemingly-random bugs or result in certain features disappearing from your code
that you know for sure are there.
In other words: initialize your dang variables!

Here's an example of uninitialized values, one on the stack and one on the heap:

~~~{.cpp .numberLines}
#include<iostream>
using namespace std;

int main()
{
  cout << "Stack uninitialized value" << endl;
  int x;
  if(x > 5)
  {
    cout << "> 5" << endl;
  }

  cout << "Heap uninitialized value" << endl;
  int* y = new int;
  if(*y < 5)
  {
    cout << "< 5" << endl;
  }

  delete y;

  return 0;
}
~~~

Your first hint that this isn't right is from the compiler itself if you use the `-Wall` flag:[^linebreak]

```
$ g++ -Wall -g uninitialized-value.cpp -o uninitialized-value
uninitialized-value.cpp: In function 'int main()':
uninitialized-value.cpp:8:3: warning: 'x' is used uninitialized ↩
  in this function [-Wuninitialized]
   if(x > 5)
   ^
```

Here GCC is smart enough to catch the uninitialized use of our stack-allocated variable.
(This warning doubles as a subtle reminder that if you start asking GCC to optimize this code, that `if` statement is probably going to be removed!)

Valgrind's Memcheck tool[^memcheck] can detect when your program uses a value uninitialized.
Memcheck can also track where the uninitialized value is created with the `--track-origins=yes` option.
To run the above program (named `uninitialized-value`) through Valgrind, do the following:

~~~
$ valgrind --track-origins=yes ./uninitialized-value
~~~

When we do this, we get messages about both of our variables.

The stack-allocated uninitialized value is accessed on line 8 and created on line 5:
```
==19296== Conditional jump or move depends on uninitialised value(s)
==19296==    at 0x4008FE: main (uninitialized-value.cpp:8)
==19296==  Uninitialised value was created by a stack allocation
==19296==    at 0x4008D6: main (uninitialized-value.cpp:5)
```

Line 8 is in fact `if(x > 5)`.
All stack-allocated variables appear to be allocated at the start of the function call, so `x` is listed as being created on line 5 rather than line 7.

The heap-allocated uninitialized value was accessed on line 15 and created by a call to `new` on line 14:
```
==19296== Conditional jump or move depends on uninitialised value(s)
==19296==    at 0x40094F: main (uninitialized-value.cpp:15)
==19296==  Uninitialised value was created by a heap allocation
==19296==    at 0x4C2E0EF: operator new(unsigned long)
==19296==    by 0x400941: main (uninitialized-value.cpp:14)
```

Heap-allocated uninitialized values[^ptr] cannot be caught by the compiler --- you must use a tool like Valgrind to find them.

Using `--track-origins=yes` is particularly handy when debugging heap uninitialized values as it is possible for something to be `new`'d in one function
and then not used until much later on.

### Unallocated or Out-of-Bounds Reads and Writes

Perhaps the most common memory bug is reading or writing to memory you ought not to.
This type of bug comes in a few flavors: you could use a pointer with an uninitialized value (as opposed to a pointer *to* an uninitialized value),
or you could access outside of an array's bounds, or you could use a pointer after deleting the thing it points at.

Sometimes this kind of error causes a segmentation fault, but sometimes the memory being accessed happens to be something else your program
is allowed to access.
In this case, these memory bugs can go about their business, silently mangling your data and making your program do very bizzarre things.

Furthermore, these types of bugs can easily turn into exploits.
Remember that at the top of each stack frame is the address of the code to jump to when the associated function returns.
An out-of-bounds write lets an attacker overwrite this address with their own!
Once they have this, they can have the computer start executing whatever code they desire as long as they know where it is in memory.
This kind of exploit is known as a buffer overflow exploit.

You can detect these kinds of bugs using either Valgrind or Address Sanitizer (a.k.a. `asan`).
`asan` is part runtime library, part compiler feature that instruments your code at compile time.
Then when you run your program, the instrumentation tracks memory information much in the way Valgrind does.
`asan` is much faster than Valgrind, but requires special compiler flags to work.

To enable address sanitizer, you must use the following flags to `g++`: `-g -fsanitize=address -fno-omit-frame-pointer`.[^vg]
Furthermore, you need to set two environment variables[^llvm] if you want function names and line numbers to appear in `asan`'s output:

```bash
export ASAN_SYMBOLIZER_PATH=`which llvm-symbolizer`
export ASAN_OPTIONS=symbolize=1
```

Let's look at some examples of this class of bugs and the relevant Valgrind and `asan` output.

#### Out-of-bounds Stack Access

First up, out-of-bounds accesses on a stack-allocated array:

```{.cpp .numberLines}
#include<iostream>

int main()
{
  int array[5];
  array[5] = 5; // Out-of-bounds write
  std::cout << array[5] << std::endl; // Out-of-bounds read

  return 0;
}
```

If you run this program normally, it probably won't crash, and in fact it will probably behave how you expect.
This is a mirage. It only works because whatever is one `int` after `array` in `main()`'s stack frame happens to not be used again.
This illustrates how important it is to check that you do not have these bugs!

Even worse, Valgrind does not detect this out-of-bounds access!
However, `asan` does.
Compile the program with this command:

~~~
$ g++ -g -fsanitize=address -fno-omit-frame-pointer ↩
    invalid-stack.cpp -o invalid-stack
~~~

`asan`'s output is somewhat terrifying to see, but the relevant parts look like this:[^edit]

```
==29210==ERROR: AddressSanitizer: stack-buffer-overflow on ↩
  address 0x7fff2f9d9654 at pc 0x000000400ce4 bp 0x7fff2f9d9610 ↩
  sp 0x7fff2f9d9600
WRITE of size 4 at 0x7fff2f9d9654 thread T0
    #0 0x400ce3 in main invalid-stack.cpp:6
    #1 0x7fa90759b82f in __libc_start_main
    #2 0x400b58 in _start (invalid-stack+0x400b58)

Address 0x7fff2f9d9654 is located in stack of thread T0 ↩
  at offset 52 in frame
    #0 0x400c35 in main invalid-stack.cpp:4

  This frame has 1 object(s):
    [32, 52) 'array' <== Memory access at offset 52 ↩
                           overflows this variable
```

In particular, we have a write of "size 4", that is, 4 bytes, on line 6 of our program.
The stack trace showing how execution reached line 6 is shown as well.
Furthermore, the stack variable we wrote out-of-bounds to was allocated on line 4 as part of `main()`'s stack frame,
which contains one object: `array`.

Address sanitizer halts the program after the first error, so we don't see output for the invalid read.
The reason for this behavior is that generally one bug causes others, and it's easier to just see the first problem and fix it
rather than wade through screenfulls of errors trying to figure out which one unleashed the torrent of access violations.

Pretty handy, eh? What more could you ask for!

#### Out-of-bounds Heap Access

Both Valgrind and `asan` can detect heap out-of-bounds accesses.
Here is a small sample program that demonstrates an out-of-bounds write:

```{.cpp .numberLines}
#include<iostream>

int main()
{
  std::cout << "Invalid write" << std::endl;
  int *arr = new int[5];
  for(int i = 0; i <= 5; i++)
  {
    arr[i] = i;
  }

  delete[] arr;
  return 0;
}
```

Here is Valgrind's output:

```
==2894== Invalid write of size 4
==2894==    at 0x40097C: main (invalid.cpp:9)
==2894==  Address 0x5ac00d4 is 0 bytes after a block of size 20 alloc'd
==2894==    at 0x4C2E80F: operator new[](unsigned long)
==2894==    by 0x400953: main (invalid.cpp:6)
```

And here is `asan`'s output:

```
==3334==ERROR: AddressSanitizer: heap-buffer-overflow on ↩
  address 0x60300000eff4 at pc 0x000000400ce1 bp 0x7ffee305a690 ↩
  sp 0x7ffee305a680
WRITE of size 4 at 0x60300000eff4 thread T0
    #0 0x400ce0 in main invalid.cpp:9
    #1 0x7fc6258f282f in __libc_start_main
    #2 0x400b88 in _start (invalid+0x400b88)

0x60300000eff4 is located 0 bytes to the right of 20-byte region ↩
  [0x60300000efe0,0x60300000eff4) allocated by thread T0 here:
    #0 0x7fc6260bf6b2 in operator new[](unsigned long)
    #1 0x400c83 in main invalid.cpp:6
    #2 0x7fc6258f282f in __libc_start_main
```

Since the memory we are accessing out-of-bounds is heap allocated, Valgrind and `asan` can track the exact line of your code
where it was allocated (in this case, line 6).
The write itself occurred on line 9.
Furthermore, they show that the write happened 0 bytes to the right[^chickens] (in other words, after the end of) our allocated chunk,
indicating that we are writing one index past the end of the array.

#### Use After Free

Finally, let's see an example of a use-after-free.
This type of bug is exploitable by means similar to using an uninitialized value, but it is usually far easier to control
the contents of memory for a use-after-free bug.[^exploit]
Like out-of-bounds accesses, this type of bug can go undetected if you don't check for it; the below example appears to work, even though it is incorrect!

```{.cpp .numberLines}
#include<iostream>

int main()
{
  int* x = new int[5]; // Declare and initialize an array
  for(int i = 0; i < 5; i++)
  {
    x[i] = i;
  }

  int* y = &x[1]; // "Accidentally" hold a pointer to an array element

  delete [] x; // Delete the array

  std::cout << *y << std::endl; // Uh-oh!

  return 0;
}
```

Valgrind shows where the invalid read occurs, where the block was deallocated, and where it was allocated:
```
==10658== Invalid read of size 4
==10658==    at 0x40090B: main (use-after-free.cpp:15)
==10658==  Address 0x5abfc84 is 4 bytes inside a block of size 20 free'd
==10658==    at 0x4C2F74B: operator delete[](void*)
==10658==    by 0x400906: main (use-after-free.cpp:13)
==10658==  Block was alloc'd at
==10658==    at 0x4C2E80F: operator new[](unsigned long)
==10658==    by 0x4008B7: main (use-after-free.cpp:5)
```

Address Sanitizer's output is similar:

```
==10827==ERROR: AddressSanitizer: heap-use-after-free on ↩
  address 0x60300000efe4 at pc 0x000000400ca6 bp 0x7ffce3f2c0e0 ↩
  sp 0x7ffce3f2c0d0
READ of size 4 at 0x60300000efe4 thread T0
    #0 0x400ca5 in main use-after-free.cpp:15
    #1 0x7f7c327c682f in __libc_start_main
    #2 0x400b08 in _start (use-after-free+0x400b08)

0x60300000efe4 is located 4 bytes inside of 20-byte region ↩
  [0x60300000efe0,0x60300000eff4)
freed by thread T0 here:
    #0 0x7f7c32f93caa in operator delete[](void*)
    #1 0x400c6e in main use-after-free.cpp:13
    #2 0x7f7c327c682f in __libc_start_main

previously allocated by thread T0 here:
    #0 0x7f7c32f936b2 in operator new[](unsigned long)
    #1 0x400be7 in main use-after-free.cpp:5
    #2 0x7f7c327c682f in __libc_start_main
```

Both outputs show that a 4-byte (the size of an `int`) read happened 4 bytes (so, at index 1) inside the block of 20 bytes
that is our array of 5 `int`s.

Invalid read or write bugs can have a number of fixes.
Sometimes they're as simple as adding a bounds check somewhere to make sure you don't write off the end of an array.
Other times, you'll have to think carefully about where your code went astray --- see the Debugging chapter for more advice on this.

### Mismatched and Double Deletes

Mismatched deletes occur when you use `delete` to delete an array or `delete []` to delete a non-array.
In the former case, not all allocated memory may be marked as free, resulting in a memory leak.
In the latter case, too much memory may be freed![^implementation]

A simple example calls `delete` on something allocated with `new[]`:
```c++
int main()
{
  int* arr3 = new int[5];
  delete arr3;

  return 0;
}
```

Valgrind reports the error like so:
```
==16964== Mismatched free() / delete / delete []
==16964==    at 0x4C2F24B: operator delete(void*)
==16964==    by 0x400667: main (mismatched.cpp:4)
==16964==  Address 0x5abfc80 is 0 bytes inside a block of size 20 alloc'd
==16964==    at 0x4C2E80F: operator new[](unsigned long)
==16964==    by 0x400657: main (mismatched.cpp:3)
```

And here is Address Sanitizer's output:
```
==17080==ERROR: AddressSanitizer: alloc-dealloc-mismatch ↩
  (operator new [] vs operator delete) on 0x60300000efe0
    #0 0x7f8cde981b2a in operator delete(void*)
    #1 0x400747 in main mismatched.cpp:4
    #2 0x7f8cde53e82f in __libc_start_main
    #3 0x400658 in _start (mismatched+0x400658)

0x60300000efe0 is located 0 bytes inside of 20-byte region ↩
  [0x60300000efe0,0x60300000eff4)
allocated by thread T0 here:
    #0 0x7f8cde9816b2 in operator new[](unsigned long)
    #1 0x400737 in main mismatched.cpp:3
    #2 0x7f8cde53e82f in __libc_start_main
```

Both identify where the delete and matching allocation occurred (here, on lines 4 and 3 respectively).
You can tell what the exact mismatch is by looking ath the operators called by the deletion and allocation lines.
In this example, `operator delete` is called to delete the allocation, but `operator new[]` is called to allocate it.

A double-delete occurs when you delete the same block of memory twice.
Double deletes may seem innocuous, but they can be easily turned into a use-after-free bug.
This is because freed memory is usually re-used in future allocations.
So deleting something, then allocating a second thing, then deleting the first thing again results in
the second thing being deleted!
Any future uses of the second thing then become a use-after-free problem, and attempting to properly clean up
that second allocation brings on a double delete.
For example,
```{.cpp .numberLines}
#include<iostream>

int main()
{
  int* x = new int(5);
  delete x;

  int* y = new int(7); // Probably allocated where x was
  // If we were to print out *x and *y, we would likely
  // see "7" for both values!

  delete x; // Either deletes y or explodes
  delete y; // If the last line didn't explode, BOOM!

  return 0;
}
```

(We don't print the values out because Valgrind and Address Sanitizer would discover a use-after-free on `x`,
 which is not what we're trying to demonstrate here.)

Valgrind properly attributes the double free to line 12 (the second `delete x`):
```
==20974== Invalid free() / delete / delete[] / realloc()
==20974==    at 0x4C2F24B: operator delete(void*)
==20974==    by 0x40078D: main (double.cpp:12)
==20974==  Address 0x5abfc80 is 0 bytes inside a block of size 4 free'd
==20974==    at 0x4C2F24B: operator delete(void*)
==20974==    by 0x40076D: main (double.cpp:6)
==20974==  Block was alloc'd at
==20974==    at 0x4C2E0EF: operator new(unsigned long)
==20974==    by 0x400757: main (double.cpp:5)
```

As does `asan`:
```
==20761==ERROR: AddressSanitizer: attempting double-free ↩
  on 0x60200000eff0 in thread T0:
    #0 0x7f0c58dbdb2a in operator delete(void*)
    #1 0x400adb in main double.cpp:12
    #2 0x7f0c585f082f in __libc_start_main
    #3 0x400958 in _start (double+0x400958)

0x60200000eff0 is located 0 bytes inside of 4-byte region ↩
  [0x60200000eff0,0x60200000eff4)
freed by thread T0 here:
    #0 0x7f0c58dbdb2a in operator delete(void*)
    #1 0x400a84 in main double.cpp:6
    #2 0x7f0c585f082f in __libc_start_main

previously allocated by thread T0 here:
    #0 0x7f0c58dbd532 in operator new(unsigned long)
    #1 0x400a37 in main double.cpp:5
    #2 0x7f0c585f082f in __libc_start_main
```

Both show the location of the allocation and the first delete.
Typically, this kind of bug arises when you don't properly keep track of whether
a pointer has been `delete`d yet.
Sometimes a quick fix for this is to set your pointers to `NULL` after you call `delete`.
Other times, you'll get a double-delete if you forget to write a copy constructor for a class (or write a buggy one).

### Memory Leaks

Last, but certainly not least, are memory leaks.
While these pose less potential for security flaws, nobody likes programs that hog memory.
Just like it's good practice to close files when you're done accessing them, it's good practice to deallocate memory when you're done using it.

There are two kinds of memory leaks: direct and indirect.
A direct memory leak occurs when you have allocated a block of memory but no longer have a pointer pointing to it.
An indirect memory leak occurs when the only pointers to an allocated block of memory are in a block of memory that has been leaked.
The distinction is drawn because typically indirect memory leaks occur due to not running a destructor on some directly leaked object.

Both Valgrind and Address Sanitizer can detect memory leaks.
Let's look at a simple example that has one directly leaked block and one indirectly leaked block:
```{.cpp .numberLines}
struct List
{
  int value;
  List* next;
};

int main()
{
  List l;
  l.value = 5;
  l.next = new List;

  l.next->value = 6;
  l.next->next = new List;

  //These two lines would fix the memory leaks
  //delete l.next->next;
  //delete l.next;

  return 0;
}
```

When using Valgrind to debug memory leaks, the `--leak-check=full` option shows where each leaked block was allocated.

Valgrind's output, with a full leak check, is shown below:

```
==649== HEAP SUMMARY:
==649==     in use at exit: 72,736 bytes in 3 blocks
==649==   total heap usage: 3 allocs, 0 frees, 72,736 bytes allocated
==649== 
==649== 32 (16 direct, 16 indirect) bytes in 1 blocks are definitely ↩
          lost in loss record 2 of 3
==649==    at 0x4C2E0EF: operator new(unsigned long)
==649==    by 0x40060F: main (leak.cpp:11)
==649== 
==649== LEAK SUMMARY:
==649==    definitely lost: 16 bytes in 1 blocks
==649==    indirectly lost: 16 bytes in 1 blocks
==649==      possibly lost: 0 bytes in 0 blocks
==649==    still reachable: 72,704 bytes in 1 blocks
==649==         suppressed: 0 bytes in 0 blocks
```

On some systems, including this one, the system runtime library allocates some memory and does not deallocate it.[^lazy]
This memory will appear in the "still reachable" section of Valgrind's output.
Do not worry yourself too much about it.

The big thing we're disturbed to see is that we have leaked 32 bytes: 16 directly and 16 indirectly.
The directly-leaked block that leaks our indirectly-leaked block was allocated on line 11 (`l.next = new List`).
Valgrind does not show the location that indirectly-leaked blocks are allocated.

Address Sanitizer also has a leak checker; it does not track "still reachable" memory, so
there are no false positives here:
```
==733==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 16 byte(s) in 1 object(s) allocated from:
    #0 0x7f4e3b152532 in operator new(unsigned long)
    #1 0x4008cf in main leak.cpp:11
    #2 0x7f4e3ad0f82f in __libc_start_main

Indirect leak of 16 byte(s) in 1 object(s) allocated from:
    #0 0x7f4e3b152532 in operator new(unsigned long)
    #1 0x40091c in main leak.cpp:14
    #2 0x7f4e3ad0f82f in __libc_start_main

SUMMARY: AddressSanitizer: 32 byte(s) leaked in 2 allocation(s).
```

As opposed to Valgrind, Address Sanitizer shows where both directly and indirectly leaked blocks are allocated.

Sometimes memory leaks come from a missing or buggy destructor.
Other times, they happen when you accidentally overwrite a pointer, say in a buggy `insert()` function.
These can be more difficult to track down; again, see the Debugging chapter for advice.

\newpage
## Questions
Name: `______________________________`

1. In your own words, what is a use-after-free error?
\vspace{10em}
2. What does `--track-origins=yes` do when used with Valgrind?
\vspace{10em}
3. What bug does Address Sanitizer catch that Valgrind does not?

\newpage

## Quick Reference

Using Valgrind: `valgrind [valgrind flags] program-to-run`

- `--track-origins=yes`: Show where undefined variables are declared.
- `--leak-check=full`: Show where directly leaked blocks are allocated.


Using Address Sanitizer:

- Compile your code with the `-g -fsanitize=address -fno-omit-frame-pointer` flags.
- Set the following environment variables:[^llvm]

	```bash
export ASAN_SYMBOLIZER_PATH=`which llvm-symbolizer`
export ASAN_OPTIONS=symbolize=1
```

- Run your code as you normally would.

## Further Reading

- [Valgrind Memcheck Manual](http://valgrind.org/docs/manual/mc-manual.html)
- [Address Sanitizer Wiki](https://github.com/google/sanitizers/wiki/AddressSanitizer)
- [GCC `-fsanitize=address` documentation](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#index-fsanitize_003daddress)
- [Paper on Address Sanitizer](https://www.usenix.org/system/files/conference/atc12/atc12-final39.pdf)

[^term]: And the computer it is running on, being that it's the 21st century and all.
[^joint]: It's a joint effort between the compiler and the operating system.
[^free]: Or if you're writing C, with the `malloc` and `free` functions.
[^random]: It's not even a good source of random numbers, unless you like random numbers that aren't very random.
[^always]: It doesn't always do this because statically analyzing software (i.e., at compile time) is Really Hard, but it still catches most obvious things.
[^memcheck]: Valgrind has a whole bunch of tools included, but it runs the Memcheck tool by default.
We'll see some other Valgrind tools in future chapters of this book.
[^ptr]: And more generally, any uninitialized value being accessed through a pointer.
[^edit]: The file paths shown have been cut down to fit on the page, and some have been removed entirely to clean up the output.
Don't be concerned if the output you see is slightly different from what is printed here.
[^linebreak]: Lines that are too long to fit on the page have been split over multiple lines; this is indicated by the `↩` symbol.
[^vg]: Note: don't try to use `asan` and Valgrind at the same time. Your output will be craaaaaazy.
It's best to make a special `asan` makefile target that turns on the relevant compiler flags.
[^chickens]: Did you know that chickens also visually organize smaller quantities on the left and larger quantities on the right?
[^exploit]: Since this is not a book on exploiting software, we won't go into further detail; writing exploits is its own universe of rabbit holes.
[^implementation]: The implementation of `delete []` isn't specified, but the size of the allocation has to be stored somewhere;
depending on where it is stored, various Bad Things can happen if you try to `delete []` something that wasn't intended to be.
[^lazy]: It's not fair to say that the runtime developers are lazy, though.
There are some technical difficulties with freeing this memory, and since it is in use up until your program exits anyway,
there is little benefit to going to the effort of freeing it since the operating system deallocates it once your program exits anyway.
[^llvm]: This requires `llvm` to be installed. Also, depending on the system you are running, you may need to append a version number, e.g., ``export ASAN_SYMBOLIZER_PATH=`which llvm-symbolizer-3.9` ``
[^allegedly]: Allegedly
[^allegedly2]: Allegedly
