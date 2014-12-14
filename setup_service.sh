#!/bin/bash

echo "Setting up OSC-webgate"

echo " - copy \"OSC-webgate\" to \"/usr/local/bin\""
cp OSC-webgate /usr/local/bin

echo " - create directory \"/etc/OSC-webgate\""
mkdir -p -m 0755 /etc/OSC-webgate
echo " - copy \"OSC-webgate.conf\" to \"/etc/OSC-webgate\""
cp OSC-webgate.conf /etc/OSC-webgate

echo " - copy \"OSC-webgate.service\" to \"/usr/lib/systemd/system/\"" 
cp OSC-webgate.service /usr/lib/systemd/system/

echo " - create directory \"/var/www\""
mkdir -p -m 0777 /var/www
echo " - copy content of \"www\" to \"/var/www\""
cp www/* /var/www
