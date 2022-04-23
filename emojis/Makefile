CC = g++-9 -x c -g
CXX = g++-9 -g

CFLAGS_CATCH = -fpermissive -w

all: main

main: add_int.o capitalize.o emoji.o main.o
	${CXX} $^ -o $@

main.o: main.c
	$(CC) $^ -c -o $@

add_int.o: add_int.c
	$(CC) $^ -c -o $@

capitalize.o: capitalize.c
	$(CC) $^ -c -o $@

emoji.o: emoji.c
	$(CC) $^ -c -o $@


test: add_int.o capitalize.o emoji.o tests/test.o tests/test-add_int.o tests/test-capitalize.o tests/test-emoji.o
	$(CXX)  $^ -o $@

tests/test.o: tests/test.cpp
	$(CXX) $^ -c -o $@

tests/test-add_int.o: tests/test-add_int.cpp
	$(CXX) $(CFLAGS_CATCH) $^ -c -o $@

tests/test-capitalize.o: tests/test-capitalize.cpp
	$(CXX) $(CFLAGS_CATCH) $^ -c -o $@

tests/test-emoji.o: tests/test-emoji.cpp
	$(CXX) $(CFLAGS_CATCH) -w $^ -c -o $@


clean:
	rm -f main test *.o tests/*.o