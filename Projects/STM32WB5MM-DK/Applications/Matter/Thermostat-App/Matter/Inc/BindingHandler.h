/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

#include "app-common/zap-generated/ids/Clusters.h"
#include "app-common/zap-generated/ids/Commands.h"
#include <app/clusters/bindings/BindingManager.h>
#include "lib/core/CHIPError.h"


struct BindingCommandData;
using InvokeCommand = void (*)(const EmberBindingTableEntry &, chip::OperationalDeviceProxy *, BindingCommandData &);
using BindingAddedCallback = void (*)(bool availableAtStart);

CHIP_ERROR InitBindingHandler();
void TempSensorWorkerFunction(intptr_t context);
void BindingWorkerFunction(intptr_t context);

struct BindingCommandData
{
    chip::EndpointId localEndpointId = 1;
    chip::CommandId commandId;
    chip::ClusterId clusterId;
    InvokeCommand InvokeCommandFunc;
    uint8_t Value = 0;
    bool isGroup = false;
};
