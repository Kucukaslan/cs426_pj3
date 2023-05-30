all: csc
all: mpi2

csr:
	@gcc -o gaussJacobi-serial-csr src/util.h src/util.c  src/gaussJacobi-serial-csr.c -lm
	@echo ""
	@./gaussJacobi-serial-csr ./input/3_matrix_A_csr.txt  ./input/3_matrix_b.txt outcsr3.txt
	@echo ""
	@./gaussJacobi-serial-csr ./input/10_matrix_A_csr.txt  ./input/10_matrix_b.txt outcsr10.txt
	@echo ""
	@./gaussJacobi-serial-csr ./input/1000_matrix_A_csr.txt  ./input/1000_matrix_b.txt outcsr1000.txt
	@echo ""
	@./xdiff/xdiff ./outcsr3.txt  ./input/3_matrix_X.txt
	@echo ""
	@./xdiff/xdiff ./outcsr10.txt  ./input/10_matrix_X.txt
	@echo ""
	@./xdiff/xdiff ./outcsr1000.txt  ./input/1000_matrix_X.txt

csc:
	@gcc -o gaussJacobi-serial-csc src/util.h src/util.c  src/gaussJacobi-serial-csc.c -lm
	@echo ""
	@./gaussJacobi-serial-csc ./input/3_matrix_A_csc.txt  ./input/3_matrix_b.txt outcsc3.txt
	@echo ""
	@./gaussJacobi-serial-csc ./input/10_matrix_A_csc.txt  ./input/10_matrix_b.txt outcsc10.txt
	@echo ""
	@./gaussJacobi-serial-csc ./input/1000_matrix_A_csc.txt  ./input/1000_matrix_b.txt outcsc1000.txt
	@echo ""
	@./xdiff/xdiff ./outcsc3.txt  ./input/3_matrix_X.txt
	@echo ""
	@./xdiff/xdiff ./outcsc10.txt  ./input/10_matrix_X.txt
	@echo ""
	@./xdiff/xdiff ./outcsc1000.txt  ./input/1000_matrix_X.txt

mpi:
	@mpicc -o sssp_parallel src/util.h src/util.c  src/sssp_parallel.c -lm
#	mpirun -np 4 ./sssp_parallel input/sample_graph_paper_csc_8v_16e_weighted.txt 4 out.txt

clear:
	@rm -f a.out out.txt gaussJacobi-serial-csr gaussJacobi-serial-csc outmpi*.txt out*.txt

c: clear

z: zip

zip:
	@zip -r muhammed_can_kucukaslan.zip muhammed_can_kucukaslan_cs426_pj3.pdf Makefile src/
