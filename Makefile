CC = gcc
CXX = g++

EXE = app

SRC_DIR = src
BUILD_DIR = build
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.cpp)
OBJ = $(subst $(SRC_DIR), $(BUILD_DIR), $(addsuffix .o, $(basename $(SRC))))

CPPFLAGS += -I$(INC_DIR) # preprocessor flags
CFLAGS += # C compiler flags
CXXFLAGS += -std=c++11 # C++ compiler flags
LDFLAGS += # linker flags
LDLIBS += -framework OpenGL -lglfw  # libraries

all: $(EXE)
$(EXE): $(OBJ)
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) -c $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) -c $<

.PHONY = run clean
run: 
	./$(EXE)
clean: 
	rm $(EXE) $(OBJ)
