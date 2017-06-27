# Debugging with GDB

## Motivation

You thought you were getting a bargain when you bought that pocket watch from that scary old lady at the flea market in the French Quarter.
Little did you realize that pocket watch did more than just tell time[^tell-time].
In fact, holding the button on the side *slows down* time.

At first it's a novelty.
You prank your roommate.
Maybe take some extra time on a test.
Steal a quick bagel for breakfast.
Rob a bank[^escalation]...

![When you can slow down time, taking candy from a baby is like taking candy from a baby.](08/candy.jpg){ width=80% }

It was all fun up to this point.
But now you're here: standing in front of at an array of red-hot lasers, trapped in the vault of the Big Bank of New York, wondering where it all went wrong, and wearing a fancy turtle neck.
At least you look good in a turtle neck.
Not everyone can pull that off.

Desperate for a way out, you start mashing and twisting the other buttons and dials on your magic pocket watch.

*BWEEEEEEEEEEEEEEE!*[^bwee]

With a deafening *bwee*, you're hurled back in time.
Once again, you're crouched in front of the control panel for the Big Bank's vault.
You stare, bewildered, at the room around you.

After you gather yourself, you analyze the nest of wires pouring from the control panel and wonder what you did to set off the vault's laser defense system.
You slow time and begin cutting the wires with the procedure you used last time.
Red wire, other red wire, otherer red wire.
You look behind you: no lasers -- everything's good.

Blue wire.

The array of lasers begins slowly cutting across the doorway.
You're trapped again.
With your last few seconds of freedom, you look in the panel and find *another blue wire*.
You cut the wrong blue wire!

You reach into your pocket to grab your watch and reset time.
Instead, you pull out that pastrami sandwich from last lab.
Also you've turned into a raccoon somehow.

You wake up[^dream].

In real life, you may not be able to slow down or reset time analyze disasters in detail[^bank-robber].
You can, however, slow down disasters as they happen in your programs.

Using a **debugger** you can slow down the execution of your programs to help you figure out why it's not working.
A debugger can't automatically tell you what's broken, but it can...

- step through your code line by line
- show you the state of variables
- show you the contents of memory

...so that you can figure out what's wrong on your own.

In your bank robber dream, the watch allowed you to slow down time to see what set off the laser defense system.
In real life, a debugger allows you to slow down the execution of your program, so that you can see where and when it breaks.
It's still up to you to figure out why it's breaking and how to fix your bugs, but a debugger can definitely give you some clues to make it easier to find them.

In this lab, we'll be using the GNU Debugger (`gdb`) to debug a couple of C++ programs.
`gdb`, like `g++`, is open source software.
There are GUI frontends available for `gdb`, but in this class, we'll be using the command line interface.

### Takeaways

- Learn to debug C++ projects with `gdb`
    - Step through the execution of a compiled C++ program
    - Inspect the contents of program variables

## Walkthrough

### Compilin' for Debuggin'

When you compile a program, you lose a lot of information.
All of the C++ code that you write gets translated into machine-executable code.
As far as your CPU is concerned, there's really no need for function names, curly braces, or comments.

As a human person, you will need those things.

If we plan to debug our code with `gdb`, we need to ask `g++` to to keep the details of our source code when we compile it.
Whenever you compile your code, simply add the `-g` flag to your `g++ command`.
For example:

~~~shell
$ g++ -g main.cpp
~~~

If you forget the `-g`, `gdb` will have a lot less information to work with.
As a result, debugging will be much less useful.

### Starting GNU Debugger

Alrighty, friend.
Let's get our hands dirty.

Pop open an editor and drop in the following C++ program.
Save it as `main.cpp`.
**Make sure** you keep all the whitespace the same.
We're going to need to refer to line numbers when we use `gdb`.

~~~c++
#include <iostream>                                 // 1
                                                    // 2
using namespace std;                                // 3
                                                    // 4
int main()                                          // 5
{                                                   // 6
  int x = 7;                                        // 7
  if (x < 10)                                       // 8
    cout << "Less than 10!" << endl;                // 9
                                                    // 10
  if (x >= 10)                                      // 11
    cout << "Greater than or equal to 10!" << endl; // 12
                                                    // 13
  return 0;                                         // 14
}                                                   // 15
~~~

