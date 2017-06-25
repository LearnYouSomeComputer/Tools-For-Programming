# Building with Make

## Motivation

Wow. You've made it seven chapters through this book. And probably some appendices too. And yet you have made not one sandwich. Not one!

Let's fix that. Time for a classic pastrami on rye.
You go to fetch ingredients from the refrigerator, but alas! It is empty.
Someone else has been eating all your sandwiches while you were engrossed in regular expressions.

You hop on your velocipede[^bike] and pedal down to the local bodega only to discover that they, too, are out of sandwich fixin's.
Just as you feared -- you are left with no choice other than to derive a sandwich from first principles.

A day of cycling rewards you with the necessities: brisket, salt, vinegar, cucumbers, yeast, rye, wheat, caraway seeds, sugar, mustard seed, garlic cloves, red bell peppers, dill, and peppercorns.
Fortunately you didn't have to tow a cow home!
You set to work, pickling the beef and the cucumbers and setting the bell peppers out to dry.
Once the meat has cured, you crush the peppers, garlic, mustard seed, and peppercorns into a delicious dry rub and fire up the smoker.
Eight hours later and your hunk of pastrami is ready to be steamed until it's tender.

Meanwhile, you make a dough of the rye and wheat flours, caraway seeds, yeast, and a little sugar for the yeast to eat.
It rises by the smoker until it's ready to bake. You bake it with a shallow pan of water underneath so it forms a crisp outer crust.

Finally, you crush some mustard seed and mix in vinegar.
At last, your sandwich is ready. You spread your mustard on a slice of bread, heap on the pastrami, and garnish it with a fresh pickle.
Bon appétit!

In between bites of your sandwich, you wonder: "Wow, that was a lot of work for a sandwich.
And whenever I eat my way through what I've prepared, I'll have to do it all over again.
Isn't there a Better Way?"

A bite of crunchy pickle is accompanied by a revelation.
If the human brain is a computer, then this sandwich is code[^code]: without it, your brain could compute nothing![^torture]
"Wow!" you exclaim through a mouthful of pickle, "This is yet another problem solved by GNU Make!"

Using the powers of `git`, you travel into the future, read the rest of this chapter, then head off your past self before they pedal headfirst down the road of wasted time.[^time]
Instead of this hippie artisanal handcrafted sandwich garbage, you sit your past self down at their terminal and whisper savory nothings[^nothings] in their ear.
They -- you -- crack open a fresh editor and pen the pastrami of your dreams:

```makefile

pickles: cucumbers vinegar dill
	brine --with=dill cucumbers

cured-brisket: brisket vinegar
	brine brisket

paprika: red-peppers
	sun-dry red-peppers | grind > paprika

rub: paprika garlic mustard-seed peppercorns
	grind paprika garlic mustard-seed peppercorns > rub

smoked-brisket: cured-brisket rub
	season --with=rub cured-brisket -o seasoned-brisket
	smoke --time=8 hours seasoned-brisket

pastrami: smoked-brisket
	steam --until=tender smoked-brisket

dough: rye wheat coriander yeast sugar water
	mix --dry-first --yeast-separately rye wheat coriander yeast \
		sugar water --output=dough

rye-loaf: dough
	rise --location=beside-smoker dough && bake -t 20m

mustard: mustard-seed vinegar
	grind mustard-seed | mix vinegar > mustard

sandwich: pastrami pickles rye-loaf
	slice rye-loaf pastrami
	stack bread-slice --spread=mustard pastrami bread-slice
	present sandwich --garnish=pickle
```

Et voilà! You type `make sandwich`. Your computer's fans spin up. Text flies past on the screen.
A bird flies past the window. Distracted momentarily, you look away to contemplate the beauty of nature.
When you turn back, there on your keyboard is a delicious sandwich, accompanied by a pickle.
You quickly get a paper towel to mop up the pickle brine before it drips into your computer.
Should have used `plate`!

Since you've already read this chapter in the future, I should not need to mention the myriad non-culinary uses of `make`.
However, for the benefit of those who skipped the time-travel portion of the `git` chapter, I will anyway.
`make` is a program for making files from other files.
Perhaps its most common application is compiling large programming projects: rather than compiling every file each time you change something,
`make` can compile each file separately, and only recompile the files that have changed.
Overall, your compile times are shorter, and typing `make` is much easier than typing `g++ *.cpp -o neat-program`.
It has other uses, too: this book is built with `make`!

### Takeaways

- Learn to make a decent pastrami on rye
- Learn how to compile and link your C`++` code
- Understand `make`'s syntax for describing how files are built
- Use variables and patterns to shorten complex makefiles

## Walkthrough

### A bit about compiling and linking

Ok so before we can set up a makefile for a C`++` project, we need to talk about compiling and linking code.
"Compiling" refers to the process of turning C`++` code into machine instructions.
Each `.cpp` file gets compiled separately, so if you use something defined in another file or library -- for example, `cin` --
the compiler leaves itself a note saying "later on when you figure out where `cin` is, put its address here".
Once your code is compiled, the linker then goes through all your compiled code and any libraries you have used and fills in all the notes
with the appropriate addresses.

You can separate these steps: `g++ -c file.cpp` just does the compilation step to `file.cpp` and produces a file named `file.o`.
This is a so-called *object file*; it consists of assembly code and cookies left out for the linker.[^milk]

To link a bunch of object files together, you call `g++` again[^confusing] like so: `g++ file1.o file2.o -o myprogram`.
`g++` notices that you have given it a bunch of object files, so instead of going through the compilation process,
it prepares a detailed list[^twice] explaining to the linker which files and libraries you used and how to combine them together.
It sets the list next to your object files[^milk2] and waits for the linker.
When the linker arrives, it paws through your object files, eats all the cookies,
and then through a terrifying process not entirely understood by humans[^exaggerating],
leaves you a beautiful executable wrapped up under your tree[^lovecraft].

![How Executables are Made](07/compile.png){width=70%}

So, what's the big deal?
Well, if you compile your code to object files and then change some of your code, you only need to rebuild the object files
associated with the code you changed!
All the other object files will stay the same.
If you have a big project with a lot of files, this can make recompiling your code significantly faster!
Of course, doing all this by hand would be awful...which is why we have `make`!

### Building Files

- File targets
- Dependencies
- Multiple targets

### Phony Targets

### Variables

### Pattern Targets

### Useful Flags

## Questions

## Quick Reference

## Further Reading

[^bike]: Velocipede (n): A bicycle for authors with access to a thesaurus.
[^code]: And your code is a sandwich: oodles of savory instructions sandwiched between the ELF header and your static variables.
[^torture]: The only thing more tortured than this analogy is the psychology 101 professors reading another term paper on how humans are just meat computers.
[^time]: Boy howdy do I wish this happened to me more often.
[^nothings]: Chaste tales of future sandwiches.
[^milk]: Good programmers always set a glass of milk out when compiling large programs.
[^confusing]: I agree that this is somewhat confusing, but trust me it is much much much less confusing than figuring out how to call `ld`, the actual linker tool, on your own.
[^twice]: And checks it twice.
[^milk2]: And the glass of milk, if you set one out for this purpose.
[^exaggerating]: I am not exaggerating here; linking executables is surprisingly full of arcane, system-dependent edge cases.
Fortunately some other poor soul (i.e., your compiler maintainer) has figured this out already and you should never need to worry about it.
[^lovecraft]: This Christmas chapter brought to you by H. P. Lovecraft.
