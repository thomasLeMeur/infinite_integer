#include <stdlib.h>

#include "infinite_int.h"

/**
 *	Free the storage and the number
**/
void		infint_free(t_inf_int **n)
{
	if (!n || !*n)
		return;
	infint_clear(*n);
	free(*n);
	*n = NULL;
}

/**
 *	Free the storage
**/
void		infint_free_storage(t_inf_int *n)
{
	if (n && n->nb)
	{
		free(n->nb);
		n->nb = NULL;
	}
}

/**
 *	Reset the number to default value
**/
t_inf_int	*infint_reset(t_inf_int *n)
{
	if (!n)
		return (NULL);
	infint_clear(n);
	if (!(n->nb = malloc(sizeof(uintmax_t))))
	{
		infint_clear(n);
		return (NULL);
	}
	n->sign = INF_INT_POSITIVE;
	n->size = 1;
	*n->nb = 0;
	return (n);
}

/**
 *	Clear all the number values
**/
void		infint_clear(t_inf_int *n)
{
	if (!n)
		return;
	n->sign = 0;
	n->carry = 0;
	n->size = 0;
	infint_free_storage(n);
}

/**
 *	Set all the values to 0 or NULL
**/
void		infint_init(t_inf_int *n)
{
	if (!n)
		return;
	n->sign = 0;
	n->carry = 0;
	n->size = 0;
	n->nb = NULL;
}
