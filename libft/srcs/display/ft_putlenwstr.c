/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putlenwstr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uboumedj <uboumedj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/20 21:46:30 by uboumedj          #+#    #+#             */
/*   Updated: 2018/01/10 15:09:52 by uboumedj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

void	ft_putlenwstr(wchar_t *str, int len)
{
	int		i;

	if (str)
	{
		i = 0;
		while (str[i] != '\0' && i < len)
		{
			ft_putwchar(str[i]);
			i++;
		}
	}
}
