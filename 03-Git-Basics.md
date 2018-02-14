# Git Basics

## Motivation

Close your eyes.[^open]

Imagine yourself standing in a wide, open field.
In that field stands a desk, and on that desk, a computer.
You sit down at the desk ready to code up the Next Big Thing.[^next-big-thing]

You start programming and find yourself ready to start writing a cool new feature.
"I better back up my code," you think to yourself. "Just in case I really goof it up."
You create a new folder, name it "old_version" and continue on your way.

As you work and work,[^daydream] you find yourself with quite a few of these backups.
You see "old_version" and "old_version2" alongside good old "sorta_works" and "almost_done"
"Good thing I made these backups", you say. "Better safe than sorry."

Time passes.

"Wait... this isn't right..." you think.
Your code is broken!
Boy, it's a good thing you kept those backups.
But wait...which of these backups actually worked?
What's different in *this* version that's breaking your project?

Open your eyes.[^close]

If you haven't already experienced this predicament outside of a daydream, you certainly will.
It's a fact that as you work on a programming project, you will add features to your code, change the way it works, and sometimes introduce bugs.
Sure, you can manage your projects by making copy after copy and manually combing through hundreds of lines of...

No, don't do that.

To solve this predicament, some smart people have developed different **version control systems**.
A version control system is a program whose job is to help you manage versions of your code.
In most cases, they *help you take snapshots of your code*, so that you can see how your code changes over time.
As a result, you *develop a timeline* of your code's state.

With a timeline of your code's state, your version control system can:

- help you figure out where bugs were introduced.
- make it easier to collaborate with other coders.
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
We say it's local because it's stored locally --- in a place where you can access its contents just like any other folder.

When you work with a local Git repository, you will:

- **ask Git to track** changes to files.
  Git *does not* automatically track files.
  You have to tell it to track stuff.
- **ask Git to take snapshots** of the files in your repository.
  Essentially, instead of copying your code into a folder to back it up, you'll tell Git to take a snapshot instead.
  Each snapshot represents the state of your repository *at that moment in time*.

Notice that each of these actions require *you* to ask Git to do stuff.
Git does not do these things by itself.
Because it's not automatic, you have the ability to take snapshots only when it makes sense.
For example, it's common to take snapshots whenever you finish a feature or before you start working on experimental code.[^experimental]

In addition to local repositories, Git also has the concept of **remote** repositories:
repositories connected to your local repository that you can push snapshots to or pull snapshots from.
Remote repositories allow you to collaborate with others on a project without everyone having to share the same computer.

This is the point at which I want to compare Git to Dropbox or Google Drive, but that would be a dangerous comparison.
Realize[^mind] that Git will feel similar to these services in some ways, but there are many features that make them *very* different.

### Trying out GitLab

To backup[^remote] work stored in a local repository, people often use an online service to store their repositories remotely.
In this course, we will be using a campus-hosted service called **GitLab**.

GitLab, like other git hosting services,[^services] allows you to log into a website to create a **remote repository**.
Once created, you can **clone** (or download) your new repository into a **local copy**, so that you can begin to work.
An empty repository will contain no files and an empty timeline (with no snapshots).

Try the following to create your own, empty repository on GitLab:

