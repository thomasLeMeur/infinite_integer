#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "infinite_int.h"

/**
 *	Perform some tests on cleans.c
**/
void	infint_test_cleans(void)
{
	t_inf_int	*ptr;
	t_inf_int	*n;

	ptr = NULL;
	printf("Enter in %s\n", __FUNCTION__);

	printf("Test infinit_free :\n");
	printf("NULL pointer\n");
	infint_free(NULL);
	printf("Pointer on NULL pointer\n");
	infint_free(&ptr);
	printf("Pointer on valid number\n");
	n = infint_new();
	infint_free(&n);
	assert(n == NULL);
	printf("\n");

	printf("Test infinit_free_storage :\n");
	printf("NULL pointer\n");
	infint_free_storage(NULL);
	printf("NULL storage\n");
	n = infint_new_empty();
	infint_free_storage(n);
	infint_free(&n);
	printf("Pointer on valid number\n");
	n = infint_new();
	infint_free_storage(n);
	assert(n != NULL && n->nb == NULL);
	infint_free(&n);
	printf("\n");

	printf("Test infinit_clear :\n");
	printf("NULL pointer\n");
	infint_clear(NULL);
	printf("Pointer on valid number\n");
	n = infint_new();
	infint_clear(n);
	assert(n != NULL && n->nb == NULL && n->sign == 0 && n->carry == 0 && n->size == 0);
	infint_free(&n);
	printf("\n");

	printf("Test infinit_reset :\n");
	printf("NULL pointer\n");
	infint_reset(NULL);
	printf("Pointer on valid number\n");
	n = infint_new();
	n = infint_reset(n);
	assert(n != NULL && n->nb != NULL && *n->nb == 0 && n->sign == INF_INT_POSITIVE  && n->carry == 0 && n->size == 1);
	infint_free(&n);
	printf("\n");

	printf("Test infinit_init :\n");
	printf("NULL pointer\n");
	infint_init(NULL);
	printf("Valid number\n");
	n = infint_new();
	free(n->nb);
	infint_init(n);
	assert(n != NULL && n->nb == NULL && n->sign == 0  && n->carry == 0 && n->size == 0);
	infint_free(&n);
	printf("\n");

	printf("Exit from %s\n", __FUNCTION__);
}

