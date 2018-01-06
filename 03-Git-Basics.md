# Git Basics

## Motivation

Close your eyes[^open].

Imagine yourself standing in a wide, open field.
In that field stands a desk, and on that desk, a computer.
You sit down at the desk ready to code up the Next Big Thing[^next-big-thing].

You start programming and find yourself ready to start writing a cool new feature.
"I better back up my code," you think to yourself. "Just in case I really goof it up."
You create a new folder, name it "old_version" and continue on your way.

As you work and work[^daydream], you find yourself with quite a few of these backups.
You see "old_version" and "old_version2" alongside good old "sorta_works" and "almost_done"
"Good thing I made these backups", you say. "Better safe than sorry."

Time passes.

"Wait... this isn't right...," you think.
Your code is broken!
Boy, it's a good thing you kept those backups.
But wait... which of these backups actually worked?
What's different in *this* version that's breaking your project?

Open your eyes.

If you haven't already experienced this predicament outside of a daydream, you certainly will.
It's a fact that as you work on a programming project, you will add features to your code, change the way it works, and sometimes introduce bugs.
Sure, you can manage your projects by making copy after copy and manually combing through hundreds of lines of....

No, don't do that.

To solve this predicament, some smart people have developed different **version control systems**.
A version control system is a program whose job is to help you manage versions of your code.
In most cases, they **help you take snapshots of your code**, so that you can see how your code changes over time.
As a result, you **develop a timeline** of your code's state.

With a timeline of your code's state, your version control system can:

- help you figure out where bugs were introduced
- make it easier to collaborate with other coders
- keep your experimental code away from your stable, working code.
- do much, much more than three things.

In this course, we will be using **Git** as our version control system.
Git is powerful and wildly popular in industry.
Your experience with Git will undoubtedly be useful throughout your career in Computer Science.

It's also fun, so that's cool.

### Takeaways

- Learn what a version control system is, as well as some common features.
- Gain experience adding files to a Git repository and tracking changes to those files over time.
- Learn how to separate work onto separate Git branches.
- Understand the difference between a local and remote repository.

## Walkthrough

### Git Repositories

When you using Git, you work within a Git **repository**.
A repository is essentially a folder for which Git has been tracking the history.
We call that folder containing files and history your **local** copy of a repository.
We say it's local because it's stored locally -- in a place where you can access its contents just like any other folder.

This is the part where I want to compare Git to Dropbox or Google Drive, but this is a dangerous comparison.
Realize[^mind] that Git will feel similar to these services in some ways, but there are many features that make them *very* different.

When you work with a local Git repository, you will:

- **ask Git to track** of changes to files.
  Git *does not* automatically track files.
  You have to tell it to track stuff.
- **ask Git to take snapshots** of the files in your repository.
  Essentially, instead of copying your code into a folder to back it up, you'll tell Git to take a snapshot instead.
  Each snapshot represents the state of your repository *at that moment in time*.

Notice that each of these actions require **you** to ask Git to do stuff.
Git does not do these things by itself.
Because it's not automatic, you have the ability to take snapshots only when it makes sense.
For example, it's common to take snapshots whenever you finish a feature or before you start working on experimental code[^experimental].

### Trying out GitLab

To backup[^remote] work stored in a local repository, people often use an online service to store their repositories remotely.
In this course, we will be using a campus-hosted service called **GitLab**.

GitLab, like other git hosting services[^services], allows you to log into a website to create a **remote repository**.
Once created, you can **clone** (or download) your new repository into a **local copy**, so that you can begin to work.
An empty repository will contain no files and an empty timeline (with no snapshots).

Try the following to create your own, empty repository on GitLab:

1. Log in to https://git-classes.mst.edu/ using your Single Sign-on credentials.
2. Click the `+` (New Project) button in the upper right to create a new repository on GitLab.
3. Under Project Name, give your project a good name. Let's call it `my-fancy-project`.
    - You can enter a description if you like, or you can leave it blank.
    - Make sure your repository's visibility is set to Private.
4. Click the Create Project button.
5. Welcome to your repository's home page! Don't close it, yet. We'll need to copy some commands from here.

Now that you've created your repository, it's ready for you to start working.
Let's try cloning the remote repository into a local repository.

