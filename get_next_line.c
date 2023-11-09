/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nagiorgi <nagiorgi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:38:47 by nagiorgi          #+#    #+#             */
/*   Updated: 2023/11/09 18:11:10 by nagiorgi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//=================================
#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
char	*ft_strndup(const char *str, size_t n);

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

typedef struct	s_gnl
{
	char	*buffer;
	char	*after_new_line;
	size_t	bytes_read;
}			t_gnl;


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

char	*get_next_line(int fd)
{
	static t_gnl	gnl = {NULL, NULL, 0};
	int				read_result;
	size_t			new_bytes_read;
	char			*new_line_pointer;

	if (gnl.buffer == NULL)
		gnl.buffer = malloc(sizeof(char) * 200000);
	
	if (gnl.after_new_line != NULL)
	{
		new_bytes_read = gnl.bytes_read - (gnl.after_new_line - gnl.buffer);
		
		ft_memmove(gnl.buffer, gnl.after_new_line, new_bytes_read);

		gnl.bytes_read = new_bytes_read;

		new_line_pointer = ft_memchr(gnl.buffer, '\n', gnl.bytes_read);

		if (new_line_pointer != NULL)
		{
			gnl.after_new_line = new_line_pointer + 1;
			return (ft_strndup(gnl.buffer, (new_line_pointer - gnl.buffer) + 1));
		}
	}

	while (1)
	{
		read_result = read(fd, gnl.buffer + gnl.bytes_read, BUFFER_SIZE);
		if (read_result < 0)
		{
			free(gnl.buffer);
			gnl.buffer = NULL;
			gnl.after_new_line = NULL;
			gnl.bytes_read = 0;
			return (NULL);
		}
		if (read_result == 0)
		{
			if (gnl.bytes_read == 0)
			{
				free(gnl.buffer);
				gnl.buffer = NULL;
				gnl.after_new_line = NULL;
				return (NULL);
			}
			gnl.after_new_line = gnl.buffer + gnl.bytes_read;
			return (ft_strndup(gnl.buffer, gnl.bytes_read));
		}
		gnl.bytes_read = gnl.bytes_read + read_result;
		new_line_pointer = ft_memchr(gnl.buffer, '\n', gnl.bytes_read);

		if (new_line_pointer != NULL)
		{
			gnl.after_new_line = new_line_pointer + 1;
			return (ft_strndup(gnl.buffer, (new_line_pointer - gnl.buffer) + 1));
		}
	}
}
/*
int _read(int fd, char *buffer, int count)
{
  static char content[] = "hello\nca\nva\n\nCool man, a fond!!";
  static int cursor = 0;
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