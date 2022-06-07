/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xle-boul <xle-boul@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 15:37:13 by cjulienn          #+#    #+#             */
/*   Updated: 2022/06/07 20:43:09 by xle-boul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// simple function that prints the pwd to the terminal then frees the vairable
int	built_in_pwd(t_shell *shell, char *cmd_args)
{
	char	*cwd;

	cwd = (char *)malloc(sizeof(char) * MAX_PATH);
	if (!cwd)
		return (ENOMEM);
	cwd = getcwd(cwd, MAX_PATH);
	if (!cwd)
		return (ENAMETOOLONG);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
