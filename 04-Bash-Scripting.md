# Bash Scripting

## Motivation

In addition to being a fully-functional[^functional] interactive shell, Bash can also run commands from a text file (known as a 'shell script').
It even includes conditionals and loops!
These scripts are the duct tape and bailing wire of computer programming -- great for connecting other programs together.
Use shell scripts to write one-off tools for odd jobs, to build utilities that make your life easier, and to customize your shell.

**Note**: There's nothing special about the contents of a shell script -- everything you learn in this lab you could type into the Bash prompt itself.

### Takeaways

- Learn to glue programs together into shell scripts
- Gain more experience working with output redirection in bash

## Walkthrough

Here's a quick example of what a shell script looks like:
```bash
#!/bin/bash

g++ *.cpp
./a.out
```

This script compiles all the C++ files in the current directory, then runs the resulting executable.
To run it, put it in a file named, say, `runit1.sh`, then type `./runit1.sh`[^path] at your shell prompt.

Two things to note:

1. The first line, called a "shebang"[^shebang], tells Bash what program to run the script through. In this case, it's a Bash script.
2. The rest of the file is a sequence of commands, one per line, just as you would type them into the shell.

For simple scripts, this may be all you need!
Just stick a list of commands you always run together into a file, and you have a shell script.

However, more complicated tasks will require an actual programming language with variables and loops and all that jazz.
Fortunately, Bash has all of these things!
Let's check out some of those features and use them to improve this example.

### Variables

#### Using Variables
There's no special keyword for declaring variables; you just define what you want them to be.
When you use them, you must prefix the variable name with a `$`:

```bash
#!/bin/bash

# Assign the value "big" to the variable COW
# (by the way, things that start with # are comments)
COW="big"

echo $COW
```

**Note:** It is *very* important that you not put any spaces around the `=` when assigning to variables in Bash.
Otherwise, Bash gets very confused and scared, as we all do when encountering something unfamiliar.
If this happens, gently pet its nose until it calms down, then take the spaces out and try again.

Variables can hold strings or numbers.
Bash is dynamically typed, so there's no need to specify `int` or `string`; Bash just works out what you (probably) want on its own.

It is traditional to name variables in uppercase, but by no means required.
Judicious use of caps lock can help keep the attention of a distractible Bash instance.

#### Special Variables
Bash provides numerous special variables that come in handy when working with programs.

To determine whether a command succeeded or failed, you can check the `$?` variable, which contains the return value[^return] of the last command run.
Traditionally, a value of `0` indicates success, and a non-zero value indicates failure.
Some programs may use different return values to indicate different types of failures; consult the man page for a program to see how it behaves.

For example, if you run `g++` on a file that doesn't exist, g++ returns `1`:
```
$ g++ no-such-file.cpp
g++: error: no-such-file.cpp: No such file or directory
g++: fatal error: no input files
compilation terminated.
$ echo $?
1
```

Bash also provides variables holding the command-line arguments passed to the script.
A command-line argument is something that you type after the command; for instance, in the command `ls /tmp`, `/tmp` is the first argument passed to `ls`.
The name of the command that started the script is stored in `$0`. This is almost always just the name of the script[^symlinks].
The variables `$1` through `$9` contain the first through ninth command line arguments, respectively.
To get the 10th argument, you have to write `${10}`, and likewise for higher argument numbers.

The array `$@` contains all the arguments except `$0`; this is commonly used for looping over all arguments passed to a command.
The number of arguments is stored in `$#`; if no arguments are passed, its value is 0.
(If you've read Appendix D, these are analogous to the `argv` and `argc` parameters passed to `int main()` in C++ programs.
However, `argc` counts the 0th argument; `$#` does not.)

#### Whitespace Gotchas

Bash is very eager to split up input on spaces.
Normally this is what you want -- `cat foo bar` should print out the contents of two files named "foo" and "bar", rather than trying to find one file named "foo bar".
But sometimes, like when your cat catches that mouse in your basement but then brings it to you rather than tossing it over the neighbor's fence like a good pal,
Bash goes a little too far with the space splitting.

