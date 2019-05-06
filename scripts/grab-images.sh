#!/bin/bash

let count=0

for f in *_files/*.jpg; do
    #cp "$f" image-$count.jpg
    jpegtopnm "$f" | pamscale -width=220 -height=220 | pnmtojpeg > image-$count.jpg
    let count=count+1
done
