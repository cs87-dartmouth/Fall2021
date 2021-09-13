
@page getting-started Getting started

@tableofcontents

This guide will take you through the main steps required to start working on CS 87/287 programming assignments. We will be using [GitHub Classroom](https://classroom.github.com) along with Github Actions to provide Continuous Integration builds within a modern development workflow.

@m_class{m-block m-dim}

If you are not enrolled in the class, you can still download the starter code and work through assignments, just [jump here](#following-along) and ignore all the GitHub classroom stuff.

At any point, please feel free to reach out to the TA if anything is unclear.

Obtaining the base code
=======================

The base code for this class is provided as a _git repository_. Git is a version control system that allows you to keep track of changes in your code over time, collaborate with other people, track down bugs and more. git is the most widely used version control system today, and this is a good opportunity to learn it if you haven't already.

In this class, you will use git to obtain new and updated versions of the base code. During assignments, you will edit your own local copy of the base code. If we hand out new assignments or add improvements to the base code, git allows you to obtain the updated code and merge it with your local copy without losing your changes.

Before proceeding to the rest of this section, you need to install git to get the base code.

Installing git
--------------

If you are using macOS or Windows, then we recommend you install [GitHub Desktop](https://desktop.github.com/), which comes with git as well as a nice graphical front-end (another good client with a few more features is [Source Tree](http://www.sourcetreeapp.com/)). If you are using Linux, then you probably already know what you are doing: install git or a git client with your chosen package manager.

@m_class{m-block m-primary m-dim}

Git is a versatile tool and allows you to achieve more than just obtaining new code from us. If you want to know more, a few external resources we can recommend are [GitHub's Git Guides](https://github.com/git-guides/), [try git](https://try.github.io/levels/1/challenges/1), [git-the simple guide](http://rogerdudler.github.io/git-guide/), the [git immersion tutorial](http://gitimmersion.com/index.html), and the extensive free [Pro Git](http://git-scm.com/book/en/v2) ebook.

Accepting the GitHub Classroom assignment
-----------------------------------------

This will give you access to the base code and create a private repository where your code and reports will live.

1. If you do not already have a [GitHub](https://github.com) account, you will need to create one using your \@dartmouth.edu email address. Then, [log in](https://github.com/login).
2. Open the link starting with `https://classroom.github.com/a/...` that was provided to you.
3. Accept the permissions required by GitHub Classroom, then click "Accept this assignment".
4. You now have a private repository that you will use throughout the quarter to implement your programming assignments, as well as to write each assignment's report. Note that this directory is empty. The following steps explain how to get the base code for the assignements.


@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-l-6 m-col-m-10 m-push-m-1 m-push-l-0}
\image html images/accept-assignment.png
@m_enddiv

@m_div{m-col-l-6 m-col-m-10 m-push-m-1 m-push-l-0}
\image html images/assignment-accepted.png
@m_enddiv

@endparblock





Cloning the repository to your local machine
--------------------------------------------

@m_div{m-col-t-6 m-col-s-5 m-col-m-5 m-right-t}
@image html images/download-code.png
@m_enddiv

1. Visit your personal assignment repository on GitHub and click the green "Code" button
2. Clone the repository by either:
   * Selecting the option to "Open with GitHub Desktop". This should open up a dialog allowing you to select the local path where you'd like to download the code. Do that, and then click the "clone" button.
   * Navigate to a directory where you'd like your code to reside, then running the command `git clone <your repo URL>`.

This will clone an empty repository. To get the base code for the assignments, you will need to do the following on the commandline (within your local repository directory)
1. **Only once**: add the base repository as a new "remote":  
`git remote add upstream https://github.com/cs87-dartmouth/Fall2021.git`
2. **Pull the code from the base repository**: `git pull upstream main`

We will remind you to run the second command periodically to pull updates/changes to the base code (e.g. for bug fixes, or to start on the next assignment.

@m_class{m-note m-danger}

@par Note
    **Since your repository will contain assignment solutions, it is (obviously) not OK to make them available online in any form. In particular, don't publicly fork the `darts` project!**


<a name="following-along"></a>
@m_class{m-block m-primary m-dim}

@par Not enrolled, but want to follow along?
    You can simply clone the base repository: `git clone https://github.com/cs87-dartmouth/Fall2021.git`


Working on the assignments
-----------------------------

Your repository is now setup, and you can start working on the homeworks. Note that **all assignments** will use the same private repository that you just created.

While implementing your solutions, don't forget to commit, push, and check the build status periodically.

Every assignment requires writing a small **report**. A template file is provided in `reports/assignment#/report.hml`. Please add your answers in place, and commit them along with the rest of your code.


Setting up a C++ compiler and building the base code
====================================================

We recommend you edit the code using [VS Code](https://code.visualstudio.com/), which is a free and powerful code editor that works on Windows, Linux, and macOS. The rest of this guide will assume you're using VS Code.

Make sure that VS Code is extracted into a directory that is writable, for example your home directory; otherwise, you won't be able to install plugins (VS Code will warn you when this happens).

You will also need to do some platform specific setup steps to get code compiled:

macOS
-----

To compile C++ on macOS, you will need to install [XCode](https://developer.apple.com/xcode/), which contains compilers and developer tools provided by Apple. If you don't have XCode installed already, you can find and download it from the app store.

You may need to install CMake too, if the VS Code plugin asks you for it. You can get it [from the CMake website](https://cmake.org/download/) (get the *.dmg installer).

Windows / Visual Studio Community
---------------------------------

To compile things on Windows, you will need to install a version of Visual Studio. We recommend [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) and a reasonably recent version of  [CMake](http://www.cmake.org/download/).

Linux
-----

If you're using linux, you most likely already know what to do - just install a C++ compiler (gcc or clang), and cmake using your package manager.

Setting up VS Code
------------------

Once you have followed the steps above and have installed the build tools for your operating system, you can go ahead and configure VS Code to compile your code. Note that the screenshots below may look slightly different than what you see depending on which version of VS Code you have.

@m_div{m-col-t-7 m-col-s-5 m-col-m-4 m-right-t}
@image html images/plugins.png
@m_enddiv

First, launch VS Code and go to the extensions tab. Install **C/C++** and **CMake Tools**:

@m_div{m-clearfix-t} @m_enddiv
@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

@m_div{m-col-s-7 m-col-m-5 m-right-s}
@image html images/c++-plugin.png
@m_enddiv

VS Code will ask you to reload to finish installing the plugins. Do that.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Open the Command Palette (⇧⌘P) and type `user settings`, select `Preferences: Open User Settings`. In the search settings field, type `cmake build directory`. Enter the string: `${workspaceFolder}/build/${buildType}` as shown in the screenshot. This will cause CMake to build your code in a different directory depending on whether you want the Debug or Release (optimized) build.

@m_class{m-row}

@parblock

@m_div{m-col-m-10 m-push-m-1 m-col-l-8 m-push-l-2}
@image html images/build-directory.png
@m_enddiv

@endparblock

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

@m_div{m-col-s-4 m-col-m-3 m-left-s}
@image html images/open-dialog.png
@m_enddiv

Then, go to File->Open and open the folder where you downloaded the darts basecode. This will bring all source files into your editor. On the left, you can explore the file system and open the individual source files.

@m_div{m-col-t-10 m-col-s-6 m-center-t m-right-s}
@image html images/configure.png
@m_enddiv

CMake will now ask you to configure your project. Do that.

@m_div{m-clearfix-s} @m_enddiv

@m_div{m-col-t-12 m-col-s-8 m-col-m-7 m-center-t m-right-s}
@image html images/kit-selection.png
@m_enddiv

Depending on your platform, it will ask you to select a "kit" - in other words, it wants to know what compiler you would like to use. Click "Scan for Kits". This brings up a small dialog at the top of your screen. The contents of this dialog depend on your platform. For macOS, you would want to select Clang; for Linux, you want GCC, and for Windows, you want Visual Studio amd64.

@m_div{m-col-t-12 m-col-s-6 m-col-m-5 m-center-t m-right-s}
@image html images/build-dialog.png
@m_enddiv

Navigate to your CMake tab (the big triangle on the left) and first configure your project (this might be a file with a rightward arrow in it, depending on your version) and then build your project (a downward arrow).

👏 Congratulations! If everything has gone well, you have just built the basecode for this assignment.

You can see the output of the build process at the bottom of the screen (in the Output panel). Things should compile fine; if not, and you think this is a problem with the base code, contact us immediately or come to office hours.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Running the executable
======================

If the previous part has gone well, you have built an executable you can run on your machine.

In the bottom panel, there is a "Terminal" tab. Open it, and type in the path to `build/darts_tutorial0`. This will run the program you just compiled. If all goes well, you should see output that looks something like this:

@m_div{m-col-t-10 m-col-s-8 m-center-t}
@image html images/run-output.png
@m_enddiv

@m_div{m-clearfix-s} @m_enddiv

Documentation
=============

Large parts of darts are documented using [doxygen](http://www.doxygen.nl/), which is a tool for automatically generating HTML documentation from specially formatted source code comments. In fact, this entire course website is generated from this documentation: it resides within the `documentation` subdirectory and can be built using CMake.

Wrapping Up
===========

If all has gone well, you are now ready to edit, compile and run your code.

You can start editing now – whenever you're satisfied with your changes, head over to the CMake tab and build your program (hit the hammer), and run it using the terminal.

You can build your code in _Debug_ mode or _Release_ mode. Debug mode adds more debugging information to your code (for when you want to troubleshoot problems), but it is much, much slower than Release mode (with a ray tracer, it is not uncommon for Debug to be 10 to 100X slower than Release). Your code will build in debug mode by default. You can switch to Release mode by clicking Debug in the bottom of the window:

@m_div{m-col-t-10 m-col-s-9 m-center-t}
@image html images/build-mode.png
@m_enddiv

This will open a dialog at the top of the window. Click on Release if you want a fast program:

@m_div{m-col-t-10 m-col-s-9 m-center-t}
@image html images/mode-selection.png
@m_enddiv


Continuous integration
======================

Once you pulled the base code into your repository, this also automatically sets up continuous integration using "GitHub Actions".

Continuous integration with GitHub Actions
---------------------------------------------

A continuous integration servers will pull your code and attempt to build it every time you push to your repository. This will help ensure that your code always builds on macOS, Windows and Linux, even if you work on another platform.

To check the latest build status, visit the "Actions" tab in your repository. 

\image html images/github-actions1.png

\image html images/github-actions2.png

\image html images/github-actions3.png

Build badge
-----------

GitHub provides a "badge" which allows you to quickly see the build status of your project from its GitHub page. 
To display the badge, you need to edit the following two lines in your repository's `README.md` file:

`![MacOS and Ubuntu](https://github.com/cs87-dartmouth/<YOUR REPOSITORY NAME>/actions/workflows/mac-linux.yml/badge.svg)`

`![Windows](https://github.com/cs87-dartmouth/<YOUR REPOSITORY NAME>/actions/workflows/windows.yml/badge.svg)`

\image html images/edit-readme1.png

\image html images/edit-readme2.png


Submitting on Canvas {#submitting-on-canvas}
====================

Before submitting, please double check that:

- All your changes have been committed and pushed to GitHub,
- The continuous integration build passes,
- Your report is written in `reports/assignment#/report.hml`, including all renders that you want to show.

The Canvas submission simply consists of a link to a specific state of your repository. With git, each state is uniquely identified by an identifier (SHA). You can view this identifier for each commit using the `git log` command.  
For convenience, we ask you to submit a direct link to the state of your report **at a specific commit**. To do so,

1. Open your repository on the GitHub web interface. 
2. Navigate to your report: `reports` > `assignment<assignment number>` > `report.html`
3. Press the `y` key on your keyboard. Note that the URL now has the form:  
`https://github.com/cs87-dartmouth/cs87-fall-2021-<username>/blob/<the commit identifier>/reports/assignment<assignment-number>/report.html`
1. Copy this URL and submit it on the Canvas assignment page.

See the animated screencap below:

![Screencap of the submission link process](images/submit-assignment.gif)



@m_class{m-note m-danger}

@parblock

**Warning**: Note that the following **cannot** be accepted, as they do not uniquely identify a specific state:

- A link to a branch or tag: `https://github.com/cs87-dartmouth/cs87-fall-2021-<username>/blob/master/reports/assignment1/report.html`

- A link to the base repository: `https://github.com/cs87-dartmouth/Fall2021/blob/17f31bff65f3476ec3f94685b68ddafeca92b431/reports/assignment1/report.html`

- A link to a specific state, but that is later overwritten and so results in a 404. You need to be extra-careful to **never rewrite the repository's history** after a submission, at the risk of loosing your submission and getting zero points.

@endparblock

---

👏 You're all set! You should now be ready to work on [Assignment 0](@ref assignment0)!
