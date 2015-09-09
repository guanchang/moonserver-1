
pid=`ps -ef|grep moon_server|grep -v grep| awk '{print $2}'`
echo $pid
#gdb -p $pid --command=gdb.cm
gdb -p $pid
