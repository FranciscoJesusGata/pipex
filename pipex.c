/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:41:58 by fgata-va          #+#    #+#             */

/*   Updated: 2021/11/25 15:16:23 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

char	*get_binary_path(char **path, char *binary)
{
	int		i;
	char	*exec_path;
	int		found;

	i = 1;
	found = -1;
	while (found == -1 && path[i])
	{
		exec_path = path_concat(path[i], binary);
		if (!exec_path)
			return (NULL);
		found = access(exec_path, 1);
		if (found < 0)
		{
			free(exec_path);
			exec_path = NULL;
		}
		i++;
	}
	return (exec_path);
}

void	finish_program(t_pipex *pipex)
{
	delete_cmd_lst(pipex->commands);
	free_matrix(pipex->path);
}

int	redirections(char *infile, char *outfile, t_command *commands)
{
	int	readfile;
	int	writefile;

	readfile = open(infile, O_RDONLY);
	if (readfile < 0)
		return (program_error(infile, 1, NULL));
	commands->fds[0] = readfile;
	writefile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (writefile < 0)
		return (program_error(outfile, 1, NULL));
	while (commands->next)
		commands = commands->next;
	commands->fds[1] = readfile;
	return (0);
}

t_command	*ft_tokenizer(int argc, char **argv)
{
	int	i;
	t_command	*commands;
	t_command	*current;

	i = 2;
	commands = new_command(argv[i]);
	current = commands;
	i++;
	while (i < argc - 1)
	{
		current->next = new_command(argv[i]);
		current = current->next;
		if (!current)
		{
			delete_cmd_lst(commands);
			exit(program_error("pipex", 1, "Allocation error"));
		}
		i++;
	}
	return (commands);
}

void	initialize(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->commands = ft_tokenizer(argc, argv);
	pipex->path = get_path(envp);
	if (!pipex->path)
	{
		finish_program(pipex);
		exit(1);
	}
}

int	main(int argc, char **argv, char *envp[])
{
	t_pipex		pipex;
	int			status;

	status = 0;
	if (argc == 5)
	{
		initialize(&pipex, argc, argv, envp);
		status = redirections(argv[1], argv[argc - 1], pipex.commands);
		if (!status)
		{
			//Try to execute
			cmd_printer(pipex.commands);
			ft_putstr_fd("Status is: ", 1);
			ft_putnbr_fd(status, 1);
			write(1, "\n", 1);
		}
		else
			return (status);
		finish_program(&pipex);
	}
	else
		status = program_error("pipex", 1, "Invalid number of arguments");
	return (status);
}
