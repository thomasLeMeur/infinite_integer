#include <stdlib.h>

#include "infinite_int.h"

/**
 *	a = max(abs(a), abs(b))
 *	b = min(abs(a), abs(b))
 *	Find if the operation result has to be reverted
 *
 *	(Needed by infint_add, infint_sub, infint_mul)
**/
static uint8_t	infint_sort_operands_with_revert(t_inf_int **a, t_inf_int **b, int8_t *olda, int8_t *oldb, uint8_t is_adding)
{
	int8_t		tmp1;
	t_inf_int	*tmp2;
	uint8_t		has_to_revert;

	/**
	 *	 3 -  2 =   3 - 2
	 *	-2 - -3 =   3 - 2
	 *	 2 - -3 =   3 + 2
	 *	 3 - -2 =   3 + 2
	 *
	 *	 2 -  3 = -(3 - 2)
	 *	-3 - -2 = -(3 - 2)
	 *	-3 -  2 = -(3 + 2)
	 *	-2 -  3 = -(3 + 2)
	**/
	if (!is_adding)
		has_to_revert = (infint_is_smaller(*a, *b));

	*olda = (*a)->sign;
	*oldb = (*b)->sign;
	(*a)->sign = INF_INT_POSITIVE;
	(*b)->sign = INF_INT_POSITIVE;
	if (infint_is_smaller(*a, *b))
	{
		tmp2 = *a;
		*a = *b;
		*b = tmp2;
		tmp1 = *olda;
		*olda = *oldb;
		*oldb = tmp1;
	}
	(*a)->sign = *olda;
	(*b)->sign = *oldb;

	/**
	 *	With a >= b :
	 *	 a +  b =   a + b
	 *	-a + -b = -(a + b)
	 *	-a +  b = -(a - b)
	 *	 a + -b =   a - b
	**/
	if (is_adding)
		has_to_revert = (infint_is_negative(*a));
	return (has_to_revert);
}

/**
 *	Set the number to positive
 *	Indicate if the result of the operation will have to be reverted
 *	Return 1 if we need to sub or 0 if we need to add
 *
 *	(Needed by infint_add, infint_sub)
**/
static uint8_t	infint_prepare_to_complement_func(t_inf_int *a, t_inf_int *b)
{
	uint8_t	ret;

	ret = (a->sign != b->sign);
	a->sign = INF_INT_POSITIVE;
	b->sign = INF_INT_POSITIVE;
	return (ret);
}

/**
 *	Invert bits with carry
 *
 *	(Needed by infint_sub)
**/
static void	infint_invert_bits(t_inf_int *n, uintmax_t elem_from, uintmax_t bit_from)
{
	uintmax_t	bit;
	uintmax_t	lim;
	uintmax_t	bit_cnt;
	uintmax_t	elem_cnt;

	if (!infint_is_valid_nb(n) || n->carry == 0 || n->nb[elem_from] & (uintmax_t)1 << bit_from)
		return;

	elem_cnt = elem_from;
	while (elem_cnt < n->size)
	{
		lim = INF_INT_NB_BITS;
		if (elem_cnt == n->size - 1)
		{
			bit_cnt = lim;
			while (bit_cnt-- > 0 && (n->nb[elem_cnt] & ((uintmax_t)1 << bit_cnt)) == 0) ;
			lim = bit_cnt + 1;
		}

		bit_cnt = (elem_cnt == elem_from) ? bit_from : 0;
		while (bit_cnt < lim)
		{
			bit = (uintmax_t)1 << bit_cnt;
			if (n->nb[elem_cnt] & bit)
			{
				n->nb[elem_cnt] &= ~bit;
				if (elem_cnt == n->size - 1 && n->nb[elem_cnt] == 0)
					n->size--;
				break;
			}
			n->nb[elem_cnt] |= bit;
			bit_cnt++;
		}
		if (bit_cnt != INF_INT_NB_BITS)
			break;
		elem_cnt++;
	}
	n->carry = 0;
}

