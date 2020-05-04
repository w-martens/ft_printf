/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/28 15:46:53 by wmartens      #+#    #+#                 */
/*   Updated: 2020/04/28 17:22:42 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static void	init_struct(t_printf *p_s)
{
	p_s->width = 0;
	p_s->prec = 0;
	p_s->prec_on = 0;
	p_s->nul_flag = 0;
	p_s->min_flag = 0;
	p_s->specifier = 0;
	p_s->print_len = 0;
}

static void	spec_chk(const char **fmt, t_printf *p_s)
{
	if (str_schr(*(*fmt), "cspdiuxX%"))
		p_s->specifier = *(*fmt);
}

static void	wp_asterisk(const char **fmt, t_printf *p_s, va_list arg, char flag)
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

static void	wp_amt(const char **fmt, t_printf *p_s, char flag)
{
	int wp_amt;
	int ret;

	ret = 0;
	while ((*(*fmt)) == '0')
		(*fmt)++;
	wp_amt = ft_atoi(*fmt);
	if (flag == 'w')
		p_s->width = wp_amt;
	if (flag == 'p')
		p_s->prec = wp_amt;
	while (wp_amt > 0)
	{
		wp_amt = wp_amt / 10;
		(*fmt)++;
	}
}

static void	flag_chk(const char **fmt, t_printf *p_s, va_list arg)
{
	init_struct(p_s);
	(*fmt)++;
	while (str_schr(*(*fmt), "-0"))
	{
		if (*(*fmt) == '-')
			p_s->min_flag = 1;
		if (*(*fmt) == '0')
			p_s->nul_flag = 1;
		(*fmt)++;
	}
	if (*(*fmt) == '*')
		wp_asterisk(fmt, p_s, arg, 'w');
	else
		wp_amt(fmt, p_s, 'w');
	if (*(*fmt) == '.')
	{
		p_s->prec_on = 1;
		(*fmt)++;
		if (*(*fmt) == '*')
			wp_asterisk(fmt, p_s, arg, 'p');
		else
			wp_amt(fmt, p_s, 'p');
	}
	spec_chk(fmt, p_s);
}
