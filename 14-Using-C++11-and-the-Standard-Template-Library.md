# Using C++11 and the Standard Template Library

## Motivation

Finally.

The powers that be have decided to build a STØR in your hometown.
It's taken months to finish construction and stock the shelves, but now you're ready to check out their colorful housewares and famous, trendy-but-fragile furniture.

You chortle with excitement as you ride the escalator into the store.
A bored sad man awaits you at the top.

"Hedge," he grunts.

After following the maze of clearly-marked paths through the furniture showcase, you marvel at your neatly compiled list of items to find in the warehouse:

- Bort
- Bort bort
- Vector
- Map
- Tuple
- Bort bort bort
- Pair

You march confidently downstairs to find your items in the housewares section.
Luckily, the `Bort( Bort)+` are on display at the front.
All that Bort is/are making it hard to hold the STØR map.
As you struggle, you hear a friendly voice say,

"Hi-diddily-ho, customer-ino! Looks like you could use a hand!"

You stare.

"Let me guide ya around the STØR-a-roonie[^not-supposed-to] here and we'll find what you're looking for!"

With an arm full of Bort you continue your mission in search of the other items on your list.
However, now you have a mustachio'd lunatic to guide you.

### Takeaways

- Learn to use several language features offered by the C++11 Language Standard:
    - `auto` types
    - `for-each` loops
- Learn to use several data structures provided by the C++ Standard Template Library:
    - `std::vector`
    - `std::map`
    - `std::pair`
    - `std::tuple`

## Walkthrough

### Language Features

C++11 introduces a couple of language features that were not available in earlier versions of C++.
This is great, but your compiler needs to know if you're using C++11 features.
Modern compilers assume you are, but if the features we're going to see confuse the living daylights out of
your version of `g++`, try passing it the `-std=c++11` flag.

~~~shell
# For example
$ g++ -std=c++11 main.cpp
~~~

#### The `auto` keyword

The `auto` keyword asks the compiler to figure out the type of a variable for you.[^cpp17]
`auto` is not magic: the compiler just looks at the type of the expression on the other side of the equals sign
and uses that for your variable type.[^hindley-milner]
Again, `auto` is not magic: if you can't see what type the compiler is supposed to use, the compiler probably won't be able
to figure it out either.

We could take this program...

~~~{.cpp .numberLines}
int main()
{
  char cstring[] = "asdf";
  string str = string("asdf");

  vector<int> thingers = vector<int>();
  return 0;
}
~~~

... and make it a little more readable ...

~~~{.cpp .numberLines}
int main()
{
  auto cstring = "asdf";
  auto str = string("asdf");

  auto thingers = vector<int>();
  return 0;
}
~~~

As you'll see later in this chapter, `auto` comes in handy when you work with template types in the standard template library.
Those types are often pretty lengthy, and `auto` keeps your lines short.
`auto` also comes in handy when you need to hold, say, the return value from a library function just long enough to pass it to
another library function --- you couldn't care less what the type of the value is, just as long as it gets to where it needs to go.

#### The for-each loop

As the name implies, a for-each allows you to perform an action "for each item" in a container.
These loops work with many types in the standard template library.
Here's a quick list of things you can use a for-each loop with:

- arrays
- certain classes
    - `std::vector`
    - `std::map`
    - any other type with `begin()` and `end()` member functions

Refer to the further reading section to get an idea of what's required to get for-each loops to work with your own classes!

Let's have a look at an example:

~~~{.cpp .numberLines}
int main()
{
  int nums[] = {1,2,3,4,5,6};

  for (auto i : nums)
  {
    cout << i * i << ", ";
  }

  return 0;
}
~~~

Which outputs

~~~
$ ./print-squares
1, 4, 9, 16, 25, 36,
~~~

In this example, we've created an array with six `int`s in it.
We then use a for-each loop to iterate over all of those items.
Upon each iteration, `i` is set to the **value** of the current element.
It starts at `1`, then `2`, on and on until it reaches `6`.
For each `int`, it computes and prints the square of that value.

One caveat to be aware of is that changing the value of your loop variable (`i` in this case) won't change the thing we're iterating over (the array of `int`s).
If we want to change the values in the array, we can use a reference variable instead.
We could write `for(int& i : nums)`, or appending `&` to `auto` tells the compiler to infer a reference type instead:

