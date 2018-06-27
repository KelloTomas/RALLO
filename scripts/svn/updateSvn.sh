rm -f QTTcpServer
svn revert --recursive ~/Documents/QtDeviceServer/
svn status ~/Documents/QtDeviceServer/ | grep ^\? | cut -c9- | xargs -d \\n rm $
svn up ~/Documents/QtDeviceServer/
qmake -qt=qt5 ~/Documents/QtDeviceServer/Trunk/Src/ "DEFINES+=ForRaspberryPi"
make
rm -f *.o *.h *.cpp Makefile
cp QTTcpServer ~/