If you wanted to make a file named "cool program.cpp" and compile it with `g++`, you'd need to put double quotes around the name: `g++ "cool program.cpp"`.
Likewise, when scripting, if you don't want a variable to be space split, surround it with double quotes.
So as a rule, rather than `$1`, use `"$1"`, and iterate over `"$@"` rather than `$@`.

#### Example

We can spiff up our example to allow the user to set the name of the executable to be produced:

```bash
#!/bin/bash

g++ *.cpp -o "$1"
./"$1"
```

You'd run this one something like `./runit2.sh program_name`.

### Conditionals

#### If statements

The `if` statement in Bash runs a program[^builtin] and checks the return value.
If the command succeeds (i.e., returns 0), the body of the if statement is executed.

Bash provides some handy commands for writing common conditional expressions:
`[ ]` is shorthand for the `test` command, and `[[  ]]` is a Bash builtin.
`[ ]` works on shells other than Bash, but `[[ ]]` is far less confusing[^portability].

Here's an example of how to write `if` statements in Bash:

```bash
#!/bin/bash

# Emit the appropriate greeting for various people

if [[ $1 = "Jeff" ]]; then
	echo "Hi, Jeff"
elif [[ $1 == "Maggie" ]]; then
	echo "Hello, Maggie"
elif [[ $1 == *.txt ]]; then
	echo "You’re a text file, $1"
elif [ "$1" = "Stallman" ]; then
	echo "FREEDOM!"
else
	echo "Who in blazes are you?"
fi
```

Be careful not to forget the semicolon after the condition or the `fi` at the end of the if statement.

#### Writing conditionals with `[[ ]]`

Since Bash is dynamically typed, `[[ ]]` has one set of operators for comparing strings and another set for comparing numbers.
That way, you can specify which type of comparison to use, rather than hoping that Bash guesses right[^javascript2].

Comparing Strings:

- `=`,`==` means either:
	- String equality, if both operands are strings, or
	- Pattern (glob) matching, if the RHS is a glob  (e.g., `*.txt`).
- `!=` means either:
	- String inequality, if both operands are strings, or
	- Glob fails to match, if the RHS is a glob.
- `<`: The LHS sorts before the RHS.
- `>`: The LHS sorts after the RHS.
- `-n`: The string is not empty (e.g., `[[ -n "$var" ]]`).
- `-z`: The string is empty (length is zero).

Comparing Numbers:

(These are all meant to be used infix, like `[[ $num -eq 5 ]]`.)

- `-eq`: Numeric equality.
- `-ne`: Numeric inequality.
- `-lt`: Less than.
- `-gt`: Greater than.
- `-le`: Less than or equal to.
- `-ge`: Greater than or equal to.

Checking Attributes of Files:

(Use these like `[[ -e story.txt ]]`.)

- `-e`: True if the file exists.
- `-f`: True if the file is a regular file.
- `-d`: True if the file is a directory.

Here's an example that, given a directory, lists the files in it; and given a file, prints the contents of the file:

```bash
#!/bin/bash

# First, check to make sure we got an argument
if [[ $# -eq 0 ]]; then
	echo "Usage: $0 <file or directory name>"
	exit 1
fi

# Then, determine what we ought to do
if [[ -f "$1" ]]; then
	cat "$1"
elif [[ -d "$1" ]]; then
	ls "$1"
else
	echo "I don't know what to do with $1!";
fi
```

