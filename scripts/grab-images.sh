#!/bin/bash

let count=0

for f in *_files/*.jpg; do
    cp "$f" images/image-$count.jpg
    #jpegtopnm "$f" | pamscale -width 220 -height 220 | pnmtojpeg > images/image-$count.jpg
    let count=count+1
done
