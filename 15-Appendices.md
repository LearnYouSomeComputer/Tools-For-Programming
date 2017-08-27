\appendix

# General PuTTY usage

In this course, we'll be writing, compiling, and running programs on the Linux operating system.
Since our campus' Computer Learning Centers are mostly equipped with computers running Windows[^another], we need a way to connect to and use computers running Linux.

To do this, we'll be making extensive use of PuTTY.

## What PuTTY **is**

PuTTY is an **s**ecure **sh**ell (SSH) client for Windows.
This means that we can use PuTTY to connect to a remote Linux computer that is running an SSH server.
Once connected, we can run programs on that remote computer.

![PuTTY (running on your Windows computer) connects to a Linux computer over a network.](15/ssh.png){ width=60% }

After you use PuTTY to log in to a remote Linux computer, you can type commands into a `bash` shell.
It's important to understand that **the shell is actually running on the Linux computer**.
All programs you run in the shell actually run on that remote computer.

Those programs are *not* running on your Windows computer.

They are *not* running in PuTTY.

PuTTY is simply communicating with the Linux computer over the network to show you the shell.
PuTTY is just a kind of window[^pun] into the remote Linux computer.

## What PuTTY **is not**

To reiterate: PuTTY **is not** Linux.

Instead, PuTTY allows us to *connect* to a computer that is running Linux.
Whenever you type commands in the PuTTY shell, you're actually typing them in `bash`, which is running on the Linux server.
Again, PuTTY is just a kind of window into the remote Linux computer.

## How to Use PuTTY

### Basics

After you log into a CLC Windows computer, simply locate PuTTY in the list of programs and start it.
It should look like Figure \ref{putty-initial}.

![The initial PuTTY screen \label{putty-initial}](15/putty-startup.png){ width=60% }

Once PuTTY is open, simply pick a connection configuration from the list.
Click the configuration you'd like to load and press the \menu{Load} button.
Once you do that, you should see the corresponding hostname in the text field as shown in Figure \ref{putty-loaded}.
You can also create your own configuration or modify the existing configurations and save them using the \menu{Save} button.

![Choose a configuration from the list and press the `Load` button to load it up. \label{putty-loaded}](15/putty-loaded-config.png){ width=60% }

Once your configuration is loaded and all the settings look right, press the \menu{Open} button to start the connection.
PuTTY will start communicating with the remote computer specified by the hostname.
If it's unable to connect, PuTTY will complain.

If you've never connected to a particular Linux hostname before, PuTTY will warn you with a message similar to Figure \ref{putty-key-confirmation}.
It will show you its SSH fingerprint[^google] and ask that you confirm the connection.

![If you've never connected to a particular Linux machine, PuTTY will ask if you're sure you want to connect. \label{putty-key-confirmation}](15/putty-key-confirmation.png){ width=60% }

If you confirm the connection, PuTTY just needs to know your login credentials.
It'll start by asking for your username (Figure \ref {putty-username}) followed by your password (Figure \ref{putty-password}).

![If necessary, tell PuTTY your username. \label{putty-username}](15/putty-login-username.png){ width=60% }

![Use your Single Sign-On password to sign in. \label{putty-password}](15/putty-login-password.png){ width=60% }

Assuming you entered your credentials correctly, PuTTY will present you with a shell as in Figure \ref{putty-logged-in}.
Take note of the number of users on your host.
If there are a lot of users connected to the computer you're using, it'll be slower.
You might consider trying a different hostname if you find the one you're using is sluggish.

![Once you've connected, you'll be presented with a welcome message and a `bash` prompt. \label{putty-logged-in} ](15/putty-logged-in.png){ width=60% }

### Other Tips

Here are a handful of tips:

- You have to be on the campus network to connect to the campus Linux hosts.
    - You can use any computer that is connected to the campus Ethernet or wireless networks.
      That includes CLC computers, your own desktop, your friend's laptop, etc.
    - You can setup a VPN connection to connect to the campus network from off campus.
      Refer to IT's help pages to set that up.
