/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Provides tests for specialization constants same name stress test
//
*******************************************************************************/

#include "../common/common.h"
#include "../common/type_coverage.h"
#include "specialization_constants_same_name_stress.h"

#define TEST_NAME specialization_constants_same_name_stress_core

namespace TEST_NAMESPACE {
using namespace sycl_cts;

/** test specialization constants
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
    using namespace specialization_constants_same_name_stress;
    sc_run_test_core<get_spec_const::sc_no_kernel_bundle>(log);
  }
};

// construction of this proxy will register the above test
util::test_proxy<TEST_NAME> proxy;

}  // namespace TEST_NAMESPACE
