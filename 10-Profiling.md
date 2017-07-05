# Profiling

## Motivation

"IT'S NOT FAIR!"

You sit at your desk, bewildered.
If you'd known you'd have to deal with actors, you would have never taken this video editing job in 1960s Hollywood.

"I'm tellin' ya. *She* gets TWICE as much screen time as ME!"

Mr. Grampton St. Rumpterfrabble.
You know he's famous and you know people like him, but you never understood why.
Especially now.

"I really don't think that's true," you calmly reply as you push up your rose-colored glasses[^literal].
"I've already spliced the film together, and I don't think Ms. Stembleburgiss is seen anymore than you are."

"Yeah? Well prove it, wise guy."

You begrudgingly roll your chair over to your latest film invention: the Time-inator.
Carefully, you load the master reel for "The Duchess Approves II: The Duchess Doesn't Approve" into the input slot.
The machine whirs and clicks -- clacks and bonks.
Finally the 8-segment display at the bottom shows its output

~~~
idiots:      30m
film:        1h30m
projector:   5s
~~~

"Look," you motion to the display.
"This machine tells me how much of the movie features... actors..., how long the movie is, and how much time the projector spends warming up and stuff.
If you're going to be angry, be angry about the fact that you and Ms. Stembleburgiss are only on screen for 30 minutes.
As I recall, the film is mostly footage of Puddles the Amazing Schnauzer balancing on a beach ball."

"How *dare* you" St. Rumpterfrabble whispers with rage.
"Puddles earned every frame she is in."

A tear runs down his cheek.

"I cannot compete with that level of talent.
I *can* compete with HER, though."

"Ms. Stembleburgiss"

"Yes, Ms. Stumbleburger... Ms. Stepplemurder... Ms.... YOU KNOW WHO I MEAN.
I want you to go figure out **exactly** how much screen time *she* has and how much I have."

Realizing he's not going to leave until you do, you come up with a shortcut.
You load the master reel into your preview machine and check every 50th frame to see who's in the shot.
The film was shot at 24 frames per second, so you'll check every 2 seconds of film.
It's not exact, but it's good enough.
Besides, the film is mostly Puddles anyway.

"You both share the same amount of screen time. Down to a granularity of 2-seconds," you blandly state.

"Two seconds?! That's enough to make or break a career!" St. Rumpterfrabble shouts.
"I want you to look at *every single frame* to see WHO has the most screen time!"

You slump onto your desk and consider going back for that degree in Computer Science.
Sure, Hollywood is grand, but at least you wouldn't be dealing with this sort of frame-by-frame tedium every day.

Well, you would, actually.

Everyone wants their programs to be fast, but it's not always obvious how to make them fast.
It is often necessary to dive *deep* to figure out where your program is spending most of its time running.
Just like watching your film frame-by-frame, sometimes you have to watch your program run line-by-line to figure out which parts are fast and which are slow.

Fortunately, you don't have to sit with a debugger counting line after line.
There are tools called **profilers** that automate this process for you.
You can think of them like souped up stop watches.
They can give you detailed breakdowns of how your program runs, so that you can identify areas for improvement.

### Takeaways

- Realize that films should feature more dogs balancing on beach balls
- Gain a basic understanding of what a profiler is and how different profilers work
- Understand how to use and interpret results from `time`, `gprof`, `callgrind`, and `massif`

## Walkthrough

### Timing Programs with `time`

You can think of `time` like a fancy stopwatch.
It tells you:

Real time

:   This is how long your program takes to run start to finish.
    We often call this "wall time" because you could measure the time elapsed using a clock on the wall.

User time

:   This is how long your program spends running on your computer's CPU.
    Your computer actually runs a lot of programs at once.
    There's a program that manages your monitor, one that handles keyboard input, one that manages your files, etc.
    The trouble is that if you have more programs than CPUs, they have to take turns.
    Your operating system will divide CPU time between the different programs.

    **User time** tells us how much time *your program* spends on the CPU.
    If it has to share the CPU with other programs (which it probably will), the user time will likely be much less than the real time.

