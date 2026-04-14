CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++11
SRC = main.cpp PmergeMe.cpp
OBJ = $(SRC:.cpp=.o)
NAME = PmergeMe
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re