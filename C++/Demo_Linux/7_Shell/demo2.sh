#! /bin/sh

echo "###########################"
if [ -f demo2.sh ]; then
	echo "Find Target!"
else
	echo "Miss Target!"
fi
echo "###########################"
i=0
while [ "$i" != "5" ];
do
	echo "$i"
	i=`expr $i + 1`
done
echo "###########################"