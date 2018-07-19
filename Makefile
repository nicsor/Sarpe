INC = -Iinclude
SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := 
CPPFLAGS := 
CXXFLAGS := 

sarpe: $(OBJ_FILES)
	g++ $(LDFLAGS) $(INC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) $(INC) $(CXXFLAGS) -c -o $@ $<
	
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) sarpe