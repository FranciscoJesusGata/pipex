/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */
/*   Updated: 2021/06/23 20:00:33 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

char	**get_path(char **envp)
{
	while (ft_strncmp(*envp, "PATH", 4))
		envp++;
	return (ft_split(*envp + 5, ':'));
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

void	program_error(char *name, int code, char *message)
{
	ft_putstr_fd("pipex: ", 1);
	ft_putstr_fd(name, 1);
	write(1, ": ", 2);
	if (!message)
		ft_putstr_fd(strerror(errno), 1);
	else
		ft_putstr_fd(message, 1);
	write(1, "\n", 1);
	exit (code);
}

char	*get_binary_path(char *path, char *binary)
{
	int		len;
	int		bin_len;
	char	*full_path;

	len = ft_strlen(path);
	bin_len = ft_strlen(binary);
	full_path = (char *)malloc(len + bin_len + 2);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, path, len + 1);
	full_path[len] = '/';
	ft_strlcpy(full_path + len + 1, binary, bin_len + 1);
	full_path[len + bin_len + 2] = '\0';
	return (full_path);

}

void	exec_binary(char **path, char **cmd, char **envp)
{
	int		i;
	int		found;
	char	*exec_path;

	i = 0;
	found = -1;
	while (found == -1 && path[i])
	{
		exec_path = get_binary_path(path[i], cmd[0]);
		if (!exec_path)
			program_error(cmd[0], -1, NULL);
		printf("%s\n", exec_path);
		found = access(exec_path, 1);
		if (found < 0)
			free(exec_path);
		i++;
	}
	if (found == 0)
		execve(exec_path, cmd, envp);
	else
		program_error(cmd[0], 127, "command not found");
}

int	ft_executor(char **cmd, char **path, char **envp)
{
	int		pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
		exec_binary(path, cmd, envp);
	else
	{
		pid = waitpid(pid, &status, WNOHANG);
		while (!pid)
			pid = waitpid(pid, &status, WNOHANG);
	}
	return (((status) & 0xff00) >> 8);
}

void	open_file(char *filename, int fd, int io)
{

	if (fd < 0 || (dup2(fd, io) < 0))
		program_error(filename, 1, NULL);
}

int	main(int argc, char **argv, char *envp[])
{
	int	i;
	char	**cmd1;
	char	**path;
	int		status;

	i = 0;
	status = 0;
	if (argc > 3)
	{
		open_file(argv[1], open(argv[1], O_RDONLY), 0);
		open_file(argv[3], open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, 0622), 1);
		path = get_path(envp);
		cmd1 = ft_split(argv[2], ' ');
		status = ft_executor(cmd1, path, envp);
		free_matrix(path);
		free_matrix(cmd1);
	}
	//system("leaks pipex");
	return (status);
}
