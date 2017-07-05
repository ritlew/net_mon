# net_mon
net_mon is a C program that monitors network usage and displays them on a 32x16 led matrix.

Tested and deployed on a Rasbperry Pi 2 model B under Raspbian 7 (wheezy).

## Installation
Compilation is done with the following commands

```
make lib
make install_lib
make
```

## Running
Simply:
```
make run
```

## Example
[ex]: http://i.imgur.com/AHTiwcb.jpg "Network monitor"
Loading a video on YouTube:

![Photo of monitor in action][ex]

Top half is upload, lower than the lower half.<br />
Bottom half is download.

Row 0 and 8 are scales for the graphs<br />
Row 0 upload scale: two dots, so the highest line in the upload graph is at least 256 KBps<br />
Row 1 download scale: seven dots, so the highest line in the download graph is at least 1536 KBps<br />
Formula is `256 * (number_of_dots - 1)`
