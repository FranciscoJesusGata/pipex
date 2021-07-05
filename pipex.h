/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 10:45:51 by fgata-va          #+#    #+#             */
/*   Updated: 2021/06/25 17:53:29 by fgata-va         ###   ########.fr       */
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
	char			*binary;
	char			**args;
	char			*path;
	int				input;
	int				output;
	struct s_command	*next;
}				t_command;

t_command	*new_command(char *cmd);

#endif
