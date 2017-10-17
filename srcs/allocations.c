#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "infinite_int.h"

/**
 *  Create a new number with 0 as value
**/
t_inf_int   *infint_new(void)
{
    return (infint_new_with_value(0, INF_INT_POSITIVE));
}

/**
 *  Create a new number with all values set to 0 or NULL
**/
t_inf_int   *infint_new_empty(void)
{
    t_inf_int   *n;

    if (!(n = malloc(sizeof(t_inf_int))))
        return (NULL);
    n->sign = 0;
    n->carry = 0;
    n->size = 0;
    n->nb = NULL;
    return (n);
}

/**
 *  Create a new number with value
**/
t_inf_int   *infint_new_with_value(uintmax_t val, int8_t sign)
{
    t_inf_int   *n;

    if (!infint_is_valid_sign(sign))
        return (NULL);
    if (!(n = malloc(sizeof(t_inf_int))))
        return (NULL);
    if (!(n->nb = malloc(sizeof(uintmax_t))))
    {
        free(n);
        return (NULL);
    }

    n->sign = sign;
    n->carry = 0;
    n->size = 1;
    *n->nb = val;
    return (n);
}

/**
 *  Create a new number from a string with a given base
**/
t_inf_int   *infint_new_from_string(char *s, uint8_t input_base)
{
    uintmax_t   ind;
    int8_t      sign;
    char        *cur_c;
    char        *find;
    t_inf_int   *n;
    t_inf_int   *exp;
    t_inf_int   *pow;
    t_inf_int   *res;
    t_inf_int   *tmp;
    t_inf_int   *val;
    t_inf_int   *base;
    t_inf_int   *incr;

    if (!s || !infint_is_valid_base(input_base))
        return (NULL);
    if (*s == '-' || *s == '+')
    {
        sign = (*s == '+') ? INF_INT_POSITIVE : INF_INT_NEGATIVE;
        s++;
    }
    else
        sign = INF_INT_POSITIVE;
    if (!*s)
        return (NULL);

    //Check if there is no unexpected character and find the last digit of the string
    cur_c = s;
    while (*cur_c)
    {
        find = strchr(INF_INT_ALPHABET, toupper(*cur_c));
        if (find == NULL || find - INF_INT_ALPHABET >= input_base)
            return (NULL);
        cur_c++;
    }

    //Prepare the begin for the calculation
    n = NULL;
    exp = NULL;
    pow = NULL;
    res = NULL;
    val = NULL;
    base = NULL;
    incr = NULL;
    if (!(n = infint_new()) || !(exp = infint_new())
    || !(incr = infint_new_with_value(1, INF_INT_POSITIVE))
    || !(base = infint_new_with_value(input_base, INF_INT_POSITIVE)))
        goto err;

    //Main loop of the calculation
    while (cur_c-- > s)
    {
        //n += val * (base ^ exp)

        //Prepare the value to add
        ind = strchr(INF_INT_ALPHABET, toupper(*cur_c)) - INF_INT_ALPHABET;
        if (!(val = infint_new_with_value(ind, INF_INT_POSITIVE)))
            goto err;
        if (!(pow = infint_pow(base, exp)))
            goto err;
        if (!(res = infint_mul(val, pow)))
            goto err;

        //update the total number
        if (!(tmp = infint_add(n, res)))
            goto err;
        infint_free(&n);
        n = tmp;

        //Update the exponent
        if (!(tmp = infint_add(exp, incr)))
            goto err;
        infint_free(&exp);
        exp = tmp;

        infint_free(&val);
        infint_free(&pow);
        infint_free(&res);
    }
    n->sign = sign;
    goto end;

err :
    infint_free(&n);
end :
    infint_free(&exp);
    infint_free(&pow);
    infint_free(&res);
    infint_free(&val);
    infint_free(&base);
    infint_free(&incr);
    return (n);
}

/**
 *  Create a new number with storage size
**/
t_inf_int   *infint_new_with_size(uintmax_t size)
{
    uintmax_t   i;
    t_inf_int   *n;

    if (size == 0)
        return (infint_new_empty());
    if (!(n = malloc(sizeof(t_inf_int))))
        return (NULL);
    if (!(n->nb = malloc(size * sizeof(uintmax_t))))
    {
        free(n);
        return (NULL);
    }

    n->sign = INF_INT_POSITIVE;
    n->carry = 0;
    n->size = size;
    i = size;
    while (i-- > 0)
        n->nb[i] = 0;
    return (n);
}

/**
 *  Resize the storage
**/
t_inf_int   *infint_update_size(t_inf_int *n, uintmax_t size)
{
    uintmax_t   *tmp;

    if (!infint_is_valid_nb(n) || size == 0)
        return (NULL);
    if (!(tmp = realloc(n->nb, size * sizeof(uintmax_t))))
        return (NULL);
    n->nb = tmp;
    while (n->size < size)
        n->nb[n->size++] = 0;
    n->size = size;
    return (n);
}

/**
 *  Create a deep copy of the passed argument
**/
t_inf_int   *infint_clone(t_inf_int *other)
{
    uintmax_t   i;
    t_inf_int   *n;

    if (!infint_is_valid_nb(other))
        return (NULL);
    if (!(n = infint_new_with_size(other->size)))
        return (NULL);

    n->sign = other->sign;
    i = n->size;
    while (i-- > 0)
        n->nb[i] = other->nb[i];
    return (n);
}

/**
 *  Create a copy of the passed argument
**/
t_inf_int   *infint_copy(t_inf_int *n, t_inf_int *other)
{
    if (!n || !infint_is_valid_nb(other))
        return (NULL);
    n->sign = other->sign;
    n->carry = other->carry;
    n->size = other->size;
    n->nb = other->nb;
    return (n);
}
