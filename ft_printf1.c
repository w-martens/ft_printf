/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf1.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/05 15:20:25 by wmartens      #+#    #+#                 */
/*   Updated: 2020/04/28 17:22:42 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"


static void	char_print(t_printf *p_s, int i)
{
	p_s->print_len = 1;
	if (p_s->min_flag == 0)
		print_width(p_s);
	write(1, &i, 1);
	p_s->ret_cnt++;
	if (p_s->min_flag == 1)
		print_width(p_s);
}

static void	str_print(t_printf *p_s, char *str)
{
	int i;

	i = 0;
	p_s->print_len = ft_strlen(str);
	if (p_s->prec_on == 1 && p_s->print_len > p_s->prec)
		p_s->print_len = (p_s->prec);
	if (p_s->min_flag == 0)
		print_width(p_s);
	while (str[i] != '\0')
	{
		if (p_s->prec_on == 1 && i >= p_s->prec)
			break ;
		write(1, &str[i], 1);
		i++;
		p_s->ret_cnt++;
	}
	if (p_s->min_flag == 1)
		print_width(p_s);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

static int	str_schr(char str, char *set)
{
	while (*set != '\0')
	{
		if (str == *set)
			return (1);
		set++;
	}
	return (0);
}

static void	print_arg(t_printf *p_s, va_list arg)
{
	if (p_s->specifier == 'c')
		char_print(p_s, va_arg(arg, int));
	if (p_s->specifier == 's')
		str_print(p_s, va_arg(arg, char *));
	if (p_s->specifier == 'p')
		ptr_print(p_s, va_arg(arg, unsigned long));
	if (p_s->specifier == 'd' || p_s->specifier == 'i')
		int_print(p_s, va_arg(arg, int));
	if (p_s->specifier == 'u')
		unint_print(p_s, va_arg(arg, unsigned int));
	if (p_s->specifier == 'x' || p_s->specifier == 'X')
		xx_print(p_s, va_arg(arg, unsigned long));
}

int	ft_printf(const char *fmt, ...)
{
	t_printf	p_s;
	va_list		arg;

	p_s.ret_cnt = 0;
	va_start(arg, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			flag_chk(&fmt, &p_s, arg);
			print_arg(&p_s, arg);
		}
		else
		{
			write(1, &(*fmt), 1);
			p_s.ret_cnt++;
		}
		if (*fmt != '\0')
			fmt++;
	}
	va_end(arg);
	return (p_s.ret_cnt);
}
