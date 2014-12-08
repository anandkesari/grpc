/*
 *
 * Copyright 2014, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <grpc/support/port_platform.h>

#ifdef GPR_ANDROID

#include <grpc/support/log.h>
#include <grpc/support/time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <android/log.h>

static android_LogPriority severity_to_log_priority(gpr_log_severity severity) {
  switch (severity) {
    case GPR_LOG_SEVERITY_DEBUG:
      return ANDROID_LOG_DEBUG;
    case GPR_LOG_SEVERITY_INFO:
      return ANDROID_LOG_INFO;
    case GPR_LOG_SEVERITY_ERROR:
      return ANDROID_LOG_ERROR;
  }
  return ANDROID_LOG_DEFAULT;
}

void gpr_log(const char *file, int line, gpr_log_severity severity,
             const char *format, ...) {
  char *final_slash;
  const char *display_file;
  char *prefix = NULL;
  char *suffix = NULL;
  char *output = NULL;
  va_list args;
  va_start(args, format);

  final_slash = strrchr(file, '/');
  if (final_slash == NULL)
    display_file = file;
  else
    display_file = final_slash + 1;

  asprintf(&prefix, "%s:%d] ", display_file, line);
  vasprintf(&suffix, format, args);
  asprintf(&output, "%s%s", prefix, suffix);
  va_end(args);

  __android_log_write(severity_to_log_priority(severity), "GRPC", output);

  /* allocated by asprintf => use free, not gpr_free */
  free(prefix);
  free(suffix);
  free(output);
}

#endif /* GPR_ANDROID */