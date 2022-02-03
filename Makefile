CC=g++
CPPFLAGS=-Wall -fopenmp

all: matrix_multiplication_serial_blocked_runner matrix_multiplication_serial_blocked my_diff matrix_multiplication_sweep matrix_multiplication_parallel matrix_multiplication_serial_sweep_runner matrix_multiplication_serial

# EXECUTABLES
matrix_multiplication_serial: matrix_multiplication_serial.o matrix_multiplication_functions.o
	$(CC) $(CPPFLAGS) -o matrix_multiplication_serial ./matrix_multiplication_serial.o ./matrix_multiplication_functions.o

matrix_multiplication_serial_sweep_runner: matrix_multiplication_serial_sweep_runner.o matrix_multiplication_functions.o
	$(CC) $(CPPFLAGS) -o matrix_multiplication_serial_sweep_runner ./matrix_multiplication_serial_sweep_runner.o ./matrix_multiplication_functions.o

matrix_multiplication_parallel: matrix_multiplication_parallel.o matrix_multiplication_functions.o
	$(CC) $(CPPFLAGS) -o matrix_multiplication_parallel ./matrix_multiplication_parallel.o ./matrix_multiplication_functions.o

matrix_multiplication_sweep: matrix_multiplication_sweep.o matrix_multiplication_functions.o
	$(CC) $(CPPFLAGS) -o matrix_multiplication_sweep ./matrix_multiplication_sweep.o ./matrix_multiplication_functions.o

my_diff: my_diff.o
	$(CC) $(CPPFLAGS) -o my_diff ./my_diff.o

matrix_multiplication_serial_blocked: matrix_multiplication_serial_blocked.o matrix_multiplication_functions.o
	$(CC) $(CPPFLAGS) -o  matrix_multiplication_serial_blocked ./matrix_multiplication_serial_blocked.o ./matrix_multiplication_functions.o

matrix_multiplication_serial_blocked_runner: matrix_multiplication_serial_blocked_runner.o matrix_multiplication_functions.o
	$(CC) $(CPPFLAGS) -o  matrix_multiplication_serial_blocked_runner ./matrix_multiplication_serial_blocked_runner.o ./matrix_multiplication_functions.o


# BASE EXECUTABLES - OBJECT FILES

matrix_multiplication_serial.o: matrix_multiplication_serial.cpp matrix_multiplication_functions.h
	$(CC) $(CPPFLAGS) -c ./matrix_multiplication_serial.cpp

matrix_multiplication_serial_sweep_runner.o: matrix_multiplication_serial_sweep_runner.cpp matrix_multiplication_functions.h
	$(CC) $(CPPFLAGS) -c ./matrix_multiplication_serial_sweep_runner.cpp

matrix_multiplication_parallel.o: matrix_multiplication_parallel.cpp matrix_multiplication_functions.h
	$(CC) $(CPPFLAGS) -c ./matrix_multiplication_parallel.cpp

matrix_multiplication_sweep.o: matrix_multiplication_sweep.cpp matrix_multiplication_functions.h
	$(CC) $(CPPFLAGS) -c ./matrix_multiplication_sweep.cpp

my_diff.o: my_diff.cpp
	$(CC) $(CPPFLAGS) -c ./my_diff.cpp

matrix_multiplication_serial_blocked.o: matrix_multiplication_serial_blocked.cpp matrix_multiplication_functions.h
	$(CC) $(CPPFLAGS) -c  ./matrix_multiplication_serial_blocked.cpp

matrix_multiplication_serial_blocked_runner.o: matrix_multiplication_serial_blocked_runner.cpp matrix_multiplication_functions.h
	$(CC) $(CPPFLAGS) -c  ./matrix_multiplication_serial_blocked_runner.cpp

# BASE FILES - FUNCTION SOURCE FILES
matrix_multiplication_functions.o: matrix_multiplication_functions.h matrix_multiplication_functions.cpp
	$(CC) $(CPPFLAGS) -c ./matrix_multiplication_functions.cpp


# MISC COMMANDS
clean:
	rm -f *.o core* *.h.gch *.out matrix_multiplication_serial_blocked_runner matrix_multiplication_serial_blocked my_diff matrix_multiplication_sweep matrix_multiplication_serial matrix_multiplication_parallel matrix_multiplication_serial_sweep_runner