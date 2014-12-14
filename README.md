OSC-webgate
===========

What is OSC-webgate?
--------------------
Open Sound Control (OSC) is a protocol used to control synthesizers, or other multimedia devices. The idea behind OSC-webgate is to provide the possibility to control these devices with a standard web-browser.

Technically OSC-webgate is a web-server that forwards web requests to an OSC host. Additionally it keeps track of the variables sent to the host. In this way it can keep all his clients synchronized.

OSC-webgate was designed to run on a Raspberry Pi Linux machine. But it was successfully tested on a Windows machine.

![OSC-webgate](doc/OSC-webgate.png?raw=true)

License
-------
This software is free software and licensed under GPL version 3. 

Install, Configure and Run (Linux)
----------------------------------

###Get Sources###
Get the sources from github to your local machine with git clone:
```
git clone http://github.com/bourgeoislab/OSC-webgate
```

###Compile###
Compile OSC-webgate:
```
cd OSC-webgate
make
```

###Configure I###
Edit file OSC-webgate.conf with a text editor.
For a first test I recommend changing the settings to:
```
root = "./www"
port = 8080
```
 
###Test OSC-webgate###
Run OSC-webgate:
```
./OSC-webgate
```
Start a web browser on a machine in the same network and enter
```
http://ip:8080/
```
where ip is the IP address of the machine OSC-webgate is running on. You should see a simple page with some controls.
 
###Configure II###
After a successful test you may edit the default web-pages located in the www sub-directory. Also consider moving the www directory to another location. /var/www/ is widely used as a web-server root. 
Also look at the file OSC-webgate.conf and read all configuration possibilities there.
 
Running OSC-webgate as a Service
--------------------------------
The best way to use OSC-webgate is to run it as a service (daemon). Service concepts may differ depending on the Linux distribution. The description here is for Arch Linux using systemd.
 
A setup file setup_service.sh is added in the distribution. Running this script will:
- copy OSC-webgate to /usr/local/bin
- create directory /etc/OSC-webgate
- copy OSC-webgate.conf to /etc/OSC-webgate
- copy OSC-webgate.service to /usr/lib/systemd/system/ 
- create directory /var/www
- copy content of www to /var/www

```
chmod 744 setup_service.sh
sudo ./setup_service.sh
```

Note that the path of the configuration file is now:
```
/etc/OSC-webgate/OSC-webgate.conf
```
and the path of the www root folder
```
/var/www
```
 
At this point you may delete the OSC-webgate directory created with the git clone command.

**Start OSC-webgate service:**
```
sudo systemctl start OSC-webgate
```
 
**Stop OSC-webgate service:**
```
sudo systemctl stop OSC-webgate
```
 
**Add OSC-webgate service to the start-up procedure:**
```
sudo systemctl enable OSC-webgate
```
 
**Remove OSC-webgate service from start-up procedure:**
```
sudo systemctl disable OSC-webgate
```
 
Communication Protocol
----------------------
The communication between OSC-webgate and the clients (web-browser, smartphone or even own  programs) is based on standard HTTP GET and/or POST requests. A detailed description can be found in the CGI module description.
 
Extensions
----------
Say you added some controls (potentiometers) and also a display to your Raspberry Pi and want to use them to control some parameters on the OSC host. There are mainly two  possibilities to achieve this.
 
###1. Web-Server Communication###
Create a program that can send HTTP requests and use the CGI API described in the CGI module documentation. Using this API you can read and write variable values.
 
###2. Direct Integration###
Extend the file datapooluser.c to your needs. The API there allows you to directly read and write variable values.
 
Credits
-------
 - **Mongoose web-server:** http://cesanta.com/mongoose.shtml
 - **OSC client by Matt Wright:** http://archive.cnmat.berkeley.edu/OpenSoundControl/src/
 - **jQuery:** http://jquery.com/
 
Release Notes
-------------
**[v1.0.0]**
- [new] Initial release.
 
Information
-----------
**Author:** Frédéric Bourgeois <bourgeoislab@gmail.com>

**Project homepage:** http://bourgeoislab.wordpress.com/osc-webgate

**Latest release:** http://github.com/bourgeoislab/osc-webgate/releases

**Latest sources:** http://github.com/bourgeoislab/osc-webgate
