#!/bin/bash
rm a.out;g++ -g -ggdb -Wall -pg doubleyou.cpp hsfeq.cpp micro_pitchshift.cpp tat.cpp ; rm /home/cbav/Music/5_o.pcm ; time ./a.out ;md5sum /home/cbav/Music/5_o.pcm ; md5sum /home/cbav/Music/5_o.pcm.6_27
