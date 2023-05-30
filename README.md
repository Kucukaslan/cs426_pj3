# Coarse-grain parallel Gauss-Jacobi (GJ) algorithm.


Beware that I've updated the util.h/c so please do not use the old versions. If you have extracted the zip to a new directory, you should be fine.

I have also written simple Go snippet in "xdiff" directory to compare my outputs
to the reference outputs. Unlike the usual `diff` it checks whether the numerical
difference between two numbers is within a certain threshold (which can also be 
changed via `e` flag).


About the auxiliary files in zip:
- `input/1000*` are the input files for the 1000x1000 matrix.
- `outcsr1000.txt` is the solution of the CSR version for 1000x1000 matrix.
- `outcsc1000.txt` is the solution of the CSC version for 1000x1000 matrix.
- `outmpi1_1000.txt` is the solution of the MPI 1 version for 1000x1000 matrix.
- `outmpi2_1000.txt` is the solution of the MPI 2 version for 1000x1000 matrix.
- `csr_out.txt`, `csc_out.txt`, `mpi1_out.txt`, `mpi2_out.txt` are the terminal 
outputs that I got during performance tests. I obtained the time measurements from them.
- `xdiff/xdiff.go` is the source code of aforementioned Go snippet
- `xdiff/xdiff` is the executable I build on my machine (WSL Ubuntu, see the specs above)
