/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/05 15:20:25 by wmartens      #+#    #+#                 */
/*   Updated: 2020/06/02 15:49:53 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

static void	print_width(t_printf *p_s)
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

static void	int_width(t_printf *p_s, int i)
{
	char	width;
	int		len;

	len = p_s->width - p_s->print_len;
	if (p_s->prec - p_s->print_len > 0)
		len -= (p_s->prec - p_s->print_len);
	if (i < 0 && p_s->min_flag == 0 && p_s->prec + 1 >= p_s->width)
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

static void	char_print(t_printf *p_s, int i)
{
	p_s->print_len = 1;
	if (p_s->min_flag == 0)
		print_width(p_s);
	write(1, &i, 1);
	p_s->ret_cnt++;
	if (p_s->min_flag == 1)
		print_width(p_s);
}

static void	str_print(t_printf *p_s, char *str)
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

static void	hex_print(t_printf *p_s, unsigned long n)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (p_s->spec == 'X')
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

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

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
	int 		ret;
	long int 	li;

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

static int prec_set(t_printf *p_s, int intlen)
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

static void print_prec(t_printf *p_s)
{
	while (p_s->prec > 0)
	{
		write(1, "0", 1);
		p_s->ret_cnt++;
		p_s->prec--;
	}
}

static void	print_min(t_printf *p_s, int i)
{
	if (i < 0)
	{
		write(1, "-", 1);
		p_s->ret_cnt++;
	}
}

static void	int_print(t_printf *p_s, int i)
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

static void	unum_print(t_printf *p_s, unsigned int n)
{
	if (n > 9)
		num_print(p_s, (n / 10));
	ft_putchar_fd('0' + (n % 10), 1);
	p_s->ret_cnt++;
}

static int	unint_len(t_printf *p_s, unsigned int i)
{
	int ret;

	ret = 0;
	if (i == 0 && p_s->prec != 0)
		ret++;
	while (i > 0)
	{
		i /= 10;
		ret++;
	}
	p_s->print_len = ret;
	return (ret);
}

static void	unint_print(t_printf *p_s, unsigned int i)
{
	int intlen;
	int prec_cpy;

	if (p_s->prec == 0 && i == 0 && p_s->prec_on == 1)
		return ;
	intlen = unint_len(p_s, i);
	if (p_s->prec_on == 1)
		p_s->nul_flag = 0;
	if (p_s->min_flag == 0)
		print_width(p_s);
	prec_cpy = p_s->prec;
	if (p_s->prec_on == 1 && p_s->prec >= intlen)
	{
		p_s->prec -= intlen;
		p_s->print_len = intlen + p_s->prec;
	}
	if (prec_cpy >= p_s->print_len && p_s->prec_on == 1)
		print_prec(p_s);
	unum_print(p_s, i);
	if (p_s->min_flag == 1)
		print_width(p_s);
}

static void	xx_print(t_printf *p_s, unsigned long n)
{
	int prec_cpy;
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
	prec_cpy = p_s->prec;
	if (p_s->print_len > hexlen)
		while (prec_cpy > 0 && p_s->prec_on == 1)
		{
			write(1, "0", 1);
			p_s->ret_cnt++;
			prec_cpy--;
		}
	hex_print(p_s, n);
	if (p_s->min_flag == 1)
		print_width(p_s);
}

static void	perc_print(t_printf *p_s)
{
	char c;

	c = '%';
	write(1, &c, 1);
	p_s->ret_cnt++;
}

static void	wp_amt(const char **fmt, t_printf *p_s, char flag)
{
	int wp_amt;

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

static void	init_struct(t_printf *p_s)
{
	p_s->width = 0;
	p_s->prec = 0;
	p_s->prec_on = 0;
	p_s->nul_flag = 0;
	p_s->min_flag = 0;
	p_s->spec = 0;
	p_s->print_len = 0;
}

static int	str_schr(char str, char *set)
{
	while (*set != '\0')
	{
		if (str == *set)
			return (1);
		set++;
	}
	return (0);
}

static void	spec_chk(const char **fmt, t_printf *p_s)
{
	if (str_schr(*(*fmt), "cspdiuxX%"))
		p_s->spec = *(*fmt);
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

static void	print_arg(t_printf *p_s, va_list arg)
{
	if (p_s->spec == 'c')
		char_print(p_s, va_arg(arg, int));
	if (p_s->spec == 's')
		str_print(p_s, va_arg(arg, char *));
	if (p_s->spec == 'p')
		ptr_print(p_s, va_arg(arg, unsigned long));
	if (p_s->spec == 'd' || p_s->spec == 'i')
		int_print(p_s, va_arg(arg, int));
	if (p_s->spec == 'u')
		unint_print(p_s, va_arg(arg, unsigned int));
	if (p_s->spec == 'x' || p_s->spec == 'X')
		xx_print(p_s, va_arg(arg, unsigned long));
	if (p_s->spec == '%')
		perc_print(p_s);
}

int	ft_printf(const char *fmt, ...)
{
	t_printf	p_s;
	va_list		arg;

	p_s.ret_cnt = 0;
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
		if (*fmt != '\0')
			fmt++;
	}
	va_end(arg);
	return (p_s.ret_cnt);
}

// # define TEST "And these in the middle |%c%s%p%d%i%u%x%X%%%%%X%x%u%i%d%p%s%c| of a sentence", c, s, p, di, di, u, x, x, x, x, u, di, di, p, s, c
# define TEST "heyy %0.0"

int	main(void)
{
	int ret;
	int retf;

	ret = 0;
	retf = 0;
	ft_printf("[");
	ret = ft_printf(TEST);
	printf("]\t: %d <wout style\n[", ret);
	retf = printf(TEST);
	printf("]\t: %d <linuxxx style", retf);
	return (0);
}
