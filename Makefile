CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -pipe -Wall -Wextra

PROBLEMS := 1 2 3 4 5 6 7 8 9 10
TARGETS := $(addprefix build/problem,$(PROBLEMS))

.PHONY: all clean
all: $(TARGETS)

build/problem%: problem%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf build