System time

:   This is how long your program spends waiting to hear back from your operating system (OS).
    Whenever you do any sort of I/O[^io] operations, your program makes a system call that asks your OS to do those things for you.
    If your OS is busy doing other stuff, your program will have to wait to hear back from the OS before it can continue running.
    **System time** reflects this time spent waiting to hear back from the OS.

`time` is very easy to use.
In order to use it, just throw `time` in front of the program you want to run.
It doesn't care if the program has command line arguments.

~~~shell
# If we want to see how long it takes to list the files in /tmp
$ time ls /tmp
time ls -a /tmp
.    ..

real    0m0.004s
user    0m0.001s
sys     0m0.002s

# If we wanted to time a hello world program...
$ g++ -o hello hello.cpp
$ time ./hello
~~~

### Profiling with `gprof`

Although `time` is handy for determining run times, it doesn't give you any indication about which parts of your programs are slow and which parts are fast.
However, `gprof` can do that for you.
`gprof` samples your program as it runs to tell you how much time you're spending in function calls.
We'll discuss two different profiles that are included in `gprof`'s output: flat profiles and call graphs.


First, let's look at an example program.
In this program, we have a few different functions.
Each one has a for-loop that just wastes time for the sake of example.

~~~cpp
#include <iostream>
using namespace std;

void new_func1()
{
    cout << "Inside new_func1" << endl;
    for (int i = 0; i < 2000000000; i++)
    {
    }
    return;
}

void func1()
{
    cout << "Inside func1" << endl;
    for (int i = 0; i < 2000000000; i++)
    {
    }
    new_func1();
    return;
}

void func2()
{
    cout << "Inside func2" << endl;
    for (int i = 0; i < 2000000000; i++)
    {
    }
    return;
}

int main(void)
{
    cout << "Inside main" << endl;
    for (int i = 0; i < 2000000000; i++)
    {
    }
    func1();
    func2();
    return 0;
}
~~~

In order to use `gprof` we have to pass an additional flag to `g++`.
The `-pg` flag tells `g++` to record profile information whenever our compiled program runs.
The generated file, called `gmon.out`, contains the information that is interpreted by `gprof`.

Let's compile the program above.
We'll assume it's called `main.cpp`.

~~~shell
$ ls
main.cpp
$ g++ -pg -o main main.cpp
$ ls
main    main.cpp
~~~

In order to generate `gmon.out`, we need to run `main`.

~~~shell
$ ls
main    main.cpp
$ ./main
Inside main
Inside func1
Inside new_func1
Inside func2
$ ls
gmon.out    main    main.cpp
~~~

Now that we have `gmon.out`, we can ask gprof to show us the profile.

~~~shell
$ gprof main
~~~

#### The Flat Profile

Whenever you run `gprof`, you'll see a flat profile at the top followed by a call graph.
In its output, `gprof` includes detailed documentation to help you better understand what you see.
For the flat profile, it describes the sampling procedure and explains the meaning of each column.
The same documentation can be found in the Further Reading section below.

For our above program, we see the following **flat profile**:

~~~
  %   cumulative   self              self     total
 time   seconds   seconds    calls   s/call   s/call  name
 26.11      5.61     5.61                             main
 24.99     10.98     5.37        1     5.37     5.37  new_func1()
 24.94     16.34     5.36        1     5.36    10.73  func1()
 24.89     21.69     5.35        1     5.35     5.35  func2()
  0.00     21.69     0.00        1     0.00     0.00  _GLOBAL__sub_I__Z9new_func1v
  0.00     21.69     0.00        1     0.00     0.00  __static_initialization_and_destruction_0(int, int)
~~~

When your program runs, it makes a note in `gmon.out`...

