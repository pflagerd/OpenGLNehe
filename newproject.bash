#!/bin/bash
file_ext=$(ls *.cpp)

file=${file_ext%.cpp}

rm -f $file".dsw" $file".dsp"

sed -e s/lesson06/$file/ ../lesson06/lesson06.vcxproj >$file".vcxproj"
sed -e s/lesson06/$file/ ../lesson06/lesson06.vcxproj.filters >$file".vcxproj.filters"
sed -e s/lesson06/$file/ ../lesson06/lesson06.vcxproj.user >$file".vcxproj.user"

echo $file