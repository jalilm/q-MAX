#!/bin/bash

stype=""

synthize_type () {
	if [ $1 == "null" ]; then
		stype="OVS";
	elif [ $1 == "heap" ]; then
		stype="Heap";
	elif [ $1 == "skiplist" ]; then
		stype="Skiplist";
	elif [ $1 == "qmax" ]; then
		stype="QMax";
	fi
}

for i in g*; do
	while IFS='_' read -ra NAME; do
		graph=${NAME[0]};
		dataset=${NAME[-1]};
		
		filename=${graph}"_"${dataset}".txt";
		
		synthize_type ${NAME[1]};
		
		size="";
		gamma="";
		if [[ ${#NAME[@]} == 4 ]]; then
			gamma="";
			size=${NAME[2]};
		elif [[ ${#NAME[@]} == 5 ]]; then
			size=${NAME[3]};
			gamma=${NAME[2]};
		fi
		if [[ $size == "" ]]; then
			line=${dataset}","${stype}",1000000,"${gamma}","
                        while read l; do
                                echo ${line}${l} >> ${filename};
                        done < "$i";
			line=${dataset}","${stype}",10000000,"${gamma}","
                        while read l; do
                                echo ${line}${l} >> ${filename};
                        done < "$i";

		else
			line=${dataset}","${stype}","${size}","${gamma}","
			while read l; do
				echo ${line}${l} >> ${filename};
			done < "$i";
		fi
	done <<< "$i";
done

