/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hex_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/28 15:54:18 by wmartens      #+#    #+#                 */
/*   Updated: 2020/04/28 17:03:22 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static void	hex_print(t_printf *p_s, unsigned long n)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (p_s->specifier == 'X')
		hex = "0123456789ABCDEF";
	if (n > 15)
		hex_print(p_s, (n / 16));
	write(1, &hex[n % 16], 1);
	p_s->ret_cnt++;
}

static int	hex_len(unsigned long n)
{
	int ret;

	ret = 0;
	while (n > 0)
	{
		n /= 16;
		ret++;
	}
	return (ret);
}

static void	ptr_print(t_printf *p_s, unsigned long n)
{
	p_s->print_len = hex_len(n) + 2;
	if (p_s->min_flag == 0)
		print_width(p_s);
	write(1, "0x", 2);
	p_s->ret_cnt += 2;
	hex_print(p_s, n);
	if (p_s->min_flag == 1)
		print_width(p_s);
}

static void	xx_print(t_printf *p_s, unsigned long n)
{
	int prec;
	int hexlen;

	hexlen = hex_len(n);
	p_s->print_len = hexlen;
	if (p_s->prec_on == 1)
		p_s->nul_flag = 0;
	if (p_s->prec_on == 1 && p_s->prec > hexlen)
	{
		p_s->prec -= hexlen;
		p_s->print_len = hexlen + p_s->prec;
	}
	if (p_s->min_flag == 0)
		print_width(p_s);
	prec = p_s->prec;
	if (p_s->print_len > hexlen)
		while (prec > 0 && p_s->prec_on == 1)
		{
			write(1, "0", 1);
			p_s->ret_cnt++;
			prec--;
		}
	hex_print(p_s, n);
	if (p_s->min_flag == 1)
		print_width(p_s);
}
