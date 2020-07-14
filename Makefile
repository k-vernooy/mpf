.PHONY: setup clean
.ONESHELL: setup clean

#==================================
# Define variables
#==================================

# compiler options
CXX := g++
OFLAGS := -O2
CXXFLAGS := $(OFLAGS) -Wall -pedantic -Wextra
STDV := -std=c++11
BIN := bin
BUILD = build
SRC = src

# object deps
OBJECTS = main.o argparse.o filesystem.o musicplayer.o util.o
OBJECT_OUTPUTS = $(patsubst %, $(BUILD)/%, $(OBJECTS))
DEPENDS := $(patsubst %.o,%.d,$(OBJECTS))

# libraries
BOOST := -lboost_filesystem -lboost_system
SDL := -lSDL2 -lSDL2main
LIBS := $(BOOST) $(SDL)

all: mpf

#====================================
# Binary targets for end compilations
#====================================
mpf: setup $(OBJECT_OUTPUTS)
	$(CXX) $(CXXFLAGS) $(STDV) $(OBJECT_OUTPUTS) $(LIBS) -o $(BIN)/mpf

#=================================
# object file targets:
#=================================

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) $(OFLAGS) $(STDV) -MMD -MP -MT $@ $< -c -o $@


#===================================
# Directory system modification
#===================================

setup:
	mkdir -p $(BIN) $(BUILD)

clean:
	rm $(BUILD)/*.o
	rm -rf $(BIN)
