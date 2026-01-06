CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic
INCLUDES := -Iinclude

.PHONY: all menu test clean

all: menu test

menu: src/main_menu.cpp include/bst.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) src/main_menu.cpp -o menu

test: tests/test_bst.cpp include/bst.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) tests/test_bst.cpp -o test_bst

clean:
	rm -f menu test_bst
