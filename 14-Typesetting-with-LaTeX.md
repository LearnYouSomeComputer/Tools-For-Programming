# Typesetting with \LaTeX

## Motivation

Close your eyes. You are [Donald Knuth](http://www-cs-faculty.stanford.edu/~uno/).

<!--
Licensed CC-BY-SA 2.5; taken by Jacob Appelbaum (ick)
source: https://upload.wikimedia.org/wikipedia/commons/4/4f/KnuthAtOpenContentAlliance.jpg
-->
![You, aka Donald Knuth](14/don.jpg){height=35%}

The year is 1977, and you have just finished hand-writing the final draft of the second edition of the second volume of *The Art of Computer Programming*.
You send the draft off to the publisher for typesetting, and the proof that comes back is just horrendous.
The letters are blurry, the myriad subscripts and superscripts unreadable smudges, and the spacing and justification is all out of whack.
Your first edition proofs were nothing like this!
You call them up to see what happened. "Sorry," they say, "we switched to a modern photographic typesetting system; that's what you get now."

This will not do. "I've spent 15 years writing these books, but if they are going to look awful I don't want to write any more."[^dt]
The first edition was typeset on a Monotype typecaster machine, which mechanically calculated the spacing required for justified lines
and cast individual bits of type that were exactly the right size needed.
What you need is a digital typesetting system that's equally as powerful!
Writing one should only take a summer, so you set to work.

In 1989 the first "finished" version of \TeX was completed.[^further-reading]

(Okay, you can stop being Don Knuth now.)

Since \TeX is relatively low-level, Leslie Lamport (a fellow known for wearing silly hats and also verification of distributed systems)
wrote a bunch of useful macros that took care of a lot of the day-to-day typesetting stuff.
This collection came to be known as \LaTeX --- **La**mport's **\TeX{}**.

So, what is \LaTeX good for?[^logo]

Do you think writing should come with more compiler errors?
Do you ever wish HTML was more arcane and confusing?
Are word processors too easy to use?
Do you think PDF is the one true document format?

Do you want to make your research on the Area 51 coverup to look professional?

Then you should learn \LaTeX{}!

### Takeaways

- Learn how to structure \LaTeX documents
- Math is beautiful and easy to typeset
- \TeX is definitely something designed in the 70's

## Walkthrough

\LaTeX is a language for marking up text.
You write plain ASCII text in a `.tex` file (say, `bob.tex`), then run it through the `pdflatex` command[^thrice] (so, `pdflatex bob.tex`).
`pdflatex`, as its name implies, spits out a PDF file that you can open with your favorite PDF viewer.
Everyone has a favorite PDF viewer, right?

First, a bit about syntax. \LaTeX commands begin with a `\`. So, to write the fancy \LaTeX logo, you'd type `\LaTeX`.
Required arguments are surrounded with `{}`s; optional arguments are listed between `[]`s.
For example: `\command[option1, option2]{required argument 1}{required argument 2}`.

\LaTeX also has 'environments', which are used to wrap larger chunks of text.
These begin with `\begin{environmentname}` and end with `\end{environmentname}`.

### Document Classes

The first thing in a `.tex` file is a document class command: `\documentclass{classname}`.
Several document classes come built-in, including the following:

- `article`: Used for conference and journal articles and typical classroom reports.
- `report`: Used for small books or longer reports that span several chapters.
- `book`: Used to make fritatta.
- `beamer`: Used to make slides for presentations.

The document class defines the look of the document, as well as what commands are available to structure your document.

### Document Structure

Between the `\documentclass` command and the rest of the document goes any package setup you desire.
To include the 'hyperref' package, which lets you make clickable links, write `\usepackage{hyperref}`.
Some packages may require additional configuration; consult the documentation to see how to use them.

Before starting the actual document, you should set the title, date, and author of the work by using the `\title`, `\date`, and `\author` commands.

The document itself is written between `\begin{document}` and `\end{document}`.

To insert the title and author, use the `\maketitle` command.

Here's a short example:

```latex
\documentclass{article}

\title{Do Lizards Run The World?}
\author{Nathan Jarus}
\date{\today}

\begin{document}
\maketitle
\end{document}
```

Now for some actual content.
Your document can be split into sections, subsections, subsubsections, paragraphs, and sub-paragraphs.
(Some document classes, such as report and book, include a 'chapter' command that is above 'section' in the hierarchy.)
The commands for each of these takes one argument: a title for the given part of the document.
The document class controls the numbering and appearance of the titles for you.

Continuing our example:

```latex
\documentclass{article}

\title{Do Lizards Run The World?}
\author{Nathan Jarus}
\date{\today}

\begin{document}
\maketitle
\section{Introduction}

\section{Methodology}
\subsection{A Perpetual Energy Source}
\subsection{A Radio Beacon for the Pyramids of Giza}
\subsection{Plans for First Contact}

\section{Results}
\subsection{Physics Dislikes Me}
\subsubsection{Physicists don’t want the truth}
\subsubsection{This foil hat is perfectly comfortable, thanks}

\section{Conclusion: Perhaps the real aliens are the friends we made along the way}
\end{document}
```

### Formatting Text

For the most part, you can write text as you normally would in a word processor.
To make a new paragraph, put two newlines in a row:

```latex
This is a sentence.
This is a second.

And here is a brand new paragraph!
```

``# $ % ^ & _ { } ` ~`` and ``\`` are reserved characters.
(Of note: `%` starts a one-line comment, much as `//` does in C`++`.)

You can write them using the escape sequences ``\# \$ \% \^{} \& \_ \{ \} \`{} \~{}`` and ``\textbackslash{}``.[^caret]

Opening quotes are written with the `` ` `` character and close quotes with the `'` character.
So, ``` ``text in double quotes''``` renders like "text in double quotes".

The age-old standbys of bold, italic, and underlined text are present in \LaTeX as well:

- `\textbf{bold face text here}`
- `\textit{italicized text here}`
- `\underline{this text is underlined}`

You can also put text in a monospaced font: `\texttt{I am a robot}` renders like \texttt{I am a robot}.

Last but not least,URLs and hyperlinks can be added.
For this, you need the `hyperref` package, which provides several commands.
The `\url` command prints a URL in monospaced font; you use it like so: `\url{http://www.funroll-loops.info/}`.
The `\href` command lets you add hyperlinks: `\href{http://url.com}{displayed, underlined text}` makes the text clickable
and provides a visual indication that there's a link to click on.

Hyperref's color scheme is not everyone's favorite.
You can configure this; for instance, to make hyperlinks black with an underline, put the following right after `\usepackage{hyperref}`:

```latex
\hypersetup{colorlinks=false,
	allbordercolors={0 0 0},
	pdfborderstyle={/S/U/W 1}
}
```

### Lists

You can make both bulleted and numbered lists in \LaTeX. The former are called 'itemized lists', while the latter are 'enumerated lists'.

Here's an example itemized list:

```latex
\begin{itemize}
	\item Itemize makes a bulleted list.
	\item Every item in the list starts with
		the item command.
	\item You can make multiline items\\
		by putting a linebreak in them.
\end{itemize}
```

And a numbered list:

```latex
\begin{enumerate}
	\item Enumerate numbers each item.
	\item Otherwise it’s exactly the same as itemize.

	\item You can also nest lists!
	\item Just start a new itemize or enumerate in a list:
		\begin{enumerate}
			\item Enumerates will change numbering style.
			\item Itemizes will use a different glyph.
		\end{enumerate}
	\item Once you’re done, you can keep adding new
		list items to the original list.
\end{enumerate}
```

### Math

Math typesetting is \LaTeX{}'s pride and joy.
We could easily write a whole book chapter just on how to format various arcane equations.
Rather than doing that, we'll just show you some examples of common usage.

Math can be placed in a sentence by putting math markup between `$` signs: `$f(x) = 2x$` shows up like $f(x) = 2x$.
For bigger, more important equations, you can put them in an 'equation' environment.
(Your document class will probably number these equations.)
For instance,

```latex
\begin{equation}
	f(x) = 2x + 4
\end{equation}
```

renders as

\begin{equation}
	f(x) = 2x + 4
\end{equation}

Subscripts and superscripts can be stacked to your heart's content:

- `x_n` produces $x_n$
- `x^2` produces $x^2$
- `x_n^k` produces $x_n^k$
- `x_{n^k}` produces $x_{n^k}$

Set notation is a breeze: `\forall n \in \{1,2,3,4\}` appears as $\forall n \in \{1,2,3,4\}$.

Summations (and products) can be done using subscripts and superscripts: `\sum_{i=0}^\infty \frac{1}{3^i} = \frac{3}{2}` renders to
\begin{equation}
	\sum_{i=0}^\infty \frac{1}{3^i} = \frac{3}{2}
\end{equation}

Fractions can be done with the `\frac{}{}` command. You can adjust the size of parentheses, brackets, and such with the `\big`, `\Big`, `\large`, and `\Large` commands.

`\Big(\frac{1}{3}\Big)^k = \frac{1}{3^k}` renders as
\begin{equation}
	\Big(\frac{1}{3}\Big)^k = \frac{1}{3^k}
\end{equation}

For more math commands, consult [the wikibook on \LaTeX{}'s math mode](https://en.wikibooks.org/wiki/LaTeX/Mathematics).

### Figures

Figures go in the 'figure' environment, which positions them and lets you give them a caption.
\LaTeX will place the figure in a spot on the page that makes sense, usually at the top or the bottom (but you can tweak this manually if you like).
The `\caption` command sets a caption for the image.
You can center the image on the page with the `\centering` command.

The 'graphicx' package allows you to include pictures (`.png`, `.jpg`, `.eps`, or `.pdf`) with the `\includegraphics` command.
Here is an example:

```latex
\documentclass{article}
\usepackage{graphicx}

\begin{document}

\begin{figure}[h] % Place 'here' instead of at top/bottom
\caption{4-corner simultaneous 4-day time cube}
\centering % Center the image

% width=\textwidth makes the image the width of the text
\includegraphics[width=\textwidth]{timecube}

\end{figure}
\end{document}
```

### Tables

Much like 'figure', the 'table' environment lets you caption and position tables.
The actual table is made using the 'tabular' environment.
Its syntax is a little strange.
Fortunately, there exist many websites, including [http://truben.no/table/](http://truben.no/table/), which generate the tabular markup for you.

Here is an example table:

```latex
\begin{table}
    \begin{tabular}{l|l|l}
    ~          & Heading   & Another Heading \\ \hline
    Sandwiches & $x > 2$   & Very Tasty      \\
    Ice Cream  & $x = 5^5$ & Excellent       \\
    \end{tabular}
\end{table}
```

\newpage
## Questions
Name: `______________________________`

1. How would you write an equation, such as $y^2 + x^2 = 1$, in the middle of a sentence?
\vspace{10em}

2. What is the environment used for numbered lists?
\vspace{10em}

3. What does the `\centering` command do?
\newpage

## Quick Reference

## Further Reading

- The [\LaTeX Wikibook](https://en.wikibooks.org/wiki/LaTeX) is a very handy reference.
- The [\TeX StackExchange](https://tex.stackexchange.com/) has a lot of tips on doing various things and fixing various errors.
- [Detexify](http://detexify.kirelabs.org/classify.html) lets you draw symbols and tells you various math commands that look similar!

<!-- -->

- You can manage citations with [Bibtex](https://en.wikibooks.org/wiki/LaTeX/Bibliography\_Management).
- [CTAN](http://www.ctan.org/) (the Comprehensive \TeX Archive Network) has documentation on zillions of neat packages.
- [MiKTeX](http://miktex.org/) is a Windows version of \LaTeX{}.
- [Pandoc](http://pandoc.org/) can convert other document formats to and from \LaTeX{}. This book is written using Pandoc!

<!-- -->

- [TeXworks](https://www.tug.org/texworks/) is a nice cross-platform editor.
- [Gummi](https://github.com/alexandervdm/gummi) is another good editor, but it is Linux-only.
- [LyX](http://www.lyx.org/) is a WYSIWYG-ish editor based on \LaTeX{}.

[^dt]: Paraphrased from *Digital Typography*, p. 5.
[^logo]: Besides having a nifty command for printing out its own logo, of course.
[^thrice]: Sometimes twice, and sometimes thrice! `pdflatex` is a one-pass parser and, well, some things just can't be done in one pass. Yes, this is poor design.
[^caret]: Usually `\^` and `\~` are used to write accents on letters; for instance, `\~n` renders like ñ.
[^further-reading]: See [this essay](http://www.practicallyefficient.com/2017/10/13/from-boiling-lead-and-black-art.html) for more about
the history of mathematical typesetting.
