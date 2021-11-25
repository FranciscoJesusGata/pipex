/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:45:51 by fgata-va          #+#    #+#             */
/*   Updated: 2021/11/25 11:23:51 by fgata-va         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "Libft/libft.h"
# include <errno.h>

typedef struct s_command
{
	char			**argv;
	int				argc;
	char			*path;
	int				fds[2];
	struct s_command	*next;
}				t_command;

typedef struct s_pipex
{
	char		**path;
	t_command	*commands;
}				t_pipex;

/*
 * Linked list functions
 */
t_command	*new_command(char *cmd);
void	delete_cmd_lst(t_command *commands);
void	cmd_printer(t_command *commands);

/*
 * Util functions
 */
void	free_matrix(char **matrix);
int		program_error(char *name, int code, char *message);
char	*path_concat(char *path, char *binary);
char	**get_path(char **envp);

#endif
