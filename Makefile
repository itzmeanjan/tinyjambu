CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
OPTFLAGS = -O3 -march=native -mtune=native
IFLAGS = -I ./include

# Consider launching all make recipes as shown in following example
#
# `FBK=32 make` // computes 32 feedback bits in-parallel
#
# or
#
# `FBK=128 make` // computes 128 feedback bits in-parallel
#
# Note :
#
# - If none defined, default choice `FBK_32` is used !
# - If one attempts to do `FBK=187 make`, which is not supported bitwidth,
#   default choice `FBK_32` to be used
DFBK = -DFBK_$(or $(FBK),0)

all: test_tinyjambu test_kat

test/a.out: test/main.cpp include/*.hpp include/test/*.hpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DFBK) $(IFLAGS) $< -o $@

test_tinyjambu: test/a.out
	./$<

test_kat:
	bash test.sh

clean:
	find . -name '*.out' -o -name '*.o' -o -name '*.so' | xargs rm -rf

format:
	find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style=Mozilla

bench/a.out: bench/main.cpp include/*.hpp include/bench/*.hpp
	# make sure you've google-benchmark globally installed
	# see https://github.com/google/benchmark/tree/60b16f1#installation
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DFBK) $(IFLAGS) $< -lbenchmark -o $@

benchmark: bench/a.out
	./$<

lib:
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DFBK) $(IFLAGS) -fPIC --shared wrapper/tinyjambu.cpp -o wrapper/libtinyjambu.so
