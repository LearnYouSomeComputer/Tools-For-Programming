# Regular Expressions

![https://xkcd.com/208/](05/regular_expressions.png)

## Motivation

Regular expressions describe patterns in strings.
They're incredibly useful for parsing input to programs.
Need to pull the digits out of a phone number? Find a particular entry in a several-megabyte log file?
Regex has got you covered!
You can even use regular expressions to transform one string to another.

In your theory of computer science class, you will learn about what makes a regular expression regular.[^chomsky]
Because nobody pays attention in theory classes, most regular expression libraries are not actually 'regular' in the theoretical sense.
That's fine, though; irregular expressions can describe patterns that strictly regular expressions cannot.[^caveat]
These regular expressions are usually called 'extended regular expressions'.

When most developers say 'regex', they're thinking of Perl Compatible Regular Expressions (PCRE), but there are several other flavors of regular expressions.[^umami]
In this chapter we will cover the flavors used by common Linux utilities; they are nearly the same as PCRE but have some minor differences.
In addition to the utilities we will discuss in this chapter, nearly every programming language (even C++) has a regular expressions library.

### Takeaways

- Learn the syntax for writing regular expressions
- Use `grep` to search files using regular expressions
- Use `sed` to search and edit files using regular expressions

## Walkthrough

The general idea of writing a regular expression is that you're writing a *search string*.
They may look complicated, but break them down piece by piece and you should be able to puzzle out what is going on.

There are several websites that will visually show you what a regular expression matches.
We recommend you try out examples from this chapter in one of these websites; try [https://regex101.com/](https://regex101.com/).

### Syntax

Letters, numbers, and spaces match themselves: the regex `abc` matches the string "abc".
In addition to literal character matches, there are several single-character-long patterns:

- `.`: Matches one of any character.
- `\w`: Matches a word character (letters, numbers, and \_).
- `\W`: Matches everything `\w` doesn’t.
- `\d`: Matches a digit.
- `\D`: Matches anything that isn’t a digit.
- `\s`: Matches whitespace (space, tab, newline, carriage return, etc.).
- `\S`: Matches non-whitespace (everything `\s` doesn’t match).

So `a\wb` matches "aab", "a2b", and so on.

`\` is also the escape character, so `\\` matches "\\".

If these character patterns don't quite meet your needs, you can make your own by listing the possible matches between `[]`s.
So if we wanted to match "abc" and "adc" and nothing else, we could write `a[bd]c`.

Custom character classes can include other character classes, and you can use `-` to indicate a range of characters.
For instance, if you wanted to match a hexadecimal digit, you could write the following: `[\da-fA-F]` to match a digit (`\d`) or a hex letter, either uppercase or lowercase.
You can also negate character classes by including a `^` at the beginning. `[^a-z]` matches everything except lowercase letters.

Now, if you want to match names, you can use `\w\w\w\w` to match "Finn" or "Jake", but that won't work to match "Bob" or "Summer".
What you really need is a variable-length match. Fortunately there are several of these!

- `{n}`: matches *n* of the previous character class.
- `{n,m}`: matches between *n* and *m* of the previous character class (inclusive).
- `{n,}`: matches at least *n* of the previous character.

So you could write `/\w{4}/` to match four-letter words, or `\w{1,}` to match one or more word characters.

Because some of these patterns are so common, there's shorthand for them:

- `*`: matches 0 or more of the previous character; short for `{0,}`.
- `+`: matches 1 or more of the previous character; short for `{1,}`.
- `?`: matches 0 or 1 of the previous character; short for `{0,1}`.

So we could write our name regex as `/\w+/`.

More examples:

- `0x[a-fA-F\d]+`: Matches a hexadecimal number (`0xdeadbeef`, `0x1337c0de`, etc.).
- `a+b+` : Matches any string containing one or more `a`s, followed by one or more `b`s.
- `\d{5}` : Matches any string containing five digits (a regular ZIP code).
- `\d{5}-\d{4}` : Matches any string containing 5 digits followed by a dash and 4 more digits (a ZIP+4 code).

What if you wanted to match a ZIP code either with or without the extension?
It's tempting to write `\d{5}-?\d{0,4}`, but this would also match "12345-", "12345-6", and so on, which are not valid ZIP+4 codes.

What we really need is a way to group parts of the match together.
Fortunately, you can do this with `()`s!
`\d{5}(-\d{4})?` matches any ZIP code with an optional +4 extension.

A group can match one of several options, denoted by `|`.
For example, `[ac][bd]` matches "ab", "cd", "ad", and "cb".
To match "ab" or "cd" but not "ad" or "cb", use `(ab|cd)`.

The real power of groups is in backreferences, which come in handy both when matching expressions and doing string transformations.
You can refer to the substring matched by the first group with `\1`, the second group with `\2`, etc.
We can match "abab" or "cdcd" but not "abcd" or "cdab" with `(ab|cd)\1`.

If you have a pattern where you need to refer to both a backreference and a digit immediately afterward, use an empty group to separate the backreference and digit.
For example, let's say you want to match "110", "220", ..., "990".
If you wrote `(\d)\10`, your regex engine would be confused because `\10` looks like a backreference to the 10th group.
Instead, write `(\d)\1()0` -- the `()` matches an empty string (i.e. nothing), so it's as if it wasn't there.

By default, regular expressions match a substring anywhere in the string.
So if you have the regex `a+b+` and the string "cccaabbddddd", that will count as a match because `a+b+` matches "aabb".
To specify that a match must start at the beginning of a line, use `^`, and to specify that the match ends at the end of a line, use `$`.
So, `a+b+$` matches "cccaabb" but not "aabbcc", and `^a+b+$` matches only lines containing some "a"s followed by some "b"s.

Now, it's the nature of regular expressions to be greedy and gobble up (match) as much as they can.
Usually this sort of self-interested behavior is fine, but sometimes it goes too far.[^politics]
You can use `?` on part of a regular expression to make that part polite (i.e. non-greedy), in which case it matches only as much as it needs for the whole regex to match.

One example of this is if you are trying to match complete sentences from lines of text.
Using `(.+\.)` (i.e. match one or more things, followed by a period) is fine, as long as there is just one sentence per line.
But if there's more than one sentence on a line, this regex will match all of them, because `.` matches "."!
If you want it to match one and only one sentence, you have to tell the `.+` to match only as much as needed, so `(.+?\.)`.

Alternatively, you could rewrite it using a custom character class: `([^\.]+\.)` -- match one or more things that aren't a period, followed by a period.

### `grep`

Imagine that you have sat yourself down at your computer. It's 1984 and you dial in to your local BBS on your brand new 9.6 kbps modem.
Your stomach growls. As your modem begins its handshake, you stand up, suddenly aware that you must have nachos.
You fetch the tortilla chips and cheese and heat them in the microwave next to the phone jack.
You sit back down at your machine. Your terminal is filling with lines of junk, `!#@$!%^IA(jfio2q4Tj1$T(!34u17f143$#` over and over and over.
Dang it, the microwave is interfering with the phone line.
You lean back, close your eyes, and listen to the cheese sizzling.

Your reverie is cut short when you suddenly remember that you have a big file that you really need to find some stuff in.
*GREP!* If only there was some program that could use that line noise from your nachos to help...

Okay, enough imagining. There *is* a command to use that line noise to look through files: `grep`.
This interjection of a command name is short for "global regular expression print", and it does exactly just that.
In this case, "just that" means it prints strings from files (or standard in) that match a given regular expression.
If you want to look for "bob" in "cool\_people.txt", you could do it with `grep` like so: `grep bob cool\_people.txt`.
If you don't specify a filename, `grep` reads from standard input, so you can pipe stuff into it as well.

`grep` has a few handy options:

- `-C LINES`: Give `LINES` lines of context around the match.
- `-v`: Print every line that doesn’t match (it inverts the match).
- `-i`: Ignore case when matching.
- `-P`: Use Perl-style regular expressions.
- `-o`: Only print the part of the line the regex matches. Handy for figuring out what a regex is matching.

Without Perl-style regexes, `grep` requires you to escape special characters to get the special meaning.[^huh]
In other words, `a+` matches "a+", whereas `a\+` matches one or more "a"s.

For these examples, we'll use STDIN as our search text.
That is, grep will use the pattern (passed as an argument) to search the input received over STDIN.

```
$ echo "bananas" | grep 'b\(an\)\+as'
bananas
$ echo "banananananananas" | grep 'b\(an\)\+as'
banananananananas
$ echo "banas" | grep 'b\(an\)\+as'
banas
$ echo "banana" | grep 'b\(an\)\+as'
```

### `sed`

`grep` is great and all but it's really only for printing out matches of regular expressions.
We can do so much more with regular expressions, though!
`sed` is a 'stream editor': it reads in a file (or standard in), makes edits, and prints the edited stream to standard out.
`sed` is noninteractive; while you *can* use it to perform any old edit, it's best for situations where you want to automate editing.

Some handy `sed` flags:
- `-r`: Use extended regular expressions. **NOTE**: even with extended regexes, `sed` is missing some character classes, such as `\d`.
- `-n`: Only print lines that match (handy for debugging).

`sed` has several commands that you can use in conjunction with reglar expressions to perform edits.
One such command is the print command, `p`. It prints every line that a particular regex matches.
`sed -n '/REGEX/ p'` works almost exactly like `grep REGEX` does.
Use this command to make sure your regexes match what you think they should.

The substitute command, `s`, substitutes the string matched by a regular expression with another string.
`sed 's/REGEX/REPLACEMENT/'` replaces the match for `REGEX` with `REPLACEMENT.
This lets you perform string transformations, or edits.

For example,
```
$ echo "bananas" | sed -r 's/(an)+/uen/'
buenas
```

You can use backreferences in your replacement strings!
```
$ echo "ab" | sed -r 's/(ab|cd)/First group matched \1/'
First group matched ab
```

The substitute command has a few options. The global option, `g`, applies the command to every match on a line, rather than just the first:
```
$ echo "ab ab" | sed 's/ab/bob/'
bob ab
$ echo "ab ab" | sed 's/ab/bob/g'
bob bob
```

The `i` option makes the match case insensitive, like `grep`'s `-i` flag.
```
$ echo "APPLES ARE GOOD" | sed 's/apple/banana/i'
bananaS ARE GOOD
```


Finally, you can combine the substitute and print commands:
```
$ echo -e "apple\nbanana\napple pie" | sed -n 's/apple/grape/ p'
grape
grape pie
```

There are even more `sed` commands, and more ways to combine them together.
Fortunately for you, though, this is not a book on `sed`, so we'll leave it at that.
It's definitely worthwhile to spend a bit of time looking through the `sed` manual if you find yourself needing to do something it's good for.

\newpage
## Questions
Name: `______________________________`

1. Suppose, for the sake of simplicity[^email], that we want to match email addresses whose addresses and domains are letters and numbers, like "abc123@xyz.wibble".
Write a regular expression to match an email address.
\vspace{8em}

2. Write a command to check if Clayton Price is in "cool_nerds.txt", a list of cool nerds.
\vspace{8em}

3. Imagine that you are the owner of Pat's Pizza Pie Pizzaria, a pizza joint that's fallen on tough times.
You're trying to reinvent the business as a hip, fancy eatery, "The Garden of Olives (And Also Peperoncinis)".
As part of this reinvention, you need to jazz up that menu by replacing "pizza" with "foccacia and fresh tomato sauce".
Suppose your menu is stored in "menu.txt". Write a command to update every instance of "pizza" and place the new, hip menu in "carte-du-jour.txt".
\newpage

## Quick Reference

Regex:

- `.`: Matches one of any character.
- `\w`: Matches a word character (letters, numbers, and \_).
- `\W`: Matches everything `\w` doesn’t.
- `\d`: Matches a digit.
- `\D`: Matches anything that isn’t a digit.
- `\s`: Matches whitespace (space, tab, newline, carriage return, etc.).
- `\S`: Matches non-whitespace (everything `\s` doesn’t match).

<!-- -->

- `{n}`: matches *n* of the previous character class.
- `{n,m}`: matches between *n* and *m* of the previous character class (inclusive).
- `{n,}`: matches at least *n* of the previous character.
- `\*`: matches 0 or more of the previous character; short for `{0,}`.
- `+`: matches 1 or more of the previous character; short for `{1,}`.
- `?`: matches 0 or 1 of the previous character; short for `{0,1}`.

`grep REGEX [FILE]`: Search for `REGEX` in `FILE`, or standard input if no file is specified

- `-C LINES`: Give `LINES` lines of context around the match.
- `-v`: Print every line that doesn’t match (it inverts the match).
- `-i`: Ignore case when matching.
- `-P`: Use Perl-style regular expressions.
- `-o`: Only print the part of the line the regex matches.

`sed COMMANDS [FILE]`: Perform `COMMANDS` to the contents of `FILE`, or standard input if no file is specified, and print the results to standard output

- `-r`: Use extended regular expressions.
- `-n`: Only print lines that match.

<!-- -->

- `/REGEX/ p`: Print lines that match `REGEX`
- `s/REGEX/REPLACEMENT/`: Replace strings that match `REGEX` with `REPLACEMENT
	- `g`: Replace every match on each line, rather than just the first match
	- `i`: Make matches case insensitive

## Further Reading

- [Regex reference](http://www.rexegg.com/regex-quickstart.html#ref)
- [`grep` manual](https://www.gnu.org/software/grep/manual/grep.html)
- [`sed` manual](https://www.gnu.org/software/sed/manual/sed.html)
- [`sed` tutorial](http://www.grymoire.com/Unix/Sed.html)
- [C`++` regex library reference](http://www.cplusplus.com/reference/regex/)


[^chomsky]: If it weren't for Noam Chomsky, we'd only have irregular expressions like "every boat is a bob".
[^caveat]: With one caveat: irregular expressions can be very slow to check; regular regular expressions can always be checked quickly.
(Whether your regex library actually checks quickly is another story for another time, because I can see you nodding off right now.)
[^umami]: The umami flavors are my favorite.
[^politics]: POLITICS!
[^huh]: You may think this actually makes some sense and that PCRE is needlessly confusing.
You may even feel slightly despondent as you realize that a piece of software being popular doesn't mean that it's good.
That's what you get for thinking.
[^email]: In practice, email addresses can have all sorts of things in them! Like spaces! Or quotes!
