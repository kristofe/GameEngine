#! /bin/bash
for file in *\ *;
do
	echo "${file// /}"
	mv "$file" "${file// /}"
done
