#ifndef __option_h__
#define __option_h__

//
// Based on https://doc.rust-lang.org/std/option/enum.Option.html
//

#include <stdbool.h>

//
// You can use the _t suffix (and typedef) to create anonymous structs when you
// don't need forward declarations. Otherwise, use the _d suffix to create
// named structs.
//
// struct anon_s {
//   option_t(bool) foo;
//   option_t(size_t) bar;
// };
//
// struct named_s;
// struct named_s option_d(struct anon_s);
//

#define option_t(_type) \
  struct option_d(_type)

#define option_d(_type) { \
  struct { _type value; } body; \
  struct { bool has_value; } header; \
}

// The intention with padding is to avoid any warnings from -Wpadded and allow
// comparing via memcmp(3).

#define option_padded_t(_type) \
  struct option_padded_d(_type)

#define option_padded_d(_type) { \
  union { void *pad; bool has_value; } header; \
  union { void *pad; _type value; } body; \
}

// There's also a packed variant if you really need it...

#define option_packed_t(_type) \
  struct option_packed_d(_type)

#define option_packed_d(_type) __attribute__((packed)) { \
    struct { _type value; } body; \
    struct { bool has_value; } header; \
  }

#define option_init_none \
  { .header.has_value = false }

#define option_init_some(_value) \
  { .header.has_value = true, .body.value = (_value) }

#define option_set_none(_option) \
  (_option).header.has_value = false

#define option_set_some(_option, _value) ( \
  (_option).header.has_value = true, \
  (_option).body.value = (_value) \
)

#define option_is_some(_option) \
  (_option).header.has_value

#define option_is_none(_option) \
  (!(_option).header.has_value)

#define option_or(_a, _b) ( \
  option_is_some(_a) \
    ? (_a) \
    : (_b) \
)

#define option_unwrap_unchecked(_option) \
  (_option).body.value

#define option_unwrap_or(_option, _default_value) ( \
  option_is_some(_option) \
    ? option_unwrap_unchecked(_option) \
    : (_default_value) \
)

#define option_unwrap_or_else_ptr(_option) \
  option_is_some(_option) \
    ? &option_unwrap_unchecked(_option) \
    : NULL; \
  \
  for (bool ran = false; !ran && option_is_none(_option); ran = true)

//
// Everything below uses GNU extensions. Supported by GCC and clang.
//

#define option_none(_option) \
  (__typeof(_option)) { .header.has_value = false }

#define option_some(_option, _value) \
  (__typeof(_option)) { .header.has_value = true, .body.value = (_value) }

#define option_and(_a, _b) ( \
  (option_is_some(_a) && option_is_some(_b)) \
    ? (_b) \
    : option_none(_a) \
)

#define option_xor(_a, _b) ( \
  option_is_some(_a) == option_is_some(_b) \
    ? option_none(_a) \
    : option_is_some(_a) \
      ? (_a) \
      : (_b) \
)

#define option_unwrap_or_else(_option) \
  option_is_some(_option) \
    ? option_unwrap_unchecked(_option) \
    : option_unwrap_unchecked(option_zero(_option)); \
  \
  for (bool ran = false; !ran && option_is_none(_option); ran = true)

#define option_zero(_option) ((__typeof(_option)) { 0 })

// This is sort of an `if let Some(value)` equivalent.
//
// NOTE: Because this declares a variable in the original scope to support an
// optional else block, you should make sure to have warnings enabled for
// variable shadowing to avoid unintended behavior (-Wshadow).
//
// You can of course scope this yourself but I admit it looks a bit funny if
// you omit the extra indentation:
//
//   {option_with(opt, value) {
//      printf("the value is %d\n", value);
//   }
//
//   else {
//     printf("there was no value\n");
//   }}

#define option_with(_option, _variable) \
  __typeof((_option).body.value) _variable = (_option).body.value; \
  \
  if (option_is_some(_option))

#define option_with_ptr(_option, _variable) \
  __typeof(&(_option).body.value) _variable = &(_option).body.value; \
  \
  if (option_is_some(_option))

//
// Scoped versions of the above.
//

#define option_scoped_with(_option, _variable, _block) { \
  __typeof((_option).body.value) _variable = (_option).body.value; \
  \
  if (option_is_some(_option)) _block; \
}

#define option_scoped_with_ptr(_option, _variable, _block) { \
  __typeof(&(_option).body.value) _variable = &(_option).body.value; \
  \
  if (option_is_some(_option)) _block; \
}

#define option_scoped_with_or_else(_option, _variable, _block, _else_block) { \
  __typeof((_option).body.value) _variable = (_option).body.value; \
  \
  if (option_is_some(_option)) _block; \
  else _else_block; \
}

#define option_scoped_with_or_else_ptr(_option, _variable, _block, _else_block) { \
  __typeof(&(_option).body.value) _variable = &(_option).body.value; \
  \
  if (option_is_some(_option)) _block; \
  else _else_block; \
}

#endif // __option_h__
