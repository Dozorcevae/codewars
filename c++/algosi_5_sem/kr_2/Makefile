CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET   := kr2
TESTBIN  := autotest

.PHONY: all run test clean dot

all: $(TARGET) $(TESTBIN)

$(TARGET): main.cpp Graph.h
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

$(TESTBIN): autotest.cpp Graph.h
	$(CXX) $(CXXFLAGS) autotest.cpp -o $(TESTBIN)

run: $(TARGET)
	./$(TARGET)

test: $(TESTBIN)
	./$(TESTBIN)

dot:
	@if [ -f graph.dot ]; then \
		dot -Tpng graph.dot -o graph.png && echo "Создан graph.png"; \
	else \
		echo "Файл graph.dot не найден. Сначала в программе выбери пункт Save DOT."; \
	fi

clean:
	rm -f $(TARGET) $(TESTBIN) *.o *.dot *.png
