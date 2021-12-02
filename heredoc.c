/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:29:49 by fgata-va          #+#    #+#             */
/*   Updated: 2021/12/02 16:31:28 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	heredoc(char *limiter)
{
	char	*line;
	int		fd;

	fd = open("/tmp/heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd < 0)
		return (fd);
	write(1, "> ", 2);
	get_next_line(0, &line);
	while (!line || ft_strncmp(line, limiter, ft_strlen(line)))
	{
		ft_putstr_fd(line, fd);
		write(fd, "\n", 1);
		free(line);
		line = NULL;
		write(1, "> ", 2);
		get_next_line(0, &line);
	}
	free(line);
	close(fd);
	fd = open("/tmp/heredoc", O_RDONLY);
	return (fd);
}