1. Log in to [https://git-classes.mst.edu/](https://git-classes.mst.edu/) using your Single Sign-On credentials.
2. Click the `+` (New Project) button in the middle of the top bar to create a new repository on GitLab.
3. Under Project Name, give your project a good name. Let's call it `my-fancy-project`.
    - You can enter a description if you like, or you can leave it blank.
    - Make sure your repository's visibility is set to Private.
4. Click the Create Project button.
5. Welcome to your repository's home page! Don't close it, yet. We'll need to copy some commands from here.

Now that you've created your repository, it's ready for you to start working.
Let's try cloning the remote repository into a local repository.

1. Look for the "Create a new repository" section and copy the command that starts with `git clone https://...my-fancy-project.git`
2. Connect to a campus Linux machine and paste that command in your bash shell.
3. Press enter, and type in your username and password when prompted.
4. Run `ls`. You should see that a folder called `my-fancy-project` was created in your current working directory.
5. Use `cd` to enter your freshly cloned repository.

Nice work!


<!-- TODO keep? -->
Now, it's *very important* that you understand the objective of this exercise.
You've now seen what it looks like to create a remote repository on GitLab and clone it down into a local repository.
If you were working on a real project, the next step would be to create files in your `my-fancy-project` folder, take snapshots of those files, and upload your snapshots to GitLab.

In this course, *you will not have to create any GitLab repositories yourself*.
Instead, your instructor will be creating repositories and sharing them with you.
The ability to share repositories on GitLab is one of its more powerful features.

### Tracking Files

At this point, you now have a (very fancy) local repository called `my-fancy-project`.
Currently, your repository has no timeline, and Git is not watching any of the files in it.

Before we get too involved, let's see what's in our repository so far.
Try running `ls -a` within `my-fancy-project`:

~~~
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

For the sake of this example, create a very simple Hello World C++ program and name it `hello.cpp`.
Let's use the `git status` command to ask Git for the **status** of the repository.

~~~
$ git status
On branch master

Initial commit

Untracked files:
  (use "git add <file>..." to include in what will be committed)

    hello.cpp

nothing added to commit but untracked files present ↩
  (use "git add" to track)
~~~

Git is telling us that it sees a new file `hello.cpp` that is currently **untracked**.
This means that Git has never seen this file before, and that Git has not been told to track the changes made to it.
Let's use the `git add` command to ask Git to do just that.

~~~
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
In other words, `hello.cpp` is ready to be included in the next snapshot.
The `git add` command does two things: it tells Git to track that file, if Git isn't already tracking it, and it stages the changes in that file for the next commit.

Whenever you take a snapshot, Git will only include the changes that are staged.
By staging changes for commit, you're essentially picking and choosing what you want to include.

### Taking a Snapshot

Although "snapshot" is a convenient term, the real Git term is **commit**.
That is, a Git repository timeline is comprised of a series of **commits**.

Now that `hello.cpp` is staged for commit, let's try committing it.

Before we commit our changes, we need to tell Git who we are.
If we don't do this first, Git will refuse to commit anything for us!
You only need to do this the first time you use Git on a machine.
Git stores your configuration in a file (`~/.gitconfig`).

~~~
$ git config --global user.name "<your_name>"
$ git config --global user.email "<your_email>"
~~~

Let's also tell Git which text editor you prefer to use.
You need to choose a console editor such as jpico, nano, emacs, or vim.
~~~
$ git config --global core.editor <editor_command>
~~~

Now we can finally commit our changes using the `git commit` command.

~~~
# It's always a good idea to run `git status` before running
# `git commit` just so we can see what we're including in our commit.
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
You *must* include a commit message here in order to commit.
Simply enter a meaningful message (like `Add hello.cpp`), save the message, and exit the text editor.

Make sure your message is meaningful!
If you use garbage commit messages,[^garbage] you will only hurt your future self and your grade.

Let's see what our repository status looks like now.

~~~
$ git status
On branch master
nothing to commit, working tree clean
~~~

Git is telling us that nothing has changed since the last commit.
That makes sense!
We added `hello.cpp`, committed it, and we haven't changed anything since that commit.

Let's also take a look at our current timeline of commits.
We'll use `git log` to ask Git to show us our current history.

~~~
$ git log
commit af9db9b5681ff748847eca6ddedb46069e1b0366 ↩
  (HEAD -> master)
Author: Homer Simpson <simpsonh@lardlad.donuts>
Date:   Wed Aug 30 10:58:28 2017 -0500

    Add hello.cpp
~~~

That's great!
Our timeline contains one commit: the commit that added `hello.cpp`.
Over time, you will commit more and more changes, building up a longer and longer timeline of commits.

### Reading a Status Report

Let's talk in more detail about `git status`.

A file in a Git repository can be in one of four states:

- **Unchanged**: Git is tracking this file, but the file looks exactly the same as it did as of the latest commit.
- **Modified**: Git is tracking this file, and the file *has changed* since the last commit.
    - **Not staged**: The changes to this file *will not* be included if you try to commit them with `git commit`.
    - **Staged**: The changes to this file *will* be included if you try to commit them with `git commit`.
- **Untracked**: Git *is not* tracking this file at all. It doesn't know anything about it has changed since the last commit.

So, what's the big deal?

Every time you get ready to run `git commit`, you should make sure you are committing what you want to commit.
If you forget to stage changes, Git *will not include them* in your commit!

How do you stage changes to files?
Use `git add`.
Even if a file is not new, you will need to stage its changes for commit using `git add`.

### Seeing What Happened in a Commit

Let's make another commit. Maybe we'll make our hello world program ask the user how their day is going as well.

~~~
$ vim hello.cpp
$ git add hello.cpp
$ git status
On branch master
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

  modified:   hello.cpp
$ git commit
~~~

Now that we've done this, let's see what our log looks like:

~~~
$ git log
commit bdf0003404901363f05b14f20bb7f7becf4b2dd5 ↩
  (HEAD -> master)
Author: Homer Simpson <simpsonh@lardlad.donuts>
Date:   Wed Aug 30 12:54:59 2017 -0500

    Add Faux Politeness

commit af9db9b5681ff748847eca6ddedb46069e1b0366
Author: Homer Simpson <simpsonh@lardlad.donuts>
Date:   Wed Aug 30 10:58:28 2017 -0500

    Add hello.cpp
~~~

Neat! There are both our commits there, listed in order from newest to oldest.
We can see who wrote them, when, and what the commit message was.
Usually this is all you need, but sometimes you might want to see what actually changed in a commit.
Maybe you've just cloned someone else's repository and you're confused about something they did, or maybe you just want to savor a particularly delicious line of code you wrote.
We don't judge.

If you want to inspect what changed in one commit, you need some way of identifying that commit to Git.
Git assigns a **hash** to each commit --- that is the long string of letters and numbers on the line starting with `commit` in the log.[^sha1]
You can refer to commits by their hash whenever you need to.
Since it's pretty unlikely that two commit hashes start with the same handful of characters, you can refer to a commit by just the first several letters of its hash; five or so is usually plenty.[^five]
(`git commit` shows the first seven characters in its output.)

The `git show` command shows you what changes happened in a commit.
So, if we want to see our beautiful, lovely, astute changes that we made in that latest commit, we can do that:

~~~
$ git show bdf00
commit bdf0003404901363f05b14f20bb7f7becf4b2dd5 ↩
  (HEAD -> master)
Author: Homer Simpson <simpsonh@lardlad.donuts>
Date:   Wed Aug 30 12:54:59 2017 -0500

    Add Faux Politeness

diff --git a/hello.cpp b/hello.cpp
index 9a52d30..e0d3a48 100644
--- a/hello.cpp
+++ b/hello.cpp
@@ -5,5 +5,7 @@ using namespace std;
 int main()
 {
   cout << "Hello world!" << endl;
+  cout << "How are you?" << endl;
+  //ignore user feelings and exit
   return 0;
 }
~~~

This shows a 'diff' of your changes. If you see a `+` in the first column, that means you added that line; a `-` means that line was removed.
Git works line-by-line, so if you change something on a line, it shows up as you deleting one line and adding another.
The rest of the output provides some context around your changes so you have a vague idea of where they happened.

### Uploading to GitLab

Alrighty.

Here you are with your fancy repository.
`git status` says that there's nothing new since the last commit.
`git log` says that there are two commits in the history.

If you visit the webpage for `my-fancy-project` on GitLab, you'll notice that there's still nothing up there.
We need to **push** our commits to GitLab first.

~~~
$ git push
~~~

Since we cloned the repository from GitLab earlier, Git assumes that we want to push our changes back to the same place.
If you refresh the project page for `my-fancy-project` on GitLab, you should see `hello.cpp` up there!

Take some time to explore your remote repository on GitLab.
You can view your commit history and even make commits!

### Oh, Fork.

Close your eyes again.[^half-closed]

Here you are working on that Next Big Thing again.
As you code, you see a beautiful person emerge[^emerge] from the field's tall grass.

"I am a muse," they say.
"Your program is terrible."

You grimace.

The muse proceeds to explain in great detail how your program can be so much better than it is.
You agree.
This is a muse after all.
Inspiration is their job.[^tony-robins]

Now here's your predicament.
The changes proposed by the muse are going to require you to *totally* rework your program.
Meanwhile, you need to continue to fix bugs in your existing program to keep your users happy.

You have two choices:

- Buck up and commit to redoing your entire project, leaving your pals (the users) grumpy about the bugs you need to fix.
- Ignore the idea from the muse and continue fixing the bugs, throwing the muse's loud, awe-inspiring ideas in the garbage.

Open your eyes.

Alright, so I lied to you.
There are a couple more choices actually.

- You *could* copy your entire local repository into a second one and name it `muse_version`, but that sounds like a bad idea.
  Soon we'll end up with the same woes we had when we were copying our code into new folders to back it up.
- You *could* let Git manage two parallel lines of development.

That last option sounds *much* better.

One of Git's most powerful features is its ability to **branch** your code's timeline.
No, it's not like Primer.[^Primer]
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

Now, here you are in real life sitting in your leather chair,[^assume1] smoking a pipe,[^assume2] sipping on bourbon,[^given] and wondering what commands you use to actually work with branches in Git.
It's time to get your hands dirty.

First, let's notice something about our repo's `status`:

~~~
$ git status
On branch master
nothing to commit, working tree clean
~~~

See that first line? Git starts you off on a branch named `master`.
Think of `master` as "pointing at" the newest commit on the master branch.
Every time you make a new commit while you're "on branch master", Git moves the `master` pointer
to point to the commit you just made.

Let's also examine the `log` with this knowledge in mind.
For this section, we're going to pass a few flags to `git log` so that it prints out beautiful ASCII art:

- `--oneline` summarizes each commit in just one line.
- `--graph` draws an ASCII commit graph on the left.[^graph]
- `--all` shows all branches in the log so that we get a view of the whole repo, not just the branch we're on now.

~~~
$ git log --oneline --graph --all
* bdf0003 (HEAD -> master) Add Faux Politeness
* af9db9b Add hello.cpp
~~~

Ok, ok, it's not *very* beautiful.
But, it does show our two commits and indicates that `master` is right now pointing at commit `bdf0003`.
It also shows this other thing, `HEAD`. What's that about?

You can think of `HEAD` as a pointer to the branch you're currently on.[^HEAD]
When you make a new commit, the branch `HEAD` is pointing at is the one that gets updated to point to your new commit.

Let's see what happens when we make a new branch.
Obviously, the command we use for this is `git checkout`, not `git branch`.
`git checkout` moves your `HEAD` around; passing the `-b` flag tells it to

1. Create a new branch pointing at the commit `HEAD` is at right now, then
2. Point `HEAD` at the branch it just made.[^git-branch]

Let's do something with those feelings the user has.

~~~
$ git checkout -b handle-feelings
Switched to a new branch 'handle-feelings'
$ git status
On branch handle-feelings
nothing to commit, working tree clean
~~~

Neat! We've made a new branch and now we're ready to make some commits on it.
Let's also check the log real quick:

~~~
$ git log --oneline --graph --all
* bdf0003 (HEAD -> handle-feelings, master) Add Faux Politeness
* af9db9b Add hello.cpp
~~~

As expected, now we have *two* branches pointing at commit `bdf0003` and our `HEAD` is pointing at the `handle-feelings` branch.

Make a few changes to `hello.cpp` --- perhaps read the user's feelings in to a string and then tell them that you (the computer)
empathize with them. Then commit your changes:

~~~
$ vim hello.cpp
$ git add hello.cpp
$ git commit
[handle-feelings e146ae9] Empathize with the user!
 1 file changed, 5 insertions(+), 1 deletion(-)
~~~

Alright, close your eyes for a second and imagine how the log will look.
Knotty? Maybe a mushroom or two growing from a split in the side? Is it a bright, cheery home to a colony of termites?

Ok, open your eyes and let's see if your imagination was right:

~~~
$ git log --oneline --graph --all
* e146ae9 (HEAD -> handle-feelings) Empathize with the user!
* bdf0003 (master) Add Faux Politeness
* af9db9b Add hello.cpp
~~~

The `master` branch is still pointing at commit `bdf0003`, but `handle-feelings` is pointing at our brand new commit, `e146ae9`.
No termites, but hopefully that's more or less what you imagined.

So far our commit graph is a straight line. Let's stick a fork in it by making some changes to the `master` branch.
First, we switch which branch `HEAD` points at:

~~~
$ git checkout master
Switched to branch 'master'
$ git log --oneline --graph --all
* e146ae9 (handle-feelings) Empathize with the user!
* bdf0003 (HEAD -> master) Add Faux Politeness
* af9db9b Add hello.cpp
~~~

The commit log doesn't change, but it does show that we've moved our `HEAD`.

Now, let's ask for the user's name!

~~~
$ vim hello.cpp
$ git add hello.cpp
$ git commit
[master 0a405b6] Ask for the user's name!
 1 file changed, 7 insertions(+)
~~~

This is where the log gets interesting.
As you can see, there's a commit on the `master` branch that's not on the `handle-feelings` branch, and vice-versa:

~~~
$ git log --oneline --graph --all
* 0a405b6 (HEAD -> master) Ask for the user's name!
| * e146ae9 (handle-feelings) Empathize with the user!
|/
* bdf0003 Add Faux Politeness
* af9db9b Add hello.cpp
~~~

This is all well and good --- in one timeline (`handle-feelings`), we've written a program that says hi and asks about the user's feelings;
in the other (`master`), we've asked for their name.
We can switch between these two timelines as much as we want by using `git checkout`.

But, what if we want a program that does *both things*?
Can we un-split these two timelines?

Yes!

For this task we use the `git merge` command.
This command merges another branch into the branch we're currently on.

Let's merge `handle-feelings` into `master`.
(If you're not already on the master branch, switch to it with `git checkout master`.)

Usually merges go off without a hitch --- Git is pretty good at figuring out how to smush code together.
However, sometimes trouble does arise, typically when both of your branches have modified the exact same bit of code.
Rather than letting you become your own grandma or otherwise introducing classic sci-fi timetravel paradoxes,
Git asks you, the omniscient time-lord,[^lord] to sort out the paradox yourself.

Let's see what happens when we merge our two branches together:

~~~
$ git merge handle-feelings
Auto-merging hello.cpp
CONFLICT (content): Merge conflict in hello.cpp
Automatic merge failed; fix conflicts and then commit the result.
~~~

When a merge conflict happens, Git marks the conflicting changes in the file it's confused about.
The bits between `<<<<<<< HEAD` and `=======` are the lines from the file on the branch you're on now.
The bits between `=======` and `>>>>>>> handle-feelings` are the lines from the file on the branch you're trying to merge.

You need to look at these changes and decide for yourself how to merge them. Sometimes you'll keep one bit or the other,
sometimes both, and sometimes parts of each.

Here's what `hello.cpp` looks like:

~~~
$ cat hello.cpp
#include<iostream>
#include<string>

using namespace std;

int main()
{
<<<<<<< HEAD
  string name;
=======
  string feelings;
>>>>>>> handle-feelings

  cout << "Hello world!" << endl;

  cout << "What's your name, friend? ";
  cin >> name;

  cout << "How are you?" << endl;
  cin >> feelings;
  cout << "I empathize with how you feel, pal" << endl;
  return 0;
}
~~~

This is not too bad --- Git is just confused because both variables got declared on the same line.
It's as if, when we split time, in one timeline we put a banana on the table, and in the other we put an apple in the exact same spot.
When we try to merge the timelines together, rather than giving us a banappleana, Git asks us what to do.
We just need to move the banana or the apple over a little so they aren't occupying the same spot on the table.

In other words, we want to keep both variables, so we can just delete the `<<<<<<< HEAD`, `=======`, and `>>>>>>> handle-feelings` lines.
Once we do this, we need to tell Git that we've resolved the conflict by `git add`ing the file, then `git commit`ing the result.
Git writes the merge commit message for you, so you can just save and quit your editor right away.

If you check `git status` as you go, it will tell you what step to do next.

~~~
$ vim hello.cpp
$ git status
On branch master
You have unmerged paths.
  (fix conflicts and run "git commit")
  (use "git merge --abort" to abort the merge)

Unmerged paths:
  (use "git add <file>..." to mark resolution)

	both modified:   hello.cpp

no changes added to commit ↩
  (use "git add" and/or "git commit -a")
$ git add hello.cpp
$ git status
On branch master
All conflicts fixed but you are still merging.
  (use "git commit" to conclude merge)

Changes to be committed:

	modified:   hello.cpp

$ git commit
[master 40996e7] Merge branch 'handle-feelings'
~~~

Let's see how this looks in the log:

~~~
$ git log --oneline --graph --all
*   40996e7 (HEAD -> master) Merge branch 'handle-feelings'
|\
| * e146ae9 (handle-feelings) Empathize with the user!
* | 0a405b6 Ask for the user's name!
|/
* bdf0003 Add Faux Politeness
* af9db9b Add hello.cpp
~~~

We can see the merge commit has *two* lines going into it!
This is because it is made from two commits --- one from each branch we merged.
The `master` branch now has both its changes and the changes from `handle-feelings` in it;
the `handle-feelings` branch is still pointing exactly where it used to be.

To get a better feel for the mechanics of branching and merging, we recommend you try out [http://learngitbranching.js.org/](http://learngitbranching.js.org/).
At the very least, work through the following exercises:

- 1.1: Introduction to Git Commits
- 1.2: Branching in Git
- 1.3: Merging in Git

Be sure to read the stuff that pops up!
This is a *very* good learning resource.

### Ignoring Stuff

If you're working on a programming project, you'll probably end up with executables (`a.out` and friends) hanging around in your directory.
Every time you run `git status`, you'll see something like

~~~
On branch master
Untracked files:
  (use "git add <file>..." to include in what will be committed)

  a.out

nothing added to commit but untracked files present ↩
  (use "git add" to track)
~~~

At first, you may be tempted to just go ahead and commit that executable too.
But, this is not typically the best of ideas.
It's very easy to forget to compile right before committing, and if you don't, you'll end up with a commit where the source code says one thing and the executable does another!

The general advice is to track your *source files* with Git and to *not* track any files that you *generate* from those (so, executables).
So, if you don't want to ever commit `a.out`, do you have to settle for just always seeing it show up in `git status`?

No!

You can tell Git to ignore files by listing their names in a file in your repository named `.gitignore`.
So, to ignore `a.out`, you could do something like this:

~~~
$ echo 'a.out' > .gitignore   # or use a text editor
$ cat .gitignore
a.out

$ git status
On branch master
Untracked files:
  (use "git add <file>..." to include in what will be committed)

  .gitignore

nothing added to commit but untracked files present ↩
  (use "git add" to track)
~~~

No more `a.out`!
And, if your cat happens to walk across your keyboard and type `git add a.out`, Git will tell you, "Hey, you said to ignore that! No way am I tracking it!"

(It's not a bad idea to commit your `.gitignore`.)

### Your Git Workflow

Your workflow will be something like this:

1. Create/Change files in your repository.
2. Use `git add` to stage changes for commit.
3. Use `git status` to check that the right changes are staged.
4. Use `git commit` to commit your changes.
5. Use `git push` to push your new commits up to GitLab.
6. View your repository on GitLab to ensure that everything looks right.
7. Repeat steps 1 through 6 as necessary.

You don't have to check GitLab every time you push, but it is *highly* recommended that you check your project before it's due.
It is easy to forget to push your code before the deadline.
Don't lose points for something so simple.

\newpage
## Questions
Name: `______________________________`

1. In your own words, what happens when you clone your `my-fancy-project` repository?
\vspace{8em}

2. View your `hello.cpp` file on GitLab. Notice that the lines are numbered on the left side of your code. Click on the `3` for line 3.

    a. What happens to that line of code?
       \vspace{8em}

    b. Copy the URL for the page and paste it in a new browser tab. What does that link point to?
       \vspace{8em}

3. In your own words, what does merging two branches together do?
\vspace{8em}

4. If you have a merge conflict in `rats.cpp`, what series of commands would you run to fix the conflict and complete the merge?

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
- [Advice on writing good commit messages](https://chris.beams.io/posts/git-commit/)

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
[^Primer]: You should *absolutely* see Primer if you haven't. Who doesn't love a good indie time travel movie?
[^assume1]: I assume.
[^assume2]: I reckon.
[^given]: That's a given.
[^close]: If you read the first footnote, close them first.
[^sha1]: If you're curious, it's a SHA1 hash of the changes, the commit message, the author and time, the parent commit hash, and one or two other things.
Basically, it gives you some way of turning all the interesting parts of a commit into a string that uniquely identifies that commit.
[^five]: There are about a million 5-digit hex numbers, so unless you're the kind of person to get hit by lightning twice, odds are in your favor that you won't have two commits starting with the same 5 digits, even if you've got a couple hundred commits in your repository!
[^HEAD]: As an aside, `HEAD` can also point directly to commits.
This is called a 'detached HEAD' state, and if you're imagining unscrewing your head from your neck and using your arms to put it someplace you can't easily
get to, you've got the right idea: it's sometimes handy, but it's a little weird and not something you'd want to do that often.
[^git-branch]: Okay, okay, you can also do this with the `git branch` command in two steps: first create the branch with `git branch handle-feelings`,
then switch to that branch with `git checkout handle-feelings`.
Why `git checkout -b` instead of `git branch -c`? I have no clue; go ask Linus Torvalds.
[^graph]: Git thinks of commits as a directed acyclic graph --- you'll learn about this in discrete math.
In short, a 'graph' is effectively connect-the-dots for mathematicians: there are vertices (in Git's case, commits)
and arrows drawn between the vertices (in Git's case, each commit has an arrow pointing to the commit before it).
[^lord]: Well, time-lord of your repository, anyhow.
