#!/usr/bin/zsh
gource -2560x1440 -o - | ffmpeg -y -r 60 -f image2pipe -vcodec ppm -i - -vcodec libx264 -preset ultrafast -pix_fmt yuv420p -crf 20 -threads 0 -bf 0 -r 30 gource.mp4