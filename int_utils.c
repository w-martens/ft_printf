/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   int_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: y4k_wm <y4k_wm@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/28 16:30:26 by y4k_wm        #+#    #+#                 */
/*   Updated: 2020/05/31 18:05:20 by y4k_wm        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	num_print(t_printf *p_s, int n)
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

int		int_len(t_printf *p_s, int i)
{
	int			ret;
	long int	li;

	li = i;
	ret = 0;
	if (li < 0)
	{
		li = li * -1;
		ret++;
	}
	if (li == 0)
		ret++;
	while (li > 0)
	{
		li /= 10;
		ret++;
	}
	p_s->print_len = ret;
	return (ret);
}

void	print_min(t_printf *p_s, int i)
{
	if (i < 0)
	{
		write(1, "-", 1);
		p_s->ret_cnt++;
	}
}

void	int_print(t_printf *p_s, int i)
{
	int intlen;
	int prec_cpy;

	if (p_s->prec_on == 1)
		p_s->nul_flag = 0;
	if (p_s->prec == 0 && i == 0 && p_s->prec_on == 1)
	{
		print_width(p_s);
		return ;
	}
	intlen = int_len(p_s, i);
	if (i < 0)
		p_s->prec++;
	if (p_s->nul_flag == 1)
		print_min(p_s, i);
	if (p_s->min_flag == 0)
		int_width(p_s, i);
	if (p_s->nul_flag == 0)
		print_min(p_s, i);
	prec_cpy = prec_set(p_s, intlen);
	if (prec_cpy >= p_s->print_len && p_s->prec_on == 1)
		print_prec(p_s);
	num_print(p_s, i);
	if (p_s->min_flag == 1)
		int_width(p_s, i);
}
