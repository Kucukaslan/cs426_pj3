all: csr csc mpi1 mpi2

csr:
	gcc -o gaussJacobi-serial-csr src/util.h src/util.c  src/gaussJacobi-serial-csr.c -lm
	@echo ""
csr_t:
	@echo "" > csr_out.txt
	./gaussJacobi-serial-csr ./input/1000_matrix_A_csr.txt  ./input/1000_matrix_b.txt outcsr1000.txt >> csr_out.txt
	./gaussJacobi-serial-csr ./input/5000_matrix_A_csr.txt  ./input/5000_matrix_b.txt outcsr5000.txt >> csr_out.txt
	./gaussJacobi-serial-csr ./input/10000_matrix_A_csr.txt  ./input/10000_matrix_b.txt outcsr10000.txt >> csr_out.txt
	@echo ""
	./xdiff/xdiff ./outcsr1000.txt  ./input/1000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outcsr5000.txt  ./input/5000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outcsr10000.txt  ./input/10000_matrix_X.txt



csc:
	gcc -o gaussJacobi-serial-csc src/util.h src/util.c  src/gaussJacobi-serial-csc.c -lm
	@echo ""
csc_t:
	@echo "" > csc_out.txt
	./gaussJacobi-serial-csc ./input/1000_matrix_A_csc.txt  ./input/1000_matrix_b.txt outcsc1000.txt >> csc_out.txt
	./gaussJacobi-serial-csc ./input/5000_matrix_A_csc.txt  ./input/5000_matrix_b.txt outcsc5000.txt >> csc_out.txt
	./gaussJacobi-serial-csc ./input/10000_matrix_A_csc.txt  ./input/10000_matrix_b.txt outcsc10000.txt >> csc_out.txt
	@echo ""
	./xdiff/xdiff ./outcsc1000.txt  ./input/1000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outcsc5000.txt  ./input/5000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outcsc10000.txt  ./input/10000_matrix_X.txt

mpi1:
	mpicc -o gaussJacobi-mpi-p1 src/util.h src/util.c   src/gaussJacobi-mpi-p1.c -lm

mpi1_t: mpi1
	@echo "" > mpi1_out.txt
	mpirun --oversubscribe -np 4 ./gaussJacobi-mpi-p1 ./input/1000_matrix_A_csr.txt  ./input/1000_matrix_b.txt outmpi1_1000.txt >> mpi1_out.txt
	mpirun --oversubscribe -np 8 ./gaussJacobi-mpi-p1 ./input/1000_matrix_A_csr.txt  ./input/1000_matrix_b.txt outmpi1_1000.txt >> mpi1_out.txt
# mpirun --oversubscribe -np 16 ./gaussJacobi-mpi-p1 ./input/1000_matrix_A_csr.txt  ./input/1000_matrix_b.txt outmpi1_1000.txt >> mpi1_out.txt
	mpirun --oversubscribe -np 4 ./gaussJacobi-mpi-p1 ./input/5000_matrix_A_csr.txt  ./input/5000_matrix_b.txt outmpi1_5000.txt >> mpi1_out.txt
	mpirun --oversubscribe -np 8 ./gaussJacobi-mpi-p1 ./input/5000_matrix_A_csr.txt  ./input/5000_matrix_b.txt outmpi1_5000.txt >> mpi1_out.txt
# mpirun --oversubscribe -np 16 ./gaussJacobi-mpi-p1 ./input/5000_matrix_A_csr.txt  ./input/5000_matrix_b.txt outmpi1_5000.txt >> mpi1_out.txt
	mpirun --oversubscribe -np 4 ./gaussJacobi-mpi-p1 ./input/10000_matrix_A_csc.txt  ./input/10000_matrix_b.txt outmpi1_10000.txt >> mpi1_out.txt
	mpirun --oversubscribe -np 8 ./gaussJacobi-mpi-p1 ./input/10000_matrix_A_csc.txt  ./input/10000_matrix_b.txt outmpi1_10000.txt >> mpi1_out.txt
	mpirun --oversubscribe -np 16 ./gaussJacobi-mpi-p1 ./input/10000_matrix_A_csc.txt  ./input/10000_matrix_b.txt outmpi1_10000.txt >> mpi1_out.txt
	@echo ""
	./xdiff/xdiff ./outmpi1_1000.txt  ./input/1000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outmpi1_5000.txt  ./input/5000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outmpi1_10000.txt  ./input/10000_matrix_X.txt
	

mpi2:
	mpicc -o gaussJacobi-mpi-p2 src/util.h src/util.c  src/gaussJacobi-mpi-p2.c -lm

mpi2_t: mpi2
	@echo "" > mpi2_out.txt
	mpirun --oversubscribe -np 4 ./gaussJacobi-mpi-p2 ./input/1000_matrix_A_csc.txt  ./input/1000_matrix_b.txt outmpi2_1000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 8 ./gaussJacobi-mpi-p2 ./input/1000_matrix_A_csc.txt  ./input/1000_matrix_b.txt outmpi2_1000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 16 ./gaussJacobi-mpi-p2 ./input/1000_matrix_A_csc.txt  ./input/1000_matrix_b.txt outmpi2_1000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 4 ./gaussJacobi-mpi-p2 ./input/5000_matrix_A_csc.txt  ./input/5000_matrix_b.txt outmpi2_5000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 8 ./gaussJacobi-mpi-p2 ./input/5000_matrix_A_csc.txt  ./input/5000_matrix_b.txt outmpi2_5000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 16 ./gaussJacobi-mpi-p2 ./input/5000_matrix_A_csc.txt  ./input/5000_matrix_b.txt outmpi2_5000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 4 ./gaussJacobi-mpi-p2 ./input/10000_matrix_A_csc.txt  ./input/10000_matrix_b.txt outmpi2_10000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 8 ./gaussJacobi-mpi-p2 ./input/10000_matrix_A_csc.txt  ./input/10000_matrix_b.txt outmpi2_10000.txt >> mpi2_out.txt
	mpirun --oversubscribe -np 16 ./gaussJacobi-mpi-p2 ./input/10000_matrix_A_csc.txt  ./input/10000_matrix_b.txt outmpi2_10000.txt >> mpi2_out.txt

	@echo ""
	./xdiff/xdiff ./outmpi2_1000.txt  ./input/1000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outmpi2_5000.txt  ./input/5000_matrix_X.txt
	@echo ""
	./xdiff/xdiff ./outmpi2_10000.txt  ./input/10000_matrix_X.txt
	

clear:
	@rm -f a.out gaussJacobi-serial-csr gaussJacobi-serial-csc gaussJacobi-mpi-p1 gaussJacobi-mpi-p2 out*.txt

c: clear

z: zip

zip:
	@zip -r ../muhammed_can_kucukaslan_p3.zip muhammed_can_kucukaslan_cs426_pj3.pdf Makefile src/ xdiff/ \
		input/1000_* outcsr1000.txt outcsc1000.txt outmpi1_1000.txt outmpi2_1000.txt \
		csr_out.txt csc_out.txt mpi1_out.txt mpi2_out.txt \
