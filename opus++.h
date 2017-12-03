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

#include <opus.h> /* for libopus */

#include <memory> /* for std::unique_ptr */

////////////////////////////////////////////////////////////////////////////////

namespace opus {
  class encoder;
  class decoder;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__encoder.html
 */
class opus::encoder {
public:
  using handle_ptr = std::unique_ptr<OpusEncoder, void(*)(OpusEncoder*)>;

protected:
  // @see https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__encoder.html#ga5f4c05b4b51cdffec5a55dbf17bbfa1c
  handle_ptr _handle{nullptr, opus_encoder_destroy};

public:
  explicit encoder() noexcept = default;

  explicit encoder(OpusEncoder* const handle) noexcept
    : _handle{handle, opus_encoder_destroy} {}

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
  handle_ptr _handle{nullptr, opus_decoder_destroy};

public:
  explicit decoder() noexcept = default;

  explicit decoder(OpusDecoder* const handle) noexcept
    : _handle{handle, opus_decoder_destroy} {}

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
