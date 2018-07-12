/// Copyright (c) 2018 Tobias Andersson (shada).
///
/// SPDX-License-Identifier: MIT
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#ifndef __TOBI_ENGINE_UTIL_FILE_HANDLER_HPP__
#define __TOBI_ENGINE_UTIL_FILE_HANDLER_HPP__

#include <fstream>
#include <string>
#include <vector>

namespace tobi_engine
{
namespace util
{

/// File handling interface
class file_handler
{
 public:
  /// Prevent creation of instances of this class
  file_handler() = delete;
  ~file_handler() = delete;
  file_handler(file_handler &&) = delete;
  file_handler(const file_handler &) = delete;
  file_handler &operator=(const file_handler &) = delete;
  file_handler &operator=(file_handler &&) = delete;

  /// Reads a binary file to a vector
  ///
  /// @param[in] file_name File name
  ///
  /// return file contents
  static std::vector<char> read_binary_file(std::string file_name);

  /// Reads a text file to a string
  ///
  /// @param[in] file_name File name
  ///
  /// return file contents
  static std::string read_file(std::string file_name);
};

}  // namespace util
}  // namespace tobi_engine

#endif // __TOBI_ENGINE_UTIL_FILE_HANDLER_HPP__