/**
 *	Perform some tests on operations.c
**/
void	infint_test_operations(void)
{
	t_inf_int	*a;
	t_inf_int	*b;
	t_inf_int	*r;

	printf("Enter in %s\n", __FUNCTION__);

	printf("Test infint_add :\n");
	a = infint_new();
	b = infint_new();
	printf("Invalid a\n");
	assert(infint_add(NULL, b) == NULL);
	printf("Invalid b\n");
	assert(infint_add(a, NULL) == NULL);
	printf("3 + 2\n");
	*a->nb = 3;
	*b->nb = 2;
	r = infint_add(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_positive(r));
	infint_free(&r);
	printf("3 + -2\n");
	infint_invert(b);
	r = infint_add(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_positive(r));
	infint_free(&r);
	printf("-3 + -2\n");
	infint_invert(a);
	r = infint_add(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_negative(r));
	infint_free(&r);
	printf("-3 + 2\n");
	infint_invert(b);
	r = infint_add(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_negative(r));
	infint_free(&r);
	printf("2 + 3\n");
	infint_invert(a);
	r = infint_add(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_positive(r));
	infint_free(&r);
	printf("2 + -3\n");
	infint_invert(a);
	r = infint_add(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_negative(r));
	infint_free(&r);
	printf("-2 + -3\n");
	infint_invert(b);
	r = infint_add(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_negative(r));
	infint_free(&r);
	printf("-2 + 3\n");
	infint_invert(a);
	r = infint_add(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_positive(r));
	infint_free(&r);
	printf("%ju + 1\n", UINTMAX_MAX);
	*a->nb = UINTMAX_MAX;
	infint_invert(b);
	r = infint_add(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 2 && *r->nb == 1 && r->nb[1] == 1 && infint_is_positive(r));
	printf("Prev + 2\n");
	infint_free(&a);
	a = r;
	r = infint_add(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 2 && *r->nb == 3 && r->nb[1] == 1 && infint_is_positive(r));
	infint_free(&r);
	infint_free(&a);
	infint_free(&b);
	printf("\n");

	printf("Test infint_sub :\n");
	a = infint_new();
	b = infint_new();
	printf("Invalid a\n");
	assert(infint_sub(NULL, b) == NULL);
	printf("Invalid b\n");
	assert(infint_sub(a, NULL) == NULL);
	printf("3 - 2\n");
	*a->nb = 3;
	*b->nb = 2;
	r = infint_sub(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_positive(r));
	infint_free(&r);
	printf("3 - -2\n");
	infint_invert(b);
	r = infint_sub(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_positive(r));
	infint_free(&r);
	printf("-3 - -2\n");
	infint_invert(a);
	r = infint_sub(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_negative(r));
	infint_free(&r);
	printf("-3 - 2\n");
	infint_invert(b);
	r = infint_sub(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_negative(r));
	infint_free(&r);
	printf("2 - 3\n");
	infint_invert(a);
	r = infint_sub(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_negative(r));
	infint_free(&r);
	printf("2 - -3\n");
	infint_invert(a);
	r = infint_sub(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_positive(r));
	infint_free(&r);
	printf("-2 - -3\n");
	infint_invert(b);
	r = infint_sub(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_positive(r));
	infint_free(&r);
	printf("-2 - 3\n");
	infint_invert(a);
	r = infint_sub(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 5 && infint_is_negative(r));
	infint_free(&r);
	printf("1024 - 1\n");
	*a->nb = 1024;
	infint_invert(b);
	*b->nb = 1;
	r = infint_sub(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1023 && infint_is_positive(r));
	infint_free(&r);
	printf("(%ju + 2) - 1\n", UINTMAX_MAX);
	*a->nb = UINTMAX_MAX;
	*b->nb = 2;
	r = infint_add(a, b);
	infint_free(&a);
	a = r;
	*b->nb = 1;
	r = infint_sub(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 2 && *r->nb == 0 && r->nb[1] == 1 && infint_is_positive(r));
	printf("Prev - 1\n");
	infint_free(&a);
	a = r;
	r = infint_sub(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == UINTMAX_MAX && infint_is_positive(r));
	infint_free(&r);
	infint_free(&a);
	infint_free(&b);
	printf("\n");

	printf("Test infint_mul :\n");
	a = infint_new();
	b = infint_new();
	printf("Invalid a\n");
	assert(infint_mul(NULL, b) == NULL);
	printf("Invalid b\n");
	assert(infint_mul(a, NULL) == NULL);
	printf("0 * 2\n");
	*b->nb = 2;
	r = infint_mul(a, b);
	assert(infint_is_zero(r) == 1);
	infint_free(&r);
	printf("2 * 0\n");
	r = infint_mul(b, a);
	assert(infint_is_zero(r) == 1);
	infint_free(&r);
	printf("1245 * 678\n");
	*a->nb = 1245;
	*b->nb = 678;
	r = infint_mul(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 844110 && infint_is_positive(r));
	infint_free(&r);
	printf("-678 * 1245\n");
	infint_invert(b);
	r = infint_mul(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 844110 && infint_is_negative(r));
	infint_free(&r);
	printf("-1245 * -678\n");
	infint_invert(a);
	r = infint_mul(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 844110 && infint_is_positive(r));
	infint_free(&r);
	printf("678 * -1245\n");
	infint_invert(b);
	r = infint_mul(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 844110 && infint_is_negative(r));
	infint_free(&r);
	printf("%ju * 3\n", UINTMAX_MAX);
	infint_invert(a);
	*a->nb = UINTMAX_MAX;
	*b->nb = 3;
	r = infint_mul(b, a);
	assert(r != NULL && r->nb != NULL && r->size == 2 && *r->nb == UINTMAX_MAX - 2 && r->nb[1] == 2 && infint_is_positive(r));
	infint_free(&r);
	infint_free(&a);
	infint_free(&b);
	printf("\n");

	printf("Test infint_pow :\n");
	a = infint_new();
	b = infint_new();
	printf("Invalid a\n");
	assert(infint_pow(NULL, b) == NULL);
	printf("Invalid b\n");
	assert(infint_pow(a, NULL) == NULL);
	printf("Negative b\n");
	*b->nb = 2;
	infint_invert(b);
	assert(infint_pow(a, b) == NULL);
	printf("3 ^ 0\n");
	*a->nb = 3;
	infint_invert(b);
	*b->nb = 0;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_positive(r));
	infint_free(&r);
	printf("3 ^ 1\n");
	*b->nb = 1;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 3 && infint_is_positive(r));
	infint_free(&r);
	printf("3 ^ 12\n");
	*b->nb = 12;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 531441 && infint_is_positive(r));
	infint_free(&r);
	printf("3 ^ 41\n");
	*b->nb = 41;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 2 && *r->nb == (uintmax_t)18026252303461234787UL && r->nb[1] == 1 && infint_is_positive(r));
	infint_free(&r);
	printf("-3 ^ 0\n");
	infint_invert(a);
	*b->nb = 0;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 1 && infint_is_positive(r));
	infint_free(&r);
	printf("-3 ^ 1\n");
	*b->nb = 1;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 3 && infint_is_negative(r));
	infint_free(&r);
	printf("-3 ^ 12\n");
	*b->nb = 12;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 1 && *r->nb == 531441 && infint_is_positive(r));
	infint_free(&r);
	printf("-3 ^ 41\n");
	*b->nb = 41;
	r = infint_pow(a, b);
	assert(r != NULL && r->nb != NULL && r->size == 2 && *r->nb == (uintmax_t)18026252303461234787UL && r->nb[1] == 1 && infint_is_negative(r));
	infint_free(&r);
	infint_free(&a);
	infint_free(&b);
	printf("\n");

	//test div : 
	//	- invalid a
	//	- invalid b
	//	- b = 0
	//	-  3 /  2
	//	-  3 / -2
	//	- -3 / -2
	//	- -3 /  2
	//	-  2 /  3
	//	-  2 / -3
	//	- -2 / -3
	//	- -2 /  3
	//	- UINTMAX_MAX / 3
	//	- (UINTMAX_MAX + 1) / 3

	//test mod : 
	//	- invalid a
	//	- invalid b
	//	- b = 0
	//	-  3 %  2
	//	-  3 % -2
	//	- -3 % -2
	//	- -3 %  2
	//	-  2 %  3
	//	-  2 % -3
	//	- -2 % -3
	//	- -2 %  3
	//	- UINTMAX_MAX % 3
	//	- (UINTMAX_MAX + 1) % 3

	printf("Exit from %s\n", __FUNCTION__);
}

