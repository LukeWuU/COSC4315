#!/usr/bin/env python3
from difflib import Differ
import subprocess

for i in range(1, 11):
    #!subprocess.run(f"./topword \"input=tc{i}.txt;output=ans{i}.txt\"", shell=True)
    subprocess.run(f"python3 freqnumber.py \"k=3;input=tc{'0' + str(i) if i < 10 else str(i)}.txt;output=myans{i}.txt\"", shell=True)

for i in range(1, 11):
    #!subprocess.run(f"diff -q ans{i}.txt ans{i}.txt", shell=True)
    #！subprocess.run(f"diff -q tc{'0' + str(i) if i < 10 else str(i)}.out myans{i}.txt", shell=True)
    #num = str(i) if i < 10 else str(i)
    #num = "0"+num
    subprocess.run(f"diff tc{'0' + str(i) if i < 10 else str(i)}.out myans{i}.txt", shell=True)

#!for i in range(1, 11):
#!    subprocess.run(f"python3 freqnumber.py \"k=3;input=in{'0' + str(i) if i < 10 else str(i)}.txt;output=out{i}.txt\"", shell=True)

#!for i in range(1, 11):
#!    subprocess.run(f"diff -q out{'0' + str(i) if i < 10 else str(i)}.txt out{i}.txt", shell=True)