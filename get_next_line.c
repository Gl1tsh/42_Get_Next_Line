/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nagiorgi <nagiorgi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:38:47 by nagiorgi          #+#    #+#             */
/*   Updated: 2023/11/06 11:49:28 by nagiorgi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

//=================================
#include <string.h>
#define ft_memchr memchr
#define ft_memcpy memcpy
#define ft_strndup strndup
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
	static char		*buffer = NULL;
	static char		*after_new_line = NULL;
	static size_t	bytes_read = 0;
	size_t			read_result;
	size_t			new_bytes_read;
	char			*new_line_pointer;




/*
Explication de : if (buffer == NULL)
====================================
Vérifie si le pointeur buffer est NULL.
Si c'est le cas, il alloue de la mémoire pour 1000 caractères à buffer 
en utilisant malloc.
*/
	if (buffer == NULL)
		buffer = malloc(sizeof(char) * 1000);
	

/*
Explication de : if (after_new_line != NULL)
============================================
vérifie si after_new_line n'est pas NULL, ce qui signifie qu'une 
nouvelle ligne a été trouvée précédemment dans le tampon. 
Si c'est le cas, elle calcule le nombre d'octets restants après la 
nouvelle ligne, copie ces octets au début du tampon, et met à jour 
bytes_read pour refléter le nombre d'octets restants.
*/
	if (after_new_line != NULL)
	{
		new_bytes_read = bytes_read - (after_new_line - buffer);
		
		ft_memcpy(buffer, after_new_line, new_bytes_read);

		bytes_read = new_bytes_read;
	}

	while (1)
	{
		read_result = read(fd, buffer + bytes_read, BUFFER_SIZE);
		/*	Cette ligne lit des données du fichier dans le tampon.
			Elle commence à écrire dans le tampon à l'endroit où elle a arrêté
			de lire lors du dernier tour de boucle.
		*/

		if (read_result <= 0)
			return (NULL);
		/*
		Si read renvoie 0 ou moins, cela signifie que la fin du fichier a été 
		atteinte ou qu'une erreur s'est produite. 
		Dans ce cas, la fonction renvoie NULL.
		*/

		bytes_read = bytes_read + read_result;
		// Cette ligne met à jour le nombre total d'octets lus jusqu'à présent.

		new_line_pointer = ft_memchr(buffer, '\n', bytes_read);
		/*
		Cette ligne cherche une nouvelle ligne dans le tampon. 
		Si une nouvelle ligne est trouvée, new_line_pointer pointe vers elle.
		*/



		/*
		Si une nouvelle ligne a été trouvée, cette partie du code est exécutée.
		Elle met à jour after_new_line pour pointer vers le caractère après
		la nouvelle ligne et renvoie une copie de la ligne lue, y compris
		la nouvelle ligne, en utilisant ft_strndup.
		*/
		if (new_line_pointer != NULL)
		{
			after_new_line = new_line_pointer + 1;
			return (ft_strndup(buffer, (new_line_pointer - buffer) + 1));
		}
	}
}
