/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/05 15:20:25 by wmartens      #+#    #+#                 */
/*   Updated: 2020/05/13 18:56:03 by wmartens      ########   odam.nl         */
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
	if (p_s->specifier == 's')
		width = ' ';
	while (len > 0)
	{
		write(1, &width, 1);
		len--;
		p_s->ret_cnt++;
	}
}

static void	int_width(t_printf *p_s, int intlen, int i)
{
	char	width;
	int		len;

	len = p_s->width - p_s->print_len;
	if (p_s->prec - p_s->print_len > 0)
		len -=  (p_s->prec - p_s->print_len);
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
		int_width(p_s, intlen, i);
	if (p_s->nul_flag == 0)
		print_min(p_s, i);
	prec_cpy = prec_set(p_s, intlen);
	if (prec_cpy >= p_s->print_len && p_s->prec_on == 1)
		print_prec(p_s);
	num_print(p_s, i);
	if (p_s->min_flag == 1)
		int_width(p_s, intlen, i);
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
	if (p_s->specifier == 'c')
		char_print(p_s, va_arg(arg, int));
	if (p_s->specifier == 's')
		str_print(p_s, va_arg(arg, char *));
	if (p_s->specifier == 'p')
		ptr_print(p_s, va_arg(arg, unsigned long));
	if (p_s->specifier == 'd' || p_s->specifier == 'i')
		int_print(p_s, va_arg(arg, int));
	if (p_s->specifier == 'u')
		unint_print(p_s, va_arg(arg, unsigned int));
	if (p_s->specifier == 'x' || p_s->specifier == 'X')
		xx_print(p_s, va_arg(arg, unsigned long));
	if (p_s->specifier == '%')
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

# define TEST "%08i", -1994

int	main(void)
{
	int broken;
	int	bonus;
	
	char			c = 'a';
	char			*s = "stringalingadingdong";
	int				di = 42;
	int				*p = &di;
	unsigned int	u = 42;
	unsigned int	xcap = 185334478;
	unsigned int	x = 185334478;
	
	char			*percent = "It goes well 100% of the time";
	char			*conv = "%c, %s, %p, %d, %i, %u, %x, %X, %%";

	broken = 0;
	bonus = 0;
	
	printf(" (%d)\n", printf("|First lets try a string without arguments|"));
	ft_printf(" (%d)\n", ft_printf("|First lets try a string without arguments|"));
	
	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Basic percentage sign conversions:"));
	ft_printf(" (%d)\n", ft_printf("Basic percentage sign conversions:"));
	printf("\n");
	
	printf(" (%d)\n", printf("|Let's try a double percentage sign: %%|"));
	ft_printf(" (%d)\n", ft_printf("|Let's try a double percentage sign: %%|"));
	printf("-\n");

	printf(" (%d)\n", printf("How does it handle a percentage sign in a string? |%s|", percent));
	ft_printf(" (%d)\n", ft_printf("How does it handle a percentage sign in a string? |%s|", percent));
	printf("-\n");

	printf(" (%d)\n", printf("It handles all these too: |%s|", "%c, %s, %p, %d, %i, %u, %x, %X, %%"));
	ft_printf(" (%d)\n", ft_printf("It handles all these too: |%s|", "%c, %s, %p, %d, %i, %u, %x, %X, %%"));
	printf("-\n");

	printf(" (%d)\n", printf("Also these: |%s|", conv));
	ft_printf(" (%d)\n", ft_printf("Also these: |%s|", conv));
	printf("-\n");
	
	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Other basic conversions:"));
	ft_printf(" (%d)\n", ft_printf("Other basic conversions:"));
	printf("\n");

	printf(" (%d)\n", printf("And a single character: |%c|", c));
	ft_printf(" (%d)\n", ft_printf("And a single character: |%c|", c));
	printf("-\n");

	printf(" (%d)\n", printf("And a string: |%s|", s));
	ft_printf(" (%d)\n", ft_printf("And a string: |%s|", s));
	printf("-\n");

	printf(" (%d)\n", printf("And a decimal: |%d|", di));
	ft_printf(" (%d)\n", ft_printf("And a decimal: |%d|", di));
	printf("-\n");

	printf(" (%d)\n", printf("And an integer: |%i|", di));
	ft_printf(" (%d)\n", ft_printf("And an integer: |%i|", di));
	printf("-\n");

	printf(" (%d)\n", printf("And a zero as integer: |%i|", 0));
	ft_printf(" (%d)\n", ft_printf("And a zero as integer: |%i|", 0));
	printf("-\n");

	printf(" (%d)\n", printf("And a pointer to the same integer: |%p|", p));
	ft_printf(" (%d)\n", ft_printf("And a pointer to the same integer: |%p|", p));
	printf("-\n");

	printf(" (%d)\n", printf("And an unsignted int |%u|", u));
	ft_printf(" (%d)\n", ft_printf("And an unsignted int |%u|", u));
	printf("-\n");

	printf(" (%d)\n", printf("AND A HEXADECIMAL |%X|", xcap));
	ft_printf(" (%d)\n", ft_printf("AND A HEXADECIMAL |%X|", xcap));
	printf("-\n");

	printf(" (%d)\n", printf("And a hexadecimal |%x|", x));
	ft_printf(" (%d)\n", ft_printf("And a hexadecimal |%x|", x));
	printf("-\n");

	printf(" (%d)\n", printf("Can it handle all these at once? %c%s%p%d%i%u%x%X%%%%%X%x%u%i%d%p%s%c", c, s, p, di, di, u, x, x, x, x, u, di, di, p, s, c));
	ft_printf(" (%d)\n", ft_printf("Can it handle all these at once? %c%s%p%d%i%u%x%X%%%%%X%x%u%i%d%p%s%c", c, s, p, di, di, u, x, x, x, x, u, di, di, p, s, c));
	printf("-\n");

	printf(" (%d)\n", printf("And these in the middle |%c%s%p%d%i%u%x%X%%%%%X%x%u%i%d%p%s%c| of a sentence", c, s, p, di, di, u, x, x, x, x, u, di, di, p, s, c));
	ft_printf(" (%d)\n", ft_printf("And these in the middle |%c%s%p%d%i%u%x%X%%%%%X%x%u%i%d%p%s%c| of a sentence", c, s, p, di, di, u, x, x, x, x, u, di, di, p, s, c));
	printf("-\n");

	ft_printf(" (%d)\n", ft_printf("My compliments in hexadecimal: %X %X %X", 4011, 49374, 47806));	

	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("How does it handle MIN & MAX values?"));
	ft_printf(" (%d)\n", ft_printf("How does it handle MIN & MAX values?"));
	printf("\n");

	printf(" (%d)\n", printf("Char: |%c|", '%'));
	ft_printf(" (%d)\n", ft_printf("Char: |%c|", '%'));
	printf("-\n");

	printf(" (%d)\n", printf("MAX Decimal: |%d|", 2147483647));
	ft_printf(" (%d)\n", ft_printf("MAX Decimal: |%d|", 2147483647));
	printf("-\n");

	printf(" (%d)\n", printf("MIN Decimal: |%d|", ((int)(-2147483648))));
	ft_printf(" (%d)\n", ft_printf("MIN Decimal: |%d|", ((int)(-2147483648))));
	printf("-\n");

	printf(" (%d)\n", printf("MAX Integer: |%i|", 2147483647));
	ft_printf(" (%d)\n", ft_printf("MAX Integer: |%i|", 2147483647));
	printf("-\n");

	printf(" (%d)\n", printf("MIN Integer: |%i|", ((int)(-2147483648))));
	ft_printf(" (%d)\n", ft_printf("MIN Integer: |%i|", ((int)(-2147483648))));
	printf("-\n");

	printf(" (%d)\n", printf("MAX Unsignted int |%u|", (u - 43)));
	ft_printf(" (%d)\n", ft_printf("MAX Unsignted int |%u|", (u - 43)));
	printf("-\n");

	printf(" (%d)\n", printf("MAX INT AS HEXADECIMAL |%X|", 2147483647));
	ft_printf(" (%d)\n", ft_printf("MAX INT AS HEXADECIMAL |%X|", 2147483647));
	printf("-\n");

	printf(" (%d)\n", printf("MAX HEXADECIMAL |%X|", ((unsigned int)(4294967295))));
	ft_printf(" (%d)\n", ft_printf("MAX HEXADECIMAL |%X|", ((unsigned int)(4294967295))));
	printf("-\n");

	printf(" (%d)\n", printf("HEXadecimal with negative number |%X|", -42));
	ft_printf(" (%d)\n", ft_printf("HEXadecimal with negative number |%X|", -42));
	printf("-\n");

	printf(" (%d)\n", printf("Hexadecimal with negative number |%x|", -42));
	ft_printf(" (%d)\n", ft_printf("Hexadecimal with negative number |%x|", -42));
	printf("-\n");

	printf(" (%d)\n", printf("MAX int as hex |%x|", 2147483647));
	ft_printf(" (%d)\n", ft_printf("MAX int as hex |%x|", 2147483647));
	printf("-\n");

	printf(" (%d)\n", printf("MAX hex |%x|", ((unsigned int)(4294967295))));
	ft_printf(" (%d)\n", ft_printf("MAX hex |%x|", ((unsigned int)(4294967295))));
	printf("-\n");

	printf("\n\n");

	printf(" (%d)\n", printf("Char: |%c|", ('%' + 1)));
	ft_printf(" (%d)\n", ft_printf("Char: |%c|", ('%' + 1)));
	printf("-\n");

	printf(" (%d)\n", printf("MAX Decimal: |%d|", (2147483647 + 1)));
	ft_printf(" (%d)\n", ft_printf("MAX Decimal: |%d|", (2147483647 + 1)));
	printf("-\n");

	printf(" (%d)\n", printf("MIN Decimal: |%d|", (((int)(-2147483648))- 1)));
	ft_printf(" (%d)\n", ft_printf("MIN Decimal: |%d|", (((int)(-2147483648))- 1)));
	printf("-\n");

	printf(" (%d)\n", printf("MAX Int: |%i|", (2147483647 + 1)));
	ft_printf(" (%d)\n", ft_printf("MAX Int: |%i|", (2147483647 + 1)));
	printf("-\n");

	printf(" (%d)\n", printf("MIN Int: |%i|", (((int)(-2147483648))- 1)));
	ft_printf(" (%d)\n", ft_printf("MIN Int: |%i|", (((int)(-2147483648))- 1)));
	printf("-\n");

	printf(" (%d)\n", printf("MAX HEXADECIMAL |%X|", (((unsigned int)(4294967295)) + 1)));
	ft_printf(" (%d)\n", ft_printf("MAX HEXADECIMAL |%X|", (((unsigned int)(4294967295)) + 1)));
	printf("-\n");

	printf(" (%d)\n", printf("MAX hex |%x|", (((unsigned int)(4294967295)) + 1)));
	ft_printf(" (%d)\n", ft_printf("MAX hex |%x|", (((unsigned int)(4294967295)) + 1)));
	printf("-\n");

	
	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling minimum field width"));
	ft_printf(" (%d)\n", ft_printf("Handling minimum field width"));

	printf(" (%d)\n", printf("(0) Mfw 0: |%0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Mfw 0: |%0i|", 0));
	printf(" (%d)\n", printf("(0) Mfw 1: |%1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Mfw 1: |%1i|", 0));
	printf(" (%d)\n", printf("Mfw 0: |%0i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 0: |%0i|", di));
	printf(" (%d)\n", printf("Mfw 1: |%1i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 1: |%1i|", di));
	printf(" (%d)\n", printf("Mfw 2: |%2i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 2: |%2i|", di));
	printf(" (%d)\n", printf("Mfw 3: |%3i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 3: |%3i|", di));
	printf(" (%d)\n", printf("Mfw 4: |%4i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 4: |%4i|", di));
	printf(" (%d)\n", printf("Mfw 5: |%5i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 5: |%5i|", di));
	printf(" (%d)\n", printf("Mfw 10: |%10i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 10: |%10i|", di));
	printf(" (%d)\n", printf("Mfw 15: |%15i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 15: |%15i|", di));
	printf(" (%d)\n", printf("Mfw 42: |%42i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw 42: |%42i|", di));

	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling precision"));
	ft_printf(" (%d)\n", ft_printf("Handling precision"));

	printf(" (%d)\n", printf("(0) Precis. 0: |%.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Precis. 0: |%.0i|", 0));
	printf(" (%d)\n", printf("(0) Precis. 1: |%.1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Precis. 1: |%.1i|", 0));
	printf(" (%d)\n", printf("(0) Precis. 2: |%.2i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Precis. 2: |%.2i|", 0));
	printf(" (%d)\n", printf("Precis. 0: |%.0i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 0: |%.0i|", di));
	printf(" (%d)\n", printf("Precis. 1: |%.1i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 1: |%.1i|", di));
	printf(" (%d)\n", printf("Precis. 2: |%.2i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 2: |%.2i|", di));
	printf(" (%d)\n", printf("Precis. 3: |%.3i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 3: |%.3i|", di));
	printf(" (%d)\n", printf("Precis. 4: |%.4i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 4: |%.4i|", di));
	printf(" (%d)\n", printf("Precis. 5: |%.5i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 5: |%.5i|", di));
	printf(" (%d)\n", printf("Precis. 10: |%.10i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 10: |%.10i|", di));
	printf(" (%d)\n", printf("Precis. 15: |%.15i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 15: |%.15i|", di));
	printf(" (%d)\n", printf("Precis. 42: |%.42i|", di));
	ft_printf(" (%d)\n", ft_printf("Precis. 42: |%.42i|", di));

	printf(" (%d)\n", printf("(0) P .00: |%.00i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) P .00: |%.00i|", 0));
	printf(" (%d)\n", printf("(0) P .01: |%.01i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) P .01: |%.01i|", 0));	
	printf(" (%d)\n", printf("P .00: |%.00i|", di));
	ft_printf(" (%d)\n", ft_printf("P .00: |%.00i|", di));	
	printf(" (%d)\n", printf("P .01: |%.01i|", di));
	ft_printf(" (%d)\n", ft_printf("P .01: |%.01i|", di));
	printf(" (%d)\n", printf("P .02: |%.02i|", di));
	ft_printf(" (%d)\n", ft_printf("P .02: |%.02i|", di));
	printf(" (%d)\n", printf("P .03: |%.03i|", di));
	ft_printf(" (%d)\n", ft_printf("P .03: |%.03i|", di));
	printf(" (%d)\n", printf("P .04: |%.04i|", di));
	ft_printf(" (%d)\n", ft_printf("P .04: |%.04i|", di));
	printf(" (%d)\n", printf("P .05: |%.05i|", di));
	ft_printf(" (%d)\n", ft_printf("P .05: |%.05i|", di));
	printf(" (%d)\n", printf("P .010: |%.010i|", di));
	ft_printf(" (%d)\n", ft_printf("P .010: |%.010i|", di));
	printf(" (%d)\n", printf("P .015: |%.015i|", di));
	ft_printf(" (%d)\n", ft_printf("P .015: |%.015i|", di));
	printf(" (%d)\n", printf("P .042: |%.042i|", di));
	ft_printf(" (%d)\n", ft_printf("P .042: |%.042i|", di));

	printf(" (%d)\n", printf("P .007: |%.007i|", di));
	ft_printf(" (%d)\n", ft_printf("P .007: |%.007i|", di));
	printf(" (%d)\n", printf("P .000000000007: |%.000000000007i|", di));
	ft_printf(" (%d)\n", ft_printf("P .000000000007: |%.000000000007i|", di));


	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling precision & minimum field width"));
	ft_printf(" (%d)\n", ft_printf("Handling precision & minimum field width"));

	printf(" (%d)\n", printf("(0) 0.0: |%0.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 0.0: |%0.0i|", 0));
	printf(" (%d)\n", printf("(0) 1.0: |%1.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 1.0: |%1.0i|", 0));
	printf(" (%d)\n", printf("(10) 1.0: |%1.0i|", 10));
	ft_printf(" (%d)\n", ft_printf("(10) 1.0: |%1.0i|", 10));
	printf(" (%d)\n", printf("(0) 0.1: |%0.1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 0.1: |%0.1i|", 0));
	printf(" (%d)\n", printf("(0) 1.1: |%1.1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 1.1: |%1.1i|", 0));
	printf(" (%d)\n", printf("0.0: |%0.0i|", di));
	ft_printf(" (%d)\n", ft_printf("0.0: |%0.0i|", di));
	printf(" (%d)\n", printf("1.1: |%1.1i|", di));
	ft_printf(" (%d)\n", ft_printf("1.1: |%1.1i|", di));
	printf(" (%d)\n", printf("2.2: |%2.2i|", di));
	ft_printf(" (%d)\n", ft_printf("2.2: |%2.2i|", di));
	printf(" (%d)\n", printf("3.2: |%3.2i|", di));
	ft_printf(" (%d)\n", ft_printf("3.2: |%3.2i|", di));
	printf(" (%d)\n", printf("2.3: |%2.3i|", di));
	ft_printf(" (%d)\n", ft_printf("2.3: |%2.3i|", di));
	printf(" (%d)\n", printf("8.4: |%8.4i|", di));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4i|", di));
	printf(" (%d)\n", printf("4.8: |%4.8i|", di));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8i|", di));
	printf(" (%d)\n", printf("8.8: |%8.8i|", di));
	ft_printf(" (%d)\n", ft_printf("8.8: |%8.8i|", di));
	printf(" (%d)\n", printf("8.2: |%8.2i|", di));
	ft_printf(" (%d)\n", ft_printf("8.2: |%8.2i|", di));
	printf(" (%d)\n", printf("2.8: |%2.8i|", di));
	ft_printf(" (%d)\n", ft_printf("2.8: |%2.8i|", di));
	printf(" (%d)\n", printf("8.1: |%8.1i|", di));
	ft_printf(" (%d)\n", ft_printf("8.1: |%8.1i|", di));
	printf(" (%d)\n", printf("1.8: |%1.8i|", di));
	ft_printf(" (%d)\n", ft_printf("1.8: |%1.8i|", di));

	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling precision & minimum field width & 0-flag"));
	ft_printf(" (%d)\n", ft_printf("Handling precision & minimum field width & 0-flag"));

	printf(" (%d)\n", printf("(0) 00.0: |%00.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 00.0: |%00.0i|", 0));
	printf(" (%d)\n", printf("(0) 01.0: |%01.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 01.0: |%01.0i|", 0));
	printf(" (%d)\n", printf("(0) 00.1: |%00.1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 00.1: |%00.1i|", 0));
	printf(" (%d)\n", printf("01.1: |%01.1i|", di));
	ft_printf(" (%d)\n", ft_printf("01.1: |%01.1i|", di));
	printf(" (%d)\n", printf("02.2: |%02.2i|", di));
	ft_printf(" (%d)\n", ft_printf("02.2: |%02.2i|", di));
	printf(" (%d)\n", printf("03.2: |%03.2i|", di));
	ft_printf(" (%d)\n", ft_printf("03.2: |%03.2i|", di));
	printf(" (%d)\n", printf("02.3: |%02.3i|", di));
	ft_printf(" (%d)\n", ft_printf("02.3: |%02.3i|", di));
	printf(" (%d)\n", printf("08.4: |%08.4i|", di));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4i|", di));
	printf(" (%d)\n", printf("04.8: |%04.8i|", di));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8i|", di));
	printf(" (%d)\n", printf("08.8: |%08.8i|", di));
	ft_printf(" (%d)\n", ft_printf("08.8: |%08.8i|", di));
	printf(" (%d)\n", printf("08.2: |%08.2i|", di));
	ft_printf(" (%d)\n", ft_printf("08.2: |%08.2i|", di));
	printf(" (%d)\n", printf("02.8: |%02.8i|", di));
	ft_printf(" (%d)\n", ft_printf("02.8: |%02.8i|", di));
	printf(" (%d)\n", printf("08.1: |%08.1i|", di));
	ft_printf(" (%d)\n", ft_printf("08.1: |%08.1i|", di));
	printf(" (%d)\n", printf("01.8: |%01.8i|", di));
	ft_printf(" (%d)\n", ft_printf("01.8: |%01.8i|", di));

	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling minimum field width & 0-flag"));
	ft_printf(" (%d)\n", ft_printf("Handling minimum field width & 0-flag"));

	printf(" (%d)\n", printf("(0) 0: |%0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 0: |%0i|", 0));
	printf(" (%d)\n", printf("(0) 00: |%00i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 00: |%00i|", 0));
	printf(" (%d)\n", printf("(0) 01: |%01i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 01: |%01i|", 0));
	printf(" (%d)\n", printf("(0) 02: |%02i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) 02: |%02i|", 0));
	printf(" (%d)\n", printf("0: |%0i|", di));
	ft_printf(" (%d)\n", ft_printf("0: |%0i|", di));
	printf(" (%d)\n", printf("00: |%00i|", di));
	ft_printf(" (%d)\n", ft_printf("00: |%00i|", di));
	printf(" (%d)\n", printf("01: |%01i|", di));
	ft_printf(" (%d)\n", ft_printf("01: |%01i|", di));
	printf(" (%d)\n", printf("02: |%02i|", di));
	ft_printf(" (%d)\n", ft_printf("02: |%02i|", di));
	printf(" (%d)\n", printf("03: |%03i|", di));
	ft_printf(" (%d)\n", ft_printf("03: |%03i|", di));
	printf(" (%d)\n", printf("08: |%08i|", di));
	ft_printf(" (%d)\n", ft_printf("08: |%08i|", di));
	printf(" (%d)\n", printf("000000000008: |%000000000008i|", di));
	ft_printf(" (%d)\n", ft_printf("000000000008: |%000000000008i|", di));
	printf(" (%d)\n", printf("010: |%010i|", di));
	ft_printf(" (%d)\n", ft_printf("010: |%010i|", di));
	printf(" (%d)\n", printf("0000000000010: |%0000000000010i|", di));
	ft_printf(" (%d)\n", ft_printf("0000000000010: |%0000000000010i|", di));

	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling minimum field width & 0-flag & '-' flag"));
	ft_printf(" (%d)\n", ft_printf("Handling minimum field width & 0-flag & '-' flag"));

	printf(" (%d)\n", printf("(0) -0: |%-0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -0: |%-0i|", 0));
	printf(" (%d)\n", printf("(0) -00: |%-00i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -00: |%-00i|", 0));
	printf(" (%d)\n", printf("(0) -01: |%-01i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -01: |%-01i|", 0));
	printf(" (%d)\n", printf("(0) -02: |%-02i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -02: |%-02i|", 0));

	printf(" (%d)\n", printf("-0: |%-0i|", di));
	ft_printf(" (%d)\n", ft_printf("-0: |%-0i|", di));
	printf(" (%d)\n", printf("-00: |%-00i|", di));
	ft_printf(" (%d)\n", ft_printf("-00: |%-00i|", di));
	printf(" (%d)\n", printf("-01: |%-01i|", di));
	ft_printf(" (%d)\n", ft_printf("-01: |%-01i|", di));
	printf(" (%d)\n", printf("-02: |%-02i|", di));
	ft_printf(" (%d)\n", ft_printf("-02: |%-02i|", di));
	printf(" (%d)\n", printf("-03: |%-03i|", di));
	ft_printf(" (%d)\n", ft_printf("-03: |%-03i|", di));
	printf(" (%d)\n", printf("-08: |%-08i|", di));
	ft_printf(" (%d)\n", ft_printf("-08: |%-08i|", di));
	printf(" (%d)\n", printf("0-8: |%0-8i|", di));
	ft_printf(" (%d)\n", ft_printf("0-8: |%0-8i|", di));
	printf(" (%d)\n", printf("-000000000008: |%-000000000008i|", di));
	ft_printf(" (%d)\n", ft_printf("-000000000008: |%-000000000008i|", di));
	printf(" (%d)\n", printf("-010: |%-010i|", di));
	ft_printf(" (%d)\n", ft_printf("-010: |%-010i|", di));
	printf(" (%d)\n", printf("-0000000000010: |%-0000000000010i|", di));
	ft_printf(" (%d)\n", ft_printf("-0000000000010: |%-0000000000010i|", di));


	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling minimum field width & left justification"));
	ft_printf(" (%d)\n", ft_printf("Handling minimum field width & left justification"));

	printf(" (%d)\n", printf("(0) Mfw -0: |%-0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Mfw -0: |%-0i|", 0));
	printf(" (%d)\n", printf("(0) Mfw -1: |%-1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Mfw -1: |%-1i|", 0));
	printf(" (%d)\n", printf("(0) Mfw -2: |%-2i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) Mfw -2: |%-2i|", 0));
	
	printf(" (%d)\n", printf("Mfw -0: |%-0i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -0: |%-0i|", di));
	printf(" (%d)\n", printf("Mfw -1: |%-1i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -1: |%-1i|", di));
	printf(" (%d)\n", printf("Mfw -2: |%-2i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -2: |%-2i|", di));
	printf(" (%d)\n", printf("Mfw -3: |%-3i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -3: |%-3i|", di));
	printf(" (%d)\n", printf("Mfw -4: |%-4i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -4: |%-4i|", di));
	printf(" (%d)\n", printf("Mfw -5: |%-5i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -5: |%-5i|", di));
	printf(" (%d)\n", printf("Mfw -10: |%-10i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -10: |%-10i|", di));
	printf(" (%d)\n", printf("Mfw --10: |%--10i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw --10: |%--10i|", di));
	printf(" (%d)\n", printf("Mfw ---10: |%---10i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw ---10: |%---10i|", di));
	printf(" (%d)\n", printf("Mfw -15: |%-15i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -15: |%-15i|", di));
	printf(" (%d)\n", printf("Mfw -42: |%-42i|", di));
	ft_printf(" (%d)\n", ft_printf("Mfw -42: |%-42i|", di));

	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling precision & minimum field width & left justification"));
	ft_printf(" (%d)\n", ft_printf("Handling precision & minimum field width & left justification"));

	printf(" (%d)\n", printf("(0) -0.0: |%-0.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -0.0: |%-0.0i|", 0));
	printf(" (%d)\n", printf("(0) -0.1: |%-0.1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -0.1: |%-0.1i|", 0));
	printf(" (%d)\n", printf("(0) -1.0: |%-1.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -1.0: |%-1.0i|", 0));
	printf(" (%d)\n", printf("(0) -1.1: |%-1.1i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -1.1: |%-1.1i|", 0));
	printf(" (%d)\n", printf("(0) -0.2: |%-0.2i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -0.2: |%-0.2i|", 0));
	printf(" (%d)\n", printf("(0) -2.0: |%-2.0i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -2.0: |%-2.0i|", 0));
	printf(" (%d)\n", printf("(0) -2.2: |%-2.2i|", 0));
	ft_printf(" (%d)\n", ft_printf("(0) -2.2: |%-2.2i|", 0));


	printf(" (%d)\n", printf("-0.0: |%-0.0i|", 5));
	ft_printf(" (%d)\n", ft_printf("-0.0: |%-0.0i|", 5));
	printf(" (%d)\n", printf("-0.1: |%-0.1i|", 5));
	ft_printf(" (%d)\n", ft_printf("-0.1: |%-0.1i|", 5));
	printf(" (%d)\n", printf("-1.0: |%-1.0i|", 5));
	ft_printf(" (%d)\n", ft_printf("-1.0: |%-1.0i|", 5));
	printf(" (%d)\n", printf("-1.1: |%-1.1i|", 5));
	ft_printf(" (%d)\n", ft_printf("-1.1: |%-1.1i|", 5));
	printf(" (%d)\n", printf("-0.2: |%-0.2i|", 5));
	ft_printf(" (%d)\n", ft_printf("-0.2: |%-0.2i|", 5));
	printf(" (%d)\n", printf("-2.0: |%-2.0i|", 5));
	ft_printf(" (%d)\n", ft_printf("-2.0: |%-2.0i|", 5));
	printf(" (%d)\n", printf("-2.2: |%-2.2i|", 5));
	ft_printf(" (%d)\n", ft_printf("-2.2: |%-2.2i|", 5));


	printf(" (%d)\n", printf("-1.1: |%-1.1i|", di));
	ft_printf(" (%d)\n", ft_printf("-1.1: |%-1.1i|", di));
	printf(" (%d)\n", printf("-2.2: |%-2.2i|", di));
	ft_printf(" (%d)\n", ft_printf("-2.2: |%-2.2i|", di));
	printf(" (%d)\n", printf("-3.2: |%-3.2i|", di));
	ft_printf(" (%d)\n", ft_printf("-3.2: |%-3.2i|", di));
	printf(" (%d)\n", printf("-2.3: |%-2.3i|", di));
	ft_printf(" (%d)\n", ft_printf("-2.3: |%-2.3i|", di));
	printf(" (%d)\n", printf("-8.4: |%-8.4i|", di));
	ft_printf(" (%d)\n", ft_printf("-8.4: |%-8.4i|", di));
	printf(" (%d)\n", printf("-4.8: |%-4.8i|", di));
	ft_printf(" (%d)\n", ft_printf("-4.8: |%-4.8i|", di));
	printf(" (%d)\n", printf("-8.8: |%-8.8i|", di));
	ft_printf(" (%d)\n", ft_printf("-8.8: |%-8.8i|", di));
	printf(" (%d)\n", printf("-8.2: |%-8.2i|", di));
	ft_printf(" (%d)\n", ft_printf("-8.2: |%-8.2i|", di));
	printf(" (%d)\n", printf("-2.8: |%-2.8i|", di));
	ft_printf(" (%d)\n", ft_printf("-2.8: |%-2.8i|", di));
	printf(" (%d)\n", printf("-8.1: |%-8.1i|", di));
	ft_printf(" (%d)\n", ft_printf("-8.1: |%-8.1i|", di));
	printf(" (%d)\n", printf("-1.8: |%-1.8i|", di));
	ft_printf(" (%d)\n", ft_printf("-1.8: |%-1.8i|", di));

	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling wildcards"));
	ft_printf(" (%d)\n", ft_printf("Handling wildcards"));

	printf(" (%d)\n", printf("Mfw *1: |%*i|", 1, di));
	ft_printf(" (%d)\n", ft_printf("Mfw *1: |%*i|", 1, di));
	printf(" (%d)\n", printf("Mfw *5: |%*i|", 5, di));
	ft_printf(" (%d)\n", ft_printf("Mfw *5: |%*i|", 5, di));
	printf(" (%d)\n", printf("Mfw *5: |%*i|",5, di));
	ft_printf(" (%d)\n", ft_printf("Mfw *5: |%*i|", 5, di));
	printf(" (%d)\n", printf("Mfw *5: |%*i|",        5, di));
	ft_printf(" (%d)\n", ft_printf("Mfw *5: |%*i|",        5, di));
	printf(" (%d)\n", printf("Mfw *10: |%*i|", 10, di));
	ft_printf(" (%d)\n", ft_printf("Mfw *10: |%*i|", 10, di));
	printf(" (%d)\n", printf("Mfw *i: |%*i|", di, di));
	ft_printf(" (%d)\n", ft_printf("Mfw *i: |%*i|", di, di));
	printf(" (%d)\n", printf("Mfw **5: |%*.*i|", 5, 6, di));
	ft_printf(" (%d)\n", ft_printf("Mfw **5: |%*.*i|", 5, 6, di));
	printf("\n");

	printf(" (%d)\n", printf("Precis. .*1: |%.*i|", 1, di));
	ft_printf(" (%d)\n", ft_printf("Precis. .*1: |%.*i|", 1, di));
	printf(" (%d)\n", printf("Precis. .*5: |%.*i|", 5, di));
	ft_printf(" (%d)\n", ft_printf("Precis. .*5: |%.*i|", 5, di));
	printf(" (%d)\n", printf("Precis. .*5: |%.*i|",5, di));
	ft_printf(" (%d)\n", ft_printf("Precis. .*5: |%.*i|",5, di));
	printf(" (%d)\n", printf("Precis. .*5: |%.*i|",      5, di));
	ft_printf(" (%d)\n", ft_printf("Precis. .*5: |%.*i|",      5, di));
	printf(" (%d)\n", printf("Precis. .*10: |%.*i|", 10, di));
	ft_printf(" (%d)\n", ft_printf("Precis. .*10: |%.*i|", 10, di));
	printf(" (%d)\n", printf("Precis. .*i: |%.*i|", di, di));
	ft_printf(" (%d)\n", ft_printf("Precis. .*i: |%.*i|", di, di));
	printf("\n");

	printf(" (%d)\n", printf("P .*0: |%.*i|", 0, di));
	ft_printf(" (%d)\n", ft_printf("P .*0: |%.*i|", 0, di));
	printf(" (%d)\n", printf("P .*01: |%.*i|", 01, di));
	ft_printf(" (%d)\n", ft_printf("P .*01: |%.*i|", 01, di));
	printf(" (%d)\n", printf("P .*05: |%.*i|", 05, di));
	ft_printf(" (%d)\n", ft_printf("P .*05: |%.*i|", 05, di));
	printf(" (%d)\n", printf("P .*010: |%.*i|", 010, di));
	ft_printf(" (%d)\n", ft_printf("P .*010: |%.*i|", 010, di));
	printf(" (%d)\n", printf("P .*000000000007: |%.*i|", 000000000007, di));
	ft_printf(" (%d)\n", ft_printf("P .*000000000007: |%.*i|", 000000000007, di));
	printf("\n");

	printf(" (%d)\n", printf("*8.4: |%*.4i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("*8.4: |%*.4i|", 8, di));
	printf(" (%d)\n", printf("*4.8: |%*.8i|", 4, di));
	ft_printf(" (%d)\n", ft_printf("*4.8: |%*.8i|", 4, di));
	printf(" (%d)\n", printf("*8.8: |%*.8i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("*8.8: |%*.8i|", 8, di));
	printf("\n");

	printf(" (%d)\n", printf("8.*4: |%8.*i|", 4, di));
	ft_printf(" (%d)\n", ft_printf("8.*4: |%8.*i|", 4, di));
	printf(" (%d)\n", printf("4.*8: |%4.*i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("4.*8: |%4.*i|", 8, di));
	printf(" (%d)\n", printf("8.*8: |%8.*i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("8.*8: |%8.*i|", 8, di));
	printf("\n");

	printf(" (%d)\n", printf("*8.*4: |%*.*i|", 8, 4, di));
	ft_printf(" (%d)\n", ft_printf("*8.*4: |%*.*i|", 8, 4, di));
	printf(" (%d)\n", printf("*4.*8: |%*.*i|", 4, 8, di));
	ft_printf(" (%d)\n", ft_printf("*4.*8: |%*.*i|", 4, 8, di));
	printf(" (%d)\n", printf("*8.*8: |%*.*i|", 8, 8, di));
	ft_printf(" (%d)\n", ft_printf("*8.*8: |%*.*i|", 8, 8, di));
	printf("\n");

	printf("WEIRD OCTAL SHIT:\n");
	printf(" (%d)\n", printf("*010.*4: |%*.*i|", 010, 4, di));
	ft_printf(" (%d)\n", ft_printf("*010.*4: |%*.*i|", 010, 4, di));
	printf(" (%d)\n", printf(" *07.*4: |%*.*i|", 07, 4, di));
	ft_printf(" (%d)\n", ft_printf(" *07.*4: |%*.*i|", 07, 4, di));
	printf(" (%d)\n", printf("*4.*010: |%*.*i|", 4, 010, di));
	ft_printf(" (%d)\n", ft_printf("*4.*010: |%*.*i|", 4, 010, di));
	printf("WEIRD HEXADECIMAL SHIT:\n");
	printf(" (%d)\n", printf("*0x10.*4: |%*.*i|", 0x10, 4, di));
	ft_printf(" (%d)\n", ft_printf("*0x10.*4: |%*.*i|", 0x10, 4, di));
	printf(" (%d)\n", printf(" *0x7.*4: |%*.*i|", 0x7, 4, di));
	ft_printf(" (%d)\n", ft_printf(" *0x7.*4: |%*.*i|", 0x7, 4, di));
	printf(" (%d)\n", printf("*4.*0x10: |%*.*i|", 4, 0x10, di));
	ft_printf(" (%d)\n", ft_printf("*4.*0x10: |%*.*i|", 4, 0x10, di));
	printf("\n");



	//int octal;

	// octal = 010;
	// octal--;

	// printf("octal: %i\n", octal);

	// //this will print 7.

	// int octal_atoi;

	// octal_atoi = atoi("010");
	// octal_atoi--;

	// printf("octal: %i\n", octal_atoi);

	// //this will print 9.

	// int hexadec;

	// hexadec = 0x10;
	// hexadec--;

	// printf("hexadec: %i\n", hexadec);

	// //this will print 15.

	// int hexadec_atoi;

	// hexadec_atoi = atoi("0x10");
	// hexadec_atoi--;

	// printf("hexadec: %i\n", hexadec_atoi);

	// //this will print -1.

	printf(" (%d)\n", printf("*8: |%*i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("*8: |%*i|", 8, di));
	printf(" (%d)\n", printf("0*8: |%0*i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("0*8: |%0*i|", 8, di));
	printf(" (%d)\n", printf("00*8: |%00*i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("00*8: |%00*i|", 8, di));
	printf(" (%d)\n", printf("00000000000000000*8: |%00000000000000000*i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("00000000000000000*8: |%00000000000000000*i|", 8, di));
	printf("\n");


	printf(" (%d)\n", printf("-*0: |%-*i|", 0, di));
	ft_printf(" (%d)\n", ft_printf("-*0: |%-*i|", 0, di));
	printf(" (%d)\n", printf("-0*0: |%-0*i|", 0, di));
	ft_printf(" (%d)\n", ft_printf("-0*0: |%-0*i|", 0, di));
	printf(" (%d)\n", printf("-0*3: |%-0*i|", 3, di));
	ft_printf(" (%d)\n", ft_printf("-0*3: |%-0*i|", 3, di));
	printf(" (%d)\n", printf("*(-03): |%*i|", -03, di));
	ft_printf(" (%d)\n", ft_printf("*(-03): |%*i|", -03, di));
	printf(" (%d)\n", printf("*(-5): |%*i|", -5, di));
	ft_printf(" (%d)\n", ft_printf("*(-5): |%*i|", -5, di));
	printf(" (%d)\n", printf("*(5): |%*i|", 5, di));
	ft_printf(" (%d)\n", ft_printf("*(5): |%*i|", 5, di));
	printf(" (%d)\n", printf("-*(5): |%-*i|", 5, di));
	ft_printf(" (%d)\n", ft_printf("-*(5): |%-*i|", 5, di));
	printf(" (%d)\n", printf("-8.4: |%-8.4i|", di));
	ft_printf(" (%d)\n", ft_printf("-8.4: |%-8.4i|", di));
	printf(" (%d)\n", printf("-*(8).4: |%-*.4i|", 8, di));
	ft_printf(" (%d)\n", ft_printf("-*(8).4: |%-*.4i|", 8, di));
	printf(" (%d)\n", printf("*(-8).4: |%*.4i|", -8, di));
	ft_printf(" (%d)\n", ft_printf("*(-8).4: |%*.4i|", -8, di));
	printf(" (%d)\n", printf("*(int)(-8.4): |%*i|", ((int)(-8.4)), di));
	ft_printf(" (%d)\n", ft_printf("*(int)(-8.4): |%*i|", ((int)(-8.4)), di));
	printf(" (%d)\n", printf("-8.*(4): |%-8.*i|", 4, di));
	ft_printf(" (%d)\n", ft_printf("-8.*(4): |%-8.*i|", 4, di));
	printf(" (%d)\n", printf("-8.*(-4): |%-8.*i|", -4, di));
	ft_printf(" (%d)\n", ft_printf("-8.*(-4): |%-8.*i|", -4, di));
	printf(" (%d)\n", printf("-8.*(0): |%-8.*i|", 0, di));
	ft_printf(" (%d)\n", ft_printf("-8.*(0): |%-8.*i|", 0, di));
	printf(" (%d)\n", printf("-8.*(1): |%-8.*i|", 1, di));
	ft_printf(" (%d)\n", ft_printf("-8.*(1): |%-8.*i|", 1, di));
	printf(" (%d)\n", printf("-*(0).4: |%-*.4i|", 0, di));
	ft_printf(" (%d)\n", ft_printf("-*(0).4: |%-*.4i|", 0, di));
	printf(" (%d)\n", printf("*(0).4: |%*.4i|", 0, di));
	ft_printf(" (%d)\n", ft_printf("*(0).4: |%*.4i|", 0, di));


	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Handling wildcards with 0 as input"));
	ft_printf(" (%d)\n", ft_printf("Handling wildcards with 0 as input"));
	printf("\n");

	printf(" (%d)\n", printf("Mfw *1: |%*i|", 1, 0));
	ft_printf(" (%d)\n", ft_printf("Mfw *1: |%*i|", 1, 0));
	printf(" (%d)\n", printf("Mfw *5: |%*i|", 5, 0));
	ft_printf(" (%d)\n", ft_printf("Mfw *5: |%*i|", 5, 0));
	printf(" (%d)\n", printf("Mfw *5: |%*i|",5, 0));
	ft_printf(" (%d)\n", ft_printf("Mfw *5: |%*i|", 5, 0));
	printf(" (%d)\n", printf("Mfw *5: |%*i|",        5, 0));
	ft_printf(" (%d)\n", ft_printf("Mfw *5: |%*i|",        5, 0));
	printf(" (%d)\n", printf("Mfw *10: |%*i|", 10, 0));
	ft_printf(" (%d)\n", ft_printf("Mfw *10: |%*i|", 10, 0));
	printf(" (%d)\n", printf("Mfw *i: |%*i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("Mfw *i: |%*i|", 0, 0));
	printf("\n");

	printf(" (%d)\n", printf("Precis. .*1: |%.*i|", 1, 0));
	ft_printf(" (%d)\n", ft_printf("Precis. .*1: |%.*i|", 1, 0));
	printf(" (%d)\n", printf("Precis. .*5: |%.*i|", 5, 0));
	ft_printf(" (%d)\n", ft_printf("Precis. .*5: |%.*i|", 5, 0));
	printf(" (%d)\n", printf("Precis. .*5: |%.*i|",5, 0));
	ft_printf(" (%d)\n", ft_printf("Precis. .*5: |%.*i|",5, 0));
	printf(" (%d)\n", printf("Precis. .*5: |%.*i|",      5, 0));
	ft_printf(" (%d)\n", ft_printf("Precis. .*5: |%.*i|",      5, 0));
	printf(" (%d)\n", printf("Precis. .*10: |%.*i|", 10, 0));
	ft_printf(" (%d)\n", ft_printf("Precis. .*10: |%.*i|", 10, 0));
	printf(" (%d)\n", printf("Precis. .*i: |%.*i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("Precis. .*i: |%.*i|", 0, 0));
	printf("\n");

	printf(" (%d)\n", printf("P .*0: |%.*i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("P .*0: |%.*i|", 0, 0));
	printf(" (%d)\n", printf("P .*01: |%.*i|", 01, 0));
	ft_printf(" (%d)\n", ft_printf("P .*01: |%.*i|", 01, 0));
	printf(" (%d)\n", printf("P .*05: |%.*i|", 05, 0));
	ft_printf(" (%d)\n", ft_printf("P .*05: |%.*i|", 05, 0));
	printf(" (%d)\n", printf("P .*010: |%.*i|", 010, 0));
	ft_printf(" (%d)\n", ft_printf("P .*010: |%.*i|", 010, 0));
	printf(" (%d)\n", printf("P .*000000000007: |%.*i|", 000000000007, 0));
	ft_printf(" (%d)\n", ft_printf("P .*000000000007: |%.*i|", 000000000007, 0));
	printf("\n");

	printf(" (%d)\n", printf("*8.4: |%*.4i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("*8.4: |%*.4i|", 8, 0));
	printf(" (%d)\n", printf("*4.8: |%*.8i|", 4, 0));
	ft_printf(" (%d)\n", ft_printf("*4.8: |%*.8i|", 4, 0));
	printf(" (%d)\n", printf("*8.8: |%*.8i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("*8.8: |%*.8i|", 8, 0));
	printf("\n");

	printf(" (%d)\n", printf("8.*4: |%8.*i|", 4, 0));
	ft_printf(" (%d)\n", ft_printf("8.*4: |%8.*i|", 4, 0));
	printf(" (%d)\n", printf("4.*8: |%4.*i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("4.*8: |%4.*i|", 8, 0));
	printf(" (%d)\n", printf("8.*8: |%8.*i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("8.*8: |%8.*i|", 8, 0));
	printf("\n");

	printf(" (%d)\n", printf("*8.*4: |%*.*i|", 8, 4, 0));
	ft_printf(" (%d)\n", ft_printf("*8.*4: |%*.*i|", 8, 4, 0));
	printf(" (%d)\n", printf("*4.*8: |%*.*i|", 4, 8, 0));
	ft_printf(" (%d)\n", ft_printf("*4.*8: |%*.*i|", 4, 8, 0));
	printf(" (%d)\n", printf("*8.*8: |%*.*i|", 8, 8, 0));
	ft_printf(" (%d)\n", ft_printf("*8.*8: |%*.*i|", 8, 8, 0));
	printf("\n");

	printf("WEIRD OCTAL SHIT:\n");
	printf(" (%d)\n", printf("*010.*4: |%*.*i|", 010, 4, 0));
	ft_printf(" (%d)\n", ft_printf("*010.*4: |%*.*i|", 010, 4, 0));
	printf(" (%d)\n", printf(" *07.*4: |%*.*i|", 07, 4, 0));
	ft_printf(" (%d)\n", ft_printf(" *07.*4: |%*.*i|", 07, 4, 0));
	printf(" (%d)\n", printf("*4.*010: |%*.*i|", 4, 010, 0));
	ft_printf(" (%d)\n", ft_printf("*4.*010: |%*.*i|", 4, 010, 0));
	printf("WEIRD HEXADECIMAL SHIT:\n");
	printf(" (%d)\n", printf("*0x10.*4: |%*.*i|", 0x10, 4, 0));
	ft_printf(" (%d)\n", ft_printf("*0x10.*4: |%*.*i|", 0x10, 4, 0));
	printf(" (%d)\n", printf(" *0x7.*4: |%*.*i|", 0x7, 4, 0));
	ft_printf(" (%d)\n", ft_printf(" *0x7.*4: |%*.*i|", 0x7, 4, 0));
	printf(" (%d)\n", printf("*4.*0x10: |%*.*i|", 4, 0x10, 0));
	ft_printf(" (%d)\n", ft_printf("*4.*0x10: |%*.*i|", 4, 0x10, 0));
	printf("\n");



	//int octal;

	// octal = 010;
	// octal--;

	// printf("octal: %i\n", octal);

	// //this will print 7.

	// int octal_atoi;

	// octal_atoi = atoi("010");
	// octal_atoi--;

	// printf("octal: %i\n", octal_atoi);

	// //this will print 9.

	// int hexadec;

	// hexadec = 0x10;
	// hexadec--;

	// printf("hexadec: %i\n", hexadec);

	// //this will print 15.

	// int hexadec_atoi;

	// hexadec_atoi = atoi("0x10");
	// hexadec_atoi--;

	// printf("hexadec: %i\n", hexadec_atoi);

	// //this will print -1.

	printf(" (%d)\n", printf("*8: |%*i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("*8: |%*i|", 8, 0));
	printf(" (%d)\n", printf("0*8: |%0*i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("0*8: |%0*i|", 8, 0));
	printf(" (%d)\n", printf("00*8: |%00*i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("00*8: |%00*i|", 8, 0));
	printf(" (%d)\n", printf("00000000000000000*8: |%00000000000000000*i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("00000000000000000*8: |%00000000000000000*i|", 8, 0));
	printf("\n");


	printf(" (%d)\n", printf("-*0: |%-*i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("-*0: |%-*i|", 0, 0));
	printf(" (%d)\n", printf("-0*0: |%-0*i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("-0*0: |%-0*i|", 0, 0));
	printf(" (%d)\n", printf("-0*3: |%-0*i|", 3, 0));
	ft_printf(" (%d)\n", ft_printf("-0*3: |%-0*i|", 3, 0));
	printf(" (%d)\n", printf("*(-03): |%*i|", -03, 0));
	ft_printf(" (%d)\n", ft_printf("*(-03): |%*i|", -03, 0));
	printf(" (%d)\n", printf("*(-5): |%*i|", -5, 0));
	ft_printf(" (%d)\n", ft_printf("*(-5): |%*i|", -5, 0));
	printf(" (%d)\n", printf("*(5): |%*i|", 5, 0));
	ft_printf(" (%d)\n", ft_printf("*(5): |%*i|", 5, 0));
	printf(" (%d)\n", printf("-*(5): |%-*i|", 5, 0));
	ft_printf(" (%d)\n", ft_printf("-*(5): |%-*i|", 5, 0));
	printf(" (%d)\n", printf("-8.4: |%-8.4i|", 0));
	ft_printf(" (%d)\n", ft_printf("-8.4: |%-8.4i|", 0));
	printf(" (%d)\n", printf("-*(8).4: |%-*.4i|", 8, 0));
	ft_printf(" (%d)\n", ft_printf("-*(8).4: |%-*.4i|", 8, 0));
	printf(" (%d)\n", printf("*(-8).4: |%*.4i|", -8, 0));
	ft_printf(" (%d)\n", ft_printf("*(-8).4: |%*.4i|", -8, 0));
	printf(" (%d)\n", printf("*(int)(-8.4): |%*i|", ((int)(-8.4)), 0));
	ft_printf(" (%d)\n", ft_printf("*(int)(-8.4): |%*i|", ((int)(-8.4)), 0));
	printf(" (%d)\n", printf("-8.*(4): |%-8.*i|", 4, 0));
	ft_printf(" (%d)\n", ft_printf("-8.*(4): |%-8.*i|", 4, 0));
	printf(" (%d)\n", printf("-8.*(-4): |%-8.*i|", -4, 0));
	ft_printf(" (%d)\n", ft_printf("-8.*(-4): |%-8.*i|", -4, 0));
	printf(" (%d)\n", printf("-8.*(0): |%-8.*i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("-8.*(0): |%-8.*i|", 0, 0));
	printf(" (%d)\n", printf("-8.*(1): |%-8.*i|", 1, 0));
	ft_printf(" (%d)\n", ft_printf("-8.*(1): |%-8.*i|", 1, 0));
	printf(" (%d)\n", printf("-*(0).4: |%-*.4i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("-*(0).4: |%-*.4i|", 0, 0));
	printf(" (%d)\n", printf("*(0).4: |%*.4i|", 0, 0));
	ft_printf(" (%d)\n", ft_printf("*(0).4: |%*.4i|", 0, 0));


	
	printf("\n------------------------------------------------------------------------------------------------\n\n");
	printf(" (%d)\n", printf("Peer evaluation tests:"));
	ft_printf(" (%d)\n", ft_printf("Peer evaluation tests:"));
	
	printf(" (%d)\n", printf("empty string: |%s|", ""));
	ft_printf(" (%d)\n", ft_printf("empty string: |%s|", ""));
	printf(" (%d)\n", printf("null pointer: |%p|", ((void *)(0))));
	ft_printf(" (%d)\n", ft_printf("null pointer: |%p|", ((void *)(0))));
	printf(" (%d)\n", printf("null pointer .p: |%.p|", ((void *)(0))));
	ft_printf(" (%d)\n", ft_printf("null pointer .p: |%.p|", ((void *)(0))));
	printf(" (%d)\n", printf("null pointer -5.p: |%-5.p|", ((void *)(0))));
	ft_printf(" (%d)\n", ft_printf("null pointer -5.p: |%-5.p|", ((void *)(0))));
	printf(" (%d)\n", printf("null pointer 1.p: |%1.p|", ((void *)(0))));
	ft_printf(" (%d)\n", ft_printf("null pointer 1.p: |%1.p|", ((void *)(0))));

	printf(" (%d)\n", printf("S Mfw 10: |%10s|", "abcdefgh90"));
	ft_printf(" (%d)\n", ft_printf("S Mfw 10: |%10s|", "abcdefgh90"));
	printf(" (%d)\n", printf("S Mfw 10: |%10s|", "abcdefgh"));
	ft_printf(" (%d)\n", ft_printf("S Mfw 10: |%10s|", "abcdefgh"));
	printf(" (%d)\n", printf("S Mfw 10: |%10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 10: |%10s|", ""));

	printf(" (%d)\n", printf("S Mfw -10: |%-10s|", "abcdefgh90"));
	ft_printf(" (%d)\n", ft_printf("S Mfw -10: |%-10s|", "abcdefgh90"));
	printf(" (%d)\n", printf("S Mfw -10: |%-10s|", "abcdefgh"));
	ft_printf(" (%d)\n", ft_printf("S Mfw -10: |%-10s|", "abcdefgh"));
	printf(" (%d)\n", printf("S Mfw -10: |%-10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -10: |%-10s|", ""));

	printf(" (%d)\n", printf("S Mfw 010: |%010s|", "abcdefgh90"));
	ft_printf(" (%d)\n", ft_printf("S Mfw 010: |%010s|", "abcdefgh90"));
	printf(" (%d)\n", printf("S Mfw 010: |%010s|", "abcdefgh"));
	ft_printf(" (%d)\n", ft_printf("S Mfw 010: |%010s|", "abcdefgh"));
	printf(" (%d)\n", printf("S Mfw 010: |%010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 010: |%010s|", ""));

	printf(" (%d)\n", printf("S Mfw -010: |%-010s|", "abcdefgh90"));
	ft_printf(" (%d)\n", ft_printf("S Mfw -010: |%-010s|", "abcdefgh90"));
	printf(" (%d)\n", printf("S Mfw -010: |%-010s|", "abcdefgh"));
	ft_printf(" (%d)\n", ft_printf("S Mfw -010: |%-010s|", "abcdefgh"));
	printf(" (%d)\n", printf("S Mfw -010: |%-010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -010: |%-010s|", ""));

	printf(" (%d)\n", printf("8.*(-5): |%8.*s|", -5, "abcdefgh9"));
	ft_printf(" (%d)\n", ft_printf("8.*(-5): |%8.*s|", -5, "abcdefgh9"));
	printf(" (%d)\n", printf("8.0: |%8.0s|", "abcdefgh9"));
	ft_printf(" (%d)\n", ft_printf("8.0: |%8.0s|", "abcdefgh9"));
	printf(" (%d)\n", printf("8.1: |%8.1s|", "abcdefgh9"));
	ft_printf(" (%d)\n", ft_printf("8.1: |%8.1s|", "abcdefgh9"));
	printf(" (%d)\n", printf("8.4: |%8.4s|", "abcdefgh9"));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", "abcdefgh9"));
	printf(" (%d)\n", printf("4.8: |%4.8s|", "abcdefgh9"));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", "abcdefgh9"));
	printf(" (%d)\n", printf("8.4: |%8.4s|", "abc"));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", "abc"));
	printf(" (%d)\n", printf("4.8: |%4.8s|", "abc"));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", "abc"));
	printf(" (%d)\n", printf("8.4: |%8.4s|", "abc4567890"));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", "abc4567890"));
	printf(" (%d)\n", printf("4.8: |%4.8s|", "abc4567890"));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", "abc4567890"));
	printf(" (%d)\n", printf("8.4: |%8.4s|", "0"));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", "0"));
	printf(" (%d)\n", printf("4.8: |%4.8s|", "0"));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", "0"));

	printf(" (%d)\n", printf("08.4: |%08.4s|", "abc45678"));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", "abc45678"));
	printf(" (%d)\n", printf("04.8: |%04.8s|", "abc45678"));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", "abc45678"));
	printf(" (%d)\n", printf("08.4: |%08.4s|", "abc"));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", "abc"));
	printf(" (%d)\n", printf("04.8: |%04.8s|", "abc"));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", "abc"));
	printf(" (%d)\n", printf("08.4: |%08.4s|", "abc4567890"));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", "abc4567890"));
	printf(" (%d)\n", printf("04.8: |%04.8s|", "abc4567890"));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", "abc4567890"));
	printf(" (%d)\n", printf("08.4: |%08.4s|", "0"));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", "0"));
	printf(" (%d)\n", printf("04.8: |%04.8s|", "0"));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", "0"));

	printf(" (%d)\n", printf("-08.4: |%-08.4s|", "abc45678"));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", "abc45678"));
	printf(" (%d)\n", printf("-09.4: |%-09.4s|", "abc45678"));
	ft_printf(" (%d)\n", ft_printf("-09.4: |%-09.4s|", "abc45678"));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", "abc45678"));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", "abc45678"));
	printf(" (%d)\n", printf("-04.9: |%-04.9s|", "abc45678"));
	ft_printf(" (%d)\n", ft_printf("-04.9: |%-04.9s|", "abc45678"));
	printf(" (%d)\n", printf("-08.4: |%-08.4s|", "abc"));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", "abc"));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", "abc"));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", "abc"));
	printf(" (%d)\n", printf("-08.4: |%-08.4s|", "abc4567890"));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", "abc4567890"));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", "abc4567890"));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", "abc4567890"));
	printf(" (%d)\n", printf("-08.4: |%-08.4s|", "0"));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", "0"));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", "0"));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", "0"));


	printf(" (%d)\n", printf("*8.4: |%*.4s|", 8, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*8.4: |%*.4s|", 8, "0abc45"));
	printf(" (%d)\n", printf("*4.8: |%*.8s|", 4, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*4.8: |%*.8s|", 4, "0abc45"));
	printf(" (%d)\n", printf("*8.8: |%*.8s|", 8, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*8.8: |%*.8s|", 8, "0abc45"));

	printf(" (%d)\n", printf("*8.4: |%8.*s|", 4, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*8.4: |%8.*s|", 4, "0abc45"));
	printf(" (%d)\n", printf("*4.8: |%4.*s|", 8, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*4.8: |%4.*s|", 8, "0abc45"));
	printf(" (%d)\n", printf("*8.8: |%8.*s|", 8, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*8.8: |%8.*s|", 8, "0abc45"));

	printf(" (%d)\n", printf("*8.*4: |%*.*s|", 8, 4, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*8.*4: |%*.*s|", 8, 4, "0abc45"));
	printf(" (%d)\n", printf("*4.*8: |%*.*s|", 4, 8, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*4.*8: |%*.*s|", 4, 8, "0abc45"));
	printf(" (%d)\n", printf("*8.*8: |%*.*s|", 8, 8, "0abc45"));
	ft_printf(" (%d)\n", ft_printf("*8.*8: |%*.*s|", 8, 8, "0abc45"));




	printf(" (%d)\n", printf("*(-1).8: |%*.8s|", -1, "0abc"));
	ft_printf(" (%d)\n", ft_printf("*(-1).8: |%*.8s|", -1, "0abc"));
	printf(" (%d)\n", printf("8.(-1): |%8.*s|", -1, "0abc"));
	ft_printf(" (%d)\n", ft_printf("8.(-1): |%8.*s|", -1, "0abc"));
	printf(" (%d)\n", printf("*(-1).*(-1): |%*.*s|", -1, -1, "0abc"));
	ft_printf(" (%d)\n", ft_printf("*(-1).*(-1): |%*.*s|", -1, -1, "0abc"));
	printf(" (%d)\n", printf("*0.8: |%*.8s|", 0, "0abc"));
	ft_printf(" (%d)\n", ft_printf("*0.8: |%*.8s|", 0, "0abc"));
	printf(" (%d)\n", printf("8.*0: |%8.*s|", 0, "0abc"));
	ft_printf(" (%d)\n", ft_printf("8.*0: |%8.*s|", 0, "0abc"));
	printf(" (%d)\n", printf("*0.*0: |%*.*s|", 0, 0, "0abc"));
	ft_printf(" (%d)\n", ft_printf("*0.*0: |%*.*s|", 0, 0, "0abc"));
	printf(" (%d)\n", printf("*1.*0: |%*.*s|", 1, 0, "0abc"));
	ft_printf(" (%d)\n", ft_printf("*1.*0: |%*.*s|", 1, 0, "0abc"));
	printf(" (%d)\n", printf("*0.*1: |%*.*s|", 0, 1, "0abc"));
	ft_printf(" (%d)\n", ft_printf("*0.*1: |%*.*s|", 0, 1, "0abc"));

	printf("\n------------------------------------------------------------------------------------------------\n\n");
	printf(" (%d)\n", printf("Mucking about with empty strings:"));
	ft_printf(" (%d)\n", ft_printf("Mucking about with empty strings:"));

	printf(" (%d)\n", printf("S Mfw 10: |%10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 10: |%10s|", ""));
	printf(" (%d)\n", printf("S Mfw 10: |%10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 10: |%10s|", ""));
	printf(" (%d)\n", printf("S Mfw 10: |%10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 10: |%10s|", ""));

	printf(" (%d)\n", printf("S Mfw -10: |%-10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -10: |%-10s|", ""));
	printf(" (%d)\n", printf("S Mfw -10: |%-10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -10: |%-10s|", ""));
	printf(" (%d)\n", printf("S Mfw -10: |%-10s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -10: |%-10s|", ""));

	printf(" (%d)\n", printf("S Mfw 010: |%010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 010: |%010s|", ""));
	printf(" (%d)\n", printf("S Mfw 010: |%010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 010: |%010s|", ""));
	printf(" (%d)\n", printf("S Mfw 010: |%010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw 010: |%010s|", ""));

	printf(" (%d)\n", printf("S Mfw -010: |%-010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -010: |%-010s|", ""));
	printf(" (%d)\n", printf("S Mfw -010: |%-010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -010: |%-010s|", ""));
	printf(" (%d)\n", printf("S Mfw -010: |%-010s|", ""));
	ft_printf(" (%d)\n", ft_printf("S Mfw -010: |%-010s|", ""));

	printf(" (%d)\n", printf("8.4: |%8.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", ""));
	printf(" (%d)\n", printf("4.8: |%4.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", ""));
	printf(" (%d)\n", printf("8.4: |%8.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", ""));
	printf(" (%d)\n", printf("4.8: |%4.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", ""));
	printf(" (%d)\n", printf("8.4: |%8.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", ""));
	printf(" (%d)\n", printf("4.8: |%4.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", ""));
	printf(" (%d)\n", printf("8.4: |%8.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("8.4: |%8.4s|", ""));
	printf(" (%d)\n", printf("4.8: |%4.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("4.8: |%4.8s|", ""));

	printf(" (%d)\n", printf("08.4: |%08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", ""));
	printf(" (%d)\n", printf("04.8: |%04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", ""));
	printf(" (%d)\n", printf("08.4: |%08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", ""));
	printf(" (%d)\n", printf("04.8: |%04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", ""));
	printf(" (%d)\n", printf("08.4: |%08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", ""));
	printf(" (%d)\n", printf("04.8: |%04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", ""));
	printf(" (%d)\n", printf("08.4: |%08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("08.4: |%08.4s|", ""));
	printf(" (%d)\n", printf("04.8: |%04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("04.8: |%04.8s|", ""));

	printf(" (%d)\n", printf("-08.4: |%-08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", ""));
	printf(" (%d)\n", printf("-09.4: |%-09.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("-09.4: |%-09.4s|", ""));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", ""));
	printf(" (%d)\n", printf("-04.9: |%-04.9s|", ""));
	ft_printf(" (%d)\n", ft_printf("-04.9: |%-04.9s|", ""));
	printf(" (%d)\n", printf("-08.4: |%-08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", ""));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", ""));
	printf(" (%d)\n", printf("-08.4: |%-08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", ""));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", ""));
	printf(" (%d)\n", printf("-08.4: |%-08.4s|", ""));
	ft_printf(" (%d)\n", ft_printf("-08.4: |%-08.4s|", ""));
	printf(" (%d)\n", printf("-04.8: |%-04.8s|", ""));
	ft_printf(" (%d)\n", ft_printf("-04.8: |%-04.8s|", ""));


	printf(" (%d)\n", printf("*8.4: |%*.4s|", 8, ""));
	ft_printf(" (%d)\n", ft_printf("*8.4: |%*.4s|", 8, ""));
	printf(" (%d)\n", printf("*4.8: |%*.8s|", 4, ""));
	ft_printf(" (%d)\n", ft_printf("*4.8: |%*.8s|", 4, ""));
	printf(" (%d)\n", printf("*8.8: |%*.8s|", 8, ""));
	ft_printf(" (%d)\n", ft_printf("*8.8: |%*.8s|", 8, ""));

	printf(" (%d)\n", printf("*8.4: |%8.*s|", 4, ""));
	ft_printf(" (%d)\n", ft_printf("*8.4: |%8.*s|", 4, ""));
	printf(" (%d)\n", printf("*4.8: |%4.*s|", 8, ""));
	ft_printf(" (%d)\n", ft_printf("*4.8: |%4.*s|", 8, ""));
	printf(" (%d)\n", printf("*8.8: |%8.*s|", 8, ""));
	ft_printf(" (%d)\n", ft_printf("*8.8: |%8.*s|", 8, ""));

	printf(" (%d)\n", printf("*8.*4: |%*.*s|", 8, 4, ""));
	ft_printf(" (%d)\n", ft_printf("*8.*4: |%*.*s|", 8, 4, ""));
	printf(" (%d)\n", printf("*4.*8: |%*.*s|", 4, 8, ""));
	ft_printf(" (%d)\n", ft_printf("*4.*8: |%*.*s|", 4, 8, ""));
	printf(" (%d)\n", printf("*8.*8: |%*.*s|", 8, 8, ""));
	ft_printf(" (%d)\n", ft_printf("*8.*8: |%*.*s|", 8, 8, ""));




	printf(" (%d)\n", printf("*(-1).8: |%*.8s|", -1, ""));
	ft_printf(" (%d)\n", ft_printf("*(-1).8: |%*.8s|", -1, ""));
	printf(" (%d)\n", printf("8.(-1): |%8.*s|", -1, ""));
	ft_printf(" (%d)\n", ft_printf("8.(-1): |%8.*s|", -1, ""));
	printf(" (%d)\n", printf("*(-1).*(-1): |%*.*s|", -1, -1, ""));
	ft_printf(" (%d)\n", ft_printf("*(-1).*(-1): |%*.*s|", -1, -1, ""));
	printf(" (%d)\n", printf("*0.8: |%*.8s|", 0, ""));
	ft_printf(" (%d)\n", ft_printf("*0.8: |%*.8s|", 0, ""));
	printf(" (%d)\n", printf("8.*0: |%8.*s|", 0, ""));
	ft_printf(" (%d)\n", ft_printf("8.*0: |%8.*s|", 0, ""));
	printf(" (%d)\n", printf("*0.*0: |%*.*s|", 0, 0, ""));
	ft_printf(" (%d)\n", ft_printf("*0.*0: |%*.*s|", 0, 0, ""));
	printf(" (%d)\n", printf("*1.*0: |%*.*s|", 1, 0, ""));
	ft_printf(" (%d)\n", ft_printf("*1.*0: |%*.*s|", 1, 0, ""));
	printf(" (%d)\n", printf("*0.*1: |%*.*s|", 0, 1, ""));
	ft_printf(" (%d)\n", ft_printf("*0.*1: |%*.*s|", 0, 1, ""));


	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Negative ints are a pain:"));
	ft_printf(" (%d)\n", ft_printf("Negative ints are a pain:"));
	printf("\n");

	int negative_int = -1994;


	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -08.7i |%-08.7i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -08.7i |%-08.7i|", negative_int));
	printf(" (%i)\n", printf("Sometimes it struggles with negative ints: -08.10i |%-08.10i|", negative_int));
	ft_printf(" (%i)\n", ft_printf("Sometimes it struggles with negative ints: -08.10i |%-08.10i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -8.7i |%-8.7i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -8.7i |%-8.7i|", negative_int));
	printf(" (%i)\n", printf("Sometimes it struggles with negative ints: -8.10i |%-8.10i|", negative_int));
	ft_printf(" (%i)\n", ft_printf("Sometimes it struggles with negative ints: -8.10i |%-8.10i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 8.7i |%8.7i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 8.7i |%8.7i|", negative_int));
	printf(" (%i)\n", printf("Sometimes it struggles with negative ints: 8.10i |%8.10i|", negative_int));
	ft_printf(" (%i)\n", ft_printf("Sometimes it struggles with negative ints: 8.10i |%8.10i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 08i |%08i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 08i |%08i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 8i |%8i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 8i |%8i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 8.5i |%8.5i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 8.5i |%8.5i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -8i |%-8i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -8i |%-8i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -8.5i |%-8.5i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -8.5i |%-8.5i|", negative_int));
	printf("\n");

	negative_int = -2147483648;

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -08.7i |%-08.7i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -08.7i |%-08.7i|", negative_int));
	printf(" (%i)\n", printf("Sometimes it struggles with negative ints: -08.10i |%-08.10i|", negative_int));
	ft_printf(" (%i)\n", ft_printf("Sometimes it struggles with negative ints: -08.10i |%-08.10i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -8.7i |%-8.7i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -8.7i |%-8.7i|", negative_int));
	printf(" (%i)\n", printf("Sometimes it struggles with negative ints: -8.10i |%-8.10i|", negative_int));
	ft_printf(" (%i)\n", ft_printf("Sometimes it struggles with negative ints: -8.10i |%-8.10i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 8.7i |%8.7i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 8.7i |%8.7i|", negative_int));
	printf(" (%i)\n", printf("Sometimes it struggles with negative ints: 8.10i |%8.10i|", negative_int));
	ft_printf(" (%i)\n", ft_printf("Sometimes it struggles with negative ints: 8.10i |%8.10i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 08i |%08i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 08i |%08i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 8i |%8i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 8i |%8i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: 8.5i |%8.5i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: 8.5i |%8.5i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -8i |%-8i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -8i |%-8i|", negative_int));
	printf("\n");

	printf(" (%d)\n", printf("Sometimes it struggles with negative ints: -8.5i |%-8.5i|", negative_int));
	ft_printf(" (%d)\n", ft_printf("Sometimes it struggles with negative ints: -8.5i |%-8.5i|", negative_int));
	printf("\n");


	printf("\n------------------------------------------------------------------------------------------------\n\n");

	printf(" (%d)\n", printf("Some edge cases I thought of whilst writing:"));
	ft_printf(" (%d)\n", ft_printf("Some edge cases I thought of whilst writing:"));
	printf("\n");

	printf(" (%d)\n", printf("8.: |%8.i|", 0));
	ft_printf(" (%d)\n", ft_printf("8.: |%8.i|", 0));
	printf(" (%d)\n", printf("8.: |%8.i|", 42));
	ft_printf(" (%d)\n", ft_printf("8.: |%8.i|", 42));
	printf(" (%d)\n", printf("8.: |%8.s|", "test"));
	ft_printf(" (%d)\n", ft_printf("8.: |%8.s|", "test"));
	printf(" (%d)\n", printf("08.*(-4): |%08.*i|", -4, 42));
	ft_printf(" (%d)\n", ft_printf("08.*(-4): |%08.*i|", -4, 42));
	printf(" (%d)\n", printf("(string) 8.*(-5): |%8.*s|", -5, "test"));
	ft_printf(" (%d)\n", ft_printf("(string) 8.*(-5): |%8.*s|", -5, "test"));
	printf(" (%d)\n", printf("Literally no conversions in this one"));
	printf(" (%d)\n", ft_printf("Literally no conversions in this one"));
	printf("\n");

	int	val = -1;
	printf(" (%d)\n", printf("Ya basic: |%p|", val));
	ft_printf(" (%d)\n", ft_printf("Ya basic: |%p|", val));


	ft_printf(" (%d)\n", ft_printf("My compliments in hexadecimal: %X %X %X", 4011, 49374, 47806));	





	

	if(broken)
	{
	printf("This is an incomplete    printf conversion: %-0*.*", 13, 7);
	printf("\n");
	ft_printf("This is an incomplete ft_printf conversion: %-0*.*", 13, 7);
	printf("\n");

	printf("This is a bogus    printf conversion: %-0*.*b", 13, 7);
	printf("\n");
	ft_printf("This is a bogus ft_printf conversion: %-0*.*b", 13, 7);
	printf("\n");

	printf("This is a bogus    printf conversion with double precision: %-0*..*b", 13, 7);
	printf("\n");
	ft_printf("This is a bogus ft_printf conversion with double precision: %-0*..*b", 13, 7);
	printf("\n");
	char* a = "main_general";
	}


	return (0);
}
