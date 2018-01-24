#!/bin/bash

RED='\033[0;31m' #Change the color of the title
NC='\033[0m' #No Color

echo -e ${RED} 1. User Information ${NC}
id joyce 
echo 
echo -e ${RED} 2. Operating System Type/Brand ${NC}
echo -e ${RED} 3. OS Distribution ${NC}
echo -e ${RED} 4. OS Version ${NC}
cat /etc/lsb-release 
echo 
echo -e ${RED} 5. Kernel Version ${NC}
uname -r
echo 
echo -e ${RED} 6. Kernel gcc version build ${NC}
ls -l /usr/bin/gcc
echo 
echo -e ${RED} 7. Kernel build time ${NC}
uname -v
echo 
echo -e ${RED} 8. System Architecture ${NC}
top -bn1
echo 
echo -e ${RED} 9. Information on File System Memory ${NC}
df -h
free -m 
