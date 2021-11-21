# scan-button
Linux daemon that monitors the pressing of buttons on the scanner panel and starts scanning. Testing on Canon PIXMA MP250.

For build and install: 

```
sudo apt install sane libsane-dev
make
make install
systemctl enable scan-button
systemctl start scan-button
```
