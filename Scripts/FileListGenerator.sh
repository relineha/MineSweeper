#!/bin/bash

#This script generates a file list given an input directory. It 
#takes all of the files in that directory and puts them into the fileList
DIR=$1
echo "Directory targeted: ${DIR}"

#Get the number of files in this directory
NUMFILES=`ls -l $DIR | wc -l`
echo "Number of files (+1) in directory: ${NUMFILES}"


COUNTER=0
for FILE in $DIR/*_.bmp
do
    echo "Inserting ${FILE} into filelist."
    if [ $COUNTER == 0 ]
    then
	echo "COUNTER IS ZERO"
	echo "${FILE}" > ../Config/PicFileList.config

    elif [ $COUNTER -lt `expr $NUMFILES - 2` ]
    then
	echo "${FILE}" >> ../Config/PicFileList.config

    else
	echo -n "${FILE}" >> ../Config/PicFileList.config
    fi
    COUNTER=`expr $COUNTER + 1`
done


