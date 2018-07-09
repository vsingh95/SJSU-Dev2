#include "config.hpp"

// #if defined INCLUDE_BACKTRACE_DUMP && INCLUDE_BACKTRACE_DUMP == true

#include <unwind.h>
#include <cstdio>

_Unwind_Reason_Code trace_fcn(_Unwind_Context *ctx, void *d)
{
    int *depth = (int*)d;
    printf("\t#%d: program counter at %p\n", *depth,
      reinterpret_cast<void *>(_Unwind_GetIP(ctx)-4));
    (*depth)++;
    return _URC_NO_REASON;
}
void print_trace()
{
  int depth = 0;
  _Unwind_Backtrace(&trace_fcn, &depth);
}

// #endif // defined INCLUDE_BACKTRACE_DUMP && INCLUDE_BACKTRACE_DUMP == true
