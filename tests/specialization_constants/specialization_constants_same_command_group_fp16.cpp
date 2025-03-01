/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
// Provides tests for specialization constants usage with same command group
// function for sycl::half
//
*******************************************************************************/

#include "../common/common.h"
#include "../common/type_coverage.h"
#include "specialization_constants_same_command_group_common.h"

#define TEST_NAME specialization_constants_same_command_group_fp16

namespace TEST_NAMESPACE {
using namespace sycl_cts;

/** test specialization constants for sycl::half
 */
class TEST_NAME : public sycl_cts::util::test_base {
public:
  /** return information about this test
   */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
   */
  void run(util::logger &log) override {
    using namespace specialization_constants_same_command_group_common;
    {
      auto queue = util::get_cts_object::queue();
      if (!queue.get_device().has(sycl::aspect::fp16)) {
        log.note("Device does not support half precision floating point "
                 "operations");
        return;
      }
#if !SYCL_CTS_ENABLE_FULL_CONFORMANCE
      check_specialization_constants_same_command_group<sycl::half> fp16_test{};
      fp16_test(log, "sycl::half");
#else
      for_type_vectors_marray<check_specialization_constants_same_command_group,
                              sycl::half>(log, "sycl::half");
#endif
    }
  }
};

// construction of this proxy will register the above test
util::test_proxy<TEST_NAME> proxy;

} /* namespace spec_const__ */
