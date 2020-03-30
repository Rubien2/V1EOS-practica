#!/bin/bash
if [ $1 = "" ]; then
	echo $0 needs 2 arguments
	exit 0
if [ $2 = "" ]; then
	echo $0 needs 2 arguments
	exit 0
[[ -e $1 ]] && echo $1 already exists, aborting && exit
cp $1 $2