/*
 * Copyright © 2009 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hash_table.h"
#include "fnv_hash.h"

static uint32_t
key_value(const void *key)
{
	return *(uint32_t *)key;
}

static int
uint32_t_key_equals(const void *a, const void *b)
{
	return key_value(a) == key_value(b);
}

static int
uint32_t_key_is_even(struct hash_entry *entry)
{
	return (key_value(entry->key) & 1) == 0;
}

int
main(int argc, char **argv)
{
	struct hash_table *ht;
	struct hash_entry *entry;
	int size = 10000;
	uint32_t keys[size];
	uint32_t i, random_value;

	ht = hash_table_create(uint32_t_key_equals);

	for (i = 0; i < size; i++) {
		keys[i] = i;

		hash_table_insert(ht, i, keys + i, NULL);
	}

	/* Test the no-predicate case. */
	entry = hash_table_random_entry(ht, NULL);
	assert(entry);

	/* Check that we're getting different entries and that the predicate
	 * works.
	 */
	for (i = 0; i < 100; i++) {
		entry = hash_table_random_entry(ht, uint32_t_key_is_even);
		assert(entry);
		assert((key_value(entry->key) & 1) == 0);
		if (i == 0 || key_value(entry->key) != random_value)
			break;
		random_value = key_value(entry->key);
	}
	assert(i != 100);

	hash_table_destroy(NULL, NULL);

	return 0;
}
