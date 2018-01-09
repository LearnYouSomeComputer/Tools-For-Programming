#!/bin/sh

echo "This book was built from commit \\href\
{https://github.com/LearnYouSomeComputer/Tools-For-Computer-Scientists/tree/$(git rev-parse HEAD)}\
{$(git rev-parse --short HEAD)}\
$(git diff-index --quiet HEAD -- || echo " with additional uncommitted changes")\
."
