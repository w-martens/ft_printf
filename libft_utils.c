/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: y4k_wm <y4k_wm@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/28 16:52:37 by y4k_wm        #+#    #+#                 */
/*   Updated: 2020/05/30 17:43:14 by y4k_wm        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_strlen(const char *s)
{
	int	x;

	x = 0;
	while (s[x] != '\0')
		x++;
	return (x);
}

int	ft_atoix(const char *str, long int res, long int sign, long int tmp)
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

int	ft_atoi(const char *str)
{
	int res;

	res = ft_atoix(str, 0, 1, 0);
	return (res);
}
