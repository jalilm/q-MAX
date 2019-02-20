BEGIN {
	prev_key="";
	sum=0
}

$2 == prev_key {
	sum += $1
}

$2 != prev_key {
	if (prev_key != "") { 
		print sum" "prev_key;
	}
	prev_key=$2;
	sum = $1
}

END {
	print sum" "prev_key
}

