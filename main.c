/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/02 17:19:04 by wmartens      #+#    #+#                 */
/*   Updated: 2020/06/02 17:26:02 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

# define TEST "%5.u%%%0.0i%7.0i%7.0i%%", 0, 0, 0, 1

int	main(void)
{
	int r_wout;
	int r_og;

	ft_printf("\nWout_____style:[");
	r_wout = ft_printf(TEST);
	ft_printf("] \t_rval =[%i]\n\n", r_wout);
	ft_printf("Original_style:[");
	r_og = printf(TEST);
	printf("] \t_rval =[%i]", r_og);
	return (0);
}
