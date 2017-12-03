/* This is free and unencumbered software released into the public domain. */

#ifndef OPUSXX_H
#define OPUSXX_H

/**
 * <opus++.h> - Public domain C++11 wrapper for the libopus <opus.h> interface.
 *
 * @author Arto Bendiken
 */

#ifndef __cplusplus
#error "<opus++.h> requires a C++ compiler"
#endif

#if __cplusplus < 201103L
#error "<opus++.h> requires a C++11 compiler (CXXFLAGS='-std=c++11')"
#endif

////////////////////////////////////////////////////////////////////////////////

#include <opus.h>   /* for libopus */

#include <assert.h> /* for assert() */
#include <memory>   /* for std::unique_ptr */

////////////////////////////////////////////////////////////////////////////////

namespace opus {
  class error;
  class encoder;
  class decoder;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__errorcodes.html
 */
class opus::error : public std::runtime_error {
protected:
  int _code;

public:
  explicit error(int code) noexcept
    : _code{code}, runtime_error{""} {}

  int code() const noexcept {
    return _code;
  }

  virtual const char* what() const noexcept override {
    switch (_code) {
      case OPUS_ALLOC_FAIL:
        return "memory allocation has failed";
      case OPUS_BAD_ARG:
        return "one or more invalid/out of range arguments";
      case OPUS_BUFFER_TOO_SMALL:
        return "not enough bytes allocated in the buffer";
      case OPUS_INTERNAL_ERROR:
        return "an internal error was detected";
      case OPUS_INVALID_PACKET:
        return "the compressed data passed is corrupted";
      case OPUS_INVALID_STATE:
        return "an encoder or decoder structure is invalid or already freed";
      case OPUS_OK:
        return "no error";
      case OPUS_UNIMPLEMENTED:
        return "invalid/unsupported request number";
      default:
        assert(_code);
        return "unknown error from libopus";
    }
  }
};

////////////////////////////////////////////////////////////////////////////////

/**
 * @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__encoder.html
 */
class opus::encoder {
public:
  using handle_ptr = std::unique_ptr<OpusEncoder, void(*)(OpusEncoder*)>;

protected:
  // @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__encoder.html#ga5f4c05b4b51cdffec5a55dbf17bbfa1c
  handle_ptr _handle{nullptr, ::opus_encoder_destroy};

public:
  // @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__encoder.html#gaa89264fd93c9da70362a0c9b96b9ca88
  static encoder create(const int frequency,
                        const int channels,
                        const int application = OPUS_APPLICATION_AUDIO) {
    int error;
    OpusEncoder* const handle = ::opus_encoder_create(frequency, channels, application, &error);
    if (error != OPUS_OK) throw opus::error{error};
    assert(handle != nullptr);
    return encoder{handle};
  }

  explicit encoder() noexcept = default;

  explicit encoder(OpusEncoder* const handle) noexcept
    : _handle{handle, ::opus_encoder_destroy} {}

  const OpusEncoder* handle() const noexcept {
    return _handle.get();
  }

  OpusEncoder* handle() noexcept {
    return _handle.get();
  }

  OpusEncoder* release() noexcept {
    return _handle.release();
  }

  void reset(OpusEncoder* const handle) noexcept {
    _handle.reset(handle);
  }
};

////////////////////////////////////////////////////////////////////////////////

/**
 * @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__decoder.html
 */
class opus::decoder {
public:
  using handle_ptr = std::unique_ptr<OpusDecoder, void(*)(OpusDecoder*)>;

protected:
  // @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__decoder.html#gafebf4cb3c29c9317cac385446a76e36e
  handle_ptr _handle{nullptr, ::opus_decoder_destroy};

public:
  // @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__decoder.html#ga753f6fe0b699c81cfd47d70c8e15a0bd
  static decoder create(const int frequency,
                        const int channels) {
    int error;
    OpusDecoder* const handle = ::opus_decoder_create(frequency, channels, &error);
    if (error != OPUS_OK) throw opus::error{error};
    assert(handle != nullptr);
    return decoder{handle};
  }

  explicit decoder() noexcept = default;

  explicit decoder(OpusDecoder* const handle) noexcept
    : _handle{handle, ::opus_decoder_destroy} {}

  const OpusDecoder* handle() const noexcept {
    return _handle.get();
  }

  OpusDecoder* handle() noexcept {
    return _handle.get();
  }

  OpusDecoder* release() noexcept {
    return _handle.release();
  }

  void reset(OpusDecoder* const handle) noexcept {
    _handle.reset(handle);
  }
};

////////////////////////////////////////////////////////////////////////////////

#endif /* OPUSXX_H */
