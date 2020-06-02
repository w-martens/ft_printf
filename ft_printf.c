/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/05 15:20:25 by wmartens      #+#    #+#                 */
/*   Updated: 2020/05/30 22:27:03 by y4k_wm        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	init_struct(t_printf *p_s)
{
	p_s->ret_cnt = 0;
	p_s->width = 0;
	p_s->prec = 0;
	p_s->prec_on = 0;
	p_s->nul_flag = 0;
	p_s->min_flag = 0;
	p_s->specifier = 0;
	p_s->print_len = 0;
}

void	reset_struct(t_printf *p_s)
{
	p_s->width = 0;
	p_s->prec = 0;
	p_s->prec_on = 0;
	p_s->nul_flag = 0;
	p_s->min_flag = 0;
	p_s->specifier = 0;
	p_s->print_len = 0;
}

void	print_arg(t_printf *p_s, va_list arg)
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
	if (p_s->specifier == '%')
		perc_print(p_s);
	reset_struct(p_s);
}

int		ft_printf(const char *fmt, ...)
{
	t_printf	p_s;
	va_list		arg;

	p_s.ret_cnt = 0;
	init_struct(&p_s);
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
		iter_fmt(&fmt);
	}
	va_end(arg);
	return (p_s.ret_cnt);
}
