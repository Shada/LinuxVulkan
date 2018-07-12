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

#include "file_handler.hpp"

namespace tobi_engine
{
namespace util
{

std::vector<char> file_handler::read_binary_file(std::string file_name)
{
  std::ifstream file_stream;
  file_stream.open(file_name.c_str(), std::ios::binary | std::ios::ate);

  // todo: change to better error handling
  if (!file_stream.is_open() || !file_stream.good())
  {
    throw "could not open file";
  }

  size_t file_size = (size_t) file_stream.tellg();
  std::vector<char> buffer(file_size);

  file_stream.seekg(0);
  file_stream.read(&buffer[0], file_size);

  file_stream.close();

  return buffer;
}

std::string file_handler::read_file(std::string file_name)
{
  std::ifstream file_stream;
  file_stream.open(file_name.c_str(), std::ios::in | std::ios::ate);

  // todo: change to better error handling
  if (!file_stream.is_open() || !file_stream.good())
  {
    throw "could not open file";
  }

  size_t file_size = (size_t) file_stream.tellg();
  std::string buffer(file_size, '\0');

  file_stream.seekg(0);
  file_stream.read(&buffer[0], file_size);

  file_stream.close();

  return buffer;
}

}
}

