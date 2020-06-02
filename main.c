/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: y4k_wm <y4k_wm@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/30 17:47:57 by y4k_wm        #+#    #+#                 */
/*   Updated: 2020/05/31 21:59:51 by y4k_wm        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

# define TEST "%6.x", NULL

int	main(void)
{
	int r_wout;
	int r_og;

	ft_printf("\nWout___style:[");
	r_wout = ft_printf(TEST);
	ft_printf("] _rval =[%i]\n\n", r_wout);
	ft_printf("Orinal_style:[");
	r_og = printf(TEST);
	printf("] _rval =[%i]", r_og);
	return (0);
}
