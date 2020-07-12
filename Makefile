.PHONY: all
.ONESHELL: all

#==================================
# Define variables
#==================================

# compiler options
CC := g++
OFLAGS := -O2
CFLAGS := $(OFLAGS) -Wall -pedantic -Wextra
STDV := -std=c++11

# filepaths
BIN := bin
BUILD = build

# object deps
# gui.o render.o mpdclient.o argparse.o song.o
OBJECTS = main.o argparse.o util.o
OBJECT_OUTPUTS = $(patsubst %, $(BUILD)/%, $(OBJECTS))


# libraries
BOOST := -lboost_filesystem -lboost_system
SDL := -lSDL2 -lSDL2main
LIBS := $(BOOST) $(SDL)

#====================================
# Binary targets for end compilations
#====================================
compile: setup $(OBJECTS)
	$(CC) $(CFLAGS) $(STDV) $(OBJECT_OUTPUTS) -o $(BIN)/music-filter

#=================================
# object file targets:
#=================================

%.o: src/%.cpp
	$(CC) $(OFLAGS) $(STDV) $< -c -o $(BUILD)/$@


#===================================
# Directory system modification
#===================================

setup:
	mkdir -p $(BIN) $(BUILD)

clean:
	rm $(BUILD)/*.o
	rm -rf $(BIN)
