CC = clang
CXX = clang++

EXE = app

SRC_DIR = src
BUILD_DIR = build
INC_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.cpp)
OBJ = $(subst $(SRC_DIR), $(BUILD_DIR), $(addsuffix .o, $(basename $(SRC))))

CPPFLAGS += -I$(INC_DIR)    # preprocessor flags
CFLAGS +=                   # C compiler flags
CXXFLAGS += -std=c++11      # C++ compiler flags
LDFLAGS +=                  # linker flags

OS = $(shell uname)
ifeq ($(OS), Darwin)
LDLIBS += -framework OpenGL -lglfw # OSX libraries 
else
LDLIBS += -ldl -lGL -lglfw # Linux libraries
endif

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
