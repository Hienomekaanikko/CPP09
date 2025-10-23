CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++20
SRC = main.cpp BitcoinExchange.cpp
OBJ = $(SRC:.cpp=.o)
NAME = btc
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

.PHONY: all clean fclean re