Now let's compile it and run it in `gdb`.

~~~shell
$ g++ -g -o main main.cpp
$ gdb main
GNU gdb ...
Reading symbols from main...done.
(gdb)
~~~

`gdb` will show you a **bunch** of license information and other junk before it give you its command prompt.
You can ignore that stuff.

You're now in `gdb`!
Here, you can issue commands to `gdb`, and it will do your bidding.
You can step through your program, run parts of it at full speed, and check the values of different variables.

### GNU Debugger Commands

#### Running programs

Assuming you followed the directions in the previous section, you should be sitting at the `(gdb)` prompt ready to run your first command.

Try issuing the `run` command.
Just type `run` and press enter.
Be sure to make a note of the output for the questions section.

The `run` command will start your program and execute it at full speed until it reaches a stopping condition.
Stopping conditions include:

1. reaching the end of a program.
2. reaching a breakpoint.
3. encountering a fatal error (like a segmentation fault).

In order to debug your program, it has to be running.
The `run` command is likely one of the first commands you will run whenever you debug a program.

If you need to pass any command line arguments to the program, you'll pass them to the run command.
For example, if we wanted to debug `g++`[^dont]...

~~~
$ gdb g++
(gdb) run funcs.cpp main.cpp
~~~

... would be similar to running `g++ funcs.cpp main.cpp` outside of `gdb`.

#### Stopping at the right time

If you don't want your program to pause at a specific line, you can place a **breakpoint**.

Assuming your program is not currently running, let's place a breakpoint at line 8 of `main.cpp`.
Then, we'll run our program.

~~~
(gdb) break main.cpp:8
(gdb) run
Starting program: main

Breakpoint 1, main () at main.cpp:8
if (x < 10)
(gdb)
~~~

Our program **is running**, but `gdb` has paused its execution at line 8.

You can set as many breakpoints as you like.
The `info breakpoints` command will show you a list of all the breakpoints you've set.

If you find that you have too many breakpoints, or if they're getting in the way, you can delete them using the `delete` command.
By itself, `delete` will delete all breakpoints.
If you want to delete a specific breakpoint, you can refer to it by its ID number.
For example:

~~~
(gdb) info breakpoints
1       breakpoint     keep y   0x000000000040086c in main() at main.cpp:8
2       breakpoint     keep y   0x000000000040088e in main() at main.cpp:10
3       breakpoint     keep y   0x0000000000400894 in main() at main.cpp:12
(gdb) delete 2
(gdb) info breakpoints
1       breakpoint     keep y   0x000000000040086c in main() at main.cpp:8
3       breakpoint     keep y   0x0000000000400894 in main() at main.cpp:12
(gdb) delete
Delete all breakpoints? (y or n) y
(gdb) info breakpoints
No breakpoints or watchpoints.
~~~

#### Where are we?

You can use the `backtrace` command to ask `gdb` where we currently are.

~~~
(gdb) backtrace
#0  main () at main.cpp:8
~~~

In the output, `gdb` will show you the current backtrace.
Depending how many functions you've called, you'll see deeper and deeper traces.
In this case, we've only called `main()`, so that's the only line we see.
You can also see the file name and line number.

You can also use `list` to ask `gdb` to show you some source code to give you context.
Sometimes a line number isn't enough if you're too lazy to tab over to your text editor.

~~~
(gdb) backtrace
#0  main () at main.cpp:8
(gdb) list
3	using namespace std;
4
5     int main()
6     {
7       int x = 7;
8       if (x < 10)
9         cout << "Less than 10!" << endl;
10
11      if (x >= 10)
12        cout << "Greater than or equal to 10!" << endl;
~~~

An interesting quirk about `list` is that it will continue to show more lines if you run it again.
If it runs out of lines to show you, it becomes grumpy.

~~~
(gdb) list
13
14      return 0;
15    }
(gdb) list
Line number 16 out of range; main.cpp has 15 lines.
~~~

#### Stepping through the code

