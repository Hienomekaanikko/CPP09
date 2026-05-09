CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++11
SRC = main.cpp RPN.cpp
OBJ = $(SRC:.cpp=.o)
NAME = RPN
RM = rm -f

all: $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re