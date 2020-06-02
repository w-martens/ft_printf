/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hex_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: y4k_wm <y4k_wm@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/28 16:13:09 by y4k_wm        #+#    #+#                 */
/*   Updated: 2020/05/31 22:00:54 by y4k_wm        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		hex_len(t_printf *p_s, unsigned int n)
{
	int ret;

	ret = 0;
	if (n == 0 && p_s->prec != 0)
		ret++;
	while (n > 0)
	{
		n /= 16;
		ret++;
	}
	return (ret);
}

void	hex_print(t_printf *p_s, unsigned int n)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (p_s->specifier == 'X')
		hex = "0123456789ABCDEF";
	if (n > 15)
		hex_print(p_s, (n / 16));
	if (n > 0)
	{
		write(1, &hex[n % 16], 1);
		p_s->ret_cnt++;
	}
	if (n == 0 && p_s->prec != 0 && p_s->specifier != 'p')
	{
		write(1, &hex[0], 1);
		p_s->ret_cnt++;
	}
	else if (n == 0 && p_s->specifier == 'p')
	{
		write(1, &hex[0], 1);
		p_s->ret_cnt++;
	}
}

void	hex_prec(t_printf *p_s)
{
	int prec_cpy;

	prec_cpy = p_s->prec;
	while (prec_cpy > 0 && p_s->prec_on == 1)
	{
		write(1, "0", 1);
		p_s->ret_cnt++;
		prec_cpy--;
	}
}

void	xx_print(t_printf *p_s, unsigned int n)
{
	int hexlen;

	hexlen = hex_len(p_s, n);
	p_s->print_len = hexlen;
	if (p_s->prec_on == 1 && n == 0 && p_s->prec == 0 && p_s->width == 0)
		return ;
	if (p_s->prec_on == 1)
		p_s->nul_flag = 0;
	if (p_s->prec_on == 1 && p_s->prec > hexlen)
	{
		p_s->prec -= hexlen;
		p_s->print_len = hexlen + p_s->prec;
	}
	if (p_s->min_flag == 0)
		print_width(p_s);
	if (p_s->print_len > hexlen)
		hex_prec(p_s);
	hex_print(p_s, n);
	if (p_s->min_flag == 1)
		print_width(p_s);
}

void	ptr_print(t_printf *p_s, unsigned int n)
{
	p_s->print_len = hex_len(p_s, n) + 2;
	if (p_s->min_flag == 0)
		print_width(p_s);
	write(1, "0x", 2);
	p_s->ret_cnt += 2;
	hex_print(p_s, n);
	if (p_s->min_flag == 1)
		print_width(p_s);
}
