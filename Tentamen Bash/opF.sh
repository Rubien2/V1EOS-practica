#!/bin/bash
if [ $1 = "" ]; then
	echo $0 needs 2 arguments
	exit 0
fi
if [ $2 = "" ]; then
	echo $0 needs 2 arguments
	exit 0
fi
if [ $2 ]; then
	echo $2 already exists; overwrite(yes/no)?\c
	if [ ! yes ]; then
		echo Copy not performed
	fi
fi
cp -w $1 $2
if [ -d $2 ]; then
	to="$2/$(basename $1)"
	if [ $to ]; then
		echo Already exists
	fi
fi