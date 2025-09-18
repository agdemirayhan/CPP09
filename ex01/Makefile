NAME := RPN
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -Iinclude

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include

SRC := $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/RPN.cpp

OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/RPN.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
