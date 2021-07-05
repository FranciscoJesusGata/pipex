/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */
/*   Updated: 2021/07/05 13:22:33 by fgata-va         ###   ########.fr       */
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
	ft_putstr_fd("pipex: ", 3);
	ft_putstr_fd(name, 3);
	write(3, ": ", 2);
	if (!message)
		ft_putstr_fd(strerror(errno), 3);
	else
		ft_putstr_fd(message, 3);
	write(1, "\n", 3);
	exit (code);
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
		return (NULL);
	ft_strlcpy(full_path, path, len + 1);
	full_path[len] = '/';
	ft_strlcpy(full_path + len + 1, binary, bin_len + 1);
	full_path[len + bin_len + 2] = '\0';
	return (full_path);

}

char	*get_binary_path(char **path, char *binary)
{
	int		i;
	char	*exec_path;
	int		found;

	i = 0;
	found = -1;
	while (found == -1 && path[i])
	{
		exec_path = path_concat(path[i], binary);
		if (!exec_path)
			program_error(binary, -1, NULL);
		printf("%s\n", exec_path);
		found = access(exec_path, 1);
		if (found < 0)
			free(exec_path);
		i++;
	}
	return (exec_path);

}

void	exec_binary(char **path, char **cmd, char **envp)
{
	int		i;
	char	*exec_path;

	i = 0;
	exec_path = get_binary_path(path, cmd[0]);
	if (exec_path)
		execve(exec_path, cmd, envp);
	else
	{
		write(1, 0, 1);
		program_error(cmd[0], 127, "command not found");
	}
}

int	ft_executor(t_command *commands, char **path, char **envp)
{
	int			pid;
	int			status;
	t_command	*current;

	status = 0;
	current = commands;
	while (current)
	{
		pid = fork();
		if (pid == 0)
			exec_binary(path, current->args, envp);
		else
		{
			pid = waitpid(pid, &status, WNOHANG);
			while (!pid)
				pid = waitpid(pid, &status, WNOHANG);
		}
		current = commands->next;
	}
	return (((status) & 0xff00) >> 8);
}

void	redirect(char *filename, int fd, int io)
{
	if (fd < 0 || (dup2(fd, io) < 0))
		program_error(filename, 1, NULL);
}

void	delete_cmd_lst(t_command *commands)
{
	t_command *nxt;

	while (commands)
	{
		nxt = commands->next;
		if (commands->input >= 0)
			close(commands->input);
		if (commands->output >= 0)
			close(commands->output);
		free_matrix(commands->args);
		if (commands->path)
			free(commands->path);
		free(commands);
		commands = nxt;
	}
}

void	open_files(t_command *commands, char *infile, char *outfile)
{
	int	input;
	int	output;
	t_command	*current;

	input = open(infile, O_RDONLY);
	redirect(infile, input, 0);
	output = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	redirect(infile, output, 1);
	current = commands;
	current->input = input;
	while (current->next)
		current = current->next;
	current->output = output;
}

void	finish_program(t_command *commands, char **path)
{
	delete_cmd_lst(commands);
	free_matrix(path);
}

int	main(int argc, char **argv, char *envp[])
{
	int			i;
	t_command	*commands;
	char		**path;
	int			status;

	i = 0;
	status = 0;
	if (argc == 5)
	{
		path = get_path(envp);
		commands = new_command(argv[2]);
		commands->next = new_command(argv[3]);
		open_files(commands, argv[1], argv[argc - 1]);
		status = ft_executor(commands, path, envp);
		finish_program(commands, path);
	}
	//system("leaks pipex");
	return (status);
}
