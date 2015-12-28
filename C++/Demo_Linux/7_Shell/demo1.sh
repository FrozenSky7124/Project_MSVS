#! /bin/sh

echo "###########################"
read a
read b
echo a=$a b=$b
if (test "$a" = "$b")
then
	echo a=b
else
	echo a!=b
fi
echo "###########################"
DATE=`date`
echo $DATE
echo "###########################"
echo $#
echo $1
echo $*
echo "###########################"