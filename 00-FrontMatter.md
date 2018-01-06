---
title: THE CS 1001 ENCHIRIDION
subtitle: Or, How To Use A Computer Real Good
author: Nathan Jarus and Michael Wisely
institute: Missouri University of Science and Technology

documentclass: book
papersize: a5paper
colorlinks: true

toc: true
toc-depth: 0

header-includes:
- \usepackage[os=win]{menukeys}
- \usepackage{newunicodechar}
- \newunicodechar{↩}{$\color{blue}\scriptstyle\hookleftarrow$}

include-before: |
    \pagestyle{empty}
    \begingroup
    Thanks to those who put up with our nonsense and those who paid us
    to write a book that is full of it.
    \endgroup

    \vfill

    \begingroup
    The source code for this book is available at [https://github.com/michaelwisely/CS1001-Prelab-Book](https://github.com/michaelwisely/CS1001-Prelab-Book).
    We welcome questions, corrections, and improvements.
    \endgroup

    \vfill

    \begingroup
    \footnotesize
    \parindent 0pt
    \parskip \baselineskip
    \textcopyright{} 2016--2018 Nathan Jarus and Michael Wisely

    \indent
    This work is licensed under the Creative Commons Attribution--ShareAlike 4.0 International License.
    To view a copy of this license, visit [http://creativecommons.org/licenses/by-sa/4.0/](http://creativecommons.org/licenses/by-sa/4.0/)
    or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

    \begin{center}
    \begin{tabular}{ll}
    Zeroth edition:  & August 2017
    \end{tabular}
    \end{center}

    \endgroup
    \clearpage
...

\frontmatter
\chapter{Introduction}

The field of Computer Science is an odd amalgamation of math, logic, statistics, philosophy, electrical engineering, psychology, and systems engineering.
Some of your classes will be hands-on and easily applicable; others will focus on ideas and theory, leaving the application in your hands.

Throughought your career as a computer scientist, computer engineer, or software engineer, you will write a lot of software.
This book focuses on software tools that will be useful in classes, internships, and personal projects.
Much as chemistry students are taught how to use lab equipment and mechanical engineering students learn the basics of machining,
this book teaches the basics of tools for writing and debugging software.

Each chapter of this book could be a book in itself --- our goal here is to give you an idea of what these tools are useful for
so that you'll know where to look when you need them in the future.
We've included links to more information on the book's topics at the end of each chapter, as well as a quick reference on how to use each tool.

To get the most out of this book, you must do more than just read each chapter.
As you read, try out the examples in the chapter.
Once you get an example working, take it a step further --- try something that seems like it might work, or combine it with something you learned previously!
Your learning does not end when you finish this book either.
Keep using the tools we present; practice will improve both how quickly you can accomplish tasks and your understanding of how
the tools, and by extension computers and software, work.

A note on the copyright license of this book: copyright laws are typically used to restrict the rights of others to copy, modify, and distribute creative works.
This book is distributed under what's known as a 'copyleft' license --- rather than restricting your rights, it ensures that you retain these rights.
The [Creative Commons Attribution--Share Alike (CC BY-SA)](http://creativecommons.org/licenses/by-sa/4.0/) license states that you have the right to copy, modify, and distribute your modifications to this book
as you please so long as you follow two rules:

1. You have to credit the authors of the work and indicate if you have made any changes.
2. You have to distribute your changes under the same license, giving others the same rights to your modifications as you (and they) have to the original work.

We hope that learning the tools in this book make your life easier and that you have a little fun along the way.
Happy hacking!

\mainmatter
