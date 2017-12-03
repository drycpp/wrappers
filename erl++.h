/* This is free and unencumbered software released into the public domain. */

#ifndef ERLXX_H
#define ERLXX_H

/**
 * <erl++.h>
 */

#ifndef __cplusplus
#error "<erl++.h> requires a C++ compiler"
#endif

#if __cplusplus < 201703L
#error "<erl++.h> requires a C++17 compiler (CXXFLAGS='-std=c++17')"
#endif

////////////////////////////////////////////////////////////////////////////////

#include <erl_interface.h>
#include <ei.h>

#include <cassert> /* for assert() */
#include <cstdio>  /* for FILE */
#include <cstring> /* for strndup() */
#include <utility> /* for std::pair */

////////////////////////////////////////////////////////////////////////////////

namespace erl {
  class term;
}

////////////////////////////////////////////////////////////////////////////////

class erl::term {
public:
  using handle_ptr = std::unique_ptr<ETERM, void(*)(ETERM*)>;

protected:
  // @see http://erlang.org/doc/man/erl_malloc.html#erl_free_term
  handle_ptr _handle{nullptr, erl_free_term};

public:
  static term decode(void* const buffer) {
    ETERM* const handle = erl_decode(reinterpret_cast<unsigned char*>(buffer));
    if (!handle) throw std::invalid_argument{"erl_decode()"};
    return term{handle};
  }

  static term make_list() {
    ETERM* const handle = erl_mk_empty_list();
    if (!handle) throw std::invalid_argument{"erl_mk_empty_list()"};
    return term{handle};
  }

  static term make_tuple(ETERM* first) {
    ETERM* elements[1] = {first};
    ETERM* const handle = erl_mk_tuple(elements, 1);
    elements[0] = nullptr;
    if (!handle) throw std::invalid_argument{"erl_mk_empty_list()"};
    return term{handle};
  }

  static term make_tuple(ETERM* first, ETERM* second) {
    ETERM* elements[2] = {first, second};
    ETERM* const handle = erl_mk_tuple(elements, 2);
    elements[0] = elements[1] = nullptr;
    if (!handle) throw std::invalid_argument{"erl_mk_empty_list()"};
    return term{handle};
  }

  static term make_pid() {
    ETERM* const handle = erl_mk_pid(erl_thisnodename(), 1, 2, 3); // FIXME
    if (!handle) throw std::invalid_argument{"erl_mk_long_ref()"};
    return term{handle};
  }

  static term make_ref() {
    ETERM* const handle = erl_mk_long_ref(erl_thisnodename(), 1, 2, 3, 0); // FIXME
    if (!handle) throw std::invalid_argument{"erl_mk_long_ref()"};
    return term{handle};
  }

  explicit term() noexcept
    : _handle{nullptr, erl_free_term} {}

  explicit term(ETERM* const handle) noexcept
    : _handle{handle, erl_free_term} {}

  const ETERM* handle() const noexcept {
    return _handle.get();
  }

  ETERM* handle() noexcept {
    return _handle.get();
  }

  ETERM* release() noexcept {
    return _handle.release();
  }

  void reset(ETERM* const handle) noexcept {
    _handle.reset(handle);
  }

  void print(FILE* const stream) const {
    assert(handle());
    assert(ERL_TYPE(handle()) != 0);
    if (erl_print_term(stream, handle()) < 0) {
      /* ignore errors */
    }
  }

  bool is_int() const noexcept {
    assert(handle());
    return ERL_IS_INTEGER(handle());
  }

  bool is_uint() const noexcept {
    assert(handle());
    return ERL_IS_UNSIGNED_INTEGER(handle());
  }

  bool is_float() const noexcept {
    assert(handle());
    return ERL_IS_FLOAT(handle());
  }

  bool is_atom() const noexcept {
    assert(handle());
    return ERL_IS_ATOM(handle());
  }

  bool is_pid() const noexcept {
    assert(handle());
    return ERL_IS_PID(handle());
  }

