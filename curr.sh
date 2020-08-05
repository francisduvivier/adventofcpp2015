CURR_DAY="day18"
CURR_COMPILE_INPUT="$CURR_DAY/*.cpp util/*"
if [ "$DEBUG" == 1 ]
then 
g++ $CURR_COMPILE_INPUT -o "bin/curr" -g
else
g++ $CURR_COMPILE_INPUT -o "bin/curr"
fi

if [ $? != 0 ]
then 
echo "Compilation failed!"
exit -1
fi

echo "Compilation [$CURR_DAY] Finished"
if [ "$DEBUG" == 1 ]
then 
gdb "bin/curr"
else
"bin/curr"
fi