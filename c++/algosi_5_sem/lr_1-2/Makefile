CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

all: menu chi2 autotest

menu: menu.cpp
	$(CXX) $(CXXFLAGS) menu.cpp -o menu

chi2: chi2.cpp
	$(CXX) $(CXXFLAGS) chi2.cpp -o chi2

lr2: lr2_driver.cpp
	$(CXX) $(CXXFLAGS) lr2_driver.cpp -o lr2

autotest: autotest.cpp
	$(CXX) $(CXXFLAGS) autotest.cpp -o autotest

clean:
	rm -f menu chi2 autotest

clean:
	rm -f menu chi2 autotest lr2

run: menu
	./menu

run_chi2: chi2
	./chi2

run_lr2: lr2
	./lr2
	
test: autotest
	./autotest
