/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmartens <wmartens@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/28 15:57:26 by wmartens      #+#    #+#                 */
/*   Updated: 2020/04/28 15:57:57 by wmartens      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>
#include <bsd/stdio.h>

# define TEST "%.1u", 0

int	main(void)
{
	int ret;
	int retf;
	void *ptr;

	ptr = 0;
	ret = ft_printf(TEST);
	printf("\t: %d <wout style\n", ret);
	retf = printf(TEST);
	printf("\t: %d <linuxxx style", retf);
	return (0);
}
