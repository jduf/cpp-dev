#!/bin/bash

for f in $(find . -name makefile)
do
	FILE=${f::-8}
	cp -r ../build $FILE
done
