#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "infinite_int.h"

/**
 *  Check if the number is zero
 *  Set the number to positive if it is true
**/
uint8_t infint_is_zero(t_inf_int *n)
{
    if (!infint_is_valid_nb(n))
        return (0);

    //Update real size
    while (n->size > 0)
        if (n->nb[--n->size] != 0)
            break;
    n->size++;
    if (n->size == 1 && *n->nb == 0)
    {
        n->sign = INF_INT_POSITIVE;
        return (1);
    }
    return (0);
}

/**
 *  Check if there are no unexpected values
**/
uint8_t infint_is_valid_nb(t_inf_int *n)
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
 *  Check if the sign has an expected value
**/
uint8_t infint_is_valid_sign(int8_t sign)
{
    return (sign == INF_INT_POSITIVE || sign == INF_INT_NEGATIVE);
}

/**
 *  Check if the base has an expected value
**/
uint8_t infint_is_valid_base(uint8_t base)
{
    return (base >= INF_INT_BASE_MIN && base <= INF_INT_BASE_MAX);
}

/**
 *  Return a string representation in the wanted bases
**/
char    *infint_to_string(t_inf_int *ref, uint8_t output_base)
{
    char        c;
    uintmax_t   i;
    t_inf_int   *n;
    uintmax_t   len;
    char        *out;
    int8_t      old_sign;
    t_inf_int   *base;
    t_inf_int   *modulo;
    t_inf_int   *divisor;

    if (!infint_is_valid_base(output_base))
        return (NULL);
    if (!infint_is_valid_nb(ref))
        return (NULL);
    if (infint_is_zero(ref))
        return (strdup("0"));

    n = NULL;
    out = NULL;
    base = NULL;

    //Prepare the calculation and the storage
    old_sign = ref->sign;
    ref->sign = INF_INT_POSITIVE;
    if (!(n = infint_clone(ref)))
        goto err;
    if (!(base = infint_new_with_value(output_base, INF_INT_POSITIVE)))
        goto err;
    len = ceil((n->size * INF_INT_NB_BITS) / (log(output_base) / log(2.0))) + 1;
    if (!(out = malloc((len + 1) * sizeof(char))))
        goto err;
    i = 0;
    while (i < len)
        out[i++] = '\0';

    //Perform the main loop
    i = 0;
    while (!infint_is_zero(n))
    {
        if (!infint_div_mod(n, base, &divisor, &modulo))
            goto err;
        out[i++] = INF_INT_ALPHABET[*modulo->nb];

        infint_free(&modulo);
        infint_free(&n);
        n = divisor;
    }
    if (old_sign == INF_INT_NEGATIVE)
        out[i++] = '-';
    len = i;

    //Reverse the string
    i = 0;
    while (i < (len + 1) / 2)
    {
        c = out[i];
        out[i] = out[len - i - 1];
        out[len - i - 1] = c;
        i++;
    }
    goto end;

err :
    if (out)
        free(out);
    out = NULL;
end :
    infint_free(&n);
    infint_free(&base);
    ref->sign = old_sign;
    return (out);
}

/**
 *  Check if the number is odd
**/
uint8_t infint_is_odd(t_inf_int *n)
{
    if (!infint_is_valid_nb(n))
        return (0);
    return (*n->nb & 1);
}

/**
 *  Check if the number is even
**/
uint8_t infint_is_even(t_inf_int *n)
{
    if (!infint_is_valid_nb(n))
        return (0);
    return (!(*n->nb & 1));
}

/**
 *  Check if the number is positive
**/
uint8_t infint_is_positive(t_inf_int *n)
{
    if (!infint_is_valid_nb(n))
        return (0);
    if (infint_is_zero(n))
        return (1);
    return (n->sign == INF_INT_POSITIVE);
}

/**
 *  Check if the number is negative
**/
uint8_t infint_is_negative(t_inf_int *n)
{
    if (!infint_is_valid_nb(n))
        return (0);
    if (infint_is_zero(n))
        return (0);
    return (n->sign == INF_INT_NEGATIVE);
}

/**
 *  Invert the sign of the number
**/
void    infint_invert(t_inf_int *n)
{
    if (infint_is_valid_nb(n) && !infint_is_zero(n))
        n->sign = infint_is_positive(n) ? INF_INT_NEGATIVE : INF_INT_POSITIVE;
}
