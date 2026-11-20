APP=CatSimulator
BUILD=build
SRC=src/main.cpp
CXX=arm-none-eabi-g++
NWLINK=nwlink
CXXFLAGS=-std=c++17 -Os -Wall -Wextra $(shell $(NWLINK) eadk-cflags)
LDFLAGS=-Wl,--relocatable -nostartfiles --specs=nano.specs

.PHONY: all clean
all: $(BUILD)/$(APP).nwa

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/main.o: $(SRC) | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/icon.o: src/icon.png | $(BUILD)
	$(NWLINK) png-icon-o $< $@

$(BUILD)/$(APP).nwa: $(BUILD)/main.o $(BUILD)/icon.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD)
