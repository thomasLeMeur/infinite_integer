#include <stdlib.h>

#include "infinite_int.h"

/**
 *  a = max(abs(a), abs(b))
 *  b = min(abs(a), abs(b))
 *  Find if the operation result has to be reverted
 *
 *  (Needed by infint_add, infint_sub, infint_mul)
**/
static uint8_t  infint_sort_operands_with_revert(t_inf_int **a, t_inf_int **b, int8_t *olda, int8_t *oldb, uint8_t is_adding)
{
    int8_t      tmp1;
    t_inf_int   *tmp2;
    uint8_t     has_to_revert;

    /**
     *   3 -  2 =   3 - 2
     *  -2 - -3 =   3 - 2
     *   2 - -3 =   3 + 2
     *   3 - -2 =   3 + 2
     *
     *   2 -  3 = -(3 - 2)
     *  -3 - -2 = -(3 - 2)
     *  -3 -  2 = -(3 + 2)
     *  -2 -  3 = -(3 + 2)
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
     *  With a >= b :
     *   a +  b =   a + b
     *  -a + -b = -(a + b)
     *  -a +  b = -(a - b)
     *   a + -b =   a - b
    **/
    if (is_adding)
        has_to_revert = (infint_is_negative(*a));
    return (has_to_revert);
}

/**
 *  Set the number to positive
 *  Indicate if the result of the operation will have to be reverted
 *  Return 1 if we need to sub or 0 if we need to add
 *
 *  (Needed by infint_add, infint_sub)
**/
static uint8_t  infint_prepare_to_complement_func(t_inf_int *a, t_inf_int *b)
{
    uint8_t ret;

    ret = (a->sign != b->sign);
    a->sign = INF_INT_POSITIVE;
    b->sign = INF_INT_POSITIVE;
    return (ret);
}

