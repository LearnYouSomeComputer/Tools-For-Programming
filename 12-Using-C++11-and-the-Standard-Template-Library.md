# Using C++11 and the Standard Template Library

## Motivation

Finally.

The powers that be decided to build a STØR in your hometown.
It's taken months to finish construction and stock the shelves, and you're ready to check out their colorful housewares and famous, trendy-but-fragile furniture.

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
As you struggle, you hear a friendly voice say

"Hi-diddily-ho, customer-ino! Looks like you could use a hand!"

You stare.

"Let me guide ya around the STØR-a-roonie[^not-supposed-to] here and we'll find what you're looking for!"

With an arm full of Bort you continue your mission in search of the other items on your list.
However, now you have a mustachio'd lunatic to guide you.

#### Vector

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

"Now don't lolligag! Let's see what else is on your list."


#### Map

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
I got a demo pair here[^they-gross]."
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

"Well alrighty!
Let's keep on truckin!"

He picks up the STØR::map and places it in the back of a golf cart sort of vehicle[^wonka], and gestures toward the passenger's seat.

#### Tuple

"The STØR::tuple isn't for everyone."

He holds what looks like a shoe box.

"You can take an put whatever you want in here, but whatever kind of thing that is... that's all it can store.
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

#### Pair

"Now don't tell anybody, but the STØR::pair is just a STØR::tuple with two items.
The rules are the same, but it adds a couple of convenience functions.
I mean features."

"Looks like we're done with your list!
Let's head to the checkout!"

As you ride toward the checkout you marvel at the amazing, Scandinavian technologies you've seen.
They've invented so many containers for so many purposes.

Your mind runs wild with C++ analogies.

### Takeaways

- Learn to use several language features offered by the C++11 Language Standard
    - `auto` types
    - `for-each` loops
- Learn to use several data structures provided by the C++ Standard Template Library
    - `std::vector`
    - `std::map`
    - `std::pair`
    - `std::tuple`

## Walkthrough

### Language Features

C++11 introduces a couple of language features that were not available in earlier versions of C++.
This is great, but your compiler needs to know if you're using C++11 features.
The features we're going to see will confuse the living daylights out of `g++` unless you pass the `-std=c++11` flag.

~~~shell
# For example
$ g++ -std=c++11 main.cpp
~~~

#### The `auto` keyword

If you use the `auto` keyword, you're letting the compiler figure out types for you.
Essentially, when you make an assignment, the compiler will look at type of the item on the right-hand-side of the assignment and set the type of the left-hand-side to match.

We could take this program...

~~~ c++
int main()
{
  char cstring[] = "asdf";
  string str = string("asdf");

  vector<int> thingers = vector<int>();
  return 0;
}
~~~

... and make it a little more readable ...

~~~ c++
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

#### The for-each loop

As the name implies, a for-each allows you to perform an action "for each item" in a container.
These loops work with many types in the standard template library.
Here's a quick list of things you can use a for-each loop with:

- arrays
- certain classes (you have to have certain member functions)
    - std::vector
    - std::map
    - any other type with `begin()` and `end()` member functions

Refer to the further reading section to get an idea of what's required to use a type with for-each loops.

Let's have a look at an example:

~~~cpp
int main()
{
  int nums[] = {1,2,3,4,5,6};

  for (auto i : nums)
  {
    cout << i * i << ", ";
  }

  return 0;
}

// Prints 1, 2, 3, 4, 5, 6,
~~~

In this example, we've created an array with six `int`s in it.
We then use a for-each loop to iterate over all of those items.
Upon each iteration, `i` is set to the **value** of the current element.
It starts at `1`, then `2`, on and on until it reaches `6`.
For each `int`, it computes and prints the square of that value.

One caveat to be aware of is that changing the value of your loop variable (`i` in this case) won't change the thing we're iterating over (the array of `int`).
If we want to change the values in the array, we have to add a `&` to the loop.

~~~ cpp
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

// Prints 0, 1, 2, 3, 4, 5,
~~~

### A Handful of Containers from the Standard Template Library

The Standard Template Library (STL) is vast.
It has a lot of storage types for any need you can think of.
More than STØR, possibly.

We're going to take a look at a few here.
Below is a list of the types, as well as their corresponding `#include`'s

- `std::vector` (`#include<vector>`)
- `std::tuple` (`#include<tuple>`)
- `std::pair` (`#include<utility>`)
- `std::map` (`#include<map>`)

#### `std::vector`

If you're not already familiar, a `std::vector` is like an array that resizes on the fly.
It's a template type, so everything stored in it must be the same type.
You can push items on the back to grow the `vector`, and pop them to shrink it.

~~~cpp
int main()
{
  vector<int> v;                  // The vector has zero items: []
  for (int i = 0; i < 10; i++)    // Then, we'll add the numbers 0 through 9
  {
    v.push_back(i);
  }

  // The numbers are added to the back of our vector, so it looks like
  // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

  // We can use a vector<int>iterator object to iterate over our vector.
  // This type is *sorta* like a pointer. We can use * to dereference the
  // iterator to get a value. However, to move the iterator along our vector,
  // we use the ++ operator
  for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++)
  {
    cout << *iter << endl;
  }

  // Alternatively, we could have used a for-each loop
  for (auto value : v)
  {
    cout << value << endl;  // No need to "dereference"!
  }

  return 0;
}
~~~

#### `std::tuple`

The `std::vector` has a weird cousin: the `std::tuple`.
Conceptually, you can think of it like a struct.
It has a set number of items, those items can have different types, but the types of those items cannot change.
Unlike a struct, they come with member functions, and they're shorter to define.

~~~ cpp
/* Here's a tuple that stores a char, a float, and int, and a string! */
tuple<char, float,int,string> thing('x', 2.5, 4, "ABC");
~~~

