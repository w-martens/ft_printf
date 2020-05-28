/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 17:50:10 by wmartens      #+#    #+#                 */
/*   Updated: 2020/04/28 17:03:03 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdarg.h>
#include <unistd.h>

# define WIDTH 0
# define PRECISION 1

size_t	ft_strlen(const char *s)
{
	size_t	x;

	x = 0;
	while (s[x] != '\0')
		x++;
	return (x);
}

static int	ft_atoix(const char *str, long int res, long int sign, long int tmp)
{
	int i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		tmp = res;
		res = (res * 10) + str[i] - '0';
		i++;
	}
	if (tmp > res)
	{
		if (sign > 0)
			return (-1);
		else
			return (0);
	}
	return ((int)res * (int)sign);
}

int			ft_atoi(const char *str)
{
	int res;

	res = ft_atoix(str, 0, 1, 0);
	return (res);
}

typedef	struct	s_printf
{
	int		ret_cnt;
	int		width;
	int		prec;
	int		prec_on;
	int		nul_flag;
	int		min_flag;
	char	specifier;
	int		print_len;
}				t_printf;

int ft_printf(const char *fmt, ...);
