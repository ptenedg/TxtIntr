# Вариант 13 — логарифмический калькулятор
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
TARGET   := calculator
SRC      := calculator.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) *.o