- every time a function is called.
  This ensures that our function call counts are exact.
- Every 0.01 seconds.
  This gives us a rough idea as to how much time we're spending in each function.
  These are referred to as "samples".

You can see in the profile that `main`, `new_func1`, `func1`, and `func2` were each called one time, and we spent roughly 25% of our time in each one.
That makes sense to see, given that each of those functions wastes time using the same kind of for-loop (one with two billion iterations).

The functions are sorted by the amount of time spent running in each.
That is, the function with the most samples is the one we spent the most time in.

#### The Call Graph

In addition to the Flat Profile, `gprof` shows you a Call Graph.
The Call Graph goes one step further than the Flat Profile by showing you how much time you spent running a function and its children.
Again, `gprof` will display a bunch of documentation for interpreting the Call Graph, and that same information is linked in the Further Reading section below.

For our program above, we see the following Call Graph:

~~~
index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    5.57   16.17                 main [1]
                5.41    5.35       1/1           func1() [2]
                5.41    0.00       1/1           func2() [3]
-----------------------------------------------
                5.41    5.35       1/1           main [1]
[2]     49.5    5.41    5.35       1         func1() [2]
                5.35    0.00       1/1           new_func1() [4]
-----------------------------------------------
                5.41    0.00       1/1           main [1]
[3]     24.9    5.41    0.00       1         func2() [3]
-----------------------------------------------
                5.35    0.00       1/1           func1() [2]
[4]     24.6    5.35    0.00       1         new_func1() [4]
-----------------------------------------------
                0.00    0.00       1/1           __libc_csu_init [18]
[11]     0.0    0.00    0.00       1         _GLOBAL__sub_I__Z9new_func1v [11]
                0.00    0.00       1/1           __static_initialization_and_destruction_0(int, int) [12]
-----------------------------------------------
                0.00    0.00       1/1           _GLOBAL__sub_I__Z9new_func1v [11]
[12]     0.0    0.00    0.00       1         __static_initialization_and_destruction_0(int, int) [12]
-----------------------------------------------
~~~

Let's start by making some observations about index 1.
The call graph shows...

- we spent 100% of our time running `main`.
  That makes sense, given that `main` is the entry point to our program.
- we spent 5.57 seconds in `main`, 5.41 seconds in `func1`, and 5.41 seconds in `func2`.
  These values agree, roughly, with our Flat Profile.
- `func1` spent 5.35 seconds running its children functions.
  This makes sense, because `func1` calls `new_func1`, which takes 5.35 seconds to run.
- `main` spent 16.17 seconds on its children.
  This makes sense because `func1` + `new_func1` + `func2` is roughly 16 seconds.

As you can see, the Call Graph essentially breaks down how much time `main` spends running itself, as well as how much time it spends calling other functions.
By accounting for these separately, you can better see where your time is going.

In the following entries, you can see more detail about where time is spent for functions other than `main`.
Index 2, for example, shows you the amount of time spent when `main` calls `func1`.
The breakdown shows the amount of time spent running code in `func1`, as well as the amount of time running its only child: `new_func1`.

Although this example does not demonstrate it, `gprof` has the ability to show you details for more complicated call graphs.
For example, if both `func1` and `func2` called `new_func1`, `gprof` would show you how much time you're spending in `new_func1` when it's called by `func1` and when it's called by `func2`.

If you have a situation where calling context changes its running time, you may find this extra information useful.
Perhaps `new_func1` is very fast when `func1` calls it, but it's very slow when `func2` calls it.
You could use this information as a clue to figure out why `new_func1` is sometimes slow.

### Profiling with `callgrind`

`gprof`'s approach to take samples every 0.01 seconds works for many people when they're trying to identify slow spots in their code.
However, it is not perfect.

Remember our film predicament?
`gprof` is like looking at every 50th frame.
It gives you a *good* idea of how much movie time we spend looking at Mr. St. Rumpterfrabble, but it's not perfect.
If we take the time to go frame-by-frame, that's the most detailed we can possibly get.

