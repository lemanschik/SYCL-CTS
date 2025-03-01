/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Provides tests for multi_ptr legacy API with fp16 data types.
//
*******************************************************************************/

#include "../common/common.h"
#include "multi_ptr_legacy_api_common.h"

#include <string>

#define TEST_NAME multi_ptr_legacy_api_fp16

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

    if (!queue.get_device().has(sycl::aspect::fp16)) {
      log.note(
          "Device does not support half precision floating point operations");
      return;
    }

    check_void_pointer_api<sycl::half>{}(log, queue, "sycl::half");
    check_pointer_api<sycl::half>{}(log, queue, "sycl::half");

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