- IT's has a list of the Linux hostnames on their website: http://it.mst.edu/services/linux/hostnames/
  Since the PuTTY default only lists the first 16 or so, most people use those.
  Try using the higher-numbered machines.
  They often have far fewer users on them, and thus, they're notably faster.
  When you connect, the Linux host welcome message will tell you how many users are connected.

### Useful Settings

### Clipboard Tips

# X-forwarding

True, we use the shell a lot in this course, but every now and then we have to run programs that have GUIs.
When we're running GUI programs on *Windows* (such as Notepad or Microsoft Word), it's easy.
Just find the program in your Start menu, click it, and off you go.
If you need to start a GUI program on a remote Linux computer, though, things are more... complex.

## Remote GUIs

Linux uses the X Window System to display GUIs and interact with you, the user.
Basically, GUI programs work like this:

> **GUI Program**: Hey, X Server. I need you to draw a window on the screen for my user.
>
> **X Server**: What's in it for me?
>
> **X Server**: I'm just kidding. What's it look like?
>
> **GUI Program**: Well, it's got a text box here, and some shapes over there.
>
> **X Server**: That sounds great. I'll draw that on this `display` over here.

X Server then sends a bunch of data to a `display`.
If that Linux computer has a monitor connected, the data would be sent to that monitor.

As it turns out, you can ask X Server to send that display data over a network.
If you ask nicely, PuTTY can request that X Server send that display data to your Windows computer.
Together with a program called Xming, we can see the windows (and such) that would have been displayed if we connected a monitor directly to the Linux computer.

But it's all remote.

## Configuring X-forwarding

As previously mentioned, we're still going to use PuTTY to connect to the remote host.
However, PuTTY doesn't know how to draw on the screen.
All it can do is the shell stuff.

To help PuTTY out, we need to start its partner in crime: Xming.
Find **Xming** within your start menu (as in Figure \ref{xming-start-menu}) and click it.
Don't start XLaunch or anything else.
We just want Xming.

![We want to start Xming. **Not** XLaunch or anything else. Xming. \label{xming-start-menu}](15/xming-start-menu.png){ width=60% }

You only need to do this one time after you log in.
Xming will run in the background until you stop it or log off.
You can check to see if Xming is running by looking in your task bar as shown in Figure \ref{xming-running}.
If you see the logo down there, there's no need to start Xming again.

![You can check your task bar to see if Xming is running. \ref{xming-running}](15/xming-running.png){ width=60% }

Now that Xming is running, we need to tell PuTTY to send all that display data to Xming.
After you load a Putty configuration but before you connect, you need to **make sure** that X11 Forwarding is enabled.

So:

1. Open PuTTY
2. Click a hostname in the list
3. Click the \menu{Load} button
4. Find the X11 Forwarding configuration and make sure it is enabled as shown in Figure \ref{putty-enable-x-forwarding}.

![Make **sure** that X11 Forwarding is enabled! \label{putty-enable-x-forwarding}](15/putty-enable-x-forwarding.png){ width=60% }

If Xming is running and X11 Forwarding is enabled, you can start your PuTTY connection by pressing the \menu{Open} button.
PuTTY will open a shell like normal.
Nothing actually looks different until you try to start a program.

Try running `gedit` (a GUI text editor for Linux), `firefox`, or `chromium-browser`.
These are all GUI programs and should start up.
Figure \ref{xming-started-gui}

![The Chromium Browser forwarded to our Windows computer. Remember that Chromium is running *on the Linux computer*. PuTTY is forwarding the display data, and Xming is drawing the browser window for us. \label{xming-started-gui}](15/xming-started-gui.png){ width=60% }

It's important to keep in mind that while your GUI program is running, your shell will be busy.
It's just like any other program you start in your shell.
Until you close the GUI program, your shell will be unavailable.
You may find it useful to have a couple of PuTTY windows open, so that you can multitask.

