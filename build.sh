#!/bin/sh
cmd="cc -o prog -I/usr/local/include -L/usr/local/lib -lX11 main.c"

echo ${cmd}
${cmd}
