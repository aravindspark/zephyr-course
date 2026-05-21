/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 *   west twister -T tests/ring_buf -p native_sim/native/64
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	zassert_ok(rb_push(99), "Pushing an int 99 shall succeed");
	zexpect_equal(rb_count(), 1, " The count is expected to be 1 after pushing one integer");
	zassert_ok(rb_init(4), "Reinit must pass");

	zassert_true(rb_is_empty(), "Reinit buffer must be empty");
	zassert_equal(rb_count(), 0, "Buffer count must be 0 after reinit");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	int v;

	zassert_ok(rb_push(42), "Pushing value int 42 shall succeed");
	zassert_ok(rb_pop(&v), "Poping an value from buffer shall succeed");

	zassert_equal(v , 42, " The poped value must be equal to 42");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	int v;

	zassert_ok(rb_push(1), "Pushing value int 1 shall succeed.");
	zassert_ok(rb_push(2), "Pushing value int 2 shall succeed.");
	zassert_ok(rb_push(3), "Pushing value int 3 shall succeed.");

	/* rb_pop must return the oldest value from front of the buffer */
	/* [1,2,3] => rb_pop() => 1 */
	zassert_ok(rb_pop(&v), "Poping an value from buffer shall succeed");
	zassert_equal(v , 1, " The poped value must be equal to 1");

	/* Next oldest value : [2,3] => rb_pop() => 2  */
	zassert_ok(rb_pop(&v), "Poping an value from buffer shall succeed");
	zassert_equal(v , 2, " The poped value must be equal to 2");

	/* Next oldest value : [3] => rb_pop() => 3  */
	zassert_ok(rb_pop(&v), "Poping an value from buffer shall succeed");
	zassert_equal(v , 3, " The poped value must be equal to 3");

	zassert_true(rb_is_empty(), "buffer must be empty after 3x pop");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */

	for(int i=1; i <= 4 ; i++) {
		zassert_ok(rb_push(i), "Pushing value int %d shall succeed.", i);
	}

	 zassert_equal(rb_count(), 4 , "Buffer count should return 4 after pushing 4 values");
	 zassert_true(rb_is_full(), "The buffer must be full after 4 push");

	 zassert_equal(rb_push(99), -ENOSPC , "Pushing to a already full buffer should return -ENOSPC");
	 zassert_equal(rb_count(), 4 , "Buffer count shall not increase for the rejected push");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	int v;

	zassert_ok(rb_push(7), "Pushing a int value 7 must succeed");
	zexpect_equal(rb_count(), 1, "The buffer count is 1 after the push");

	zassert_ok(rb_peek(&v), "Peek on the buffer should succeed");
	zassert_equal(v, 7, "The peeked value must be 7 (oldest value)");
	zassert_equal(rb_count(), 1, "The peek should not consume the value rb_count must return 1");
	//reset v
	v = 0;
	zassert_ok(rb_peek(&v), "Second peek on the buffer should succeed");
	zassert_equal(v, 7, "Second peeked value must be 7 (oldest value)");
	zassert_equal(rb_count(), 1, "The peek should not consume the value rb_count must return 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */

	// zexpect_true(rb_is_empty(), "The buffer is expected to be empty");
	zassert_equal(rb_pop(NULL), -EINVAL , "A pop with NULL pointer argument must return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */

	for(int i=1; i <= 4 ; i++) {
		zassert_ok(rb_push(i), "Pushing value int %d shall succeed.", i);
	}

	zassert_true(rb_is_full(), "The buffer must be full after 4 pushes");
	zassert_equal(rb_count(), 4, "The buffer count must be 4");
}
