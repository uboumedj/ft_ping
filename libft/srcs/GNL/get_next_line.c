/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uboumedj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 16:09:56 by uboumedj          #+#    #+#             */
/*   Updated: 2018/05/15 18:20:05 by uboumedj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char				*join_str(char **str1, char **str2)
{
	char			*new_str;

	if (str1 && str2 && *str1 && *str2)
	{
		if (!(new_str = malloc(sizeof(char) * (ft_strlen(*str1)
			+ ft_strlen(*str2) + 1))))
			return (NULL);
		new_str = ft_strcpy(new_str, *str1);
		new_str = ft_strcat(new_str, *str2);
		ft_strdel(str1);
		ft_strdel(str2);
	}
	else if (!(*str1))
	{
		new_str = ft_strdup(*str2);
		ft_strdel(str2);
	}
	else
	{
		new_str = ft_strdup(*str1);
		ft_strdel(str1);
	}
	return (new_str);
}

int					parse_fd(const int fd, char **line)
{
	char			buffer[BUFF_SIZE + 1];
	char			*tmp;
	int				rd;

	if ((rd = read(fd, buffer, BUFF_SIZE)) < 0)
		return (-1);
	buffer[rd] = '\0';
	*line = ft_strdup(buffer);
	while (rd > 0 && !(ft_strchr(*line, '\n')))
	{
		if ((rd = read(fd, buffer, BUFF_SIZE)) > 0)
		{
			buffer[rd] = '\0';
			tmp = ft_strdup(buffer);
			*line = join_str(line, &tmp);
		}
	}
	return (rd);
}

int					newline_index(char *str)
{
	int				i;

	i = 0;
	while (str && str[i] && str[i] != '\n')
		i++;
	return (i);
}

int					get_next_line(int const fd, char **line)
{
	static	char	*buff;
	static	int		rd;
	char			*tmp;

	if (fd < 0 || !line || BUFF_SIZE <= 0)
		return (-1);
	if ((rd = parse_fd(fd, line)) <= -1)
		return (-1);
	tmp = join_str(&buff, line);
	if (ft_strchr(tmp, '\n') && ft_strchr(tmp, '\n') + 1)
		buff = ft_strdup(ft_strchr(tmp, '\n') + 1);
	*line = ft_strsub(tmp, 0, newline_index(tmp));
	ft_strdel(&tmp);
	ft_strdel(&buff);
	return ((ft_strlen(*line) || rd || buff) ? 1 : rd);
}
