tail -n 1000 /home/rasllo/logs/temp.log > /home/rasllo/logs/temp.log.tmp
mv /home/rasllo/logs/temp.log.tmp /home/rasllo/logs/temp.log
echo "STARTED" >> /home/rasllo/logs/temp.log
while true
do
    echo "$(date) $(sudo /opt/vc/bin/vcgencmd measure_temp)" >> /home/rasllo/logs/temp.log
    sleep 600
done
