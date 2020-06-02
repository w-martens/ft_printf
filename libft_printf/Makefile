# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wmartens <wmartens@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/06/02 17:24:02 by wmartens      #+#    #+#                  #
#    Updated: 2020/06/02 17:24:03 by wmartens      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

SRCS =	ft_printf.c\
		parse_utils.c\
		libft_utils.c\
		unsigned_utils.c\
		wp_utils.c\
		hex_utils.c\
		chrstr_utils.c\
		int_utils.c\

OBJS = $(SRCS:.c=.o)

NAME = libftprintf.a

INPUT = "%s", "string"

%.o: %.c
	gcc -Wall -Wextra -Werror $^ -I. -c

all: $(NAME)

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -f test

re: fclean all
