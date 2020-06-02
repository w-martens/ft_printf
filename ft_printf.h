/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 17:50:10 by wmartens      #+#    #+#                 */
/*   Updated: 2020/05/31 21:53:05 by y4k_wm        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# define WIDTH 0
# define PRECISION 1

# include <stdarg.h>
# include <unistd.h>

typedef	struct	s_printf
{
	int		ret_cnt;
	int		width;
	int		prec;
	int		prec_on;
	int		nul_flag;
	int		min_flag;
	char	specifier;
	int		print_len;
}				t_printf;

int				ft_printf(const char *fmt, ...);
void			print_arg(t_printf *p_s, va_list arg);
void			init_struct(t_printf *p_s);
void			wp_amt(const char **fmt, t_printf *p_s, char flag);
int				str_schr(char str, char *set);
void			spec_chk(const char **fmt, t_printf *p_s);
void			wp_asterisk(const char **fmt, t_printf *p_s, va_list arg, char \
				flag);
void			print_width(t_printf *p_s);
void			int_width(t_printf *p_s, int i);
int				prec_set(t_printf *p_s, int intlen);
void			print_prec(t_printf *p_s);
void			flag_chk(const char **fmt, t_printf *p_s, va_list arg);
int				ft_strlen(const char *s);
int				ft_atoix(const char *str, long int res, long int sign, long int\
				tmp);
int				ft_atoi(const char *str);
void			num_print(t_printf *p_s, int n);
int				int_len(t_printf *p_s, int i);
void			print_min(t_printf *p_s, int i);
void			int_print(t_printf *p_s, int i);
int				hex_len(t_printf *p_s, unsigned int n);
void			hex_print(t_printf *p_s, unsigned int n);
void			xx_print(t_printf *p_s, unsigned int n);
void			ptr_print(t_printf *p_s, unsigned int n);
void			perc_print(t_printf *p_s);
void			ft_putchar_fd(char c, int fd);
void			str_print(t_printf *p_s, char *str);
void			char_print(t_printf *p_s, int i);
void			unum_print(t_printf *p_s, unsigned int n);
int				unint_len(t_printf *p_s, unsigned int i);
void			unint_print(t_printf *p_s, unsigned int i);
void			iter_fmt(const char **fmt);

#endif
