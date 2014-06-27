#usage: python clean_fitness.py subject_fitness.txt mlines > subject.txt

import sys

f = open(sys.argv[1],"r")
ls = f.readlines()
f.close()

num = len(ls)
i = 0
ori=[]
mut={}
while i < num:
        if i < 3:
                ori.append(ls[i].strip())
                i = i + 1
        else: 
                if(i+3) < num:
                        name = ls[i].strip()
                        mem = ls[i+1].strip()
                        tim = ls[i+2].strip()
                        tes = ls[i+3].strip()
                        if(tes == "1"):
                                mut[name]=[]
                                mut[name].append(mem)
                                mut[name].append(tim)
                        i = i + 4

f = open(sys.argv[2],"r")
ls = f.readlines()
f.close()

for l in ls:
        ll = l.strip().split(" ")
        if ll[0] in mut:
                mut[ll[0]].append(ll[1])



print "Ori 0 " +  ori[1] + " " + ori[2] 
for m in mut:
        if mut[m][2] != "0": 
                print m + " " + mut[m][2] + " " +   mut[m][0] + " " + mut[m][1] 