/**
 *	a + b
 *
 *	Complexity (N is the number of bits in base2 storage):
 *	- Stay with add	: O(N / (sizeof(uintmax_t) * CHAR_BIT))
 *	- Go to sub		: O(2 * N)
**/
t_inf_int		*infint_add(t_inf_int *a, t_inf_int *b)
{
	t_inf_int	*n;
	int8_t		old_sign_a;
	int8_t		old_sign_b;
	uint8_t		has_to_carry;
	uint8_t		has_to_revert;

	if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
		return (NULL);
	has_to_revert = infint_sort_operands_with_revert(&a, &b, &old_sign_a, &old_sign_b, 1);
	if (infint_prepare_to_complement_func(a, b))
	{
		if ((n = infint_sub(a, b)) && has_to_revert)
			infint_invert(n);
		a->sign = old_sign_a;
		b->sign = old_sign_b;
		return (n);
	}

	if (!(n = infint_new_with_size(a->size + 1)))
	{
		a->sign = old_sign_a;
		b->sign = old_sign_b;
		return (NULL);
	}

	n->size = 0;
	while (n->size < a->size)
	{
		if (a->nb[n->size] & ((uintmax_t)1 << (INF_INT_NB_BITS - 1)))
			has_to_carry = 1;
		else
			has_to_carry = 0;
		n->nb[n->size] = a->nb[n->size] + n->carry;
		if (n->size < b->size)
			n->nb[n->size] += b->nb[n->size];
		if (has_to_carry && !(n->nb[n->size] & ((uintmax_t)1 << (INF_INT_NB_BITS - 1))))
			n->carry = 1;
		else
			n->carry = 0;
		n->size++;
	}
	if (n->carry)
		n->nb[n->size++] = 1;

	if (has_to_revert)
		infint_invert(n);
	a->sign = old_sign_a;
	b->sign = old_sign_b;
	return (n);
}

/**
 *	a - b
 *
 *	Complexity (N is the number of bits in base2 storage):
 *	- Stay with add	: O(N / (sizeof(uintmax_t) * CHAR_BIT))
 *	- Go to sub		: O(2 * N)
**/
t_inf_int		*infint_sub(t_inf_int *a, t_inf_int *b)
{
	t_inf_int	*n;
	uintmax_t	i;
	uintmax_t	j;
	uintmax_t	bit;
	uint8_t		nb_bits;
	int8_t		old_sign_a;
	int8_t		old_sign_b;
	uint8_t		has_to_revert;

	if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
		return (NULL);

	has_to_revert = infint_sort_operands_with_revert(&a, &b, &old_sign_a, &old_sign_b, 0);
	if (infint_prepare_to_complement_func(a, b))
	{
		if ((n = infint_add(a, b)) && has_to_revert)
			infint_invert(n);
		a->sign = old_sign_a;
		b->sign = old_sign_b;
		return (n);
	}

	if (!(n = infint_clone(a)))
	{
		a->sign = old_sign_a;
		b->sign = old_sign_b;
		return (NULL);
	}

	/**
	 * Truth table (C = old carry, c = new carry)
	 *	C A B R
	 *	0 0 0 0
	 *	0 0 1 1 -> c
	 *	0 1 0 1
	 *	0 1 1 0
	 *	1 0 0 0
	 *	1 0 1 0
	 *	1 1 0 0
	 *	1 1 1 1 -> c
	**/
	i = 0;
	while (i < n->size)
	{
		j = 0;
		while (j < INF_INT_NB_BITS)
		{
			infint_invert_bits(n, i, j);

			bit = (uintmax_t)1 << j;
			nb_bits = (n->carry + (n->nb[i] & bit) != 0);
			if (i < b->size)
				nb_bits += ((b->nb[i] & bit) != 0);
			if (nb_bits % 2)
			{
				n->nb[i] |= bit;
				if (i < b->size)
					n->carry = ((b->nb[i] & bit) != 0);
			}
			else
			{
				n->nb[i] &= ~bit;
				n->carry = 0;
			}
			j++;
		}
		i++;
	}

	if (has_to_revert)
		infint_invert(n);
	a->sign = old_sign_a;
	b->sign = old_sign_b;

	//Update the size of the buffer free memory
	return (infint_update_size(n, n->size));
}

