/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Provides class to verify conformity with named requirement
//  LegacyRandomAccessIterator
//
*******************************************************************************/

#ifndef __SYCLCTS_TESTS_ITERATOR_REQUIREMENTS_LEGACY_RANDOM_ACCESS_ITERATOR_H
#define __SYCLCTS_TESTS_ITERATOR_REQUIREMENTS_LEGACY_RANDOM_ACCESS_ITERATOR_H

#include "common.h"
#include "legacy_bidirectional_iterator.h"

namespace named_requirement_verification {

/**
 * @brief Class helps to verify conformity to LegacyRandomAccessIterator named
 * requirement. Safe to use inside kernel with SYCL 2020.
 *
 */
class legacy_random_access_iterator_requirement {
 public:
  // Will be used as size of container for error messages.
  // Value should be equal to the number of add_error invocations.
  // Don't forget to update this value if there is any changes in class.
  // As we also verify other requirements, we have to keep in mind that result
  // of those verifications also increase size of container with messages.
  static constexpr int count_of_possible_errors =
      legacy_bidirectional_iterator_requirement::count_of_possible_errors + 21;

 private:
  error_messages_container<count_of_possible_errors> m_test_error_messages;

 public:
  /**
   * @brief Member function performs different checks for the requirement
   * verification
   *
   * @tparam It Type of iterator for verification
   * @return std::pair<bool,array<string_view>> First represents
   * satisfaction of the requirement. Second contains error messages
   */
  template <typename It>
  std::pair<bool, std::array<string_view, count_of_possible_errors>>
  is_satisfied_for(It valid_iterator) {
    auto legacy_bidir_iterator_res =
        legacy_bidirectional_iterator_requirement{}.is_satisfied_for<It>(
            valid_iterator);
    if (!legacy_bidir_iterator_res.first) {
      m_test_error_messages.add_errors(legacy_bidir_iterator_res.second);
    }

    constexpr bool has_greater_than_operator =
        type_traits::has_comparison::greater_than_v<It>;
    constexpr bool has_less_than_operator =
        type_traits::has_comparison::less_than_v<It>;
    constexpr bool has_greater_or_equal_operator =
        type_traits::has_comparison::greater_or_equal_v<It>;
    constexpr bool has_less_or_equal_operator =
        type_traits::has_comparison::less_or_equal_v<It>;
    constexpr bool has_subscript_operator = has_subscript_operator_v<It>;

    constexpr bool has_reference_member =
        type_traits::has_field::reference_v<It>;
    constexpr bool has_difference_type_member =
        type_traits::has_field::difference_type_v<It>;

    if (!has_difference_type_member) {
      m_test_error_messages.add_error(
          "Iterator must have difference_type member typedef.");
    } else {
      using diff_t = typename std::iterator_traits<It>::difference_type;

      constexpr bool has_addition_compound_operator =
          type_traits::compound_assignment::addition_v<It, diff_t>;
      constexpr bool has_subtraction_compound_operator =
          type_traits::compound_assignment::subtraction_v<It, diff_t>;

      constexpr bool has_diff_type_plus_iterator_operator =
          type_traits::has_arithmetic::addition_v<diff_t, It>;
      constexpr bool has_iterator_plus_diff_type_operator =
          type_traits::has_arithmetic::addition_v<It, diff_t>;
      constexpr bool has_iterator_minus_diff_type_operator =
          type_traits::has_arithmetic::subtraction_v<It, diff_t>;
      constexpr bool has_iterator_minus_iterator_operator =
          type_traits::has_arithmetic::subtraction_v<It, It>;

      if (!has_addition_compound_operator) {
        m_test_error_messages.add_error(
            "Iterator must have operator+=() between Iterator instance and "
            "iterator_traits::difference_type.");
      }

      if (!has_subtraction_compound_operator) {
        m_test_error_messages.add_error(
            "Iterator must have operator-=() between Iterator instance and "
            "iterator_traits::difference_type.");
      }

      if (!has_iterator_minus_iterator_operator) {
        m_test_error_messages.add_error(
            "Iterator must have subtraction between iterators.");
      }

      if (!has_diff_type_plus_iterator_operator) {
        m_test_error_messages.add_error(
            "Iterator must have operator+() iterator_traits::difference_type "
            "plus Iterator object with operator.");
      }

      if (!has_subscript_operator) {
        m_test_error_messages.add_error("Iterator must have operator[].");
      }

      if constexpr (has_addition_compound_operator &&
                    has_subtraction_compound_operator) {
        bool is_compund_ops_correct = std::is_same_v<
            decltype(std::declval<It&>() += std::declval<diff_t>()), It&>;
        is_compund_ops_correct &= std::is_same_v<
            decltype(std::declval<It&>() -= std::declval<diff_t>()), It&>;
        is_compund_ops_correct &= std::is_same_v<
            decltype(std::declval<It&>() += -std::declval<diff_t>()), It&>;
        is_compund_ops_correct &= std::is_same_v<
            decltype(std::declval<It&>() -= -std::declval<diff_t>()), It&>;

        if (!is_compund_ops_correct)
          m_test_error_messages.add_error(
              "operator+=() and operator-=() must return It& for positive "
              "and negative vales.");
      }

      if constexpr (has_diff_type_plus_iterator_operator &&
                    has_iterator_plus_diff_type_operator) {
        diff_t n = 1;
        {
          It a = valid_iterator;
          if (
              // For positive n
              !std::is_same_v<decltype(a + n), It> ||
              !std::is_same_v<decltype(n + a), It> ||
              // For negative n
              !std::is_same_v<decltype(a - n), It> ||
              !std::is_same_v<decltype(-n + a), It>) {
            m_test_error_messages.add_error(
                "operator+() and operator-() must return It& for positive "
                "and negative vales.");
          }
        }

        {
          It a = valid_iterator;
          if (!(a + n == n + a)) {
            m_test_error_messages.add_error(
                "Iterator operator+=() must be commutative.");
          }
        }
      } else {
        m_test_error_messages.add_error(
            "Iterator must have operator+() with "
            "iterator_traits::difference_type operator.");
      }

      if constexpr (has_iterator_minus_diff_type_operator) {
        It a{};
        diff_t n = 1;

        if (!std::is_same_v<decltype(a - n), It>) {
          m_test_error_messages.add_error(
              "Iterator object minus iterator_traits::difference_type must "
              "return Iterator instance.");
        }
      } else {
        m_test_error_messages.add_error(
            "Iterator must have operator-() with "
            "iterator_traits::difference_type operator.");
      }

      It a{};
      It b{};
      diff_t n = 1;
      // If current iterator has iterator plus difference_type make `b`
      // iterator differ than `a` iterator
      if constexpr (has_iterator_plus_diff_type_operator) {
        b = b + n;
        using it_traits = std::iterator_traits<It>;
        if constexpr (has_iterator_minus_iterator_operator &&
                      has_difference_type_member) {
          if (!std::is_same_v<decltype(b - a),
                              typename it_traits::difference_type>) {
            m_test_error_messages.add_error(
                "operator-() of It instances must return "
                "iterator_traits::difference_type.");
          }
        }
        if constexpr (has_subscript_operator && has_reference_member) {
          if (!std::is_convertible_v<decltype(a[0]),
                                     typename it_traits::reference>) {
            m_test_error_messages.add_error(
                "operator[]() return value must be convertible to "
                "iterator_traits::reference.");
          }
        }
      }
    }

    {
      It a{};
      It b{};
      if constexpr (has_greater_than_operator) {
        if (!std::is_convertible_v<decltype(a > b), bool>) {
          m_test_error_messages.add_error(
              "operator>() return value must be contextually convertible to "
              "bool.");
        }
      } else {
        m_test_error_messages.add_error("Iterator must have operator>().");
      }
      if constexpr (has_less_than_operator) {
        if (!std::is_convertible_v<decltype(a < b), bool>) {
          m_test_error_messages.add_error(
              "operator<() return value must be contextually convertible to "
              "bool.");
        }
      } else {
        m_test_error_messages.add_error("Iterator must have operator<().");
      }
      if constexpr (has_greater_or_equal_operator) {
        if (!std::is_convertible_v<decltype(a >= b), bool>) {
          m_test_error_messages.add_error(
              "operator>=() return value must be contextually convertible to "
              "bool.");
        }
      } else {
        m_test_error_messages.add_error("Iterator must have operator>=().");
      }
      if constexpr (has_less_or_equal_operator) {
        if (!std::is_convertible_v<decltype(a <= b), bool>) {
          m_test_error_messages.add_error(
              "operator<=() return value must be contextually convertible to "
              "bool.");
        }
      } else {
        m_test_error_messages.add_error("Iterator must have operator<=().");
      }
    }

    const bool is_satisfied = !m_test_error_messages.has_errors();
    // According to spec std::pair with device_copyable types(in this case:
    // bool, string_view) can be used on device side
    return std::make_pair(is_satisfied, m_test_error_messages.get_array());
  }
};
}  // namespace named_requirement_verification

#endif  // __SYCLCTS_TESTS_ITERATOR_REQUIREMENTS_LEGACY_RANDOM_ACCESS_ITERATOR_H
