CURR_DAY="day15"
CURR_COMPILE_INPUT="$CURR_DAY/*.cpp util/*"
g++ $CURR_COMPILE_INPUT -o "bin/curr"
echo "Compilation [$CURR_DAY] Finished"
"bin/curr"