/**
 *  Invert bits with carry
 *
 *  (Needed by infint_sub)
**/
static void infint_invert_bits(t_inf_int *n, uintmax_t elem_from, uintmax_t bit_from)
{
    uintmax_t   bit;
    uintmax_t   lim;
    uintmax_t   bit_cnt;
    uintmax_t   elem_cnt;

    if (!infint_is_valid_nb(n) || n->carry == 0)
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
 *  a + b
 *
 *  Complexity (N is the number of bits in base2 storage):
 *  - Stay with add : O(N / (sizeof(uintmax_t) * CHAR_BIT))
 *  - Go to sub     : O(N * 2)
**/
t_inf_int       *infint_add(t_inf_int *a, t_inf_int *b)
{
    t_inf_int   *n;
    int8_t      old_sign_a;
    int8_t      old_sign_b;
    uint8_t     has_to_carry;
    uint8_t     has_to_revert;

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
        if (n->size < b->size && UINTMAX_MAX - a->nb[n->size] - n->carry < b->nb[n->size])
            has_to_carry = 1;
        else
            has_to_carry = 0;
        n->nb[n->size] = a->nb[n->size] + n->carry;
        if (n->size < b->size)
            n->nb[n->size] += b->nb[n->size];
        n->carry = (has_to_carry) ? 1 : 0;

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
 *  a - b
 *
 *  Complexity (N is the number of bits in base2 storage):
 *  - Stay with sub : O(2 * N)
 *  - Go to add     : O(N / (sizeof(uintmax_t) * CHAR_BIT))
**/
t_inf_int       *infint_sub(t_inf_int *a, t_inf_int *b)
{
    t_inf_int   *n;
    uintmax_t   i;
    uintmax_t   j;
    uintmax_t   bit;
    uint8_t     nb_bits;
    int8_t      old_sign_a;
    int8_t      old_sign_b;
    uint8_t     has_to_revert;

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
     *  C A B R
     *  0 0 0 0
     *  0 0 1 1 -> c
     *  0 1 0 1
     *  0 1 1 0
     *  1 0 0 0
     *  1 0 1 0
     *  1 1 0 0
     *  1 1 1 1 -> c
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
 *  a * b
 *
 *  Complexity (N is the maximum number of bits in base2 storage):
 *  - O(N * O(add))
**/
t_inf_int       *infint_mul(t_inf_int *a, t_inf_int *b)
{
    t_inf_int   *n;
    uint8_t     sum;
    uintmax_t   i, j, bit;
    uintmax_t   a_i, a_j, a_bit;
    uintmax_t   n_i, n_j, n_bit;
    int8_t      old_sign_a;
    int8_t      old_sign_b;

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
 *  Perform the real division of the two numbers (assuming they are positives)
 *
 *  Complexity (N is the maximum number of bits in base2 storage):
 *  - O(N * O(sub))
 *
 *  (Needed by infint_div, infint_mod)
**/
uint8_t         infint_div_mod(t_inf_int *dividend, t_inf_int *divisor, t_inf_int **quotient, t_inf_int **remainder)
{
    uint8_t     ret;
    int8_t      old_sign_divisor;
    uintmax_t   cur_case;
    uintmax_t   cur_bit;
    t_inf_int   *incr;
    t_inf_int   *tmp;

    ret = 1;
    incr = NULL;
    if (remainder)
        *remainder = NULL;
    if (quotient)
        *quotient = NULL;

    if (!infint_is_valid_nb(dividend) || !infint_is_valid_nb(divisor) || !quotient || !remainder)
        return (0);
    if (infint_is_zero(divisor))
        return (0);

    old_sign_divisor = divisor->sign;
    divisor->sign = INF_INT_POSITIVE;
    if (!(*quotient = infint_new()) || !(*remainder = infint_new()) || !(incr = infint_new_with_value(1, INF_INT_POSITIVE)))
        goto err;

    cur_case = dividend->size;
    while (cur_case-- > 0)
    {
        cur_bit = INF_INT_NB_BITS;
        while (cur_bit-- > 0)
        {
            //remainder = (remainder << 1) | cur_bit
            if (!(tmp = infint_shift_left(*remainder, 1)))
                goto err;
            infint_free(remainder);
            *remainder = tmp;
            (*remainder)->nb[0] |= ((dividend->nb[cur_case] & ((uintmax_t)1 << cur_bit)) != 0);

            //Prepare the quotient to maybe add a 1
            if (!(tmp = infint_shift_left(*quotient, 1)))
                goto err;
            infint_free(quotient);
            *quotient = tmp;

            //If the quotient is smaller than the dividend, add a 0 (set by default with shift left) and continue
            if (infint_is_smaller(*remainder, divisor))
                continue;

            //Else add a 1 and update the value of the remainder
            (*quotient)->nb[0] |= 1;
            if (!(tmp = infint_sub(*remainder, divisor)))
                goto err;
            infint_free(remainder);
            *remainder = tmp;
        }
    }


    if (dividend->sign != old_sign_divisor)
    {
        if (!infint_is_zero(*remainder))
        {
            if (!(tmp = infint_sub(divisor, *remainder)))
                goto err;
            infint_free(remainder);
            *remainder = tmp;

            if (!(tmp = infint_add(*quotient, incr)))
                goto err;
            infint_free(quotient);
            *quotient = tmp;
        }
        infint_invert(*quotient);
    }
    if (old_sign_divisor == INF_INT_NEGATIVE)
        infint_invert(*remainder);
    goto end;

err :
    ret = 0;
    infint_free(quotient);
    infint_free(remainder);
end :
    infint_free(&incr);
    divisor->sign = old_sign_divisor;
    return (ret);
}

/**
 *  a / b
 *
 *  Complexity (N is the maximum number of bits in base2 storage):
 *  - O(N * O(sub))
**/
t_inf_int       *infint_div(t_inf_int *a, t_inf_int *b)
{
    t_inf_int   *quotient;
    t_inf_int   *remainder;

    if (infint_div_mod(a, b, &quotient, &remainder))
        infint_free(&remainder);
    return (quotient);
}

/**
 *  a % b
 *
 *  Complexity (N is the maximum number of bits in base2 storage):
 *  - O(N * O(sub))
**/
t_inf_int       *infint_mod(t_inf_int *a, t_inf_int *b)
{
    t_inf_int   *quotient;
    t_inf_int   *remainder;

    if (infint_div_mod(a, b, &quotient, &remainder))
        infint_free(&quotient);
    return (remainder);
}

/**
 *  a ^ b
 *
 *  Complexity (ip is the pow):
 *  - O(p * O(mul))
**/
t_inf_int       *infint_pow(t_inf_int *a, t_inf_int *b)
{
    t_inf_int   *cnt;
    t_inf_int   *incr;
    t_inf_int   *cur;
    t_inf_int   *next;

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

/**
 *  n << shift
**/
t_inf_int       *infint_shift_left(t_inf_int *n, uintmax_t shift)
{
    uintmax_t   case_n;
    uintmax_t   case_new;
    uintmax_t   bit_n;
    uintmax_t   bit_new;
    t_inf_int   *new;

    if (!(infint_is_valid_nb(n)))
        return (NULL);
    if (shift == 0)
        return (infint_clone(n));
    if (!(new = infint_new_with_size(n->size + 1 + ((shift - 1) / INF_INT_NB_BITS))))
        return (NULL);

    case_n = n->size;
    case_new = case_n + ((shift - 1) / INF_INT_NB_BITS);
    shift %= INF_INT_NB_BITS;
    while (case_n-- > 0)
    {
        bit_n = INF_INT_NB_BITS;
        while (bit_n-- > 0)
        {
            bit_new = (bit_n + shift) % INF_INT_NB_BITS;
            new->nb[case_new] |= (uintmax_t)((n->nb[case_n] & ((uintmax_t)1 << bit_n)) != 0) << bit_new;

            if (bit_new == 0)
                case_new--;
        }
    }

    //Update real size
    while (new->size > 0)
        if (new->nb[--new->size] != 0)
            break;
    new->size++;
    return (new);
}

/**
 *  n >> right
**/
t_inf_int       *infint_shift_right(t_inf_int *n, uintmax_t shift)
{
    uintmax_t   case_n;
    uintmax_t   case_new;
    uintmax_t   bit_n;
    uintmax_t   bit_new;
    t_inf_int   *new;

    if (!(infint_is_valid_nb(n)))
        return (NULL);
    if (shift == 0)
        return (infint_clone(n));
    if (shift / INF_INT_NB_BITS > n->size)
        return (infint_new());
    if (!(new = infint_new_with_size(n->size - (shift / INF_INT_NB_BITS))))
        return (NULL);

    case_n = n->size;
    case_new = case_n - (shift / INF_INT_NB_BITS) - 1;
    shift %= INF_INT_NB_BITS;
    while (case_n-- > 0)
    {
        bit_n = INF_INT_NB_BITS;
        while (bit_n-- > 0)
        {
            bit_new = (INF_INT_NB_BITS + (bit_n - shift)) % INF_INT_NB_BITS;
            new->nb[case_new] |= (uintmax_t)((n->nb[case_n] & ((uintmax_t)1 << bit_n)) != 0) << bit_new;

            if (case_new == 0 && bit_new == 0)
                break;
            if (bit_new == 0)
                case_new--;
        }
        if (case_new == 0 && bit_new == 0)
            break;
    }

    //Update real size
    while (new->size > 0)
        if (new->nb[--new->size] != 0)
            break;
    new->size++;
    return (new);
}
