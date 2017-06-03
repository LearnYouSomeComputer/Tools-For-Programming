MD_PIECES = 00-FrontMatter.md	\
	01-Exploring-Text-Editors.md	\
	02-Bash-Basics.md	\
	03-Git-Basics.md	\
	04-Bash-Scripting.md	\
	05-Regular-Expressions.md	\
	06-Integrated-Development-Environments.md	\
	07-Building-with-Make.md	\
	08-Debugging-with-GDB.md	\
	09-Locating-memory-leaks-with-Memcheck.md	\
	10-Profiling.md	\
	11-Unit-testing-with-Boost-Unit-Test-Framework.md	\
	12-Using-C++11-and-the-Standard-Template-Library.md	\
	13-Graphical-User-Interfaces-with-Qt.md	\
	14-Typesetting-with-LaTeX.md

all: ${MD_PIECES}
	pandoc --from markdown+raw_tex --output cs1001_prelab.pdf --include-in-header packages.tex $^
