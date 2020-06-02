/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unsigned_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/02 17:19:16 by wmartens      #+#    #+#                 */
/*   Updated: 2020/06/02 17:19:20 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	unum_print(t_printf *p_s, unsigned int n)
{
	if (n == 0 && p_s->prec == 0 && p_s->prec_on == 1)
		return ;
	if (n > 9)
		num_print(p_s, (n / 10));
	ft_putchar_fd('0' + (n % 10), 1);
	p_s->ret_cnt++;
}

int		unint_len(t_printf *p_s, unsigned int i)
{
	int ret;

	ret = 0;
	if (i == 0 && p_s->prec_on == 0)
		ret++;
	while (i > 0)
	{
		i /= 10;
		ret++;
	}
	p_s->print_len = ret;
	return (ret);
}

void	unint_print(t_printf *p_s, unsigned int i)
{
	int intlen;
	int prec_cpy;

	if (p_s->prec == 0 && i == 0 && p_s->prec_on == 1 && p_s->width == 0)
		return ;
	intlen = unint_len(p_s, i);
	if (p_s->prec_on == 1)
		p_s->nul_flag = 0;
	prec_cpy = p_s->prec;
	if (p_s->prec_on == 1 && p_s->prec >= intlen)
	{
		p_s->prec -= intlen;
		p_s->print_len = intlen + p_s->prec;
	}
	if (p_s->min_flag == 0)
		print_width(p_s);
	if (prec_cpy >= p_s->print_len && p_s->prec_on == 1)
		print_prec(p_s);
	unum_print(p_s, i);
	if (p_s->min_flag == 1)
		print_width(p_s);
}