/**
 *	Perform some tests on utilities.c
**/
void	infint_test_utilities(void)
{
	int8_t		i8_cnt;
	uint8_t		u8_cnt;
	void		*back;
	t_inf_int	n;

	n.nb = NULL;
	printf("Enter in %s\n", __FUNCTION__);

	u8_cnt = 0;
	printf("Test infinit_is_valid_base :\n");
	while (1)
	{
		printf("%d\n", u8_cnt);
		assert(infint_is_valid_base(u8_cnt) == ((u8_cnt >= INF_INT_BASE_MIN && u8_cnt <= INF_INT_BASE_MAX) ? 1 : 0));
		if (++u8_cnt == 0)
			break;
	}
	printf("\n");

	i8_cnt = INT8_MIN;
	printf("Test infinit_is_valid_sign :\n");
	while (1)
	{
		printf("%d\n", i8_cnt);
		assert(infint_is_valid_sign(i8_cnt) == ((i8_cnt == INF_INT_POSITIVE || i8_cnt == INF_INT_NEGATIVE) ? 1 : 0));
		if (++i8_cnt == INT8_MIN)
			break;
	}
	printf("\n");

	printf("Test infinit_is_zero :\n");
	infint_reset(&n);
	n.sign = INF_INT_NEGATIVE;
	n.size = 0;
	printf("Invalid number\n");
	assert(infint_is_zero(&n) == 0);
	n.size = 2;
	printf("Number stored with two elements\n");
	assert(infint_is_zero(&n) == 0);
	n.size = 1;
	*n.nb = 1;
	printf("Number stored with one element of value non zero\n");
	assert(infint_is_zero(&n) == 0);
	*n.nb = 0;
	printf("Number stored with one element of value 0\n");
	assert(infint_is_zero(&n) == 1 && n.sign == INF_INT_POSITIVE);
	infint_clear(&n);
	free(n.nb);
	printf("\n");

	printf("Test infinit_is_valid_nb :\n");
	infint_reset(&n);
	printf("NULL pointer\n");
	assert(infint_is_valid_nb(NULL) == 0);
	n.sign = 0;
	printf("Invalid sign\n");
	assert(infint_is_valid_nb(&n) == 0);
	n.sign = INF_INT_POSITIVE;
	n.size = 0;
	printf("Invalid size\n");
	assert(infint_is_valid_nb(&n) == 0);
	n.size = 1;
	back = n.nb;
	n.nb = NULL;
	printf("NULL storage pointer\n");
	assert(infint_is_valid_nb(&n) == 0);
	n.nb = back;
	printf("Valid number\n");
	assert(infint_is_valid_nb(&n) == 1);
	infint_clear(&n);
	free(n.nb);
	printf("\n");

	printf("Test infint_is_odd : \n");
	printf("Invalid number\n");
	assert(infint_is_odd(NULL) == 0);
	printf("0\n");
	infint_reset(&n);
	assert(infint_is_odd(&n) == 0);
	printf("1\n");
	*n.nb = 1;
	assert(infint_is_odd(&n) == 1);
	infint_clear(&n);
	printf("\n");

	printf("Test infint_is_even : \n");
	printf("Invalid number\n");
	assert(infint_is_even(NULL) == 0);
	printf("0\n");
	infint_reset(&n);
	assert(infint_is_even(&n) == 1);
	printf("1\n");
	*n.nb = 1;
	assert(infint_is_even(&n) == 0);
	infint_clear(&n);
	printf("\n");

	printf("Test infint_is_positive : \n");
	printf("Invalid number\n");
	assert(infint_is_positive(NULL) == 0);
	printf("+0\n");
	infint_reset(&n);
	assert(infint_is_positive(&n) == 1);
	printf("-0\n");
	n.sign = INF_INT_NEGATIVE;
	assert(infint_is_positive(&n) == 1);
	printf("-5\n");
	*n.nb = 5;
	n.sign = INF_INT_NEGATIVE;
	assert(infint_is_positive(&n) == 0);
	printf("+5\n");
	n.sign = INF_INT_POSITIVE;
	assert(infint_is_positive(&n) == 1);
	infint_clear(&n);
	printf("\n");

	printf("Test infint_is_negative : \n");
	printf("Invalid number\n");
	assert(infint_is_negative(NULL) == 0);
	printf("+0\n");
	infint_reset(&n);
	assert(infint_is_negative(&n) == 0);
	printf("-0\n");
	n.sign = INF_INT_NEGATIVE;
	assert(infint_is_negative(&n) == 0);
	printf("-5\n");
	*n.nb = 5;
	n.sign = INF_INT_NEGATIVE;
	assert(infint_is_negative(&n) == 1);
	printf("+5\n");
	n.sign = INF_INT_POSITIVE;
	assert(infint_is_negative(&n) == 0);
	infint_clear(&n);
	printf("\n");

	printf("Test infint_invert :\n");
	printf("Invalid number\n");
	infint_invert(NULL);
	infint_reset(&n);
	printf("+0\n");
	infint_invert(&n);
	assert(n.sign == INF_INT_POSITIVE);
	printf("-0\n");
	n.sign = INF_INT_NEGATIVE;
	infint_invert(&n);
	assert(n.sign == INF_INT_POSITIVE);
	printf("+5\n");
	n.sign = INF_INT_POSITIVE;
	*n.nb = 5;
	infint_invert(&n);
	assert(n.sign == INF_INT_NEGATIVE);
	printf("-5\n");
	infint_invert(&n);
	assert(n.sign == INF_INT_POSITIVE);
	infint_clear(&n);
	printf("\n");

	printf("Test infinit_to_string :\n");
	//create some number (neg, pos, 0, short, big)
	//test some output bases
	printf("\n");

	printf("Exit from %s\n", __FUNCTION__);
}

