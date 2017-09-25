#!/bin/bash
for dir in l*; do
	cd $dir
	echo $dir
	for file in L*; do
		mv $file l${file#L}
	done
	cd ..
done
