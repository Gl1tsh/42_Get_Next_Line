/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nagiorgi <nagiorgi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:38:47 by nagiorgi          #+#    #+#             */
/*   Updated: 2023/11/10 18:09:55 by nagiorgi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//=================================
#include "get_next_line_bonus.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

//=================================

/*
SIGNIFICATION DE VARIABLE 
=========================
buffer : C'est un tampon de mémoire alloué dynamiquement qui est utilisé pour 
stocker les données lues à partir du fichier.
----
after_new_line : C'est un pointeur vers le caractère suivant la nouvelle ligne 
dans le tampon. Il est utilisé pour copier les données restantes dans le tampon 
après avoir trouvé une nouvelle ligne.
----
bytes_read : C'est une variable statique qui garde le nombre total d'octets 
lus jusqu'à présent. Elle est statique, ce qui signifie que sa valeur est 
conservée entre les appels de fonction.
----
read_result : Cette variable stocke le résultat de l'appel à 
la fonction read. Elle contient le nombre d'octets réellement lus lors du 
dernier appel à read.
----
new_bytes_read : Cette variable est utilisée pour calculer le nombre d'octets 
restants dans le tampon après avoir trouvé une nouvelle ligne.
----
new_line_pointer : C'est un pointeur vers le caractère de nouvelle ligne (\n) 
dans le tampon, s'il existe.
----
----
fd : C'est le descripteur de fichier à partir duquel les données sont lues. 
Il est passé en argument à la fonction read.
----
BUFFER_SIZE : C'est la taille du tampon de lecture utilisé dans l'appel à read. 
Il détermine combien d'octets seront lus à chaque appel à read.
*/

void	*reset_mem_gnl(t_gnl *gnl)
{
	free(gnl->buffer);
	gnl->buffer = NULL;
	gnl->after_new_line = NULL;
	gnl->bytes_read = 0;
	return (NULL);
}

char	*fill_buffer(int fd, t_gnl *gnl)
{
	int		read_result;
	char	*newline_found;

	while (1)
	{
		read_result = read(fd, gnl->buffer + gnl->bytes_read, BUFFER_SIZE);
		if (read_result < 0)
			return (reset_mem_gnl(gnl));
		if (read_result == 0)
		{
			if (gnl->bytes_read == 0)
				return (reset_mem_gnl(gnl));
			gnl->after_new_line = gnl->buffer + gnl->bytes_read;
			return (ft_strndup(gnl->buffer, gnl->bytes_read));
		}
		gnl->bytes_read = gnl->bytes_read + read_result;
		newline_found = ft_memchr(gnl->buffer, '\n', gnl->bytes_read);
		if (newline_found != NULL)
		{
			gnl->after_new_line = newline_found + 1;
			return (ft_strndup(gnl->buffer, (newline_found - gnl->buffer) + 1));
		}
	}
}

char	*get_next_line(int fd)
{
	static t_gnl	all_gnl[OPEN_MAX]
		= {[0 ...OPEN_MAX - 1] = {NULL, NULL, 0}};
	t_gnl			*gnl;
	size_t			new_bytes_read;
	char			*newline_found;

	if (fd < 0 || fd >= OPEN_MAX)
		return (NULL);
	gnl = &all_gnl[fd];
	if (gnl->buffer == NULL)
		gnl->buffer = malloc(sizeof(char) * 2000000);
	if (gnl->after_new_line != NULL)
	{
		new_bytes_read = gnl->bytes_read - (gnl->after_new_line - gnl->buffer);
		ft_memmove(gnl->buffer, gnl->after_new_line, new_bytes_read);
		gnl->bytes_read = new_bytes_read;
		newline_found = ft_memchr(gnl->buffer, '\n', gnl->bytes_read);
		if (newline_found != NULL)
		{
			gnl->after_new_line = newline_found + 1;
			return (ft_strndup(gnl->buffer, (newline_found - gnl->buffer) + 1));
		}
	}
	return (fill_buffer(fd, gnl));
}

/*
int _read(int fd, char *buffer, int count)
{
  if (cursor >= sizeof(content)) return 0;
  if ((cursor + count) > sizeof(content)) count = sizeof(content) - cursor;
  memcpy(buffer, content + cursor, count);
  cursor += count;
  return count;
}

#include <stdio.h>
int main()
{
    char *line;
    while ((line = get_next_line(0)))
    {
        printf("%s", line);
    }
    return 0;
}
*/