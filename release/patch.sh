#!/bin/sh

home="${0%/*}"
drop="${1%/*}"
ext="${1##*.}"
version=`cat version.txt`

if [ "$home" = "$0" ]; then
    echo "Please add \"./\" to the start of the shell script path so it is actually a filepath."
    exit
fi

if [ $# -eq 0 ] || [ "$drop" = "$1" ]; then
    echo "Please pass the filepath to a Pikmin 2 USA v1.00 *.gcm file as an argument"
	echo "HINT: If the iso is in the same folder as this script, prepend \"./\" to the iso filename."
    exit
fi

# Make sure NOD Tool is allowed to execute
cd $home
chmod +x "./nodtool.linux"
if [ $? -eq 1 ]; then
    echo "chmod +x'ing nodtool.linux failed."
    exit
fi

if [ -d "$drop/root" ]; then
    echo "Please remove the existing root folder and relaunch this script."
    exit
fi

if [ -f "$1" ]; then
	echo "P2GZ v.$version Installer!"
	echo "Extracting Pikmin 2 iso..."
	cd $home
	./nodtool.linux extract "$1" "$drop/root"
	echo "iso extracted..."
	chmod -R o+rw "$drop/root"

	cp -r -f $home/Patch/root/sys $drop/root/sys/
	cp -r -f $home/Patch/root/files $drop/root/files/

	echo "Now building new iso..."
	cd $home
	./nodtool.linux makegcn "$drop/root" "$drop/p2gz-$version.$ext"
	rm -r "$drop/root"
	echo "done! :)"
	read
else
	echo "Pikmin 2 iso file not found. Please run the installer with an iso in the arguments"
	read
fi
