#!/bin/bash

# Die on any errors
set -e

if [[ `whoami` != "root" ]]
then
  echo "Script must be run as root (Skript musi byt spusteny ako admin) 'sudo'"
  exit 1
fi

echo -n "Enable DHCP (Povolit DHCP)? [y/n]"
read DHCP

if [[ $DHCP == "y" ]]
then
  echo "Setting DHCP (Nastavujem DHCP)"
  sudo cp dhcpcd.conf /etc/dhcpcd.conf
else
  echo -n "Set IP: [10.10.10.10/24]"
  read NEW_IP

  echo -n "Default Gateway (Predvolena brana): [10.10.10.1]"
  read NEW_GW

  echo -n "DNS server: [10.0.0.1]"
  read NEW_DNS

  # Update IPs
  echo "hostname" > /etc/dhcpcd.conf
  echo "interface eth0" >> /etc/dhcpcd.conf
  echo "static ip_address=$NEW_IP" >> /etc/dhcpcd.conf
  echo "static routers=$NEW_GW" >> /etc/dhcpcd.conf
  echo "static domain_name_servers=$NEW_DNS" >> /etc/dhcpcd.conf
fi

echo "Want reboot (restartovat zariadenie)? [y/n]"
read REBOOT

if [[ $REBOOT == "y" ]]
then
  sudo reboot
fi