~~~{.cpp .numberLines}
int main()
{
  int nums[] = {1,2,3,4,5,6};

  // decrement every value by one
  for (auto& i : nums)
  {
    i--;
  }

  for (auto i : nums)
  {
    cout << i * i << ", ";
  }

  return 0;
}
~~~

Output:

~~~
0, 1, 4, 9, 16, 25,
~~~

Just to be clear: for-each loops don't *have* to be used with `auto` --- although they are quite a handy application for `auto`!
Also, all of the containers we are about to look at in this chapter can be used with for-each loops.

### A Handful of Containers from the Standard Template Library

The Standard Template Library (STL) is vast.
It has a lot of storage types for any need you can think of.
More than STØR, possibly.

#### `std::vector` (`#include<vector>`)

"Here she is! The STØR::vector. Ain't she a beaut'?"

You stare[^loss-for-words] at the accordion-looking contraption in his hands.

"Wanna see how it works?"

He doesn't actually give you time to answer.

"You see, it's empty now, so it's totally flat.
This indicator on the top says `size: 0` to let you know that.
All we gotta do is **push** something into this compartment here, and whaddaya know?
It says `size: 1` now.
You can push as many items on the back here as ya please.
It adjusts its space to accommodate whatever you put in here."

"The only trouble is that everything in there has to be the same kind of thing.
If you set it up to store Bort, that's all it can store.
It can't store any Snell or Löjlig or anything.
Only more Bort."

He wedges it in your arms between the Bort and Bort-Bort.

"Now don't lollygag! Let's see what else is on your list."

---

If that explanation didn't sit quite right with you, a `std::vector` is like an array that resizes on the fly.
Everything stored in it must be the same type, but it is templated so you can choose what the type is.
You can push items on the back to grow the `vector`, and pop them off to shrink it.

~~~{.cpp .numberLines}
vector<int> v; // An empty vector of ints
for (int i = 0; i < 10; i++)
{
  v.push_back(i);
}

for(int index = 0; index < v.size(); index++)
{
  cout << v[index] << ' ';
}
~~~

Output:

~~~
0 1 2 3 4 5 6 7 8 9
~~~

A word of caution: if you give the `[]` operator an index outside the bounds of the vector, it will (probably) segfault, just like a
regular C++ array would.
However, the `at` function is range--checked, and will throw an `out_of_range` exception if you pass it an index too large.
We could rewrite line 11 above as `cout << v.at(index) << endl;` if we wanted to ensure we don't walk off the end of the vector.

STL vectors also feature *iterators*, which are objects that help iterate over the elements of a vector.
They're used sort of like a pointer: you use `*` to 'dereference' the value the iterator is at,
and you can use `++` and `--` to move forward or backward through the elements of the vector.

So, we could write the above loop using iterators like so:

~~~{.cpp .numberLines startFrom=13}
for (vector<int>::iterator iter = v.begin(); iter != v.end(); ++iter)
{
	cout << *iter << endl;
}

// For-each loops use iterators under the hood
for (int value : v)
{
	cout << value << endl;  // No need to "dereference"!
}
~~~

(`auto` is quite handy for replacing `vector<int>::iterator` on line 13 above.)

Iterators are also used to erase elements from or insert elements into the vector.
To remove the third element from `v`:

~~~{.cpp .numberLines startFrom=23}
// Adding 2 to begin() gives an iterator at the 3rd item
v.erase(v.begin() + 2);
// Now v = [0, 1, 3, 4, 5, 6, 7, 8, 9]
~~~

Items are inserted *before* the item the iterator points at.
We can put 2 back into our vector like so:

~~~{.cpp .numberLines startFrom=26}
v.insert(v.begin() + 2, 2);
~~~

Using `erase` and `insert` saves you the effort of writing a loop every time you want to slide something out of or into the middle
of a vector.

#### `std::tuple` (`#include<tuple>`)

"The STØR::tuple isn't for everyone."

He holds what looks like a shoe box.

"You can take an put whatever you want in here, but whatever kind of thing that is...that's all it can store.
In fact, there are a bunch more rules about what it can and can't store.
It's a pretty advanced little piece of container technology, really.
I'll leave you to read the manual about that."

"Now what's cool about this fella is you can attach a bunch of them together!
We can snap three together and it's a three-tuple.
And each compartment can store *different kinds of things*!
We could put a Bort here and a Sbibble here and even a Blagoonga here on the end!"

"Now, unlike that STØR::vector, you can't change the size once it's got stuff in it.
This three-tuple has to have three items.
We can't add to it, and we can't take away.
It's stuck storing this many items, and it's stuck storing these kinds of items."

