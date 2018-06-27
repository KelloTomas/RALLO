tail -n 1000 /home/pi/logs/cpuUsage.log > /home/pi/logs/cpuUsage.log.tmp
mv /home/pi/logs/cpuUsage.log.tmp /home/pi/logs/cpuUsage.log
rm /home/pi/logs/cpuUsage.log.tmp
printf "\n\n\n            DATE               PID  CPU MEM  CMD\n" >> /home/pi/logs/cpuUsage.log
while true
do
    echo "$(date) $(ps -C QTTcpServer -o pid,%cpu,%mem,cmd | grep QTT)" >> /home/pi/logs/cpuUsage.log
    sleep 600
done
