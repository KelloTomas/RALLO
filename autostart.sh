#!/bin/sh

writelog() {
  now=`date`
  echo "$now $*" >> "autostart.log"
}

writelog "Starting"
while true ; do
  ./RalloApp
  writelog "Exited with status $?"
  writelog "Restarting"
done

