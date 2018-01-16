MD_PIECES = 00-FrontMatter.md	\
	01-Exploring-Text-Editors.md	\
	02-Bash-Basics.md	\
	03-Git-Basics.md	\
	04-Bash-Scripting.md	\
	05-Building-with-Make.md	\
	06-Debugging-with-GDB.md	\
	07-Locating-memory-leaks-with-Memcheck.md	\
	08-Unit-testing.md	\
	09-Integrated-Development-Environments.md	\
	10-Profiling.md	\
	11-Regular-Expressions.md	\
	12-Graphical-User-Interfaces-with-Qt.md	\
	13-Typesetting-with-LaTeX.md \
	14-Using-C++11-and-the-Standard-Template-Library.md	\
	15-Appendices.md

EXTENSIONS = raw_tex+fenced_code_attributes

TITLE = tools-for-computer-scientists

# Runs every time `make` is called;
# check to see whether the index has become dirty!
DIRTY := $(shell ./dirty.sh)

.PHONY: all tex dirty

all: ${TITLE}.pdf

tex: ${TITLE}.tex

quick: tex
	xelatex ${TITLE}.tex

book: ${TITLE}.pdf
	pdfbook --short-edge --letterpaper ${TITLE}.pdf
	@echo -e "\n\nDone! Be sure to print that bad-boy using short-edge duplexing."

${TITLE}.pdf: ${MD_PIECES} template.tex .commit-info.tex
	pandoc --latex-engine=xelatex --template=template.tex --from markdown+${EXTENSIONS} --output ${TITLE}.pdf ${MD_PIECES}

${TITLE}.tex: ${MD_PIECES} template.tex .commit-info.tex
	pandoc --latex-engine=xelatex --template=template.tex --standalone --from markdown+${EXTENSIONS} --output ${TITLE}.tex ${MD_PIECES}

%.pdf: 00-FrontMatter.md %*.md .commit-info.tex
	pandoc --latex-engine=xelatex --template=template.tex --from markdown+${EXTENSIONS} --output $@ $(filter %.md, $^)

# .git/index is updated every time a commit, checkout, etc. occurs.
# .dirty is updated the first time the index goes clean -> dirty.
.commit-info.tex: .dirty .git/index commit-info.sh
	./commit-info.sh > .commit-info.tex
