/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/02 17:21:36 by wmartens      #+#    #+#                 */
/*   Updated: 2020/06/02 17:21:37 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	iter_fmt(const char **fmt)
{
	if (*(*fmt) != '\0')
		(*fmt)++;
}

int		str_schr(char str, char *set)
{
	while (*set != '\0')
	{
		if (str == *set)
			return (1);
		set++;
	}
	return (0);
}

void	spec_chk(const char **fmt, t_printf *p_s)
{
	if (str_schr(*(*fmt), "cspdiuxX%"))
		p_s->spec = *(*fmt);
}

void	wp_asterisk(const char **fmt, t_printf *p_s, va_list arg, char flag)
{
	int width;
	int precision;

	if (flag == 'w')
	{
		width = va_arg(arg, int);
		if (width < 0)
		{
			width *= -1;
			p_s->min_flag = 1;
		}
		p_s->width = width;
	}
	if (flag == 'p')
	{
		precision = va_arg(arg, int);
		if (precision < 0)
			p_s->prec_on = 0;
		p_s->prec = precision;
	}
	(*fmt)++;
}

void	flag_chk(const char **fmt, t_printf *p_s, va_list arg)
{
	iter_fmt(fmt);
	while (str_schr(*(*fmt), "-0"))
	{
		if (*(*fmt) == '-')
			p_s->min_flag = 1;
		if (*(*fmt) == '0')
			p_s->nul_flag = 1;
		iter_fmt(fmt);
	}
	if (*(*fmt) == '*')
		wp_asterisk(fmt, p_s, arg, 'w');
	else
		wp_amt(fmt, p_s, 'w');
	if (*(*fmt) == '.')
	{
		p_s->prec_on = 1;
		iter_fmt(fmt);
		if (*(*fmt) == '*')
			wp_asterisk(fmt, p_s, arg, 'p');
		else
			wp_amt(fmt, p_s, 'p');
	}
	spec_chk(fmt, p_s);
}