`callgrind` is our frame-by-frame approach.
Instead of going fame-by-frame, we're going instruction-by-instruction.
`callgrind` is one of several tools built using the Valgrind framework.
Tools built using Valgrind can see *every single instruction* that is run by a program.
This level of detail can be very powerful, but it can also be pretty slow.

Let's consider our program from the `gprof` section, but let's use ten thousand iterations for each for-loop instead of two billion.
This time when we compile it, we need to pass the `-g` flag instead of `-pg`.
Then we'll run our program through `callgrind` as shown.

~~~shell
$ g++ -g -o main main.cpp
$ valgrind --tool=callgrind ./main
~~~

Like `gprof`, callgrind will generate a data file for us.
However, the output file does not always have the same name.
Each `callgrind.out.NNNN` file is named according to its process ID.
When we use `callgrind_annotate` to view the profile information, we need to make sure we pass the right `callgrind.out.NNNN`.

~~~shell
# Be careful! The process ID (31147 in this case) will change!
$ callgrind_annotate --auto=yes callgrind.out.31147
~~~

In its output, `callgrind_annotate` will show you how many CPU instructions were run for each line of code.
Let's have a look at the annotated source for `main`.

~~~
.      int main(void)
3      {
16         cout << "Inside main" << endl;
8,991  => ???:std::ostream::operator<<(std::ostream& (*)(std::ost...
2,425  => /build/eglibc-SvCtMH/eglibc-2.19/elf/../sysdeps/x86_64/...
4,735  => ???:std::basic_ostream<char, std::char_traits<char> >& ...
.
30,004     for (int i = 0; i < 10000; i++) {
.          }
.
1          func1();
61,383 => main.cpp:func1() (1x)
1          func2();
30,672 => main.cpp:func2() (1x)
.
1          return 0;
20     }
~~~

Let's break this down a bit:

- We spend 16 CPU instructions printing `"Inside main"` to the console.
  In reality, though, those 16 instructions simply make calls to other functions thanks to `cout` and the `<<` operator.
  We actually spend 16 + 8,991 + 2,425 + 4,735 instructions printing to the console if you count the functions that we called.
- We spent a total of 30,004 CPU instructions instantiating an int called `i`, checking that it's less than 10,000, and incrementing it.
  All 30,004 of those instructions were used to perform the loop initialization, check, and post-loop actions.
  Zero instructions were used in the body of the for-loop.
- It took 1 CPU instruction to call `func1` one time (`1x`), but running `func1` used 61,383 instructions.
- It took 1 CPU instruction to call `func2` one time (`1x`), but running `func2` used 30,672 instructions.

As you can see, `callgrind` gives us different details that `gprof` cannot.
Based on where you run the most instructions, you can identify parts of your code that may need to be rewritten.


\newpage
## Questions

Name: `______________________________`

1. Use `time` to time the execution of a simple Hello World program. What were the values for `real`, `user`, and `sys`? Do those values make sense?
\vspace{8em}

2. Briefly explain, in your own words, the difference between the way `gprof` (`g++ -pg`) and `callgrind` profile programs.
\vspace{8em}

2. Briefly summarize the output you get when you use `time`, `gprof`, and `callgrind_annotate` to profile a program.
\vspace{8em}

\newpage

## Quick Reference

## Further Reading

### `gprof`

- [Interpreting Flat Profiles](https://sourceware.org/binutils/docs/gprof/Flat-Profile.html#Flat-Profile)
- [Interpreting Call Graphs](https://sourceware.org/binutils/docs/gprof/Call-Graph.html#Call-Graph)


[^literal]: They're literally classes with rose-colored lenses. It's not a metaphor or any such nonsense.
[^io]: Input/Output


<!--  LocalWords:  profiler profilers gprof callgrind
 -->
