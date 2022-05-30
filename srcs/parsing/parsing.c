/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjulienn <cjulienn@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 15:49:57 by cjulienn          #+#    #+#             */
/*   Updated: 2022/05/30 17:37:22 by cjulienn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* bash allows CMDS to work regardless of the case
e.g echo will works but also Echo or eChO */

// static void	*lowercase_cmds(t_token *token)
// {
// 	int			i;

// 	i = 0;
// 	while (token->item[i])
// 	{
// 		if (token->item[i] >= 97 && token->item[i] <= 122)
// 			token->item[i] = token->item[i] - 32;
// 		i++;
// 	}
// }

// /* as asked by the subject, we need to expand the env variables (preceeded by $)*/

// int	modify_tokens(t_token *token)
// {
// 	while (token)
// 	{
// 		expand_env_var(token);
// 		if (remove_quotes(token) == -1)
// 			; // handle this
// 		if (token->type == CMD)
// 			;// implement  lowercase_cmds(token) later !!!
// 		token = token->next;
// 	}
// 	return (0);
// }

/* should check wether token type is a CMD or not
return 1 if the case, 0 otherwise 
the 2 cases identified are :
1) the token is the first token or is locatec just after a pipe
2) the token is an ARG situated after the filename for a redirection
(or the heredoc DELIMITER) */

static int	is_cmd(t_token *token)
{
	if (token->type == ARG)
	{
		if (!token->prev || (token->prev && token->prev->type == PIPE))
			return (1);
		if (token->prev && token->prev->type == REDIR_ARG)
			return (1);
	}
	return (0);
}

/* find thew type of the token (among | < < >> >>)
check also if the token is entirely surrounded
by simple or double quotes (with is_within_quotes func)
ARG can be flags, commandes, arguments for function or > >> < << */

static void	find_token_type(t_token *token)
{		
	while (token)
	{
		if (!ft_strncmp(token->item, "|", 1) && ft_strlen(token->item) == 1)
			token->type = PIPE;
		else if (!ft_strncmp(token->item, "<<", 2)
			&& ft_strlen(token->item) == 2)
			token->type = HERE_DOC;
		else if (!ft_strncmp(token->item, ">>", 2)
			&& ft_strlen(token->item) == 2)
			token->type = RO_APPEND;
		else if (!ft_strncmp(token->item, "<", 1)
			&& ft_strlen(token->item) == 1)
			token->type = REDIR_INPUT;
		else if (!ft_strncmp(token->item, ">", 1)
			&& ft_strlen(token->item) == 1)
			token->type = REDIR_OUTPUT;
		else if (token->prev && token->prev->type <= 5
			&& token->prev->type > 1)
			token->type = REDIR_ARG;
		else
			token->type = ARG;
		if (is_cmd(token) == 1)
			token->type = CMD;
		token = token->next;
	}
}

/* the parse_user_input func is converting the string user_input
into tokens. Tokens are words, quotted expressions separated
by space or | < << >> >, or | < << >> >.
every token is a part of a linked list containing the str,
the type of the token, if it is surrounded by closed quotes or not
*/

static void	add_new_token(t_token *token, t_shell *shell, char *item)
{
	t_token		*new_elem;

	new_elem = token_new(item);
	free(item);
	item = NULL;
	if (!new_elem)
		free_parent_case_err(shell, token);
	token_add_back(&token, new_elem);
}

t_token	*parse_user_input(t_shell *shell)
{
	t_token		*token;
	char		*item;
	char		*ui_cpy;

	token = NULL;
	ui_cpy = shell->user_input;
	while (ui_cpy)
	{
		item = isolate_item(ui_cpy, shell, token);
		if (!item && shell->item_length == -1)
		{
			token_clear(&token);
			return (NULL);
		}
		else if (!item && shell->item_length == 0)
			break ;
		ui_cpy = ui_cpy + shell->item_length;
		add_new_token(token, shell , item);
	}
	find_token_type(token);
	// modify_tokens(tokens); WILL DO THIS AFTER
	return (token);
}