/**
 *	a * b
 *
 *	Complexity (N and M are the maximum number of bits in base2 storages):
 *	- O(M * N)
**/
#include <stdio.h>
t_inf_int		*infint_mul(t_inf_int *a, t_inf_int *b)
{
	t_inf_int	*n;
	uint8_t		sum;
	uintmax_t	i, j, bit;
	uintmax_t	a_i, a_j, a_bit;
	uintmax_t	n_i, n_j, n_bit;
	int8_t		old_sign_a;
	int8_t		old_sign_b;

	if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
		return (NULL);

	infint_sort_operands_with_revert(&a, &b, &old_sign_a, &old_sign_b, 1);
	a->sign = INF_INT_POSITIVE;
	b->sign = INF_INT_POSITIVE;

	if (!(n = infint_new_with_size(a->size + b->size)))
	{
		a->sign = old_sign_a;
		b->sign = old_sign_b;
		return (NULL);
	}

	i = 0;
	while (i < b->size)
	{
		j = 0;
		while (j < INF_INT_NB_BITS)
		{
			bit = (uintmax_t)1 << j;
			if (b->nb[i] & bit)
			{
				a_i = 0;
				n_i = i;
				n_j = j;
				n->carry = 0;
				while (a_i < a->size)
				{
					a_j = 0;
					while (a_j < INF_INT_NB_BITS)
					{
						a_bit = (uintmax_t)1 << a_j;
						n_bit = (uintmax_t)1 << n_j;

						sum = n->carry + ((n->nb[n_i] & n_bit) != 0) + ((a->nb[a_i] & a_bit) != 0);
						n->carry = (sum > 1);
						if (sum % 2)
							n->nb[n_i] |= n_bit;
						else
							n->nb[n_i] &= ~n_bit;

						++a_j;
						if(++n_j >= INF_INT_NB_BITS)
						{
							++n_i;
							n_j = 0;
						}
					}
					++a_i;
				}
				if (n->carry)
					n->nb[n_i] |= (uintmax_t)1 << n_j;
			}
			++j;
		}
		++i;
	}

	a->sign = old_sign_a;
	b->sign = old_sign_b;
	n->sign = (a->sign == b->sign) ? INF_INT_POSITIVE : INF_INT_NEGATIVE;
	while (n->size > 0)
		if (n->nb[--n->size])
			break;
	n->size++;
	return (n);
}

/**
 *	a / b
**/
t_inf_int		*infint_div(t_inf_int *a, t_inf_int *b)
{
	t_inf_int	*n;

	if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
		return (NULL);
	(void)n;
	return (NULL);
}

/**
 *	a % b
**/
t_inf_int		*infint_mod(t_inf_int *a, t_inf_int *b)
{
	t_inf_int	*n;

	if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
		return (NULL);
	(void)n;
	return (NULL);
}

/**
 *	a ^ b
 *
 *	Complexity (N and M are the maximum number of bits in base2 storages):
 *	- O(p * (M * N))
**/
t_inf_int		*infint_pow(t_inf_int *a, t_inf_int *b)
{
	t_inf_int	*cnt;
	t_inf_int	*incr;
	t_inf_int	*cur;
	t_inf_int	*next;

	if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
		return (NULL);
	if (infint_is_negative(b))
		return (NULL);

	cnt = NULL;
	incr = NULL;
	cur = NULL;
	if (!(cnt = infint_new_with_value(0, INF_INT_POSITIVE))
	|| !(incr = infint_new_with_value(1, INF_INT_POSITIVE))
	|| !(cur = infint_new_with_value(1, INF_INT_POSITIVE)))
		goto err;

	while (infint_is_smaller(cnt, b))
	{
		if (!(next = infint_mul(cur, a)))
			goto err;
		infint_free(&cur);
		cur = next;
		if (!(next = infint_add(cnt, incr)))
			goto err;
		infint_free(&cnt);
		cnt = next;
	}
	goto end;

err :
	infint_free(&cur);
end :
	infint_free(&cnt);
	infint_free(&incr);
	return (cur);
}
