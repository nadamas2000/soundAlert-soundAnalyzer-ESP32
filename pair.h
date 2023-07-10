/**
 * @file pair.h
 * @brief Defines the Pair structure.
 */

#pragma once

/**
 * @brief Represents a pair of two values.
 *
 * The Pair structure provides a convenient way to store and manipulate a pair of two values.
 *
 * @tparam T1 The type of the first value.
 * @tparam T2 The type of the second value.
 */
template <class T1, class T2>

struct Pair {
  T1 first;   /**< The first value of the pair. */
  T2 second;  /**< The second value of the pair. */

  /**
   * @brief Default constructor.
   *
   * Initializes the first and second values to their default values.
   */
  Pair() : first(T1()), second(T2()) {}

  /**
   * @brief Constructs a pair with specified values.
   *
   * @param a The value to assign to the first element of the pair.
   * @param b The value to assign to the second element of the pair.
   */
  Pair(const T1& a, const T2& b) : first(a), second(b) {}
};