He sets the tuple on top of your vast pile of items.

---

The `std::tuple` is kind of like a struct.
It has a set number of items which can have different types, but the types of those items cannot change.
Unlike a struct, you cannot name the members of a tuple --- you just access them by their index.
They come in handy primarily in situations where you want something like a struct, but don't want to go to the effort
of building a struct for a one-off use.

Here's a tuple that stores a char, a float, and int, and a string!

~~~{.cpp .numberLines}
tuple<char,float,int,string> thing('x', 2.5, 4, "ABC");
~~~

If you want to get the items out of a tuple, you need to use the `get<N>()` function.
The template parameter to `get` (`N`) is the index of the item you want.
If you want the first item in a tuple named `tup`, you'd use `get<0>(tup)` to get it.

We can print out that above tuple like so:

~~~{.cpp .numberLines startFrom=2}
cout << get<0>(thing) << ' '
     << get<1>(thing) << ' '
     << get<2>(thing) << ' '
     << get<3>(thing) << endl;
~~~

Alternatively, you can use the `tie()` function to "tie" variables to values.[^references]
This lets you assign each element of the tuple to a variable in one line:

~~~{.cpp .numberLines startFrom=5}
char c;
float f;
int i;
string s;

tie(c, f, i, s) = thing;

cout << c << ' ' << f << ' '
     << i << ' ' << s << endl;
~~~

In the above example, we use `tie()` to assign the items in `thing` to `c`, `f`, `i`, and `s` respectively.
After the last line, `c == 'x'`, `f == 2.5`, `i == 4`, and `s == "ABC"`.

If you want to use `tie`, but have some values in your tuple that you don't want to assign to any variable, you can use the `ignore` object.
Consider the following example:

~~~{.cpp .numberLines}
// A coordinate with a name
tuple<int,int,string> coord_name(2,4,"A");

// Prints (A, 2, 4)
cout << get<2>(coord_name) << ": ("
     << get<0>(coord_name) << ","
     << get<1>(coord_name) << ")\n";

int x, y;

// Unpacks the first two items into x and y, and ignores the last item.
tie(x, y, ignore) = coord_name;

// Prints (2, 4)
cout << "(" << x << "," << y << ")\n";
~~~

One cool use for the `tie` function is to "return multiple values".
See those quotes?
You're really just returning one `tuple`, but with a call to `tie()`, it's kinda like you're returning more than one thing at a time.

~~~{.cpp .numberLines}
tuple<int,int> divide(int divisor, int dividend)
{
  // You can use make_tuple instead of the constructor if you want.
  // If the template type is crazy (lots of items in the tuple)
  // you might find make_tuple easier to read.
  return make_tuple(divisor / dividend, divisor % dividend);
}

int main()
{
  int quotient, remainder;

  // Whoa! We set those two variables at once!
  tie(quotient, remainder) = divide(13,5);

  cout << "13 / 5 = " << quotient
       << " with remainder " << remainder << endl;

  return 0;
}
~~~

#### `std::pair` (`#include<utility>`)

"Now don't tell anybody, but the STØR::pair is just a STØR::tuple with two items.
The rules are the same, but it adds a couple of convenience functions.
I mean features."

---

The `std::pair` type is a lot like the `std::tuple` type, but it holds exactly two items.
You get to decide what types those two things have.
You can access those items like a struct using `.first` and `.second` to get the first and second item, respectively.

~~~{.cpp .numberLines}
// You can use the good ol' constructor of course
pair<int,string> origin = pair<int,string>(0,"bleep");
cout << "origin: (" << origin.first << ","
     << origin.second << ")" << endl;

// There's also a handy function to make a pair
pair<int,int> coord = make_pair(3,5);
cout << "coord: (" << coord.first << ","
     << coord.second << ")" << endl;
~~~

We'll see `pair` used in the next section as a handy way to pass around two values at once.

#### `std::map` (`#include<map>`)

The STØR guide leads you to a boxy contraption with a trap door on the top and some kind of laser scanner on the side.

"Now this is our STØR::map.
It's a bit more complex than the STØR::vector, but it sure is handy!
Yes indeedily do!"

More staring.

"Lots of our customers struggle with keeping pairs of things together.
Shoes, socks, you name it!
They find one part of the pair, and they can't find its buddy!"

"The STØR::map helps you keep track of your pairs.
Watch this!"

