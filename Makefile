.PHONY: setup clean
.ONESHELL: setup clean

#==================================
# Define variables
#==================================
CXX := g++
OFLAGS := -O2
CXXFLAGS := $(OFLAGS) -Wall -pedantic -Wextra
STDV := -std=c++11
BIN := bin
BUILD = build
SRC = src

# object deps
OBJECTS = main.o argparse.o filesystem.o musicplayer.o audiofile.o util.o gui.o
OBJECT_OUTPUTS = $(patsubst %, $(BUILD)/%, $(OBJECTS))
DEPENDS := $(patsubst %.o,%.d,$(OBJECTS))

# libraries
TAGLIB := `taglib-config --cflags` `taglib-config --libs` -lz
BOOST := -lboost_filesystem -lboost_system
SDL := `sdl2-config --cflags` `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf
LIBS := $(BOOST) $(SDL) $(TAGLIB)

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
