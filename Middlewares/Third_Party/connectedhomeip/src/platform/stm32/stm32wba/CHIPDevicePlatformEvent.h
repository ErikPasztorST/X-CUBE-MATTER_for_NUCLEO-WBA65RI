/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include <platform/CHIPDeviceEvent.h>
#include <platform/stm32/stm32wba/CHIPPlatformConfig.h>

namespace chip {
namespace DeviceLayer {
namespace DeviceEventType {
/**
 * Enumerates platform-specific event types that are visible to the application.
 */
enum PublicPlatformSpecificEventTypes
{
    /* None currently defined */
};

/**
 * Enumerates platform-specific event types that are internal to the chip Device Layer.
 */
enum InternalPlatformSpecificEventTypes
{
    kSTMBLEConnected = kRange_InternalPlatformSpecific,
    kSTMBLEDisconnected,
    kCHIPoBLECCCWriteEvent,
    kCHIPoBLERXCharWriteEvent,
    kCHIPoBLETXCharWriteEvent,
};

} // namespace DeviceEventType

/**
 * Represents platform-specific event information.
 */
struct ChipDevicePlatformEvent final
{
    // TODO - add platform specific definition extension
    union
    {
        struct
        {
            uint8_t dummy;
        } STMBLEConnected;
        struct
        {
            uint8_t dummy;
        } CHIPoBLECCCWriteEvent;
        struct
        {
            uint8_t dummy;
        } CHIPoBLERXCharWriteEvent;
        struct
        {
            uint8_t dummy;
        } CHIPoBLETXCharWriteEvent;
    };
};

} // namespace DeviceLayer
} // namespace chip
