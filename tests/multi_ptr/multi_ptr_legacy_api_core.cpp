/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright (c) 2017-2022 Codeplay Software LTD. All Rights Reserved.
//  Copyright (c) 2022 The Khronos Group Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//  Provides tests for multi_ptr legacy API
//
*******************************************************************************/

#include "../common/common.h"
#include "../common/type_list.h"
#include "multi_ptr_legacy_api_common.h"

#include <string>

#define TEST_NAME multi_ptr_legacy_api_core

namespace TEST_NAMESPACE {
using namespace sycl_cts;
using namespace multi_ptr_legacy_api_common;

/** tests the api for explicit pointers
 */
class TEST_NAME : public util::test_base {
 public:
  /** return information about this test
   */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute this test
   */
  void run(util::logger &log) override {
#if SYCL_CTS_ENABLE_DEPRECATED_FEATURES_TESTS
    auto queue = util::get_cts_object::queue();

    const auto types = get_types();

    for_all_types<check_void_pointer_api>(types, log, queue);
    for_all_types<check_pointer_api>(types, log, queue);

    check_pointer_api<user_struct>{}(log, queue, "user_struct");

    queue.wait_and_throw();
#else
    log.note(
        "The test is skipped because tests for the deprecated features are "
        "disabled.");
#endif  // SYCL_CTS_ENABLE_DEPRECATED_FEATURES_TESTS
  }
};

// register this test with the test_collection
util::test_proxy<TEST_NAME> proxy;

} /* namespace TEST_NAMESPACE */
