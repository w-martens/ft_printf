# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: y4k_wm <y4k_wm@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2020/05/28 17:07:29 by y4k_wm        #+#    #+#                  #
#    Updated: 2020/05/31 21:35:03 by y4k_wm        ########   odam.nl          #
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
