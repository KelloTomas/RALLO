# Rasllo

## Ubuntu
### Add/Remove user
```
adduser rasllo
usermod -aG sudo rasllo
sudo visudo
rasllo ALL=(ALL) NOPASSWD: ALL
```
### Change hostname
```
\etc\hostname
\etc\hosts
```
### Remove splash screen
```
sudo nano /boot/cmdline.txt
```
- add "logo.nologo"
```
sudo nano /boot/config.txt
```
 - add "disable_splash=1"
### Update device
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get clean
sudo apt-get dist-upgrade
```
### Xserver Xorg
```
sudo apt-get install --no-install-recommends xserver-xorg xinit x11-xserver-utils
```
### Libs for running QT application
```
sudo apt-get install libqt5serialport5-dev
```
### VNC Server
```
sudo apt-get install x11vnc
x11vnc -storepasswd
echo "alias vnc='x11vnc -rfbauth .vnc/passwd -q -display :0 &'" | sudo tee -a ~/.bash_aliases
```
### Startup programs
```
sudo nano /etc/rc.local 

/home/rasllo/scripts/temp.sh &
sudo xinit /home/rasllo/QTTcpServer :0 -- -v &
```
### Set timezone
```
sudo dpkg-reconfigure tzdata
```
### Test Serial read

```
sudo apt install minicom
sudo minicom -s
```
 - Serial device /dev/serial0
 - 9600 8N1
 - CLOSE: CTRL+A, X
