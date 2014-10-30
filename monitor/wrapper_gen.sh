SCRIPT_NAME=$0

function usage {
	echo "Usage: ${SCRIPT_NAME} api_file.txt [run_time|link_time]"
	exit 1;
}

function parse_fn {
	FN_NAME_START=$(echo "$1" | awk 'match($0,"([0-9]|[A-Z]|[a-z]|_)* *\\("){print RSTART-1}')
	FN_NAME_LENGTH=$(echo "$1" | awk 'match($0,"([0-9]|[A-Z]|[a-z]|_)* *\\("){print RLENGTH-1}')

	FN_OUTPUT=${1::$FN_NAME_START}
	FN_OUTPUT=$(echo "$FN_OUTPUT" | sed "s/ *$//g" | sed "s/^ *//g")

	FN_NAME=${1:$FN_NAME_START:$FN_NAME_LENGTH}
	FN_INPUT=${1:$FN_NAME_START+$FN_NAME_LENGTH}
	FN_INPUT=$(echo "$FN_INPUT" | sed "s/;//g" | sed "s/(//g" | sed "s/)//g" | sed 's/ *$//' | sed 's/^ *//')

	if [ "x$FN_INPUT" == "xvoid" ]; then
		FN_INPUT_VAR_NAME=""
		FN_INPUT_VAR_TYPE="void"
	else
		FN_INPUT_VAR_NAME=$(echo $FN_INPUT | awk -F "," '{
						for(i=1;i<=NF;i++) {
							match($i, /([a-z]|[A-Z]|[0-9])*( )*$/)
							{printf "%s", substr( $i, RSTART, RLENGTH )}
							if(i!=NF)
								{printf ", "}
						};
					      }')
		FN_INPUT_VAR_TYPE=$(echo $FN_INPUT | awk -F "," '{
						for(i=1;i<=NF;i++) {
							match($i, /([a-z]|[A-Z]|[0-9])*( )*$/)
							{printf "%s", substr( $i, 1, RSTART-1 )}
							if(i!=NF)
								{printf ", "}
						};
					      }')
	fi
}

# Check number of arguments
if [ "x$#" != "x2" ]; then
	usage
fi

#Check first argument
if [ ! -s $1 ]; then
	echo "ERROR: File is not exist or empty: $1"
	usage
fi

#Check second argument
if [ "x$2" == "xrun_time" ]; then
	WRAP_FL_HEADER="#define _GNU_SOURCE\n#include <stdio.h>\n#include <dlfcn.h>\n\n"
else
	if [ "x$2" == "xlink_time" ]; then
		WRAP_FL_HEADER="#include <stdio.h>\n\n"
	else
		echo "ERROR: Bad time wrapping: $2."
		usage
	fi
fi

WRAP_FL_NAME="fwrap.c"

echo "" > ${WRAP_FL_NAME}
echo "#define _GNU_SOURCE" >> ${WRAP_FL_NAME}
echo "#include <stdio.h>" >> ${WRAP_FL_NAME}
echo "#include <dlfcn.h>" >> ${WRAP_FL_NAME}
echo "#include <stdarg.h>" >> ${WRAP_FL_NAME}
echo "#include \"monitor.h\"" >> ${WRAP_FL_NAME}
echo "#include \"sqlite.h\"" >> ${WRAP_FL_NAME}
echo "" >> ${WRAP_FL_NAME}

while read line
do
	parse_fn "${line}"

#	echo "($FN_OUTPUT) ($FN_NAME) ($FN_INPUT)" >> ${WRAP_FL_NAME}

	echo "$FN_OUTPUT $FN_NAME($FN_INPUT) {"  >> ${WRAP_FL_NAME}
	if [ "x$FN_OUTPUT" != "xvoid" ]; then
		echo "	$FN_OUTPUT ret;" >> ${WRAP_FL_NAME}
	fi
	echo "	MON_MSG_T(0, \"$FN_NAME start\");" >> ${WRAP_FL_NAME}
	echo "	$FN_OUTPUT (*real_$FN_NAME)($FN_INPUT_VAR_TYPE) = dlsym(RTLD_NEXT, \"$FN_NAME\");" >> ${WRAP_FL_NAME}
	if [ "x$FN_OUTPUT" != "xvoid" ]; then
		echo "	ret = real_$FN_NAME($FN_INPUT_VAR_NAME);" >> ${WRAP_FL_NAME}
	else
		echo "	real_$FN_NAME($FN_INPUT_VAR_NAME);" >> ${WRAP_FL_NAME}
	fi
	echo "	MON_MSG_T(1, \"$FN_NAME stop\");" >> ${WRAP_FL_NAME}
	if [ "x$FN_OUTPUT" != "xvoid" ]; then
		echo "	return ret;" >> ${WRAP_FL_NAME}
	fi
	echo "}" >> ${WRAP_FL_NAME}
	echo "" >> ${WRAP_FL_NAME}
done < $1;

