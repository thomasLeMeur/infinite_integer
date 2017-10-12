#include <stdio.h>
#include <stdlib.h>

#include "infinite_int.h"

/**
 *	Check if the number is zero
 *	Set the number to positive if it is true
**/
uint8_t	infint_is_zero(t_inf_int *n)
{
	if (!infint_is_valid_nb(n))
		return (0);
	if (n->size == 1 && *n->nb == 0)
	{
		n->sign = INF_INT_POSITIVE;
		return (1);
	}
	return (0);
}

/**
 *	Check if there are no unexpected values
**/
uint8_t	infint_is_valid_nb(t_inf_int *n)
{
	if (!n)
		return (0);
	if (!infint_is_valid_sign(n->sign))
		return (0);
	if (n->size == 0 || n->nb == NULL)
		return (0);
	return (1);
}

/**
 *	Check if the sign has an expected value
**/
uint8_t	infint_is_valid_sign(int8_t sign)
{
	if (sign == INF_INT_POSITIVE || sign == INF_INT_NEGATIVE)
		return (1);
	//printf("The sign has the value %hhi (%hhi (positive) or %hhi (negative) expected)\n", sign, INF_INT_POSITIVE, INF_INT_NEGATIVE);
	return (0);
}

/**
 *	Check if the base has an expected value
**/
uint8_t	infint_is_valid_base(uint8_t base)
{
	if (base >= INF_INT_BASE_MIN && base <= INF_INT_BASE_MAX)
		return (1);
	//printf("The base has the value %hhu (%hhu <= base <= %hhu expected)\n", base, INF_INT_BASE_MIN, INF_INT_BASE_MAX);
	return (0);
}

/**
 *	Return a string representation in the wanted bases
**/
char	*infint_to_string(t_inf_int *n, uint8_t output_base)
{
	char	*out;

	if (!infint_is_valid_base(output_base))
		return (NULL);
	if (!infint_is_valid_nb(n))
		return (NULL);

	out = NULL;
	return (out);
}

/**
 *	Check if the number is odd
**/
uint8_t	infint_is_odd(t_inf_int *n)
{
	if (!infint_is_valid_nb(n))
		return (0);
	return (*n->nb & 1);
}

/**
 *	Check if the number is even
**/
uint8_t	infint_is_even(t_inf_int *n)
{
	if (!infint_is_valid_nb(n))
		return (0);
	return (!(*n->nb & 1));
}

/**
 *	Check if the number is positive
**/
uint8_t	infint_is_positive(t_inf_int *n)
{
	if (!infint_is_valid_nb(n))
		return (0);
	if (infint_is_zero(n))
		return (1);
	return (n->sign == INF_INT_POSITIVE);
}

/**
 *	Check if the number is negative
**/
uint8_t	infint_is_negative(t_inf_int *n)
{
	if (!infint_is_valid_nb(n))
		return (0);
	if (infint_is_zero(n))
		return (0);
	return (n->sign == INF_INT_NEGATIVE);
}

/**
 *	Invert the sign of the number
**/
void	infint_invert(t_inf_int *n)
{
	if (infint_is_valid_nb(n) && !infint_is_zero(n))
		n->sign = infint_is_positive(n) ? INF_INT_NEGATIVE : INF_INT_POSITIVE;
}