  bool is_port() const noexcept {
    assert(handle());
    return ERL_IS_PORT(handle());
  }

  bool is_ref() const noexcept {
    assert(handle());
    return ERL_IS_REF(handle());
  }

  bool is_tuple() const noexcept {
    assert(handle());
    return ERL_IS_TUPLE(handle());
  }

  bool is_binary() const noexcept {
    assert(handle());
    return ERL_IS_BINARY(handle());
  }

  bool is_nil() const noexcept {
    assert(handle());
    return ERL_IS_NIL(handle());
  }

  bool is_empty_list() const noexcept {
    assert(handle());
    return ERL_IS_EMPTY_LIST(handle());
  }

  bool is_cons() const noexcept {
    assert(handle());
    return ERL_IS_CONS(handle());
  }

  bool is_list() const noexcept {
    assert(handle());
    return ERL_IS_LIST(handle());
  }

  std::size_t length() const {
    assert(handle());
    const auto result = erl_length(handle());
    if (result == -1) {
      throw std::logic_error{"not a proper list"};
    }
    return result;
  }

  std::size_t size() const {
    assert(handle());
    const auto result = erl_size(handle());
    if (result == -1) {
      throw std::logic_error{"not a tuple or binary object"};
    }
    return result;
  }

  std::size_t encoded_size() const {
    assert(handle());
    const auto result = erl_term_len(const_cast<ETERM*>(handle()));
    return static_cast<std::size_t>(result);
  }

  erl::term operator[](const std::size_t index) {
    assert(handle());
    return erl::term{erl_element(index + 1, handle())};
  }

  std::size_t encode(void* buffer) const {
    return ::erl_encode(const_cast<ETERM*>(handle()),
      reinterpret_cast<unsigned char*>(buffer));
  }

  void prepend(ETERM* const element) {
    _handle.reset(erl_cons(element, release()));
  }

  void prepend(const int element) {
    prepend(erl_mk_int(element));
  }

  bool as_bool() const {
    assert(handle());
    return as_int() ? true : false;
  }

  int as_int() const {
    assert(handle());
    return ERL_INT_VALUE(handle());
  }

  long as_long() const {
    assert(handle());
    return as_int();
  }

  std::size_t as_size() const {
    assert(handle());
    return as_long();
  }

  float as_float() const {
    assert(handle());
    return ERL_FLOAT_VALUE(handle());
  }

  double as_double() const {
    assert(handle());
    return as_float();
  }

  char* as_c_str() const {
    assert(handle());
    return strndup(
      reinterpret_cast<const char*>(ERL_BIN_PTR(handle())),
      static_cast<std::size_t>(ERL_BIN_SIZE(handle())));
  }

  std::string as_string() const {
    assert(handle());
    switch (ERL_TYPE(handle())) {
      case ERL_ATOM:
        return std::string{
          reinterpret_cast<const char*>(ERL_ATOM_PTR_UTF8(handle()))};
      case ERL_BINARY:
        return std::string{
          reinterpret_cast<const char*>(ERL_BIN_PTR(handle())),
          static_cast<std::size_t>(ERL_BIN_SIZE(handle()))};
      default: break;
    }
    throw std::logic_error("not convertible to a string");
  }

  std::pair<std::size_t, std::size_t> as_size_pair() const {
    assert(handle());
    assert(ERL_IS_TUPLE(handle()));
    assert(erl_size(handle()) == 2);
    const erl::term first{erl_element(1, handle())};
    const erl::term second{erl_element(2, handle())};
    return {first.as_size(), second.as_size()};
  }
};

////////////////////////////////////////////////////////////////////////////////

namespace erl {
  template<typename... Args> inline term
  format(const char* fmt, Args&&... args) {
    // @see http://erlang.org/doc/man/erl_format.html#erl_format
    ETERM* const handle = erl_format(const_cast<char*>(fmt), args...);
    if (!handle) throw std::invalid_argument{"erl_format()"};
    return term{handle};
  }
}

////////////////////////////////////////////////////////////////////////////////

#endif /* ERLXX_H */