Now that we've used a breakpoint to pause our code, we can step through the execution.
There are a handful of commands that will help us do this:

- `continue`: Resume `run`ning the program at full speed.
  We'll only stop/pause execution if we reach a stopping condition as described for `run`.
- `step`: Runs **one** line of code, stepping **into** function calls.
  If you reach a function call, `step` will enter that function.
- `next`: Runs **one** line of code, stepping **over** function calls.
  If you reach a function call, `next` will run the entire function until it returns.
- `finish`: Runs code until the current function returns.

Assuming you're following along, we should be paused at line 8 of `main.cpp`.
You can verify this with `backtrace`.
If necessary, run `kill` to stop debugging followed by `run` again.

Try running `step` three times.
Make note of the line you end up on.

#### Looking at contents of variables

`gdb` also allows you to look at what's stored in different variables.
This can be a *very* handy alternative to placing `cout` statements all over the place.

Let's make sure we're on the same metaphorical page here:

1. Run `kill` to stop your program.
2. Run `delete` to delete all of your breakpoints.
3. Run `break main.cpp:7` to create a new breakpoint at line 7.
3. Run `run`

`gdb` should pause execution at line 7 (`int x = 7;`).
Now let's use the `print` command to have a look at the contents of `x`.

~~~
(gdb) print x
$1 = 1231764817
~~~

Well that's interesting, isn't it?
`1231764817` is not `7` at all!
That's because at this point, `gdb` hasn't actually run line 7 yet.
Let's step forward and check it again.

~~~
(gdb) step
8	if (x < 10)
(gdb) print x
$2 = 7
~~~

That makes a lot more sense!

You can use `print` to print out any variable that's in scope.
You can also use `info locals` to check on the value of local variables.
Try running it and make a note of the output.

\newpage
## Questions
Name: `______________________________`

1. What was the output from using the `run` command?
\vspace{10em}

2. How would you set a breakponit for line 10 of file `my_funcs.cpp`?
\vspace{10em}

3. What line number did you end up on after you ran `step` three times?
\vspace{10em}

4. What was the output from running `info locals`?
\newpage

## Quick Reference

### Using `gdb`

- `gdb PROGRAM` launches the debugger for debugging `PROGRAM`
    - **Note**: You will want to pass `g++` the `-g` flag when you compile!
- `run arg1 arg2` runs the command with command line arguments `arg1` and `arg2`
- `backtrace` or `bt` shows the call stack

### Setting breakpoints with `gdb`

- `break main.cpp:10` will stop execution whenever line `10` in `main.cpp` is reached.
- `continue` resumes running as normal.
- `step` runs one more line of code; steps **into** functions as necessary.
- `next` runs until execution is on the next line; steps **over** functions.
- `finish` runs until the current function returns.
- `delete` removes all breakpoints.

### Looking at variables with `gdb`

- `print VARIABLE` prints the contents of variable with name `VARIABLE`
    - `p VARIABLE` also works
    - `p` also works with expressions of just about any sort
- `x address` examines one word memory at a given address
- `x/2 address` examines two words of memory
- `info registers` lists all CPU register values
- `p $REGNAME` prints the value of a CPU register

### Miscellaneous

- Conditional Breakpoints: `condition BPNUMBER EXPRESSION`
- Editing variables at runtime with `gdb`:
    - `set var VARIABLE = VALUE` assigns `VALUE` to `VARIABLE`
    - `set {int}0x1234 = 4` writes `4` (as an integer) to the memory address `0x1234`
- Disassembling code: `disassemble FUNCTION` prints the assembly for a function named `FUNCTION`

## Further Reading

- [More on breakpoints](https://sourceware.org/gdb/current/onlinedocs/gdb/Breakpoints.html)


[^tell-time]: Actually it doesn't tell time at all.
    The hands didn't move when you bought it, and they still don't.
    It sure looks cool, though.
[^escalation]: That escalated pretty quickly.
[^bwee]: *BWEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*
[^dream]: It was a dream that whole time. What a slap in the face!
[^bank-robber]: In real life, hopefully you're not a bank robber either.
[^dont]: Don't actually do this, though.
    We're just demonstrating how you'd pass command line arguments.
