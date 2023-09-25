#include "core/defs.h"
#include "core/w_stringify.h"
#include "option.h"

/*sublime-c-static-fn-hoist-start*/
static void test_inits_none_directly(void **ts);
static void test_inits_some_directly(void **ts);
static void test_updates_value_from_none_to_some(void **ts);
static void test_updates_value_from_some_to_none(void **ts);
static void test_updates_value_from_some_to_some(void **ts);
static void test_and_returns_none_for_both_none(void **ts);
static void test_and_returns_none_for_none_and_some(void **ts);
static void test_and_returns_the_second_some_for_two_somes(void **ts);
static void test_or_returns_none_for_both_none(void **ts);
static void test_or_returns_some_for_none_and_some_1(void **ts);
static void test_or_returns_some_for_none_and_some_2(void **ts);
static void test_or_returns_the_first_some_for_two_somes(void **ts);
static void test_xor_returns_none_for_both_none(void **ts);
static void test_xor_returns_none_for_both_some(void **ts);
static void test_xor_returns_some_for_none_and_some_1(void **ts);
static void test_xor_returns_some_for_none_and_some_2(void **ts);
static void test_unwrap_or_for_none_returns_default_value(void **ts);
static void test_unwrap_or_for_some_returns_original_value(void **ts);
static void test_option_with_for_none_does_nothing(void **ts);
static void test_option_with_for_none_runs_else_block(void **ts);
static void test_option_with_for_some_runs_block_with_value(void **ts);
static void test_option_with_for_some_does_not_run_else_block(void **ts);
static void test_unwrap_or_else_with_some_returns_the_value(void **ts);
static void test_unwrap_or_else_with_none_returns_zeroed_value_and_runs_the_block(void **ts);
static void test_unwrap_or_else_with_some_and_missing_else_block_returns_the_value(void **ts);
static void test_unwrap_or_else_with_none_and_missing_else_block_returns_zeroed_value(void **ts);
static void test_pads_to_multiple_of_pointer_size(void **ts);
static void test_allows_forward_declaration(void **ts);
/*sublime-c-static-fn-hoist-end*/

#define assert_none(_option) \
  assert_true(option_is_none(_option)); \
  assert_false(option_is_some(_option)); \

#define assert_some(_option, _value) \
  assert_false(option_is_none(_option)); \
  assert_true(option_is_some(_option)); \
  assert_int_equal((_value), option_unwrap_unchecked(_option));

static void test_inits_none_directly(void **ts) {
  option_t(int) opt = option_init_none;

  assert_none(opt);
}

static void test_inits_some_directly(void **ts) {
  option_t(int) opt = option_init_some(111);

  assert_some(opt, 111);
}

static void test_updates_value_from_none_to_some(void **ts) {
  option_t(int) opt = option_init_none;
  opt = option_some(opt, 111);

  assert_some(opt, 111);
}

static void test_updates_value_from_some_to_none(void **ts) {
  option_t(int) opt = option_init_some(111);
  opt = option_none(opt);

  assert_none(opt);
}

static void test_updates_value_from_some_to_some(void **ts) {
  option_t(int) opt = option_init_some(111);
  opt = option_some(opt, 222);

  assert_some(opt, 222);
}

static void test_updates_value_from_none_to_some_via_set(void **ts) {
  option_t(int) opt = option_init_none;
  option_set_some(opt, 111);

  assert_some(opt, 111);
}

static void test_updates_value_from_some_to_none_via_set(void **ts) {
  option_t(int) opt = option_init_some(111);
  option_set_none(opt);

  assert_none(opt);
}

static void test_updates_value_from_some_to_some_via_set(void **ts) {
  option_t(int) opt = option_init_some(111);
  option_set_some(opt, 222);

  assert_some(opt, 222);
}

static void test_and_returns_none_for_both_none(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_none;
  option_int_t b = option_init_none;

  assert_none(option_and(a, b));
  assert_none(option_and(b, a));
}

static void test_and_returns_none_for_none_and_some(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_none;
  option_int_t b = option_init_some(222);

  assert_none(option_and(a, b));
  assert_none(option_and(b, a));
}

static void test_and_returns_the_second_some_for_two_somes(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_some(111);
  option_int_t b = option_init_some(222);

  assert_some(option_and(a, b), 222);
}

static void test_or_returns_none_for_both_none(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_none;
  option_int_t b = option_init_none;

  assert_none(option_or(a, b));
}

static void test_or_returns_some_for_none_and_some_1(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_some(111);
  option_int_t b = option_init_none;

  assert_some(option_or(a, b), 111);
}

static void test_or_returns_some_for_none_and_some_2(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_none;
  option_int_t b = option_init_some(222);

  assert_some(option_or(a, b), 222);
}

