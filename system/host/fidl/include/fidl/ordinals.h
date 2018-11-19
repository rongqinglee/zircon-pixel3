// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ZIRCON_SYSTEM_HOST_FIDL_INCLUDE_FIDL_ORDINALS_H_
#define ZIRCON_SYSTEM_HOST_FIDL_INCLUDE_FIDL_ORDINALS_H_

#include "raw_ast.h"

namespace fidl {
namespace ordinals {

// Returns the OrdinalName. If the OrdinalName attribute is present, the
// function returns its value; otherwise, it returns the name parameter.
std::string GetOrdinalName(const raw::AttributeList* attributes,
                           SourceLocation name);

// Retrieves the correct ordinal for this method per the FIDL spec.
//
// If |method.ordinal| is not null, this method will return |method.ordinal|.
// Otherwise, the ordinal value is equal to
//    *((int32_t *)sha256(library_name + "." + interface_name + "/" + method_name)) & 0x7fffffff;
// If |method| has an OrdinalName attribute, that value will be used as the
// method_name.
raw::Ordinal GetOrdinal(const std::vector<StringView>& library_name,
                        const StringView& interface_name,
                        const raw::InterfaceMethod& method);

} // namespace ordinals
} // namespace fidl

#endif // ZIRCON_SYSTEM_HOST_FIDL_INCLUDE_FIDL_ORDINALS_H_
