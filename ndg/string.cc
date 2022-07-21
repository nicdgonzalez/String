#include "ndg/string.hh"

#include <sstream>  // ostringstream
#include <iomanip>  // fixed, setprecision

#include <cstring>  // strcpy, strlen
#include <cstdint>  // int16_t, int64_t, uint8_t, uint64_t

namespace algo {

  // Boyer-Moore string-search algorithm visualization:
  // https://cmps-people.ok.ubc.ca/ylucet/DS/BoyerMoore.html

  // Refers to the total number of ASCII codes for case-sensitive searching:
  const int kAlphabetLength = 256U;

  void MakeDelta1(int64_t *_delta1, const char *_target) {
    uint64_t target_len = strlen(_target);

    for (uint64_t index = kAlphabetLength; index--; ) {
      _delta1[index] = 0;
    }

    for (int16_t index = target_len; index--; ) {
      _delta1[(int) _target[index]] = (target_len - index - 1);
    }

    return;
  }

  bool IsPrefix(const char *_word, uint64_t _index) {
    uint64_t suffix_len = (strlen(_word) - _index);

    for (uint64_t index = 0; index < suffix_len; index++) {
      if (_word[index] != _word[index + _index]) {
        return false;
      }
    }

    return true;
  }

  uint64_t SuffixLength(const char *_word, int64_t _index) {
    uint64_t word_len = strlen(_word);
    uint64_t index = 0ULL;

    while ( (_word[_index - index] == _word[word_len - index - 1])
            && ((int64_t) index <= _index) ) {
      index++;
    }

    return index;
  }

  void MakeDelta2(int64_t *_delta2, const char *_target) {
    uint64_t target_len = strlen(_target);
    uint64_t last_index = 1ULL;
    int64_t index;

    for (index = (target_len - 1ULL); index >= 0LL; index--) {
      if (IsPrefix(_target, (index + 1LL))) {
        last_index = (index + 1LL);
      }
      _delta2[index] = (last_index + (target_len - index - 1));
    }

    for (index = 0LL; index < (int64_t) (target_len - 1ULL); index++) {
      uint64_t suffix_len = SuffixLength(_target, index);

      {
        int64_t index1 = (index - (int64_t) suffix_len);
        uint64_t index2 = (target_len - suffix_len - 1ULL);
        if (_target[index1] != _target[index2]) {
          _delta2[index2] = (target_len - index + suffix_len - 1ULL);
        }
      }
    }

    return;
  }

  uint64_t BoyerMooreSearch( const char *_source,
                             const char *_target,
                             uint64_t _index ) {
    uint64_t target_len = strlen(_target);
    uint64_t source_len = strlen(_source);
    int64_t delta1[kAlphabetLength];
    int64_t delta2[target_len];

    MakeDelta1(delta1, _target);
    MakeDelta2(delta2, _target);

    if (target_len == 0ULL) {
      return target_len;
    }

    uint64_t s_index = (_index + (target_len - 1LL));

    while (s_index < source_len) {
      int64_t t_index = (target_len - 1LL);

      while ((t_index >= 0LL) && (_source[s_index] == _target[t_index])) {
        s_index--;
        t_index--;
      }

      if (t_index < 0LL) {
        return (s_index + 1ULL);
      }

      auto max = [&](uint8_t a, uint8_t b) -> int64_t {
        return ((a < b)? b: a);
      };

      int64_t shift = max(delta1[(int) _source[s_index]], delta2[t_index]);
      s_index += shift;
    }

    return source_len;
  }

}  // namespace algo

namespace ndg {

  // Constructors/Destructors ------------------------------------------------

  String::String(const char *_str)
      : length_(strlen(_str)) {
    if (this->length_ > 0ULL) {
      this->str_ = new char[this->length_];
      strcpy(this->str_, _str);

      this->format_args_ += this->CountFormatArgs(_str, 0);
    }

    return;
  }

  String::String(const String &_that) {
    this->length_ = _that.length();

    if (this->length_ > 0ULL) {
      this->str_ = new char[this->length_];
      this->format_args_ = _that.format_args();
      strcpy(this->str_, _that.c_str());
    }

    return;
  }

  String::~String() noexcept {
    delete[] this->str_;

    return;
  }

  // Methods -----------------------------------------------------------------

  void String::Resize(size_t _new_size) {
    char *local_copy = new char[_new_size];
    strcpy(local_copy, this->str_);

    delete[] this->str_;

    this->str_ = local_copy;
    this->length_ = _new_size;

    return;
  }

  uint64_t String::Index(const char *_target, uint64_t _index) {
    return (algo::BoyerMooreSearch(this->str_, _target, _index));
  }

  bool String::Contains(const char *_target) {
    return (this->Index(_target) < this->length_);
  }