static void test_or_returns_the_first_some_for_two_somes(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_some(111);
  option_int_t b = option_init_some(222);

  assert_some(option_or(a, b), 111);
}

static void test_xor_returns_none_for_both_none(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_none;
  option_int_t b = option_init_none;

  assert_none(option_xor(a, b));
}

static void test_xor_returns_none_for_both_some(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_some(111);
  option_int_t b = option_init_some(222);

  assert_none(option_xor(a, b));
}

static void test_xor_returns_some_for_none_and_some_1(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_some(111);
  option_int_t b = option_init_none;

  assert_some(option_xor(a, b), 111);
}

static void test_xor_returns_some_for_none_and_some_2(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t a = option_init_none;
  option_int_t b = option_init_some(222);

  assert_some(option_xor(a, b), 222);
}

static void test_unwrap_or_for_none_returns_default_value(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t none = option_init_none;

  assert_int_equal(555, option_unwrap_or(none, 555));
}

static void test_unwrap_or_for_some_returns_original_value(void **ts) {
  typedef option_t(int) option_int_t;

  option_int_t none = option_init_some(111);

  assert_int_equal(111, option_unwrap_or(none, 555));
}

static void test_option_with_for_none_does_nothing(void **ts) {
  option_t(int) none = option_init_none;

  option_with(none, value) {
    (void) value;
    fail();
  }
}

static void test_option_with_for_none_runs_else_block(void **ts) {
  option_t(int) none = option_init_none;

  option_with(none, value) {
    (void) value;
  }

  else {
    return;
  }

  fail();
}

static void test_option_with_for_some_runs_block_with_value(void **ts) {
  option_t(int) some = option_init_some(123);

  int received_value = 0;

  option_with(some, value) {
    received_value = value;
  }

  assert_int_equal(123, received_value);
}

static void test_option_with_for_some_does_not_run_else_block(void **ts) {
  option_t(int) some = option_init_some(123);

  option_with(some, value) {
    (void) value;
  }

  else {
    fail();
  }
}

static void test_unwrap_or_else_with_some_returns_the_value(void **ts) {
  option_t(int) some = option_init_some(123);

  int value = option_unwrap_or_else(some) {
    fail();
  };

  assert_int_equal(123, value);
}

static void test_unwrap_or_else_with_none_returns_zeroed_value_and_runs_the_block(void **ts) {
  option_t(int) some = option_init_none;

  bool block_was_run = false;
  int value = option_unwrap_or_else(some) {
    block_was_run = true;
  }

  assert_int_equal(0, value);
  assert_true(block_was_run);
}

static void test_unwrap_or_else_with_some_and_missing_else_block_returns_the_value(void **ts) {
  // NOTE: While the missing else block is not really an intended use case, we
  // should still make sure nothing bad happens if it's left out.

  option_t(int) some = option_init_some(123);

  int value = option_unwrap_or_else(some);
  int other_value = 123;

  assert_int_equal(123, value);
  assert_int_equal(123, other_value);
}

static void test_unwrap_or_else_with_none_and_missing_else_block_returns_zeroed_value(void **ts) {
  // NOTE: While the missing else block is not really an intended use case, we
  // should still make sure nothing bad happens if it's left out.

  option_t(int) some = option_init_none;

  int value = option_unwrap_or_else(some);
  int other_value = 123;

  assert_int_equal(0, value);
  assert_int_equal(123, other_value);
}

#define assert_size_is_multiple_of(_option, _expected) { \
  size_t size = sizeof(_option); \
  \
  if ((size % (_expected)) != 0) { \
    fail_msg(\
      "sizeof(%s) was expected to be a multiple of %zu but it wasn't (got %zu)", \
      w_stringify(_option), \
      _expected, \
      size \
    ); \
  } \
}