# Markdown

# Parsing command-line arguments in C++

Throughout this book you'll find commands like `ls -l` or `grep needle haystack.txt`.
Everything following the command name (`ls` or `grep`) is a command *argument*.
You too can write programs that take arguments!

In C++, the arguments given to a command are passed as parameters to your `main` function.
Instead of writing `int main()`, your `main` function will take two arguments:

- An `int` which gives the number of arguments passed on the command line.
- A `char**` which is an array of NTCAs[^2d] (also known as C-strings) which are the string values of the arguments passed.

These arguments are traditionally named `argc` and `argv`, for "argument count" and "argument values", respectively.
You can name them whatever you like, however; C++ doesn't care.

Let's do an example: here is a program that prints its arguments out to the screen, one per line:

```C++
#include<iostream>
using namespace std;

int main(int argc, char** argv)
{
  cout << "Index\tArgument" << endl;
  for(int i = 0; i < argc; i++)
  {
    cout << i << "\t" << argv[i] << endl;
  }

  return 0;
}
```

Let's run it and see what happens:

```
$ ./print a b c
Index   Argument
0       ./print
1       a
2       b
3       c
```

The "0th" argument is always passed; it is the name of the program that was run.
So the first argument on the command line is at `argc[1]`, and so on.

For a practical example, let's write a program that counts the number of lines in a file (like `wc -l`, but less fancy).
If the user doesn't pass us exactly one file, we want to print out a message telling them how to use the program;
otherwise, we should open the file and count the number of newlines in it.

```{.cpp .numberLines}
#include<iostream>
#include<fstream>
using namespace std;

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    cout << "Counts the number of newlines in the given filename"
         << endl << "Usage: " << argv[0] << " [<filename>]" << endl;
    return 1; // exit the program
  }

  ifstream fin(argv[1]);
  char buffer;
  int newline_count = 0;

  while(fin.get(buffer))
  {
    if(buffer == '\n')
    {
      newline_count++;
    }
  }

  fin.close();

  cout << argv[1] << " has " << newline_count << " lines." << endl;

  return 0;
}
```

In this example, you can see the use of `argv[0]` to display the program name
It is a common pattern to check the arguments passed before doing anything else and exit the program if they are incorrect.
Finally, we can use `argv[1]` as we would any other NTCA.
We could even access the individual characters of the filename by doing something like `argv[1][0]`.

