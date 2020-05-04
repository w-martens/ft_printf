/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wp_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/28 15:57:04 by wmartens      #+#    #+#                 */
/*   Updated: 2020/04/28 17:22:42 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static void print_min(t_printf *p_s)
{
	write(1, "-", 1);
	p_s->prec++;
	p_s->ret_cnt++;
}

static void	print_width(t_printf *p_s)
{
	char	width;
	int		len;

	len = p_s->width - p_s->print_len;
	width = p_s->nul_flag == 0 || p_s->min_flag == 1 ? ' ' : '0';
	while (len > 0)
	{
		write(1, &width, 1);
		len--;
		p_s->ret_cnt++;
	}
}

static void print_prec(t_printf *p_s)
{
	while (p_s->prec > 0)
	{
		write(1, "0", 1);
		p_s->ret_cnt++;
		p_s->prec--;
	}
}