static void test_pads_to_multiple_of_pointer_size(void **ts) {
  assert_size_is_multiple_of(option_padded_t(uint8_t), sizeof(void *));
  assert_size_is_multiple_of(option_padded_t(uint16_t), sizeof(void *));
  assert_size_is_multiple_of(option_padded_t(uint32_t), sizeof(void *));
  assert_size_is_multiple_of(option_padded_t(uint64_t), sizeof(void *));
  assert_size_is_multiple_of(option_padded_t(void *), sizeof(void *));

  typedef option_padded_t(struct { char x[1]; }) opt_char_1;
  assert_size_is_multiple_of(opt_char_1, sizeof(void *));

  typedef option_padded_t(struct { char x[2]; }) opt_char_2;
  assert_size_is_multiple_of(opt_char_2, sizeof(void *));

  typedef option_padded_t(struct { char x[3]; }) opt_char_3;
  assert_size_is_multiple_of(opt_char_3, sizeof(void *));

  typedef option_padded_t(struct { char x[4]; }) opt_char_4;
  assert_size_is_multiple_of(opt_char_4, sizeof(void *));

  typedef option_padded_t(struct { char x[5]; }) opt_char_5;
  assert_size_is_multiple_of(opt_char_5, sizeof(void *));

  typedef option_padded_t(struct { char x[6]; }) opt_char_6;
  assert_size_is_multiple_of(opt_char_6, sizeof(void *));

  typedef option_padded_t(struct { char x[7]; }) opt_char_7;
  assert_size_is_multiple_of(opt_char_7, sizeof(void *));

  typedef option_padded_t(struct { char x[8]; }) opt_char_8;
  assert_size_is_multiple_of(opt_char_8, sizeof(void *));

  typedef option_padded_t(struct { char x[9]; }) opt_char_9;
  assert_size_is_multiple_of(opt_char_9, sizeof(void *));

  typedef option_padded_t(struct { char x[10]; }) opt_char_10;
  assert_size_is_multiple_of(opt_char_10, sizeof(void *));

  typedef option_padded_t(struct { char x[11]; }) opt_char_11;
  assert_size_is_multiple_of(opt_char_11, sizeof(void *));

  typedef option_padded_t(struct { char x[12]; }) opt_char_12;
  assert_size_is_multiple_of(opt_char_12, sizeof(void *));

  typedef option_padded_t(struct { char x[13]; }) opt_char_13;
  assert_size_is_multiple_of(opt_char_13, sizeof(void *));

  typedef option_padded_t(struct { char x[14]; }) opt_char_14;
  assert_size_is_multiple_of(opt_char_14, sizeof(void *));

  typedef option_padded_t(struct { char x[15]; }) opt_char_15;
  assert_size_is_multiple_of(opt_char_15, sizeof(void *));

  typedef option_padded_t(struct { char x[16]; }) opt_char_16;
  assert_size_is_multiple_of(opt_char_16, sizeof(void *));
}

static void test_allows_forward_declaration(void **ts) {
  struct option_forward_decl_test_s;
  struct option_forward_decl_test_s option_d(int);

  w_unused struct option_forward_decl_test_s foo = option_init_some(123);

  // no assertions because the point is to make sure the above compiles
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_inits_none_directly),
    cmocka_unit_test(test_inits_some_directly),
    cmocka_unit_test(test_updates_value_from_none_to_some),
    cmocka_unit_test(test_updates_value_from_some_to_none),
    cmocka_unit_test(test_updates_value_from_some_to_some),
    cmocka_unit_test(test_updates_value_from_none_to_some_via_set),
    cmocka_unit_test(test_updates_value_from_some_to_none_via_set),
    cmocka_unit_test(test_updates_value_from_some_to_some_via_set),
    cmocka_unit_test(test_and_returns_none_for_both_none),
    cmocka_unit_test(test_and_returns_none_for_none_and_some),
    cmocka_unit_test(test_and_returns_the_second_some_for_two_somes),
    cmocka_unit_test(test_or_returns_none_for_both_none),
    cmocka_unit_test(test_or_returns_some_for_none_and_some_1),
    cmocka_unit_test(test_or_returns_some_for_none_and_some_2),
    cmocka_unit_test(test_or_returns_the_first_some_for_two_somes),
    cmocka_unit_test(test_xor_returns_none_for_both_none),
    cmocka_unit_test(test_xor_returns_none_for_both_some),
    cmocka_unit_test(test_xor_returns_some_for_none_and_some_1),
    cmocka_unit_test(test_xor_returns_some_for_none_and_some_2),
    cmocka_unit_test(test_unwrap_or_for_none_returns_default_value),
    cmocka_unit_test(test_unwrap_or_for_some_returns_original_value),
    cmocka_unit_test(test_option_with_for_none_does_nothing),
    cmocka_unit_test(test_option_with_for_none_runs_else_block),
    cmocka_unit_test(test_option_with_for_some_runs_block_with_value),
    cmocka_unit_test(test_option_with_for_some_does_not_run_else_block),
    cmocka_unit_test(test_unwrap_or_else_with_some_returns_the_value),
    cmocka_unit_test(test_unwrap_or_else_with_none_returns_zeroed_value_and_runs_the_block),
    cmocka_unit_test(test_unwrap_or_else_with_some_and_missing_else_block_returns_the_value),
    cmocka_unit_test(test_unwrap_or_else_with_none_and_missing_else_block_returns_zeroed_value),
    cmocka_unit_test(test_pads_to_multiple_of_pointer_size),
    cmocka_unit_test(test_allows_forward_declaration),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
