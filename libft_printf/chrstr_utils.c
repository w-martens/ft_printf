/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   chrstr_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/02 17:20:12 by wmartens      #+#    #+#                 */
/*   Updated: 2020/06/02 17:20:12 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	char_print(t_printf *p_s, int i)
{
	p_s->nul_flag = 0;
	p_s->print_len = 1;
	if (p_s->min_flag == 0)
		print_width(p_s);
	write(1, &i, 1);
	p_s->ret_cnt++;
	if (p_s->min_flag == 1)
		print_width(p_s);
}

void	str_print(t_printf *p_s, char *str)
{
	int i;

	i = 0;
	if (str == NULL)
		str = "(null)";
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

void	perc_print(t_printf *p_s)
{
	char c;

	c = '%';
	write(1, &c, 1);
	p_s->ret_cnt++;
}