  String & String::Append(const char *_source) {
    uint64_t index = this->length_;

    {
      size_t new_size = index + strlen(_source);
      this->Resize(new_size);
    }

    const char *temp_ptr = _source;
    while ((this->str_[index++] = *temp_ptr++)) {}

    this->format_args_ += this->CountFormatArgs(_source);

    return *this;
  }

  String & String::Replace( const char *_target,
                            const char *_source,
                            int16_t _max ) {
    uint64_t index = this->Index(_target);

    if ((index == this->length_) || (_max == 0)) {
      // Target is not a substring of this->str_, return unaltered original.
      return *this;
    }

    uint64_t old_len = this->length_;
    {
      size_t new_size = ( old_len
                          - strlen(_target)
                          + strlen(_source) );
      this->Resize(new_size);
    }

    uint64_t remaining = (old_len - index);
    char *split_str_end = new char[remaining];

    for (uint64_t i = remaining; i--; ) {
      uint64_t offset = (index + strlen(_target) + i);
      split_str_end[i] = this->str_[offset];
    }

    {
      uint64_t temp_index = 0ULL;
      while((this->str_[index] = _source[temp_index])) {
        index++;
        temp_index++;
      }
    }

    char *temp_ptr = split_str_end;

    while ((this->str_[index] = *temp_ptr++)) {
      index++;
    }

    delete[] split_str_end;
    split_str_end = 0;

    bool target_has_fmt_args = ( algo::BoyerMooreSearch(_target, "{}", 0ULL)
                                 != strlen(_target) );
    this->format_args_ -= 1 * target_has_fmt_args;

    _max -= 1 * (_max > 0);
    if ((this->Index(_target) < this->length_) && (_max > 0 || _max == -1)) {
      this->Replace(_target, _source, _max);
    }

    return *this;
  }

  String & String::Trim(const char *_target) {
    uint64_t index = this->Index(_target);
    this->format_args_ -= this->CountFormatArgs(this->str_, index);

    if (index == this->length_) {
      return *this;
    }

    size_t new_size = index;

    while (index < this->length_) {
      this->str_[index++] = '\0';
    }

    this->Resize(new_size);

    return *this;
  }

  String & String::ToUpper(void) {
    uint64_t index = this->length_;
    char *upper = this->str_;

    while (index--) {
      upper[index] -= 32 * ((upper[index] > 'a') && (upper[index] < 'z'));
    }

    return *this;
  }

  String & String::ToLower(void) {
    uint64_t index = this->length_;
    char *lower = this->str_;

    while (index--) {
      lower[index] += 32 * ((lower[index] >= 'A') && (lower[index] <= 'Z'));
    }

    return *this;
  }

  String & String::ToTitle(void) {
    uint64_t index = this->length_;
    char *upper = this->str_;

    while (index > 0ULL) {
      upper[index] += 32 * ((upper[index] > 'A') && (upper[index] < 'Z'));
      index--;
    }
    upper[index] -= 32 * ((upper[index] > 'a') && (upper[index] < 'z'));

    return *this;
  }

  /** CountFormatArgs is a private method */
  uint64_t String::CountFormatArgs(const char *_source, uint64_t _index) {
    uint64_t count = 0ULL;
    uint64_t length = strlen(_source);

    uint64_t index = _index;
    while (index < length) {
      index = algo::BoyerMooreSearch(_source, "{}", index);
      count += 1 * (index++ < length);
    }

    return count;
  }

  // Operator Overloading ----------------------------------------------------

  String & String::operator=(const String &_that) noexcept {
    this->length_ = _that.length();

    if (this->length_ > 0ULL) {
      char *local_copy = new char[this->length_];
      strcpy(local_copy, _that.c_str());

      delete[] this->str_;
      this->str_ = local_copy;
      this->format_args_ = _that.format_args();
    }

    return *this;
  }

  char String::operator[](int64_t _index) {
    if (_index < 0) {
      // Start from the end of the array if _index is negative.
      _index += this->length_;
    }
    else if ( (_index > (int64_t) this->length_)
              || (_index < (-1 * (int64_t) this->length_)) ) {
      throw IndexError();
    }

    return this->str_[_index];
  }

  std::ostream & operator<<(std::ostream &_os, String &_str) {
    if (_str.format_args_ > 0) {
      throw TooFewArgsError();
    }

    _os << _str.str_;

    return _os;
  }

  template <typename T>
  String & operator%(String &_str, T _source) {
    std::ostringstream str_stream;

    str_stream << std::fixed
              << std::setprecision(_str.precision)
              << _source;

    return operator%(_str, (const char *) str_stream.str().c_str());
  }

  String & operator%(String &_str, const char *_source) {
    if (_str.format_args_ <= 0) {
      throw TooManyArgsError();
    }

    _str.Replace("{}", _source, 1);

    return _str;
  }

  String & operator%(String &_str, char *_source) {
    return operator%(_str, (const char *) _source);
  }

}  // namespace ndg
