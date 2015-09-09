
pid=`ps -ef|grep moon_server|grep -v grep|awk '{print $2}'`

kill -9 $pid