1. Look for the "Create a new repository" section and copy the command that starts with `git clone https://...my-fancy-project.git`
2. Connect to a campus Linux machine using PuTTY and paste that command in your bash shell.
3. Press enter, and type in your username and password when prompted.
4. Run `ls`. You should see that a folder called `my-fancy-project` was created in your current working directory.
5. Run `cd` to enter your freshly cloned repository.

Nice work!

Now, it's **very important** that you understand the objective of this exercise.
You've now seen what it looks like to create a remote repository on GitLab and clone it down into a local repository.
If you were working on a real project, the next step would be to create files in your `my-fancy-project` folder, take snapshots of those files, and upload your snapshots to GitLab.

In this course, **you will not have to create any GitLab repositories yourself**.
Instead, your instructor will be creating repositories and sharing them with you.
The ability to share repositories on GitLab is one of its more powerful features.

### Tracking Files

At this point, you now have a (very fancy) local repository called `my-fancy-project`.
Currently, your repository has no timeline, and Git is not watching any of the files in it.

Before we get too involved, let's see what's in our repository so far.
Try running `ls -a` within `my-fancy-project`

~~~bash
$ ls -a
.    ..   .git
~~~

See that `.git` directory there?
That is a hidden directory that Git uses to store your timeline of snapshots and a bunch of other data about your repository.
If you delete it, Git will not know what to do with the files in your directory.
In other words, deleting the `.git` directory turns a Git repository into a plain old folder.

So don't do that.

An empty repository isn't much use to us.
Let's try asking Git to watch some files for us.

Create a very simple Hello World C++ program and name it `hello.cpp`

~~~bash
# Let's see what's in here, first...
$ ls -a
.    ..   .git

# Now let's write that Hello World program
$ emacs hello.cpp

# Cool. There it is.
$ ls -a
.    ..   .git	hello.cpp
~~~

Now, let's use the `git status` command to ask Git for the **status** of the repository.

~~~bash
$ git status
On branch master

Initial commit

Untracked files:
  (use "git add <file>..." to include in what will be committed)

    hello.cpp

nothing added to commit but untracked files present (use "git add" to track)
~~~

Git is telling us that it sees a new file `hello.cpp` that is currently **untracked**.
This means that Git has never seen this file before, and that Git has not been told to track the changes made to it.
Let's use the `git add` command to ask Git to do just that.

~~~bash
$ git add hello.cpp

$ git status
On branch master

Initial commit

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)

    new file:   hello.cpp
~~~

Now, you can see that `hello.cpp` is listed under "Changes to be committed".
In Git terminology, we would say that `hello.cpp` is **staged for commit**.
In other words, `hello.cpp` is **ready to be included in the next snapshot**.

Whenever you take a snapshot, Git will only include the changes that are staged.
By staging changes for commit, you're essentially picking and choosing what you want to include.

### Taking a Snapshot

Although "snapshot" is a convenient term, the real Git term is **commit**.
That is, a Git repository timeline is comprised of a series of **commits**.

Now that `hello.cpp` is staged for commit, let's try committing it.

First, let's see what `git status` says

~~~bash
$ git status
On branch master

Initial commit

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)

    new file:   hello.cpp
~~~

OK, that looks good.

Let's also take a look at our current timeline of commits.
We'll use `git log` to ask Git to show us our current history.

~~~bash
$ git log
fatal: your current branch 'master' does not have any commits yet
~~~

Remember that, so far, all we've done is clone an empty repository from GitLab and *stage* a new file for commit.
It makes sense that we don't see any commits in our history yet.

Before we commit our changes, we need to tell Git who we are.
If we don't do this first, Git will refuse to commit anything for us!

~~~bash
# Please use your first and last name for the sake of grading.
$ git config --global user.name "<your_name>"

# Please use your university email address, again, for the sake of grading.
$ git config --global user.email "<your_email>"

# Let's also tell Git which text editor you prefer to use.
# You will need to choose a console editor such as jpico, emacs, or vim.
$ git config --global core.editor <editor_command>
~~~

Now we can finally commit our changes using the `git commit` command.

~~~bash
# It's always a good idea to run `git status` before running `git commit`
# just so we can see what we're including in our commit.
$ git status
On branch master

Initial commit

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)

    new file:   hello.cpp

# That looks good, so let's commit it!
$ git commit
~~~

