#ifndef STRING_STRING_H_
# define STRING_STRING_H_

# include <limits>     // max_digits10
# include <ostream>    // ostream
# include <exception>  // exception

# include <cstdint>    // int16_t, int64_t, uint64_t

# define _NDG_STRING_VERSION_ 100

  namespace ndg {

    class StringException : public std::exception {
      public:
        const char * what(void) const noexcept override {
          return "Base exception for String related errors.";
        }
    };

    class IndexError : public StringException {
      public:
        const char * what(void) const noexcept override {
          return "Attempting to reach a non-existant index of the array.";
        }
    };

    class TooManyArgsError : public StringException {
      public:
        const char * what(void) const noexcept override {
          return "Too many args. No '{}'s remain in the string to format.";
        }
    };

    class TooFewArgsError : public StringException {
      public:
        const char * what(void) const noexcept override {
          return "Insufficient args provided for the formatted string.";
        }
    };

    class String {
      public:
        /**
         * @brief Constructor for new String objects.
         *
         * @param _str The initial content to wrap the String class around.
         *
         * @return String
         */
        String(const char * = "");

        /**
         * @brief Copy constructor for new String objects.
         *
         * @param _that A reference to the String object to copy.
         *
         * @return String
         */
        String(const String &);

        /**
         * @brief Destructor for String objects.
         *
         * @return void
         */
        ~String() noexcept;

        /**
         * @brief Reallocate memory for the wrapped string.
         *
         * @note This method is mostly used internally but public since there
         *       is no other way for the user to destroy and recreate the
         *       wrapped string.
         *
         * @param _new_size The new amount of memory to allocate (1 = 1 byte).
         *
         * @return void
         */
        void Resize(uint64_t);

        /**
         * @brief Get the starting index of a target substring.
         *
         * @param _target The substring to search for.
         * @param _index An optional starting index. Defaults to 0ULL.
         *
         * @return uint64_t
         */
        uint64_t Index(const char *, uint64_t = 0ULL);

        /**
         * @brief Checks if the wrapped string contains the target substring.
         *
         * @param _target The substring to search for.
         *
         * @return bool
         */
        bool Contains(const char *);

        /**
         * @brief Concatenates the wrapped string with the new string.
         *
         * @param _source The string to add.
         *
         * @return String &
         */
        String & Append(const char *);

        /**
         * @brief Swap the target substring with another substring.
         *
         * @param _target The substring to be replaced.
         * @param _source The content to replace the target substring with.
         * @param _max The number of substrings to replace. Defaults to 1.
         *
         * @return String &
         */
        String & Replace(const char *, const char *, int16_t = 1);

        /**
         * @brief Removes everything following the first match of
         *        the target substring.
         *
         * @param _target The substring to search for.
         *
         * @return String &
         */
        String & Trim(const char *);

        /**
         * @brief Converts the wrapped string to uppercase.
         *
         * @return String &
         */
        String & ToUpper(void);


        /**
         * @brief Converts the wrapped string to lowercase.
         *
         * @return String &
         */
        String & ToLower(void);

        /**
         * @brief Convert the first character of the wrapped string to
         *        uppercase and the remaining characters to lowercase.
         *
         * @return String &
         */
        String & ToTitle(void);

        /**
         * @brief Accessor method to the wrapped null-terminated string.
         *
         * @return constexpr char *
         */
        constexpr char * c_str(void) const;

        /**
         * @brief Checks if the object is wrapping a string.
         *
         * @return constexpr bool
         */
        constexpr bool IsEmpty(void) const;

        /**
         * @brief Accessor method for the current length of the string.
         *
         * @return constexpr uint64_t
         */
        constexpr uint64_t length(void) const;

        /**
         * @brief Accessor method for the current number of formatting args
         *        remaining in the string (i.e., how many "{}" are present).
         *
         * @return constexpr int64_t
         */
        constexpr int64_t format_args(void) const;

        /**
         * @brief A public member used to manipulate an Input/Output stream's
         *        precision. Minimum value can be 0, maximum value can be 17.
         */
        uint8_t precision = std::numeric_limits<double>::max_digits10;

      protected:
        String & operator=(const String &) noexcept;
        char operator[](int64_t);
        template <typename T1, typename T2>
          friend T1 operator<<(T1, T2);
          friend std::ostream & operator<<(std::ostream &, String &);
        template <typename T>
          friend String & operator%(String &, T);
          friend String & operator%(String &, const char *);
          friend String & operator%(String &, char *);

      private:
        char *str_ = nullptr;
        uint64_t length_ = 0ULL;
        int64_t format_args_ = 0LL;

        /**
         * @brief [Internal Method]
         *        Counts the number of "{}" that appear within the @p _source
         *        string. Used for incrementing and decrementing the member
         *        @p format_args_.
         *
         * @param _source The string to be evaluated.
         * @param _index An optional starting index. Defaults to 0ULL.
         *
         * @return uint64_t
         */
        uint64_t CountFormatArgs(const char *, uint64_t = 0ULL);
    };

    constexpr char * String::c_str(void) const {
      return this->str_;
    }

    constexpr bool String::IsEmpty(void) const {
      return ((this->str_) && (this->length_ > 0ULL));
    }

    constexpr uint64_t String::length(void) const {
      return this->length_;
    }

    constexpr int64_t String::format_args(void) const {
      return this->format_args_;
    }

  }  // namespace ndg

#endif  // !STRING_STRING_H_
