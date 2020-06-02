/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wp_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/02 17:19:27 by wmartens      #+#    #+#                 */
/*   Updated: 2020/06/02 17:19:28 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	wp_amt(const char **fmt, t_printf *p_s, char flag)
{
	int wp_amt;

	while ((*(*fmt)) == '0')
		iter_fmt(fmt);
	wp_amt = ft_atoi(*fmt);
	if (flag == 'w')
		p_s->width = wp_amt;
	if (flag == 'p')
		p_s->prec = wp_amt;
	while (wp_amt > 0)
	{
		wp_amt = wp_amt / 10;
		iter_fmt(fmt);
	}
}

void	print_width(t_printf *p_s)
{
	char	width;
	int		len;

	len = p_s->width - p_s->print_len;
	width = p_s->nul_flag == 0 || p_s->min_flag == 1 ? ' ' : '0';
	if (p_s->spec == 's')
		width = ' ';
	while (len > 0)
	{
		write(1, &width, 1);
		len--;
		p_s->ret_cnt++;
	}
}

void	int_width(t_printf *p_s, int i)
{
	char	width;
	int		len;

	len = p_s->width - p_s->print_len;
	if (p_s->prec - p_s->print_len > 0)
		len -= (p_s->prec - p_s->print_len);
	if (i < 0 && p_s->min_flag == 0 && p_s->prec + 1 > p_s->width)
		len--;
	if (i < 0 && p_s->prec > p_s->print_len)
		len = p_s->width - p_s->prec;
	width = p_s->nul_flag == 0 || p_s->min_flag == 1 ? ' ' : '0';
	while (len > 0)
	{
		write(1, &width, 1);
		len--;
		p_s->ret_cnt++;
	}
}

int		prec_set(t_printf *p_s, int intlen)
{
	int prec_cpy;

	prec_cpy = p_s->prec;
	if (p_s->prec_on == 1 && p_s->prec >= intlen)
	{
		p_s->prec -= intlen;
		p_s->print_len = intlen + p_s->prec;
	}
	return (prec_cpy);
}

void	print_prec(t_printf *p_s)
{
	while (p_s->prec > 0)
	{
		write(1, "0", 1);
		p_s->ret_cnt++;
		p_s->prec--;
	}
}
