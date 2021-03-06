# GImalloc
Genetic Improvement for dlmalloc

This repository consists of all the scripts and subjects for Deep Parameter Optimisation. 
The paper was first published on GECCO'15.

The structure of the repository is:
Milu: open source C mutation testing tool developed by Yue. The tool is available at https://github.com/yuejia/Milu.

Experiments: scripts for conducting sensitivity analysis on First Order Mutants generated by Milu.

Subjects: slightly modified and configured subjects used in this work.

killps: contains a script to kill a process that exceeds the execution time budget.

locate: scripts to find out suiteble locations to expose deep parameter, once the sensitivity experiments are done.

memory: a tool that measures a program's cpu time and memory consumption by instrumenting malloc. It's own repository is at https://github.com/FanWuUCL/memory.

myga: implementation of NSGAII for optimising the values of deep parameters.

A typical deep parameter optimisation process should be:
1) Prepare a subject,including test cases, settings and linking to instrumented malloc. 
Use tool memory to make sure it is correctly configured.
2) Run sensitivity analysis on FOMs using the scripts in Experiments.
3) Identify sensitive locations and expose deep parameters in malloc.c.
4) Update the subject settings to include exposed deep parameters for this subject.
5) Run NSGAII to optimise the values for these deep parameters.
