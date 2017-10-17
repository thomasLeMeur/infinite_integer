#include "infinite_int.h"

/**
 *  a == b
**/
uint8_t infint_is_equal(t_inf_int *a, t_inf_int *b)
{
    uintmax_t   i;

    if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
        return (0);
    if (infint_is_zero(a) && infint_is_zero(b))
        return (1);
    if (a->sign != b->sign)
        return (0);
    if (a->size != b->size)
        return (0);

    i = a->size;
    while (i-- > 0)
    {
        if (a->nb[i] != b->nb[i])
            return (0);
    }
    return (1);
}

/**
 *  a != b
**/
uint8_t infint_is_nequal(t_inf_int *a, t_inf_int *b)
{
    if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
        return (0);
    else
        return (!infint_is_equal(a, b));
}

/**
 *  a > b
**/
uint8_t infint_is_greater(t_inf_int *a, t_inf_int *b)
{
    uintmax_t   i;

    if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
        return (0);
    if (infint_is_zero(a) && infint_is_zero(b))
        return (0);
    if (a->sign != b->sign)
        return (infint_is_positive(a) ? 1 : 0);
    if (a->size != b->size)
    {
        if (infint_is_positive(a))
            return ((a->size > b->size) ? 1 : 0);
        else
            return ((a->size < b->size) ? 1 : 0);
    }

    i = a->size;
    while (i-- > 0)
    {
        if (a->nb[i] != b->nb[i])
        {
            if (infint_is_positive(a))
                return ((a->nb[i] > b->nb[i]) ? 1 : 0);
            else
                return ((a->nb[i] < b->nb[i]) ? 1 : 0);
        }
    }
    return (0);
}

/**
 *  a < b
**/
uint8_t infint_is_smaller(t_inf_int *a, t_inf_int *b)
{
    uintmax_t   i;

    if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
        return (0);
    if (infint_is_zero(a) && infint_is_zero(b))
        return (0);
    if (a->sign != b->sign)
        return (infint_is_negative(a) ? 1 : 0);
    if (a->size != b->size)
    {
        if (infint_is_positive(a))
            return ((a->size < b->size) ? 1 : 0);
        else
            return ((a->size > b->size) ? 1 : 0);
    }

    i = a->size;
    while (i-- > 0)
    {
        if (a->nb[i] != b->nb[i])
        {
            if (infint_is_positive(a))
                return ((a->nb[i] < b->nb[i]) ? 1 : 0);
            else
                return ((a->nb[i] > b->nb[i]) ? 1 : 0);
        }
    }
    return (0);
}

/**
 *  a >= b
**/
uint8_t infint_is_greater_equal(t_inf_int *a, t_inf_int *b)
{
    if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
        return (0);
    else
        return (!infint_is_smaller(a, b));
}

/**
 *  a <= b
**/
uint8_t infint_is_smaller_equal(t_inf_int *a, t_inf_int *b)
{
    if (!infint_is_valid_nb(a) || !infint_is_valid_nb(b))
        return (0);
    else
        return (!infint_is_greater(a, b));
}
