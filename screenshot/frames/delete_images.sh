#!/bin/bash


val=0
for f in images/*.png ; do
    if ($val = 0 ); then
    rm -r cropped
    fi
    val='expr $val + 1'
    if ($val = 10 ) then
    val=0
    fi

 done