If you want to get the items out, you need to use the `get<N>()` function.
The template parameter to `get` (`N`) is the index of the item you want.
If you want the first item in a tuple named `tup`, you'd use `get<0>(tup)` to get it.

Alternatively, you can use the `tie()` function to "tie" variables to values.

~~~ cpp
tuple<float,int,string> thing(2.5,4,"A");

float x;
int y;
string z;

tie(x, y, z) = thing;
~~~

In the above example, we use `tie()` to assign the items in `thing` to `x`, `y`, and `z` respectively.
After the last line, `x == 2.5`, `y == 4`, and `z == "A"`.
Sometimes, `tie()` is easier to use than making a bunch of calls to `get<>()`.

If you find that your tuple contains items that you don't need, you can use the `ignore` variable.
Consider the following example:

~~~cpp
int main()
{
  tuple<int,int,string> coord_name(2,4,"A");

  // Prints (A, 2, 4)
  cout << get<2>(coord_name) << ": ("
       << get<0>(coord_name) << ","
       << get<1>(coord_name) << ")\n";

  int x, y;

  // Unpacks the first two items into x and y, and ignores the third item.
  tie(x, y, ignore) = coord_name;

  // Prints (2, 4)
  cout << "(" << x << "," << y << ")\n";

  return 0;
}
~~~

One cool use for the `tie` function is to "return multiple values".
See those quotes?
You're really just returning one `tuple`, but with a call to `tie()`, it's kinda like you're returning more than one thing at a time.

~~~ cpp
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

#### `std::pair`

The `std::pair` type is a lot like the `std::tuple` type, but it's set at holding exactly two items.
You **can't** set the size, but you **can** decide what types those two things have.
You can also access those items like a struct using `.first` and `.second` to get the first item and second item respectively.

~~~ cpp
int main()
{
  // You can use the good ol' constructor of course
  pair<int,string> origin = pair<int,string>(0,"bleep");
  cout << "origin: (" << origin.first << "," << origin.second << ")" << endl;

  // There's also a handy function to make a pair
  pair<int,int> coord = make_pair(3,5);
  cout << "coord: (" << coord.first << "," << coord.second << ")" << endl;

  return 0;
}
~~~

#### `std::map`

As its name implies, a `std::map` maps keys to values.
It's sort of like a real-life dictionary.
If you look up a word (key), you'll find its definition (value).

With an `std::map`, you get to decide on the type for the key and the type for the value.
Like a vector, a `std::map` size changes on the fly.
It can hold as many key/value pairs as you'd like.
The size of a `std::map` corresponds to the number of key/value **pairs** that have been set.

~~~cpp
// We can create a map that maps strings to floats
map<string, float> costs;

// We use the bracket operator and assignment operator to set key/pair values
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

~~~cpp
// Prints out zero!!!
cout << costs["orange juice"] << endl;
~~~

You might expect the `std::map` to say "Hey man. I don't know what that is" and throw an exception at your face.
That's not what happens.

Watch out for that.

Like a `std::vector`, you can iterate over a `std::map`.
One way to do this is using a `std::map::iterator`.
Let's look at an example to see what this looks like.

~~~ cpp
int main()
{
  map<string, int> ages;
  ages["rick"] = 70;
  ages["morty"] = 14;

  // See that type? What a doozy.
  // This is why auto is nice.
  for (map<string, int>::iterator it = ages.begin(); it != ages.end(); it++)
  {
    // Dereferencing the iterator gives us key/value pairs
    pair<const string, int> p = *it;
    cout << p.first << " is "
         << p.second << " years old." << endl;
  }

  for (auto it = ages.begin(); it != ages.end(); it++)
  {
    // Alternatively, we can dereference/access in one step.
    cout << it->first << " is "
         << it->second << " years old." << endl;
  }

  // Alternatively alternatively, we can use a for-each loop!
  for (auto it : ages)
  {
    // No need to dereference!
    cout << it.first << " is "
         << it.second << " years old." << endl;
  }

  return 0;
}
~~~

Whenever you iterate over a `std::map`, you iterate over its **key/value pairs**.
It's super handy, but it's tricky.

In order to iterate over the key/value pairs, the `std::map` iterator points to instances of `std::pair`.
The type of the `std::pair` corresponds to the type of the `std::map`.
Iterating over a `std::map<string,int>` will give you `std::pair<string, int>`.
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

2.  In a single line of valid C++, define and initialize a variable called `toad` of type `std::tuple` that stores `5` (an `int`), `2.3` (a `float`, and `"boots"` (a `string`) in that order.
    Use `make_tuple()`. You may use `auto` if you want to.
    \vspace{10em}

3.  What's wrong with the following code snippet?

    ~~~cpp
    map<string, int> stuff;

    stuff["bob"] = 10;
    stuff["linda"] = 12;

    cout << stuff[10] << endl;
    ~~~


\newpage
## Quick Reference

TBD!

## Further Reading

- [The `auto` keyword](http://en.cppreference.com/w/cpp/language/auto)
- [For-each loops](http://en.cppreference.com/w/cpp/language/range-for)
- [`std::vector`](http://en.cppreference.com/w/cpp/container/vector)
- [`std::tuple`](http://www.cplusplus.com/reference/tuple/tuple/)
- [`std::pair`](http://en.cppreference.com/w/cpp/utility/pair)
- [`std::map`](http://en.cppreference.com/w/cpp/container/map)


[^not-supposed-to]: He's not supposed to call it that. His managers call it "a violation of brand standards and common decency".
[^loss-for-words]: This fella's truly got you at a loss for word-iddly-ords.
[^despite]: Despite all objections.
[^they-gross]: They're gross.
[^wonka]: If golf carts were manufactured by Willy Wonka.