There are a number of other file checks that you can perform; they are listed in [the Bash manual](https://www.gnu.org/software/bash/manual/html_node/Bash-Conditional-Expressions.html).

Boolean Logic:

- `&&`: Logical AND.
- `||`: Logical OR.
- `!`: Logical NOT.

You can also group statements using parentheses:

```bash
#!/bin/bash

num=5

if [[ ($num -lt 3) && ("story.txt" == *.txt) ]]; then
  echo "Hello, text file!"
fi
```

#### Writing conditionals with `(( ))`

`(( ))` is used for arithmetic, but it can also be used to do numeric comparisons in the more familiar C style:

- `>`,`>=`: Greater than/Greater than or equal
- `<`,`<=`: Less than/Less than or equal
- `==`,`!=`: Equality/inequality

When working with `(( ))`,  you do not need to prefix variable names with `$`:

```bash
#!/bin/bash

x=5
y=7

if (( x < y )); then
  echo "Hello there"
fi
```


#### Case statements

Case statements in Bash work similar to the `==` operator for `[[ ]]`; you can make cases for strings and globs.

Here is an example case statement:

```bash
#!/bin/bash

case $1 in
	a)
		echo "a, literally"
		;;
	b*)
		echo "Something that starts with b"
		;;
	*c)
		echo "Something that ends with c"
		;;
	"*d")
		echo "*d, literally"
		;;
	*)
		echo "Anything"
		;;
esac
```

Do not forget the double semicolon at the end of each case -- `;;` is *required* to end a case.
They are analogous to `break` in C++; bash case statements do not have fallthrough.

As with `if`, `case` statements end with `esac`.

#### Example

We can use conditional statements to spiff up our previous `runit2.sh` script.
This example demonstrates numeric comparison using both `(( ))` and `[[ ]]`.

```bash
#!/bin/bash

# If the user specifies an executable name, use that name;
# otherwise, name the executable 'a.out'
if (( $# > 0 )); then
	g++ *.cpp -o "$1"
	exe="$1"
else
	g++ *.cpp
	exe=a.out
fi

# Run the program only if it successfully compiled!
if [[ $? -eq 0 ]]; then
	./"$exe"
fi
```

Can you make this example even spiffier using file attribute checks?

### Arithmetic

`(( ))` also performs arithmetic; the syntax is pretty much borrowed from C.
Inside `(( ))`, you do not need to prefix variable names with `$`!

For example,
```bash
#!/bin/bash

x=5
y=7
(( sum = x +  y ))
echo $sum
```

Operator names follow those in C; `(( ))` supports arithmetic, bitwise, and logical operators.
You can also write ternary expressions!
One difference between C and `(( ))` is that `**` can be used for exponentiation: `((four = 2**2))`.
See [the Bash manual](https://www.gnu.org/software/bash/manual/html_node/Shell-Arithmetic.html#Shell-Arithmetic) for an exhaustive list of operators.

### Looping

#### For Loops

Bash for loops typically follow a pattern of looping over the contents of an array (or array-ish thing).

For (heh) example, you can print out the names of all `.sh` files in the current directory like so:

```bash
#!/bin/bash

for file in *.sh; do
    echo $file
done
```

Or sum all command-line arguments:

```bash
#!/bin/bash

sum=0

for arg in "$@"; do
  (( sum += arg ))
done

echo $sum
```

If you need a counting for loop (C-style loop), you can get one of those with `(( ))`:

```bash
#!/bin/bash

for (( i=1; i < 9; i++ )); do
    echo $i;
done
```

With for loops, do not forget the semicolon after the condition.
The body of the loop is enclosed between the `do` and `done` keywords (sorry, no `rof` for you!).

#### While Loops

Bash also has while loops, but no do-while loops.
As with for loops, the loop body is enclosed between `do` and `done`.
Any conditional you'd use with an if statement will also work with a while loop.

For example,
```bash
#!/bin/bash

input=""
while [[ "$input" != "4" ]]; do
    echo "Please guess the random number: "
    read input
done
```

This example uses the `read` command, which is built in to Bash, to read a line of input from the user (i.e., STDIN).
`read` takes one argument: the name of a variable to read the line into.
It is quite similar to `getline()` in C++.

### "Functions"

Bash functions are better thought of as small programs, rather than functions in the typical programming sense.
They are called the same way as commands, and inside a function, its arguments are available in `$1`, `$2`, etc.
Furthermore, they can only return an error code; "returning" other values requires some level of trickery.

Here's a simple function example:

```bash
#!/bin/bash

# Declare a function named 'parrot'
parrot() {
	while (( $# > 0 )); do
		echo "$1"
		shift
	done
}

# Call parrot() with some arguments
parrot These are "several arguments"
```

(`shift` is a built-in that throws away the first argument and shifts all the remaining arguments down one.)

To return something, the easiest solution is to `echo` it and have the caller catch the value:

```bash
#!/bin/bash

average() {
	sum=0
	for num in "$@"; do
		(( sum += num ))
	done

	(( avg = sum / $# ))
	echo $avg
}

my_average=$(average 1 2 3 4)

echo $my_average
```

Here, `my_average=$(average 1 2 3 4)` calls `average` with the arguments `1 2 3 4` and stores the STDOUT of `average` in the variable `my_average`.

One word of warning: in Bash, all variables are globally scoped by default, so it is easy to accidentally clobber a variable:

```bash
#!/bin/bash

# create a bunch of files with "hello" in them
create_some_files() {
	for ((i=0; i < $1; i++)); do
		echo "What file would you like to create?"
		read input
		echo "hello" > "$input"
	done
}

# get the number of files the user wants to create
echo "How many files do you want me to create?"
read input
create_some_files "$input"

# BUG: this prints the last created filename!
echo "Created $input files"
```

You can scope variables to functions with the `local` builtin; run `help local` in a Bash shell for details.

### Tips

To write a literal ``\ , `, $, ", ’, #``, escape it with `\`; for instance, `"\$"` gives a literal `$`.

When writing scripts, sometimes you will want to change directories -- for instance, maybe you want to write some temporary files in `/tmp`.
Rather than using `cd` and keeping track of where you were so you can `cd` back later, use `pushd` and `popd`.
`pushd` pushes a new directory onto the directories stack and `popd` removes a directory from this stack.
Use `dirs` to print out the stack.

For instance, suppose you start in `~/cool_code`.
```
$ pwd
~/cool_code
$ dirs
~/cool_code
```

`pushd /tmp` changes the current directory to `/tmp`.

```
$ pushd /tmp
$ pwd
/tmp
$ dirs
/tmp ~/cool_code
```

Calling `popd` then removes `/tmp` from the stack and changes to the next directory in the stack, which is `~/cool_code`.
```
$ pwd
/tmp
$ popd
~/cool_code
$ pwd
~/cool_code
$ dirs
~/cool_code
```

Putting `set -u` at the top of your script will give you an error if you try to use a variable without setting it first.
This is particularly handy if you make a typo; for example, `rm -r $delete_mee/*` will call `rm -r /*` if you haven't set `$delete_mee`!

Bash contains a help system for its built-in commands: `help pushd` tells you information about the `pushd` command.

### Customizing Bash

Let's say you've typed out our example script for compiling and running C++ code and you've put it in a directory called `scripts` in your home directory.
So, the path to the script is `~/scripts/runit.sh`.

Now, if you're off in some other directory (say `~/cs1510/hw3`), you can run that script like so:

```
$ ~/scripts/runit.sh
```

But this isn't very cool; you want to be able to run it like a regular ol' command and not have to type the dang path out each time!
Well, there is a solution.
When you type a command in at the Bash prompt that isn't the full path to the program you want to run, Bash has a list of places it looks.
This list is stored in the `$PATH` variable; the list of directories is delimited by colons.

Your `$PATH` may look something like this:
```
$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
```

When you type in a command, say, `g++`, Bash looks through each directory in `$PATH` in order until it finds one with an executable named `g++` in it.
You can see which one it picks out using the `which` command:

```
$ which g++
/usr/bin/g++
```

You can add your own directories to `$PATH`!
So, if you want to be able to call `runit.sh` from wherever, you could do the following:

```
$ PATH=~/scripts:$PATH
$ which runit.sh
~/scripts/runit.sh
```

The first command puts your `scripts` directory as the first directory in the path to search.
The second command demonstrates that now Bash knows where to find `runit.sh` regardless of where you execute it from!

There's one problem with this setup: Bash won't remember that you added `~/scripts` to the `$PATH`.
You need some way to run that assignment every time a new Bash instance executes.
Fortunately, there is a file in your home directory called `.bashrc` that Bash executes every time it runs.

So, open up `~/.bashrc` and at the end, enter the following line:

```bash
export PATH=~/scripts:$PATH
```

(The `export` command causes the value of `$PATH` set in the script to apply even after the script exits.)

You can also add other customizations in your `.bashrc`.
Rather than writing a script as its own file, you can write it as a function instead and store it in your `.bashrc`.

There are also a number of variables that you can set to configure other parts of your environment --- for instance, setting `$PS1` changes your shell prompt's text.
See `help variables` for a list of knobs and dials you can fiddle with.

\newpage
## Questions
Name: `______________________________`

1. What does the `let` builtin do?
\vspace{10em}

2. Write a script that prints "fizz" if the first argument is divisible by 3, "buzz" if it is divisible by 5, and "fizzbuzz" if it is divisible by both 3 and 5.[^interview]
\vspace{20em}

3. Write a script that prints "directory" if the first argument is a directory and "file" if the first argument is a file.
\newpage

## Quick Reference

### Conditions

#### `[[ ]]`

Comparing Strings:

- `=`,`==` means either:
	- String equality, if both operands are strings, or
	- Pattern (glob) matching, if the RHS is a glob  (e.g., `*.txt`).
- `!=` means either:
	- String inequality, if both operands are strings, or
	- Glob fails to match, if the RHS is a glob.
- `<`: The LHS sorts before the RHS.
- `>`: The LHS sorts after the RHS.
- `-n`: The string is not empty (e.g., `[[ -n "$var" ]]`).
- `-z`: The string is empty (length is zero).

Comparing Numbers:

- `-eq`: Numeric equality.
- `-ne`: Numeric inequality.
- `-lt`: Less than.
- `-gt`: Greater than.
- `-le`: Less than or equal to.
- `-ge`: Greater than or equal to.

Checking Attributes of Files:

- `-e`: True if the file exists.
- `-f`: True if the file is a regular file.
- `-d`: True if the file is a directory.

Boolean Logic:

- `&&`: Logical AND.
- `||`: Logical OR.
- `!`: Logical NOT.


#### `(( ))`

- `>`,`>=`: Greater than/Greater than or equal
- `<`,`<=`: Less than/Less than or equal
- `==`,`!=`: Equality/inequality

### Statements

#### If

```bash
if <condition>; then
	<commands to run>
fi
```

#### Case

```bash
case <variable or expression> in
	first-case)
		<commands>
		;;

	second-case)
		<commands>
		;;
esac
```

#### Loops

Iterating loop:

```bash
for <variable-name> in <array>; do
	<commands>
done
```

Counting loop:

```bash
for ((i=0; i < 7; i++)); do
	<commands>
done
```

While loop:

```bash
while <condition>; do
	<commands>
done
```

## Further Reading

- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Bash Guide](http://mywiki.wooledge.org/BashGuide)
- [Bash Tutorial](http://tldp.org/LDP/Bash-Beginners-Guide/html/)
- [Advanced Bash Scripting Guide](http://tldp.org/LDP/abs/html/)


[^functional]: Disclaimer: Bash may be neither full nor functional for your use case.
Consult your primary care physician to see if Bash is right for you.
[^shebang]: A combination of "sharp" (#) and "bang" (!).
[^path]: `.` is shorthand for the current directory, so this tells bash to look in the current directory for a file named `runit1.sh` and execute that file.
We'll talk more about why you have to write this later on.
[^return]: This is the very same value as what you return from `int main()` in a C++ program!
[^symlinks]: If you must know, the other possibility is that it is started through a link (either a hard link or a symbolic link) to the script.
In this case, `$0` is the name of the link instead.
Any way you slice it, `$0` contains what the user typed in order to execute your script.
[^builtin]: Or a builtin shell command (see `man bash` for details).
[^portability]: If you're writing scripts for yourself and your friends, using `[[ ]]` is a-ok;
the only case you'd care about using `[ ]` is if you're writing scripts that have to run on a lot of different machines.
In this book, we'll use `[[ ]]` because it has fewer gotchas.
[^javascript2]: If you know some JavaScript you might be familiar with the problem of too-permissive operators:
in JS, `"4" + 1 == "41"`, but `"4" - 1 == 3`.
[^interview]: Also, why do so many people ask this as an interview question!?