/**
 *	Perform some tests on comparisons.c
**/
void	infint_test_comparisons(void)
{
	t_inf_int	a;
	t_inf_int	b;

	a.nb = NULL;
	b.nb = NULL;
	printf("Enter in %s\n", __FUNCTION__);

	printf("Test infint_is_equal (==) :\n");
	infint_reset(&a);
	infint_reset(&b);
	a.size = 0;
	printf("Invalid a\n");
	assert(infint_is_equal(&a, &b) == 0);
	a.size = 1;
	b.size = 0;
	printf("Invalid b\n");
	assert(infint_is_equal(&a, &b) == 0);
	b.size = 1;
	b.sign = INF_INT_NEGATIVE;
	printf("a = +0 / b = -0\n");
	assert(infint_is_equal(&a, &b) == 1);
	b.sign = INF_INT_NEGATIVE;
	*a.nb = 5;
	*b.nb = 5;
	printf("a = +5 / b = -5\n");
	assert(infint_is_equal(&a, &b) == 0);
	b.sign = INF_INT_POSITIVE;
	a.size = 2;
	printf("Storage size of a = 2 / storage size of b = 1\n");
	assert(infint_is_equal(&a, &b) == 0);
	a.size = 1;
	*b.nb = 4;
	printf("a = 5 / b = 4\n");
	assert(infint_is_equal(&a, &b) == 0);
	*b.nb = 5;
	printf("a = 5 / b = 5\n");
	assert(infint_is_equal(&a, &b) == 1);
	printf("\n");

	printf("Test infint_is_nequal (!=) :\n");
	infint_reset(&a);
	infint_reset(&b);
	a.size = 0;
	printf("Invalid a\n");
	assert(infint_is_nequal(&a, &b) == 0);
	a.size = 1;
	b.size = 0;
	printf("Invalid b\n");
	assert(infint_is_nequal(&a, &b) == 0);
	b.size = 1;
	b.sign = INF_INT_NEGATIVE;
	printf("a = +0 / b = -0\n");
	assert(infint_is_nequal(&a, &b) == 0);
	b.sign = INF_INT_NEGATIVE;
	*a.nb = 5;
	*b.nb = 5;
	printf("a = +5 / b = -5\n");
	assert(infint_is_nequal(&a, &b) == 1);
	b.sign = INF_INT_POSITIVE;
	a.size = 2;
	printf("Storage size of a = 2 / storage size of b = 1\n");
	assert(infint_is_nequal(&a, &b) == 1);
	a.size = 1;
	*b.nb = 4;
	printf("a = 5 / b = 4\n");
	assert(infint_is_nequal(&a, &b) == 1);
	*b.nb = 5;
	printf("a = 5 / b = 5\n");
	assert(infint_is_nequal(&a, &b) == 0);
	printf("\n");

	printf("Test infint_is_greater (>) :\n");
	infint_reset(&a);
	infint_reset(&b);
	a.size = 0;
	printf("Invalid a\n");
	assert(infint_is_greater(&a, &b) == 0);
	a.size = 1;
	b.size = 0;
	printf("Invalid b\n");
	assert(infint_is_greater(&a, &b) == 0);
	b.size = 1;
	b.sign = INF_INT_NEGATIVE;
	printf("a = +0 / b = -0\n");
	assert(infint_is_greater(&a, &b) == 0);
	b.sign = INF_INT_NEGATIVE;
	*a.nb = 5;
	*b.nb = 5;
	printf("a = +5 / b = -5\n");
	assert(infint_is_greater(&a, &b) == 1);
	b.sign = INF_INT_POSITIVE;
	a.size = 2;
	printf("Storage size of +a = 2 / storage size of +b = 1\n");
	assert(infint_is_greater(&a, &b) == 1);
	a.sign = INF_INT_NEGATIVE;
	b.sign = INF_INT_NEGATIVE;
	printf("Storage size of -a = 2 / storage size of -b = 1\n");
	assert(infint_is_greater(&a, &b) == 0);
	a.size = 1;
	*b.nb = 4;
	printf("a = -5 / b = -4\n");
	assert(infint_is_greater(&a, &b) == 0);
	a.sign = INF_INT_POSITIVE;
	b.sign = INF_INT_POSITIVE;
	printf("a = 5 / b = 4\n");
	assert(infint_is_greater(&a, &b) == 1);
	*b.nb = 7;
	printf("a = 5 / b = 7\n");
	assert(infint_is_greater(&a, &b) == 0);
	*b.nb = 5;
	printf("a = 5 / b = 5\n");
	assert(infint_is_greater(&a, &b) == 0);
	printf("\n");

	printf("Test infint_is_smaller_equal (<=) :\n");
	infint_reset(&a);
	infint_reset(&b);
	a.size = 0;
	printf("Invalid a\n");
	assert(infint_is_smaller_equal(&a, &b) == 0);
	a.size = 1;
	b.size = 0;
	printf("Invalid b\n");
	assert(infint_is_smaller_equal(&a, &b) == 0);
	b.size = 1;
	b.sign = INF_INT_NEGATIVE;
	printf("a = +0 / b = -0\n");
	assert(infint_is_smaller_equal(&a, &b) == 1);
	b.sign = INF_INT_NEGATIVE;
	*a.nb = 5;
	*b.nb = 5;
	printf("a = +5 / b = -5\n");
	assert(infint_is_smaller_equal(&a, &b) == 0);
	b.sign = INF_INT_POSITIVE;
	a.size = 2;
	printf("Storage size of a = 2 / storage size of b = 1\n");
	assert(infint_is_smaller_equal(&a, &b) == 0);
	a.size = 1;
	*b.nb = 4;
	printf("a = 5 / b = 4\n");
	assert(infint_is_smaller_equal(&a, &b) == 0);
	*b.nb = 7;
	printf("a = 5 / b = 7\n");
	assert(infint_is_smaller_equal(&a, &b) == 1);
	printf("\n");

	printf("Test infint_is_smaller (<) :\n");
	infint_reset(&a);
	infint_reset(&b);
	a.size = 0;
	printf("Invalid a\n");
	assert(infint_is_smaller(&a, &b) == 0);
	a.size = 1;
	b.size = 0;
	printf("Invalid b\n");
	assert(infint_is_smaller(&a, &b) == 0);
	b.size = 1;
	b.sign = INF_INT_NEGATIVE;
	printf("a = +0 / b = -0\n");
	assert(infint_is_smaller(&a, &b) == 0);
	b.sign = INF_INT_NEGATIVE;
	*a.nb = 5;
	*b.nb = 5;
	printf("a = +5 / b = -5\n");
	assert(infint_is_smaller(&a, &b) == 0);
	b.sign = INF_INT_POSITIVE;
	a.size = 2;
	printf("Storage size of +a = 2 / storage size of +b = 1\n");
	assert(infint_is_smaller(&a, &b) == 0);
	a.sign = INF_INT_NEGATIVE;
	b.sign = INF_INT_NEGATIVE;
	printf("Storage size of -a = 2 / storage size of -b = 1\n");
	assert(infint_is_smaller(&a, &b) == 1);
	a.size = 1;
	*b.nb = 4;
	printf("a = -5 / b = -4\n");
	assert(infint_is_smaller(&a, &b) == 1);
	a.sign = INF_INT_POSITIVE;
	b.sign = INF_INT_POSITIVE;
	printf("a = 5 / b = 4\n");
	assert(infint_is_smaller(&a, &b) == 0);
	*b.nb = 7;
	printf("a = 5 / b = 7\n");
	assert(infint_is_smaller(&a, &b) == 1);
	*b.nb = 5;
	printf("a = 5 / b = 5\n");
	assert(infint_is_smaller(&a, &b) == 0);
	printf("\n");

	printf("Test infint_is_greater_equal (>=) :\n");
	infint_reset(&a);
	infint_reset(&b);
	a.size = 0;
	printf("Invalid a\n");
	assert(infint_is_greater_equal(&a, &b) == 0);
	a.size = 1;
	b.size = 0;
	printf("Invalid b\n");
	assert(infint_is_greater_equal(&a, &b) == 0);
	b.size = 1;
	b.sign = INF_INT_NEGATIVE;
	printf("a = +0 / b = -0\n");
	assert(infint_is_greater_equal(&a, &b) == 1);
	b.sign = INF_INT_NEGATIVE;
	*a.nb = 5;
	*b.nb = 5;
	printf("a = +5 / b = -5\n");
	assert(infint_is_greater_equal(&a, &b) == 1);
	b.sign = INF_INT_POSITIVE;
	a.size = 2;
	printf("Storage size of a = 2 / storage size of b = 1\n");
	assert(infint_is_greater_equal(&a, &b) == 1);
	a.size = 1;
	*b.nb = 4;
	printf("a = 5 / b = 4\n");
	assert(infint_is_greater_equal(&a, &b) == 1);
	*b.nb = 7;
	printf("a = 5 / b = 7\n");
	assert(infint_is_greater_equal(&a, &b) == 0);
	printf("\n");

	infint_free_storage(&a);
	infint_free_storage(&b);
	printf("Exit from %s\n", __FUNCTION__);
}