The man takes his shoes off[^despite] and scans the left shoe.
The trap door opens.
He then drops his right shoe into the machine and closes the door again.
The display on the machine shows `<LeftShoe,RightShoe>`.

"All ri-diddly-ight, my friend.
Now, you see, the machine is configured to store pairs of shoes.
You scan the left one, and it stores the corresponding right shoe.
Now watch this!
Give me your shoes!"

Nope.

"That's alright.
I got a demo pair here."[^they-gross]
He uses the same procedure to store the demo pair: scan, store, close.
"Now, I've got two left shoes here."
He holds up his left shoe
"If I want to get Lefty's twin, I just place it under the scanner here."

He does.
The trap door opens, and he pulls out his right shoe.

"You see?
And this works for any kind of thing.
I can match left shoes to pepper shakers or apples to oranges!
The only trick is that whatever you scan has to be the same kind of thing, and whatever you store has to be the same kind of thing.
If you scan apples, that's all it can scan.
Right now this STØR::map scans `LeftShoe`s and stores `RightShoe`s.
See the display?"

You nod.

He picks up the STØR::map and places it in the back of a golf cart sort of vehicle[^wonka], and gestures toward the passenger's seat.

"Looks like we're done with your list!
Let's head to the checkout!"

As you ride toward the checkout you marvel at the amazing, Scandinavian technologies you've seen.
They've invented so many containers for so many purposes.

Your mind runs wild with C++ analogies.

---

As its name implies, a `std::map` maps keys to values.
It's sort of like a real-life dictionary.
If you look up a word (key), you'll find its definition (value).
Another way of thinking about it is it's like an array, but instead of having to use the integers 0, 1, ... for indices,
you can use whatever type you like.

With a `std::map`, you get to decide on the type for the key and the type for the value.
Like a vector, a `std::map` can change size to hold more items on the fly.
It can hold as many key/value pairs as you'd like.
The size of a `std::map` corresponds to the number of key/value **pairs** that have been set.

~~~{.cpp .numberLines}
// Create a map that maps strings to floats
map<string, float> costs;

// Use the bracket and assignment operators to set values for keys
costs["beer"] = 5.5;
costs["soda"] = 6.0;

// oh wait. Beer is cheaper than that.
costs["beer"] = 4.85;
~~~

So, here we have a dictionary with the following definitions:

- `"beer"` maps to 4.85
- `"soda"` maps to 6.0

We can use the bracket operator to get the set values out, too.
Be careful, though!
If you try to access a value using a key that doesn't exist, the `std::map` will give you a **default value**.

~~~{.cpp .numberLines startFrom=10}
// Prints out zero!!!
cout << costs["orange juice"] << endl;
~~~

You might expect the `std::map` to say "Hey man. I don't know what that is" and throw an exception at your face.
That's not what happens.
Instead, you're responsible for checking that a key exists before you try to access it.
Here are a couple of ways to do that:

Use the count() member function to see if the key is there:

~~~{.cpp .numberLines startFrom=12}
if(costs.count("beer") > 0)
{
  // Use the bracket operator to actually get the value
  cout << "beer costs " << costs["beer"] << endl;
}
~~~

Or we can use the find() member function.
If the item is in the map, find() returns an iterator that points to the item; otherwise, it returns an iterator to after the end of the map.

~~~{.cpp .numberLines startFrom=17}
map<string, float>::iterator iter = costs.find("beer");
if(iter != costs.end())
{
  // We can just use the iterator to access the item
  cout << "beer costs " << *iter << endl;
}
~~~

Like a `std::vector`, you can iterate over a `std::map`.
Unlike a `std::vector`, there's no easy way to just loop through all the indices.
So, we'll have to use an iterator instead.
Let's look at an example to see how:

~~~{.cpp .numberLines}
map<string, int> ages;
ages["rick"] = 70;
ages["morty"] = 14;

// That iterator type would be quite a doozy to write!
for (auto it = ages.begin(); it != ages.end(); it++)
{
  // Dereferencing the iterator gives us key/value pairs
  pair<const string, int> p = *it;
  cout << p.first << " is "
       << p.second << " years old." << endl;
}

for (auto it = ages.begin(); it != ages.end(); it++)
{
  // Alternatively, we can dereference and access in one step.
  cout << it->first << " is "
       << it->second << " years old." << endl;
}
~~~

Alternatively, we can use a for-each loop!
This handles all the iterator stuff for us; we just see the `pair<key,value>` objects.

