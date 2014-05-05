#!/bin/bash

choice=$(zenity --entry --title "Menu" --text "What do you want to do?" "Recognize somebody" "Import a database of persons" "Recalculate the eigenfaces")

case "$choice" in
"Recalculate the eigenfaces")
	srcDirectory=$(zenity --file-selection --filename="./*" --directory --title="Select a directory with a database of faces")
	if [ $? = 0 ]; then
		targetDirectory=$(zenity --file-selection --filename="./*" --directory --title="Select a directory where the eigenfaces should be saved")
		if [ $? = 0 ]; then
			kernelDirectory=$(zenity --file-selection --filename="./*" --directory --title="Select the directory where the OpenCL kernels are")
			if [ $? = 0 ]; then
				echo img2eigvec $kernelDirectory $targetDirectory $srcDirectory
				img2eigvec $kernelDirectory $targetDirectory $srcDirectory
			fi
		fi
	fi
	;;
"Import a database of persons")
	srcDirectory=$(zenity --file-selection --filename="./*" --directory --title="Select a directory with your database of faces")
	if [ $? = 0 ]; then
	echo $srcDirectory
	#...
	fi
	;;
"Recognize somebody")
	srcImage=$(zenity --file-selection --filename="./*" --title="Select a .jpg or .jpeg file for the program to analyse it")
	if [ $? = 0 ]; then
	echo $srcImage
	#...
	fi
	;;
esac
