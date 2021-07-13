/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 21:02:51 by fgata-va          #+#    #+#             */
/*   Updated: 2021/07/13 12:39:41 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (matrix)
	{
		while (matrix[i])
		{
			free(matrix[i]);
			i++;
		}
		free(matrix);
	}
}

int	program_error(char *name, int code, char *message)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(name, 2);
	write(2, ": ", 2);
	if (!message)
		ft_putstr_fd(strerror(errno), 2);
	else
		ft_putstr_fd(message, 2);
	write(2, "\n", 1);
	return(code);
}

char	*path_concat(char *path, char *binary)
{
	int		len;
	int		bin_len;
	char	*full_path;

	len = ft_strlen(path);
	bin_len = ft_strlen(binary);
	full_path = (char *)malloc(len + bin_len + 2);
	if (!full_path)
	{
		program_error("pipex", 1, "Allocation error");
		return (NULL);
	}
	ft_strlcpy(full_path, path, len + 1);
	full_path[len] = '/';
	ft_strlcpy(full_path + len + 1, binary, bin_len + 1);
	full_path[len + bin_len + 2] = '\0';
	return (full_path);
}

char	**get_path(char **envp)
{
	while (ft_strncmp(*envp, "PATH", 4))
		envp++;
	return (ft_split(*envp + 5, ':'));
}

