/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stackfree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uboumedj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/16 14:27:11 by uboumedj          #+#    #+#             */
/*   Updated: 2018/01/23 13:26:17 by uboumedj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

void	ft_stackfree(t_stack **stack)
{
	t_stack *next;
	t_stack *cur;

	if (stack)
	{
		cur = *stack;
		while (cur != NULL)
		{
			next = cur->next;
			cur->content = 0;
			free(cur);
			cur = next;
		}
		*stack = NULL;
	}
}
