#pragma once

#include "config.hpp"
#include "L2_Utilities/macros.hpp"
// #pragma message "INCLUDE_BACKTRACE_DUMP = " SJ2_STRINGIFY(INCLUDE_BACKTRACE_DUMP)

// #if defined INCLUDE_BACKTRACE_DUMP && INCLUDE_BACKTRACE_DUMP == true
#include <unwind.h>

_Unwind_Reason_Code trace_fcn(_Unwind_Context *ctx, void *d);
void print_trace();

// #endif // defined INCLUDE_BACKTRACE_DUMP && INCLUDE_BACKTRACE_DUMP == true
