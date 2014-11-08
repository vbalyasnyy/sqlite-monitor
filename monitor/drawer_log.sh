SCRIPT_NAME=$0

SO_NAME="sqlite3.so"

function usage {
	echo "Usage: ${SCRIPT_NAME} log_file"
	exit 1;
}

# Check number of arguments
if [ "x$#" == "x0" ]; then
	LOG_FILE="/tmp/sqlite.log"
elif [ "x$#" == "x1" ]; then
	if [ ! -s $1 ]; then
		echo "ERROR: File is not exist or empty: $1"
		usage
	fi
	LOG_FILE=$1
else
	usage
fi

#clean temp file
rm ${LOG_FILE}.*

#create temp files for every PID
#create gnuplot command to draw graphic
for PID in `cat ${LOG_FILE} | awk '{print $1}'| sort | uniq`
do
#	echo PID=$PID
	LOG_FILE_BY_PID=${LOG_FILE}.${PID}
	cat ${LOG_FILE} | awk -v pid="${PID}" '{if($1==pid) print $0; else print $1}' > ${LOG_FILE_BY_PID}
	if [ -n "$GNUPLOT_CMD" ]; then
		GNUPLOT_CMD="${GNUPLOT_CMD}, \"${LOG_FILE_BY_PID}\" using 0:2:xtic(3) t \"PID=${PID}\" with boxes "
	else
		GNUPLOT_CMD=" \"${LOG_FILE_BY_PID}\" using 0:2:xtic(3) t \"PID=${PID}\" with boxes"
	fi
done
GNUPLOT_CMD="plot $GNUPLOT_CMD"

#size of PNG FILE
LOG_FILE_SIZE=`cat ${LOG_FILE} | wc -l`
let "PNG_WIDTH=$LOG_FILE_SIZE*30"
if [ $PNG_WIDTH -le 400 ]; then
	PNG_WIDTH=400;
fi
#echo PNG_WIDTH=${PNG_WIDTH}

gnuplot -e "\
set term png size ${PNG_WIDTH},1000 ;\
set title \"${SO_NAME}\";\
set xlabel \"Function\";\
set ylabel \"Time\";\
set output \"sqlite.png\"; \
set xtics rotate by 270;\
set grid;\
set style fill solid 0.25 border;\
set boxwidth 1;\
set timestamp;\
${GNUPLOT_CMD};\
quit;
"