Git will pop open an editor for you.
You **must** include a commit message here in order to commit.
Simply enter a meaningful message (like `Add hello.cpp`), save the message, and exit the text editor.

Make sure your message is meaningful!
If you use garbage commit messages[^garbage], you will only hurt your future self and your grade.

Let's see what our repository status looks like now.

~~~bash
$ git status
On branch master
nothing to commit, working tree clean
~~~

Git is telling us that nothing has changed since the last commit.
That makes sense!
We added `hello.cpp`, committed it, and we haven't changed anything since that commit.

What about the log?

~~~bash
$ git log
commit 648203a12a0b8ab1e0e37336d891b0420994739d (HEAD -> master)
Author: Homer Simpson <simpsonh@lardlad.donuts>
Date:   Mon Jun 12 12:38:01 2017 -0500

    Add hello.cpp
~~~

That's great!
Our timeline now contains one commit: the commit that added `hello.cpp`.
Over time, you will commit more and more changes, building up a longer and longer timeline of commits.

### Reading a Status Report

Let's talk in more detail about `git status`.

A file in a Git repository can be in one of **four** states:

- **Unchanged**: Git is tracking this file, but the file looks exactly the same as it did as of the latest commit.
- **Modified**: Git is tracking this file, and the file *has changed* since the last commit.
    - **Not staged**: The changes to this file **will not** be included if you try to commit them with `git commit`.
    - **Staged**: The changes to this file **will** be included if you try to commit them with `git commit`.
- **Untracked**: Git **is not** tracking this file at all. It doesn't know if/how it has changed since the last commit.

So, what's the big deal?

Every time you get ready to run `git commit`, you should make sure you are committing what you want to commit.
If you forget to stage changes, Git **will not include them** in your commit!

How do you stage changes to files?
Use `git add`.
Even if a file is not new, you will need to stage its changes for commit using `git add`.

### Uploading to GitLab

Alrighty.

Here you are with your fancy repository.
`git status` says that there's nothing new since the last commit.
`git log` says that there's one commit in the history.

If you visit the webpage for `my-fancy-project` on GitLab, you'll notice that there's still nothing up there.
We need to **push** our new commit to GitLab first.

~~~bash
# Enter your Single Sign-on credentials when prompted
$ git push
~~~

Since we cloned the repository from GitLab earlier, Git assumes that we want to push our changes back to the same place.
If you refresh the project page for `my-fancy-project` on GitLab, you should see `hello.cpp` up there!

Take some time to explore your remote repository on GitLab.

### Oh, Fork.

Close your eyes again[^half-closed].

Here you are working on that Next Big Thing again.
As you code, you see a beautiful person emerge[^emerge] from the field's tall grass.

"I am a muse," they say.
"Your program is terrible."

You grimace.

The muse proceeds to explain in great detail how your program can be so much better than it is.
You agree.
This is a muse after all.
Inspiration is their job[^tony-robins].

Now here's your predicament.
The changes proposed by the muse are going to require you to *totally* rework your program.
Meanwhile, you need to continue to fix bugs in your existing program to keep your customers happy.

You have two choices:

- Buck up and commit to redoing your entire project, leaving your customers grumpy about the bugs you need to fix.
- Ignore the idea from the muse and fix continue the bugs, throwing the muse's loud, awe-inspiring ideas in the garbage.

Open your eyes.

Alright, so I lied to you.
There are a couple more choices actually.

- You *could* copy your entire local repository into a second one and name it `muse_version`, but that sounds like a bad idea.
  Soon we'll end up with the same woes we had when we were copying our code into new folders to back it up.
- You *could* let Git manage two parallel lines of development.

That second option sounds **much** better.

One of Git's most powerful features is its ability to **branch** your code's timeline.
No, it's not like Primer[^Primer].
You're not going to have separate crazy timelines going back and forth and every which way.

It's more like having parallel universes.
Your original universe (branch) is called `master`.

You tell Git to branch at a specific commit, and from there on out, what happens on that branch is separate from other branches.
In other words, you enter an alternate universe, and all changes only affect the alternate universe, not the original (`master`) universe.

Now that's dandy, but let's say that we're happy with our experimental branch.
How can we integrate that back into the branch of stable code (`master`) again?
Well, Git has the ability to **merge** one branch into another.
When you merge two branches together, Git will figure out what's different between the two branches, and copy the important stuff from your experimental branch into your stable branch (`master`).