~~~{.cpp .numberLines startFrom=20}
for (auto kv : ages)
{
  // No need to dereference!
  cout << kv.first << " is "
       << kv.second << " years old." << endl;
}
~~~

Whenever you iterate over a `std::map`, you iterate over its **key/value pairs**.
It's super handy, but it can be a little tricky at first.
In order to iterate over the key/value pairs, the `std::map` iterator points to instances of `std::pair`.
The type of the `std::pair` corresponds to the type of the `std::map`.
Iterating over a `std::map<string,int>` will give you `std::pair<string, int>`s.
For each pair, you can access the key using `.first` and the value using `.second`.

\newpage
## Questions

Name: `______________________________`

1.  Describe each of the following items (in your own) terms using a single sentence (one sentence a piece):
    a. `std::vector`
       \vspace{5em}
    b. `std::map`
       \vspace{5em}
    c. `std::pair`
       \vspace{5em}

2.  In a single line of valid C++, define and initialize a variable called `toad` of type `std::tuple`
that stores `5` (an `int`), `2.3` (a `float`), and `"boots"` (a `string`) in that order.
    Use `make_tuple()`. You may use `auto` if you want to.
    \vspace{10em}

3.  What's wrong with the following code snippet?

    ~~~{.cpp .numberLines}
    map<string, int> stuff;

    stuff["bob"] = 10;
    stuff["linda"] = 12;

    cout << stuff[10] << endl;
    ~~~


\newpage
## Quick Reference

### Using iterators

- Use `*it` to get the value the iterator is pointing at
- Use `it->member` to access member variables and functions of the value the iterator is pointing at
- Increment to the next element with `++`
- Some iterators can move backwards with `--`
- Some iterators can skip forward or backward a number of elements if you add that number to them
- An iterator equal to `end()` means you have reached the end of elements in a container

### `std::vector`

- `push_back(element)` adds new elements to the end of a vector
- `pop_back()` removes elements from the end of a vector
- `operator[]` and `at()` access elements; `at()` throws an `out_of_range` exception if the index is out of range
- `size()` returns the number of elements in a vector
- `erase(iterator)` removes an element from a vector
- `insert(iterator, element)` inserts an element into the middle of a vector

### `std::tuple`

- `make_tuple(elem1, elem2, ...)` creates a tuple with the given elements: `auto two_things = make_tuple("Bender", "Fry");`
- `get<N>(tuple)` gets the `N`th element from a tuple
- Use `tie(var1, var2, ...) = tuple` to assign each value from the tuple to a variable in one line

### `std::pair`

- Like a tuple, but has exactly two elements
- `make_pair(thing1, thing2)` creates a pair containing the two things
- Access the first element with `pair.first` and the second with `pair.second`

### `std::map`

- Add new elements with `operator[]`: `my_map["coffee"] = 1.99;`
- `count(key)` returns the number of entries for that key in the map (either 0 or 1)
- `find(key)` returns an iterator pointing at the key-value pair corresponding to that key
- A key is in the map if `my_map.count(key) > 0` or `my_map.find(key) != my_map.end()`
- Iterating over a map iterates over the pairs of keys and associated values in the map


## Further Reading

- [The `auto` keyword](http://en.cppreference.com/w/cpp/language/auto)
- [For-each loops](http://en.cppreference.com/w/cpp/language/range-for)
- [`std::vector`](http://www.cplusplus.com/reference/vector/vector/)
- [`std::tuple`](http://www.cplusplus.com/reference/tuple/tuple/)
- [`std::pair`](http://www.cplusplus.com/reference/utility/pair/)
- [`std::map`](http://www.cplusplus.com/reference/map/map/)


[^not-supposed-to]: He's not supposed to call it that. His managers call it "a violation of brand standards and common decency".
[^loss-for-words]: This fella's truly got you at a loss for word-iddly-ords.
[^despite]: Despite all objections.
[^they-gross]: They're gross.
[^wonka]: If golf carts were manufactured by Willy Wonka.
[^hindley-milner]: Sorry if you were expecting Haskell or Rust levels of type deduction!
[^cpp17]: C++14 and C++17 add some more meanings for `auto`. They're way cool, but they're beyond the scope of this book.
[^references]: If you want to know how this works: `tie` returns a tuple of references to the variables it is passed.
If we have an `int a` and a `char b`, the return type of `tie(a,b)` is `tuple<int&, char&>`.

