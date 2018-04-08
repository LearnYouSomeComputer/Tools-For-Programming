.PHONY: list vector

list:
	g++ -g -o list list.cpp
	valgrind --tool=massif \
		--ignore-fn='_GLOBAL__sub_I_eh_alloc.cc' \
		--alloc-fn='call_init.part.0' --ignore-fn='call_init.part.0' \
		--time-unit=B --massif-out-file=list-massif.out \
		./list
	massif-visualizer list-massif.out

vector:
	g++ -g -o vector vector.cpp
	valgrind --tool=massif \
		--detailed-freq=1 \
		--ignore-fn='_GLOBAL__sub_I_eh_alloc.cc' \
		--alloc-fn='call_init.part.0' --ignore-fn='call_init.part.0' \
		--time-unit=B --massif-out-file=vector-massif.out \
		./vector
	massif-visualizer vector-massif.out