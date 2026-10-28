// SPDX-License-Identifier: GPL-3.0-or-later
// Compatibility terminology; Common owns all arithmetic.
#ifndef INFILTRATR_RESOURCE_CLOCK_CIVIL_H
#define INFILTRATR_RESOURCE_CLOCK_CIVIL_H
#include <infiltratr/arithmetic.h>
#include <infiltratr/temporal.h>
#include <stdint.h>
#define CLOCK_CIVIL_DAY_US INT64_C(86400000000)
#define CLOCK_CIVIL_SECOND_US INT64_C(1000000)
static inline int64_t clock_civil_modulo(int64_t v, int64_t d)
{ int64_t rem=0; return infiltratr_i64_floor_divmod(v,d,NULL,&rem)?rem:0; }
static inline uint64_t clock_civil_local_phase(int64_t t,int32_t o)
{ return (uint64_t)infiltratr_temporal_local_microseconds_of_day(t,o); }
#endif
