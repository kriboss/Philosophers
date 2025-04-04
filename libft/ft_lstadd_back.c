/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:40:26 by kbossio           #+#    #+#             */
/*   Updated: 2024/11/25 11:40:27 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!new | !lst)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}
/*
#include <stdio.h>

int main() {
    t_list *head = ft_lstnew("Primo");
    head->next = ft_lstnew("Secondo");
    head->next->next = ft_lstnew("Terzo");

    t_list *node1 = ft_lstnew("Node1");
    t_list *node2 = ft_lstnew("Node2");

    ft_lstadd_back(&head, node1);
    ft_lstadd_back(&head, node2);

    // Print the list
    t_list *current = head;
    while (current) {
        printf("%s\n", (char *)current->content);
        current = current->next;
    }

    return 0;
}*/
