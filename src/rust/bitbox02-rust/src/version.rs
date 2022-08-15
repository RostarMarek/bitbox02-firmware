// Copyright 2022 Shift Cryptosecurity AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/// Firmware version, short format, e.g. "v9.12.0".
// This is evaluated at compile time, if the env var is missing or not set, there will be a compiler error.
pub static FIRMWARE_VERSION_SHORT: &str = {
    let version = env!("FIRMWARE_VERSION_SHORT");
    // Need explicit check as the env var could be set to an empty string accidentally (env!() only
    // panics if it is not set at all).
    if version.is_empty() {
        panic!("FIRMWARE_VERSION_SHORT is not set");
    }
    version
};
