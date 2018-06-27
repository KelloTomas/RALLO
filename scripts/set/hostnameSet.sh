#!/bin/bash

# Die on any errors
set -e

if [[ `whoami` != "root" ]]
then
  echo "Script must be run as root (Skript musi byt spusteny ako admin) 'sudo'"
  exit 1
fi
echo -n "Choose a hostname (Zadajte meno zariadenia): "
read NEW_HOSTNAME

# Update hostname
echo "$NEW_HOSTNAME" > /etc/hostname
sed -i "s/$HOSTNAME/$NEW_HOSTNAME/" /etc/hosts

echo "Want reboot (restartovat zariadenie)? [y/n]"
read REBOOT
if [[ $REBOOT == "y" ]]
then
  sudo reboot
fi
