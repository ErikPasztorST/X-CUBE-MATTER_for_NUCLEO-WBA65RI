/*
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  Copyright (c) 2024 STMicroelectronics. All rights reserved.
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*/
#include <stddef.h>
#include "mbedtls/entropy.h"
void RNG_DeInit(void);
void RNG_Init(void);
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen);
void RNG_GetBytes(uint8_t *output, size_t length, size_t *output_length);
