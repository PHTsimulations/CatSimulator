APP=CatSimulator
BUILD=build
SRC=src/main.cpp

CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
NWLINK=npx --yes -- nwlink@0.0.16

CXXFLAGS=-std=c++11 -fno-exceptions -Os -Wall -Wextra $(shell $(NWLINK) eadk-cflags)
LDFLAGS=-Wl,--relocatable -nostartfiles --specs=nano.specs

.PHONY: all clean

all: $(BUILD)/$(APP).nwa

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/main.o: $(SRC) | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/icon.o: src/icon.png
	$(NWLINK) png-icon-o $< $@

$(BUILD)/$(APP).nwa: $(BUILD)/main.o $(BUILD)/icon.o
	$(CC) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD)
