CC=gcc                  
CFLAGS=-I.
DEPS =                  

all: compile 
	gcc scan-button.o sane-lib.o -o scan-button -lsane 
	
compile: clean 
	gcc -I . -c scan-button.cpp sane-lib.cpp

clean:
	rm -rf *.o
	rm -rf scan-button

install:
	cp scan-button -f /usr/bin/
	cp scan-button.service -f /etc/systemd/system/
	systemctl daemon-reload
	mkdir /var/scan

uninstall:
	rm /usr/bin/scan-button
	rm /etc/systemd/system/scan-button.service
	systemctl daemon-reload
