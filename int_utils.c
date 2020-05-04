/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   int_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/28 15:40:48 by wmartens      #+#    #+#                 */
/*   Updated: 2020/04/28 18:32:15 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static void	num_print(t_printf *p_s, int n)
{
	unsigned int nb;

	nb = n;
	if (n < 0)
	{
		nb = n * -1;
	}
	if (nb > 9)
		num_print(p_s, (nb / 10));
	ft_putchar_fd('0' + (nb % 10), 1);
	p_s->ret_cnt++;
}

static int	int_len(t_printf *p_s, int i)
{
	int ret;
	
	ret = 0;
	if (i < 0 && p_s->precision != 0)
	{
		i = i * -1;
		ret++;
	}
	if (i == 0 && p_s->precision != 0)
		ret++;
	while (i > 0)
	{
		i /= 10;
		ret++;
	}
	p_s->print_len = ret;
	return (ret);
}


static void	int_print(t_printf *p_s, int i)
{
	int intlen;
	int prec;

	if (p_s->precision == 0 && i == 0 && p_s->prec_on == 1)
		return;
	intlen = int_len(p_s, i);
	if (p_s->prec_on == 1)
		p_s->nul_flag = 0;
	if (p_s->min_flag == 0)
		print_width(p_s);
	if (i < 0)
		print_min(p_s);
	prec = p_s->precision;
	if (p_s->prec_on == 1 && p_s->precision >= intlen)
	{
		p_s->precision -= intlen;
		p_s->print_len = intlen + p_s->precision;
	}
	if (prec >= p_s->print_len && p_s->prec_on == 1)
		print_prec(p_s);
	num_print(p_s, i);
	if (p_s->min_flag == 1)
		print_width(p_s);
}