/**
 *	Perform some tests on allocations.c
**/
void	infint_test_allocations(void)
{
	int			i;
	t_inf_int	*n;
	t_inf_int	*clone;
	int			proof;

	printf("Enter in %s\n", __FUNCTION__);

	printf("Test infint_new_empty :\n");
	n = infint_new_empty();
	assert(n != NULL && n->sign == 0 && n->carry == 0 && n->size == 0 && n->nb == NULL);
	infint_free(&n);
	printf("\n");

	printf("Test infint_new :\n");
	n = infint_new();
	assert(n != NULL && n->sign == INF_INT_POSITIVE && n->carry == 0 && n->size == 1 && n->nb != NULL && *n->nb == 0);
	infint_free(&n);
	printf("\n");

	printf("Test infint_new_with_size :\n");
	printf("Size = 0\n");
	n = infint_new_with_size(0);
	assert(n != NULL && n->sign == 0 && n->carry == 0 && n->size == 0 && n->nb == NULL);
	infint_free(&n);
	printf("Size > 0\n");
	n = infint_new_with_size(100);
	proof = 0;
	if (n && n->nb && *n->nb == 0)
	{
		i = 0;
		proof = 1;
		while (i < 100)
			proof = (proof && (n->nb[i++] == 0));
	}
	assert(n != NULL && n->sign == INF_INT_POSITIVE && n->carry == 0 && n->size == 100 && n->nb != NULL && proof);
	infint_free(&n);
	printf("\n");

	printf("Test infint_new_with_value :\n");
	printf("Invalid sign\n");
	n = infint_new_with_value(0, 0);
	assert(n == NULL);
	printf("Value = +0\n");
	n = infint_new_with_value(0, INF_INT_POSITIVE);
	assert(n != NULL && n->sign == INF_INT_POSITIVE && n->carry == 0 && n->size == 1 && n->nb != NULL && *n->nb == 0);
	infint_free(&n);
	printf("Value = -9876543210\n");
	n = infint_new_with_value(9876543210L, INF_INT_NEGATIVE);
	assert(n != NULL && n->sign == INF_INT_NEGATIVE && n->carry == 0 && n->size == 1 && n->nb != NULL && *n->nb == 9876543210L);
	infint_free(&n);
	printf("\n");

	printf("Test infint_clone :\n");
	printf("Invalid number\n");
	assert(infint_clone(NULL) == NULL);
	printf("Valid number\n");
	n = infint_new();
	clone = infint_clone(n);
	assert(clone != NULL && clone->nb != NULL && clone->sign == n->sign && clone->size == n->size && *clone->nb == *n->nb);
	infint_free(&n);
	infint_free(&clone);
	printf("\n");

	printf("Test infint_copy :\n");
	printf("Invalid number\n");
	assert(infint_copy(NULL, NULL) == NULL);
	printf("Valid number\n");
	n = infint_new();
	clone = infint_new_empty();
	clone = infint_copy(clone, n);
	assert(clone != NULL && clone->sign == n->sign && clone->size == n->size && clone->nb == n->nb);
	infint_free(&n);
	clone->nb = NULL;
	infint_free(&clone);
	printf("\n");

	printf("Test infint_update_size :\n");
	printf("Invalid number\n");
	assert(infint_update_size(NULL, 2) == NULL);
	printf("Size = 0\n");
	n = infint_new();
	assert(infint_update_size(n, 0) == NULL);
	printf("Equal size\n");
	*n->nb = 42;
	n = infint_update_size(n, n->size);
	assert(n != NULL && n->nb != NULL && n->size == 1 && *n->nb == 42);
	printf("Greater size\n");
	n = infint_update_size(n, n->size + 1);
	assert(n != NULL && n->nb != NULL && n->size == 2 && *n->nb == 42 && n->nb[1] == 0);
	printf("Smaller size\n");
	n = infint_update_size(n, n->size - 1);
	assert(n != NULL && n->nb != NULL && n->size == 1 && *n->nb == 42);
	infint_free(&n);
	printf("\n");

	printf("Test infint_from_string :\n");
	//Test :
	//	- wrong alphabet
	//	- wrong +-
	//	- some bases
	//	- some short and big numbers
	//	- comparison with infint_to_string ?
	printf("\n");

	printf("Exit from %s\n", __FUNCTION__);
}