Now, you may be wondering, "how do I get fancy-dancy options parsing, like `ls` and friends?"
Well, there are a few options.
The classic choice is [`getopt`](https://www.gnu.org/software/libc/manual/html_node/Getopt.html),
but the C standard library also has [a few other options](https://www.gnu.org/software/libc/manual/html_node/Parsing-Program-Arguments.html).
Boost also has a C++-style library for argument parsing, [program_options](http://www.boost.org/doc/libs/1_65_0/doc/html/program_options.html).
Or, you can write your own!

# Submitting homework with Git

Your instructor may have you submit your assignments using Git and GitLab.
These tools (or tools like them) are commonly used in industry, so getting accustomed to them early will pay off in the long run.

Git is an open source, distributed version control system that allows programmers to track changes made to source files and share those changes with collaborators.
It was created by Linus Torvalds (the Linux guy) and has become hugely popular in the last several years.

GitLab is an open source web application that makes it a bit easier for programmers to collaborate on programming projects.
GitLab lets users create repositories (projects) on a central server.
Users can then push their code to GitLab using git.
From there, users can clone their code to other machines, push up new changes, etc.
You could think of it (kind of) like Dropbox – GitLab is a central place to store your code.

In addition to storing code, GitLab has many features to help users collaborate with others.
Users can share projects with other users, giving them the ability to push/pull code to/from the project.
There are also bug tracking utilities, code review tools, and much more.

In this class, we'll be using just the git parts of GitLab, so don't worry if the fancier features sound difficult.
All you need to be able to do is

- View the GitLab website.
- Retrieve (clone) the lab assignments.
- Record (commit) your solutions to those assignments.
- Upload (push) those solutions back to GitLab.

Once you are done with an assignment, you should **always** check GitLab to make sure that your solutions are properly uploaded.

## Warning

Do not blindly copy and paste git commands from the internet and run them.
(This isn't specific to git --- that's just plain ol' good life advice right there.)
While all the common git commands are quite good at not losing your hard work, there *are* commands that can wreak havoc if used improperly.
If you don't know what a command does, take a few minutes to read some documentation on it first.

If things are not working right, first check `git status` and `git log` to see what git thinks has happened.
When that doesn't clear things up, have a look on your favorite search engine and/or ask a friend or your instructor for help.
*Definitely* ask someone if the internet is recommending you run unfamiliar git commands!

## Configuring Git

Before you can use git, you need to configure a couple of things.

### Who Are You?

Don't worry, no need for an existential crisis.
Git just wants to know your name and your email address.

Open a terminal (i.e., PuTTY) and run these commands (substituting your own name and email, of course):

```
$ git config --global user.name "Hank Chicken"
$ git config --global user.email hank@chickens.com
```

Git probably won't say anything when you run these commands; it's the strong silent type.
Don't worry, you're fine.

### Editing

Git sometimes will open a text editor so you can type in certain things (mainly commit messages).
The default editor it uses is joe, which almost no one knows how to use.[^exit-joe]
You will most likely want to set the editor to one you know and love.

To change the editor to `jpico`, run the following command (substitute a different editor if you like):

```
$ git config --global core.editor jpico
```

<!--
### Authentication (optional)

GitLab supports two forms of authentication: HTTPS and SSH.
HTTPS authentication "just works" - - - there's no setup needed.
It does, however, require you to enter your username and password each time you pull or push.

SSH authentication requires some setup and still won't work out of the box on the CLC machines.
So just give up and use HTTPS, because computers are terrible.

If you do want to use it, though, read [this guide](https://git-classes.mst.edu/help/ssh/README).
-->

## Working on assignments

Here's the gist of how to do assignments:

1. Get the assignment
    - Clone the repository
    - Open the assignment directions
2. Do the assignment
    - Commit changes as you go
    - Push to GitLab if you like
3. Turn it in
    - Push all your work to GitLab
    - Check the GitLab website to make sure your submission looks right

### Getting the assignment (`git clone`)

For each lab assignment you will be granted access to a git repository on GitLab that contains starter code and examples.
These repositories are visible only to you and the instructor and graders.
You will be granted [developer](http://docs.gitlab.com/ce/user/permissions.html) permissions to your repositories.
This will allow you to view the project and push changes to it.

When you start an assignment, the first thing you must do is download a copy of the repository so you can make changes to it.

1. Log in to [https://git-classes.mst.edu](https://git-classes.mst.edu).
2. Navigate to the project that you want retrieve.
3. Copy your repository’s URL from the text box in the middle of the page beneath the assignment name.
    - If you don't see it, try making your browser window wider. Hooray responsive web design!
    - Cloning over HTTPS (easy):
          1. Select 'HTTPS' from the dropdown next to the text box.
          2. Copy the URL from the text box. It should start with `https://git-classes.mst.edu`.
    - Cloning over SSH (if you have [ssh keys](https://git-classes.mst.edu/help/ssh/README) configured):
          1. Select 'SSH' from the dropdown next to the text box.
          2. Copy the URL from the text box. It should start with `git@git-classes.mst.edu`.
4. Open up a terminal (i.e., PuTTY) and navigate to the directory (using `cd`) where you want to put your copy of the repository.
5. Run `git clone <URL you copied in step 3>`.

That’s it! Now you’ve cloned your repository down, and you’re ready to get started.

**Resources:**

* [Pro Git: Cloning an Existing Repository](https://git-scm.com/book/en/v2/Git-Basics-Getting-a-Git-Repository#Cloning-an-Existing-Repository)
* [Atlassian Git Tutorials: git clone](https://www.atlassian.com/git/tutorials/setting-up-a-repository/git-clone)

### Working on the assignment

As far as working on the files go, there’s nothing special you have to do. Edit them, compile them, have fun with them. When you’re ready to commit (take a snapshot of) your code, you’ll need to interact with git.

#### Committing code (`git commit`)

1. Run `git status` to see which files are **staged**, **modified**, or **untracked**.
    - **staged** files will be included in the next commit.
    - **modified** files have been modified, but they haven’t been staged for commit.
    - **untracked** files are unfamiliar to git. They exist in the folder, but git isn’t paying any attention to changes made to them.
2. Stage all changes that you want to commit.
    - Stage files by using the `git add` command.
    - For example: `git add main.cpp`.
3. Run `git status` again to make sure that the correct files are staged for commit.
4. Run `git commit`. This will create a snapshot – recording the staged changes.
5. Enter a meaningful commit message in the text editor that opens. Then save and close it.

Now your changes have been committed!

**Resources:**

* [Pro Git: Recording Changes](https://git-scm.com/book/en/v2/Git-Basics-Recording-Changes-to-the-Repository#Checking-the-Status-of-Your-Files)
* [Atlassian Git Tutorials: Saving changes](https://www.atlassian.com/git/tutorials/saving-changes)


#### Looking through your Repository (`git status`, `git log`)

There are a couple of commands that come in handy for viewing the state of your repository:

- `git status`: View the current state of files. What has changed? What’s new in the repository?
- `git log`: Shows a timeline of commits. See a history of changes made to a repository.

**Resources:**

* [Atlassian Git Tutorials: Inspecting a repository](https://www.atlassian.com/git/tutorials/inspecting-a-repository)


#### Pushing your Commits (`git push`)

After you’ve committed something, you can push it to GitLab with git push.
We recommend pushing frequently, just for peace of mind.

1. Run `git push`
    - If you cloned over HTTPS, Git will ask for your username and password. Those are the same as the ones you use to log into GitLab.
2. Check GitLab to ensure your code looks as expected.
    - This step isn’t strictly required. However, if you’re not used to working with Git, it is in your best interest to ensure your code looks right. We’re going to grade what’s in GitLab, after all.

**Resources:**

* [Pro Git: Pushing to your Remotes](https://git-scm.com/book/en/v2/Git-Basics-Working-with-Remotes#Pushing-to-Your-Remotes)
* [Atlassian Git Tutorials: git push](https://www.atlassian.com/git/tutorials/syncing/git-push)


### Turning in your assignment (`git push`)

Once you've completed your assignment, make sure to commit and push all your work!
If your work isn't on GitLab, we can't grade it.
You should also check GitLab to make sure your submission isn't missing any files.

## Helpful Resources

These are some nice resources for learning more about Git:

* [In-browser Git Tutorial](https://try.github.io/levels/1/challenges/1)
  (**Do this one.** It's a nice tutorial.)
* [Atlassian's Git Tutorials](https://www.atlassian.com/git/tutorials)
* [Pro Git](https://www.git-scm.com/book/en/v2)
* [GitLab's Help Pages](https://git-classes.mst.edu/help/gitlab-basics/README.md)
  (You may need to log into git-classes.mst.edu for that to work.)
* [Introductory Videos from GitHub](https://git-scm.com/videos)

If you see mentions of BitBucket or GitHub around, they're just web applications like GitLab.
Same basic idea, created different people.


[^another]: Windows is also an operating system.
[^google]: Uh... Google it.
[^pun]: Pun intended.
[^2d]: Yep, it's a two-dimensional array. We use `char**` rather than something like `char[][100]` since we don't know how big the array or its subarrays will be;
the operating system works this out when it runs your program.
[^exit-joe]: By the way, \keys{\ctrl + k + x} will exit joe, should you forget to configure your editor before using git.

<!--  LocalWords:  PuTTY
 -->
