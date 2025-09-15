NAME    = btc
CXX     = c++
# CXXFLAGS= -Wall -Wextra -Werror -std=c++17 -Iinclude
RM      = rm -f
RMDIR   = rm -rf

SRCDIR  = src
OBJDIR  = obj

SRC     = $(SRCDIR)/main.cpp \
          $(SRCDIR)/BitcoinExchange.cpp

OBJ     = $(OBJDIR)/main.o \
          $(OBJDIR)/BitcoinExchange.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp include/BitcoinExchange.hpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/BitcoinExchange.o: $(SRCDIR)/BitcoinExchange.cpp include/BitcoinExchange.hpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/BitcoinExchange.cpp -o $(OBJDIR)/BitcoinExchange.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	$(RMDIR) $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re
