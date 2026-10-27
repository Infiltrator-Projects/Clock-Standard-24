// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Independent civil wall-clock resource, compiled for standard-12 and
 * standard-24. Common supplies discovery/ABI, not the time arithmetic.
 * Only explicit selections use this clock. OS locale-following "standard"
 * stays in Cinnamon's own WallClock and is deliberately not overridden.
 */
#define _GNU_SOURCE 1
#include <infiltratr/temporal_clock_resource.h>
#include "clock-civil.h"
#include <langinfo.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DAY_US INT64_C(86400000000)
#define SECOND_US INT64_C(1000000)
#define CLOCK_ID "standard-24"

static bool render(const InfiltratrTemporalClockProvider *provider,
                   const char *locale, int64_t instant, int32_t offset,
                   bool seconds, bool vertical, double latitude,
                   double longitude, char *buffer, size_t cap, size_t *length)
{
    char text[128] = {0};
    const uint64_t whole = clock_civil_local_phase(instant, offset) / SECOND_US;
    const unsigned hour24 = (unsigned)(whole / 3600U);
    const unsigned minute = (unsigned)((whole / 60U) % 60U);
    const unsigned second = (unsigned)(whole % 60U);
    const char *sep = vertical ? "\n" : ":";
    int written;
    (void)provider; (void)latitude; (void)longitude;
    (void)locale;
    if (seconds)
        written = snprintf(text, sizeof(text), "%02u%s%02u%s%02u",
                           hour24, sep, minute, sep, second);
    else
        written = snprintf(text, sizeof(text), "%02u%s%02u",
                           hour24, sep, minute);
    if (buffer == NULL || cap == 0U || written <= 0 ||
        (size_t)written >= cap || (size_t)written >= sizeof(text))
        return false;
    memcpy(buffer, text, (size_t)written + 1U);
    if (length != NULL) *length = (size_t)written;
    return true;
}

static bool next_tick(const InfiltratrTemporalClockProvider *provider,
                      int64_t instant, int32_t offset, bool seconds,
                      double latitude, double longitude, uint32_t *millis)
{
    const uint64_t period = seconds ? UINT64_C(1000000) : UINT64_C(60000000);
    const uint64_t phase = clock_civil_local_phase(instant, offset) % period;
    const uint64_t remain = period - phase;
    (void)provider; (void)latitude; (void)longitude;
    if (millis == NULL) return false;
    *millis = (uint32_t)((remain + UINT64_C(999)) / UINT64_C(1000));
    return *millis > 0U;
}

static const InfiltratrTemporalResourceMetadata metadata = {
    sizeof(InfiltratrTemporalResourceMetadata),
    INFILTRATR_TEMPORAL_RESOURCE_METADATA_ABI,
    INFILTRATR_TEMPORAL_RESOURCE_CLOCK,
    CLOCK_ID, INFILTRATR_TEMPORAL_DISPLAY_NAME, true, false, false
};
static const InfiltratrTemporalClockProvider provider = {
    sizeof(InfiltratrTemporalClockProvider),
    INFILTRATR_TEMPORAL_CLOCK_PROVIDER_ABI,
    CLOCK_ID, render, next_tick
};

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif
EXPORT const InfiltratrTemporalResourceMetadata *
infiltratr_temporal_resource_metadata_v1(void);
EXPORT const InfiltratrTemporalClockProvider *
infiltratr_temporal_clock_provider_v1(void);
EXPORT const InfiltratrTemporalResourceMetadata *
infiltratr_temporal_resource_metadata_v1(void) { return &metadata; }
EXPORT const InfiltratrTemporalClockProvider *
infiltratr_temporal_clock_provider_v1(void) { return &provider; }
