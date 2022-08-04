/*******************************************************************************
//
//  SYCL 1.2.1 Conformance Test Suite
//
//  Copyright (c) 2018-2022 Codeplay Software LTD. All Rights Reserved.
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
*******************************************************************************/

#include "../common/common.h"

#define TEST_NAME group_mem_fence

namespace group_mem_fence__ {
using namespace sycl_cts;

void test_mem_fence(util::logger &log, cl::sycl::queue &queue) {
  /* set workspace size */
  const int globalSize = 64;

  /* allocate and assign host data */

  /* init ranges*/
  cl::sycl::range<1> globalRange(globalSize);

  /* run kernel to check mem_fence interface is available*/
  {

    queue.submit([&](cl::sycl::handler &cgh) {

      cgh.parallel_for_work_group<class TEST_NAME>(
          globalRange, [=](cl::sycl::group<1> test_group) {

            test_group.mem_fence(cl::sycl::access::fence_space::local_space);
            test_group.mem_fence(cl::sycl::access::fence_space::global_space);
            test_group.mem_fence(
                cl::sycl::access::fence_space::global_and_local);
            test_group.mem_fence();

          });
    });
  }
}

/** test cl::sycl::group::mem_fence
*/
class TEST_NAME : public util::test_base {
 public:
  /** return information about this test
  *  @param info, test_base::info structure as output
  */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
  *  @param log, test transcript logging class
  */
  void run(util::logger &log) override {
    try {
      auto cmdQueue = util::get_cts_object::queue();

      test_mem_fence(log, cmdQueue);

      cmdQueue.wait_and_throw();
    } catch (const cl::sycl::exception &e) {
      log_exception(log, e);
      cl::sycl::string_class errorMsg =
          "a SYCL exception was caught: " + cl::sycl::string_class(e.what());
      FAIL(log, errorMsg.c_str());
    }
  }
};

util::test_proxy<TEST_NAME> proxy;

} /* namespace group_mem_fence__ */
