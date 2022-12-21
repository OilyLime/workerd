// Copyright (c) 2017-2022 Cloudflare, Inc.
// Licensed under the Apache 2.0 license found in the LICENSE file or at:
//     https://opensource.org/licenses/Apache-2.0

#pragma once

#include <workerd/io/compatibility-date.capnp.h>
#include <workerd/io/worker.h>

namespace workerd {

enum class CompatibilityDateValidation {
  CODE_VERSION,
  // Allow dates up through the date specified by `supportedCompatibilityDate` in
  // `compatiility-date.capnp`.

  CURRENT_DATE_FOR_CLOUDFLARE,
  // Allow dates up to through the current date.
  //
  // This should ONLY be used by Cloudflare. If Cloudflare fails to deploy a compatibility flag
  // before its default-on date passes, then the flag's default-on date needs to be pushed back.
  //
  // TODO(someday): We may want to consider eliminating this and using CODE_VERSION even for
  //   Cloudflare. Once people are testing their code using the open source runtime in wrangler,
  //   they will be forced to set a date no later than `supportedCompatibilityDate` anyway. If
  //   we make sure Cloudflare is updated before the runtime used locally by Wrangler is updated,
  //   then there's no real need for Cloudflare to accept newer dates. This would then allow us
  //   to avoid the situation where a release misses the deadline implied by its compat flag
  //   default-on dates.

  FUTURE_FOR_TEST
  // Allow any future date. This should only be used to test `compileCompatibilityFlags` itself.
};

void compileCompatibilityFlags(kj::StringPtr compatDate, capnp::List<capnp::Text>::Reader compatFlags,
                         CompatibilityFlags::Builder output,
                         Worker::ValidationErrorReporter& errorReporter,
                         bool allowExperimentalFeatures,
                         CompatibilityDateValidation dateValidation);

kj::Array<kj::StringPtr> decompileCompatibilityFlagsForFl(CompatibilityFlags::Reader input);
// Return an array of compatibility enable-flags which express the given FeatureFlags. The returned
// StringPtrs point to FeatureFlags annotation parameters, which live in static storage.
//
// TODO(soon): Introduce a codec which can minimize the number of flags generated by choosing a good
//   compatibility date.

kj::Maybe<kj::String> normalizeCompatDate(kj::StringPtr date);
// Exposed to unit test the parser.

}  // namespace workerd
