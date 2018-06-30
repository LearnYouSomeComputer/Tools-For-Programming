.PHONY: default coverage test cleancoverage clean
CFLAGS=

### Actual Program
default: average

average: average.o
	g++ ${CFLAGS} average.o -o average

average.o: average.cpp vector.h vector.hpp
	g++ ${CFLAGS} -c average.cpp



### Test Suite

# Generates coverage reports
coverage: test
	gcov -mr test_vector.cpp

# Builds and runs tests
test: CFLAGS=--coverage
test: testsuite
	@./testsuite

testsuite: test_main.o test_vector.o
	g++ ${CFLAGS} $^ -o testsuite

test_vector.o: test_vector.cpp vector.h vector.hpp
	g++ ${CFLAGS} -c test_vector.cpp

test_main.o: test_main.cpp
	g++ -O3 -c test_main.cpp

cleancoverage:
	-@rm -f *.gcov *.gcno *.gcda

clean: cleancoverage
	-@rm -f *.o
	-@rm -f testsuite
	-@rm -f average