Branching is incredibly useful.
Here are just a handful of cases where it comes in handy:

- You want to keep experimental code away from stable, working code.
- You want to keep your work separate from your teammates' code.
- You want to keep your commit history clean by clearly showing where new features were added.

Now, here you are in real life sitting in your leather chair[^assume1], smoking a pipe[^assume2], sipping on bourbon[^given], and wondering what commands you use to actually work with branches in Git.
It's time to get your hands dirty.

Instead of asking you to create a bunch of commits and branches by hands, we're going to use an online tool to work with branches.
It's a game, actually.

Pop open a browser and pull up http://learngitbranching.js.org/
Then, work through the following exercises:

- 1.1: Introduction to Git Commits
- 1.2: Branching in Git
- 1.3: Merging in Git

Be sure to read the stuff that pops up!
This is a *very* good learning resource.

### Your Git Workflow

Your workflow will be something like this:

1. Create/Change files in your repository.
2. Use `git add` to stage changes for commit.
3. Use `git status` to check that the right changes are staged.
4. Use `git commit` to commit your changes.
5. Use `git push` to push your new commits up to GitLab.
6. View your repository on GitLab to ensure that everything looks right.
7. Repeat steps 1 through 6 as necessary.

You don't have to check GitLab every time you push, but it is **highly** recommended that you check your project before it's due.
It is easy to forget to push your code before the deadline.
Don't lose points for something so simple.

\newpage
## Questions
Name: `______________________________`

1. What is the **full** command you ran to clone your `my-fancy-project` repository? (Note, we **don't** want your username/password... we just want the command.)
\vspace{8em}

2. View your `hello.cpp` file on GitLab. Notice that the lines are numbered on the left side of your code. Click on the `3` for line 3.

    a. What happens to that line of code?
       \vspace{8em}

    b. Copy the URL for the page and paste it in a new browser tab. What does that link point to?
       \vspace{8em}

3. What is the series of commands you used to get through Level 1.3 of http://learngitbranching.js.org/ ? Hint: It's possible to do it in 5 commands.
\vspace{10em}

\newpage

## Quick Reference

`git add`

- Stages new, untracked files for commit
- Stages modified files for commit

`git commit`

- Creates a new commit (snapshot) on your commit timeline
- Opens an editor and requires that you enter a log message

`git status`

- Allows you to check the status of your repository
- Shows which branch you are currently on.
- Files can be **untracked**, **unstaged**, **staged**, or **unchanged**
- It's a good practice to check the status of your repository before you commit.

`git log`

- Shows you a list of commits in your repository

`git push`

- Pushes new **commits** to from a local repository to a remote repository.
- You cannot push files, you can only push commits.

`git checkout`

- Can be used to check out different branches.
- Can be used to *create* a new branch.
- Can be used (with great caution!) to check out specific commits.

`git branch`

- Can be used to create or delete branches.
- Can be used to list all local and remote branches.

`git merge`

- Merges two branches together.


## Further Reading

- [The Git Book](https://git-scm.com/book/en/v2)
- [Git Branching Tutorial](http://learngitbranching.js.org/)
- [GitHub's Git Tutorial](https://try.github.io/levels/1/challenges/1)

[^open]: Now open them again, because it's hard to read with your eyes shut.
[^next-big-thing]: This is your daydream, friend. I have no idea what this program is or does.
[^daydream]: Yes, you're daydreaming about work.
[^remote]: And other things. Remotes are actually *extremely* useful.
[^mind]: Using your mind.
[^experimental]: Maybe you're rewriting a function, and you don't know if it'll work. It's convenient to take a snapshot, so that if things go bad, you can always revert back to a working state.
[^services]: GitLab, GitHub, BitBucket, etc.
[^garbage]: Such as "asdf", "stuff", "work", or "finished the lab".
[^half-closed]: Maybe just half-closed this time.
[^emerge]: It's as though they were laying there in the grass the whole time. So weird.
[^tony-robins]: You later realize that the muse was just Tony Robbins getting you super amped about everything.
[^Primer]: Although you should *absolutely* see it if you haven't. Who doesn't love a good indie time travel movie?
[^assume1]: I assume.
[^assume2]: I reckon.
[^given]: That's a given.
