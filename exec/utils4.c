/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha <abdamoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:31:26 by abdamoha          #+#    #+#             */
/*   Updated: 2023/04/27 19:35:51 by abdamoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_exit_code(t_cmds *p)
{
	if (p[0].cmd[2] != NULL)
	{
		g_exit_code = 1;
		write(2, "too many arguments\n", 19);
		return (0);
	}
	if (check_exit(p, 0) == 0 || check_exit(p, 1) == 1)
	{
		write(2, " numeric argument required\n", 28);
		g_exit_code = 255;
	}
	else if (p[0].cmd[1])
		g_exit_code = ft_atoi(p[0].cmd[1]) % 256;
	return (1);
}
