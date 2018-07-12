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


#ifndef STD_FUNCTIONS_HPP_
#define STD_FUNCTIONS_HPP_

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>


namespace tobi_engine
{
namespace util
{
namespace cpp14
{
/** make_unique is missing in CPP11. Re-implement it according to the standard proposal. */

/**<Template for single object */
template <class T>
struct _Unique_if
{
    typedef std::unique_ptr<T> _Single_object; /**< Single object type */
};

/** Template for array */
template <class T>
struct _Unique_if<T[]>
{
    typedef std::unique_ptr<T[]> _Unknown_bound; /**< Array type */
};

/** Template for array with known bounds (to throw an error).
 *
 * @note this is intended to never be hit.
 */
template <class T, size_t N>
struct _Unique_if<T[N]>
{
    typedef void _Known_bound; /**< Should never be used */
};

/** Construct a single object and return a unique pointer to it.
 *
 * @param[in] args Constructor arguments.
 *
 * @return a unique pointer to the new object.
 */
template <class T, class... Args>
typename _Unique_if<T>::_Single_object
make_unique(Args &&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/** Construct an array of objects and return a unique pointer to it.
 *
 * @param[in] n Array size
 *
 * @return a unique pointer to the new array.
 */
template <class T>
typename _Unique_if<T>::_Unknown_bound
make_unique(size_t n)
{
    typedef typename std::remove_extent<T>::type U;
    return std::unique_ptr<T>(new U[n]());
}

/** It is invalid to attempt to make_unique an array with known bounds. */
template <class T, class... Args>
typename _Unique_if<T>::_Known_bound
make_unique(Args &&...) = delete;
} // namespace cpp14
} // namespace util
} // namespace tobi_engine


#endif // STD_FUNCTIONS_HPP_
