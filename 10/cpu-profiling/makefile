.PHONY: gperftool callgrind

gperftool:
	g++ -g -lprofiler -o fib_gperftool fibonnaci.cpp
	CPUPROFILE=gperftool.prof CPUPROFILE_FREQUENCY=100 ./fib_gperftool
	pprof --callgrind ./fib_gperftool gperftool.prof > gperftool.out
	kcachegrind gperftool.out

callgrind:
	g++ -g -o fib_callgrind fibonnaci.cpp
	valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./fib_callgrind
	kcachegrind callgrind.out