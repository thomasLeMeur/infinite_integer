#ifndef INFINITE_INT_H
# define INFINITE_INT_H

# include <stdint.h>
# include <limits.h>

/**
 *	The theorical limit is +/-(2 ^ ((sizeof(uintmax_t) * CHAR_BIT) * UINTMAX_T))
 *		With 64bits usual computer, this number is +/-(2 ^ 1180591620717411303424)
 *	Be carreful, there are concrete limits :
 *		- size uintmax_t overflows : will crash
 *		- depending on malloc (usually limited to SIZET_MAX) : will crash
 *			-> theorical : +/-(2 ^ SIZET_MAX)
 *			-> usual     : +/-(2 ^ 4294967296)
 *		- depending on time complexity for very big numbers : will loop
 *			-> add : O(2 * N)
 *			-> sub : O(2 * N)
 *			-> mul : O(M * N)
 *			-> pow : O(p * M * N)
 *			-> div : O()
 *			-> mod : O()
**/

# define INF_INT_NB_BITS	(sizeof(uintmax_t) * CHAR_BIT)
# define INF_INT_ALPHABET	("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ")
# define INF_INT_POSITIVE	(1)
# define INF_INT_NEGATIVE	(-1)
# define INF_INT_BASE_MIN	(2)
# define INF_INT_BASE_MAX	(36)

/**
 *	The struct which represents an infinite number grows to store it
 *	The scheme is [right bits, next left bits, next left bits, ...]
 *	Ex : 
 *		The number 18446744073709551616 (base 10) is stored as 
 *			10000000000000000000000000000000000000000000000000000000000000000 (base 2)
 *		With uintmax_t = uint64_t the number will be stored as :
 *			[
 *			0000000000000000000000000000000000000000000000000000000000000000,
 *			0000000000000000000000000000000000000000000000000000000000000001
 *			]
**/
typedef struct
{
	int8_t		sign;															///Sign of the number (positive or negative)
	uint8_t		carry;															///Carry for the operations

	uintmax_t	size;															///Number of uintmax_t needed to store the total number
	uintmax_t	*nb;															///Array of uintmax_t which grows to store the total number
}			t_inf_int;

/**
 *	Comparisons
**/
uint8_t		infint_is_equal(t_inf_int *a, t_inf_int *b);						///a == b
uint8_t		infint_is_nequal(t_inf_int *a, t_inf_int *b);						///a != b (!infint_is_equal)
uint8_t		infint_is_greater(t_inf_int *a, t_inf_int *b);						///a >  b
uint8_t		infint_is_smaller(t_inf_int *a, t_inf_int *b);						///a <  b
uint8_t		infint_is_greater_equal(t_inf_int *a, t_inf_int *b);				///a >= b (!infint_smaller)
uint8_t		infint_is_smaller_equal(t_inf_int *a, t_inf_int *b);				///a <= b (!infint_greater)

/**
 *	Operations
 *	Each operation allocate a new number to process. If it success, a is updated
**/
t_inf_int	*infint_add(t_inf_int *a, t_inf_int *b);							///a + b
t_inf_int	*infint_sub(t_inf_int *a, t_inf_int *b);							///a - b
t_inf_int	*infint_mul(t_inf_int *a, t_inf_int *b);							///a * b
t_inf_int	*infint_div(t_inf_int *a, t_inf_int *b);							///a / b
t_inf_int	*infint_mod(t_inf_int *a, t_inf_int *b);							///a % b
t_inf_int	*infint_pow(t_inf_int *a, t_inf_int *b);							///a ^ b (b has to be positive)

/**
 *	Allocations
**/
t_inf_int	*infint_new(void);													///Create a new number with 0 as value
t_inf_int	*infint_new_empty(void);											///Create a new number with all values set to 0 or NULL
t_inf_int	*infint_clone(t_inf_int *other);									///Create a deep copy of the passed argument
t_inf_int	*infint_new_with_size(uintmax_t size);								///Create a new number with storage size
t_inf_int	*infint_copy(t_inf_int *n, t_inf_int *other);						///Create a copy of the passed argument
t_inf_int	*infint_new_from_string(char *s, uint8_t base);						///Create a new number from a string with a given base
t_inf_int	*infint_update_size(t_inf_int *n, uintmax_t size);					///Resize the storage
t_inf_int	*infint_new_with_value(uintmax_t val, int8_t sign);					///Create a new number with value

/**
 *	Cleans
**/
void		infint_init(t_inf_int *n);											///Set all the values to 0 or NULL
void		infint_free(t_inf_int **n);											///Free the storage and the number
t_inf_int	*infint_reset(t_inf_int *n);										///Reset the number to default value
void		infint_clear(t_inf_int *n);											///Clear all the numbers values
void		infint_free_storage(t_inf_int *n);									///Free the storage

/**
 *	Utilities
**/
void		infint_invert(t_inf_int *n);										///Invert the sign of the number
uint8_t		infint_is_odd(t_inf_int *n);										///Check if the number is odd
uint8_t		infint_is_even(t_inf_int *n);										///Check if the number is even
uint8_t		infint_is_zero(t_inf_int *n);										///Check if the nb is zero, and set it sign to positive if it is true
uint8_t		infint_is_positive(t_inf_int *n);									///Check if the number is positive
uint8_t		infint_is_negative(t_inf_int *n);									///Check if the number is negative
uint8_t		infint_is_valid_nb(t_inf_int *n);									///Check if there are no unexpected values
uint8_t		infint_is_valid_sign(int8_t sign);									///Check if the sign has an expected value
uint8_t		infint_is_valid_base(uint8_t base);									///Check if the base has an expected value
char		*infint_to_string(t_inf_int *n, uint8_t output_base);				///Return a string representation in a wanted base

/**
 *	Unit tests
**/
void		infint_test_cleans(void);											///Perform some tests on the file cleans.c
void		infint_test_utilities(void);										///Perform some tests on the file utilities.c
void		infint_test_operations(void);										///Perform some tests on the file operations.c
void		infint_test_comparisons(void);										///Perform some tests on the file comparisons.c
void		infint_test_allocations(void);										///Perform some tests on the file allocations.c

#endif
