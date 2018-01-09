#!/bin/sh

# The .dirty file is edge-triggered:
# it will have its timestamp updated each time
# the index goes clean -> dirty.

# The .update-dirty file is level-triggered; it is created
# after .dirty is updated and persists until the index is clean
# so that we know when to next update .dirty.

if [ ! -f .dirty ]; then
	touch .dirty
fi

if git diff-index --quiet HEAD --; then
	rm -f .update-dirty
elif [ ! -f .update-dirty ]; then
	touch .dirty
	touch .update-dirty
fi
