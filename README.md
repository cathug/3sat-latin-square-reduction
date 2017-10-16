# 3-SAT to Latin Square reduction

This respository contains all the files required to perform a 3-SAT to Latin
Square reduction to a given instance.  The instance can then be certified with 
a SAT solver ex. `glucose`.

---

## Contents
﻿There are three files `ps3.cpp`, `ps3b.cpp` and `ps3c.cpp` stored in 
`/asn3/minimal-subset`, `/asn3/minimal-plus-1`, and `asn3/minimal-plus-2`
respectively.

Each folder contains the same set of selected instances, a version of qcp2cnf 
that corresponds to:
1.  in `/asn3/minimal-subset`, a minimal subset of properties of order n latin 
square (properties a, c, d);
2.  `/asn3/minimal-plus-1`, four properties of order n latin square (a, c, d, b);
3.  and `/asn3/minimal-plus-2`, five properties of order n latin square 
(a, c, d, b, e).

The code is quickly hacked together in C/C++. The reduction has already been 
compiled, but depending on the your Linux machine you may have to modify all 
access rights with `chmod 777 qcp2cnf`.

---

## Procedure to reduce and certify if Latin Squares are polytime solvable

1.  Get `glucose`.  When the program was written I used the single core 
version of `glucose-syrup`.
2.  Open Linux terminal.
3.  Go to any of the three folders `/minimal-plus-2`, `/minimal-plus-1`, or 
`/minimal-subset`
4.  The program `qcp2cnf` takes two arguments.  The first argument corresponds 
to the qcp input file containing a Latin Square instance and the second 
the desired cnf output in DIMACS format.
So for example:
```bash
$	./qcp2cnf q_10_01.qcp q_10_01.cnf
```
takes the qcp instance file `q_10_01.qcp` and converts it to a DIMACS format 
file called `q_10_01.cnf`
4.  Feed the DIMACS file to the SAT solver:
```bash
$	./glucose < q_10_01.cnf > q_10_01.out
```
where `q_10_01.out` contains all the standard output text.

---

### Compiling instructions
Note:  If the files still can't run, try to recompile the certifier/sat solver.

To compile certifier files with g++, open Linux terminal and type input
``` bash
$	g++ ps3.cpp -o qcp2cnf -std=c++11
$	g++ ps3b.cpp -o qcp2cnf -std=c++11
$	g++ ps3c.cpp -o qcp2cnf -std=c++11
```
To compile single core version of glucose.  In `/asn3/glucose-syrup/simp`, type
```bash
$	make -rm
```

