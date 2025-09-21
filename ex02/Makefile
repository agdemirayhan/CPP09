NAME := PmergeMe

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -Iinclude

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj

SRC := $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/PmergeMe.cpp

OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/PmergeMe.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re


