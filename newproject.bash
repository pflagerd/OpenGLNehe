#!/bin/bash
file_ext=$(ls *.cpp)

file=${file_ext%.cpp}

rm -f $file".dsw" $file".dsp"

sed -e s/lesson01/$file/ ../lesson01/lesson01.vcxproj >$file".vcxproj"
sed -e s/lesson01/$file/ ../lesson01/lesson01.vcxproj.filters >$file".vcxproj.filters"
sed -e s/lesson01/$file/ ../lesson01/lesson01.vcxproj.user >$file".vcxproj.user"

echo $file