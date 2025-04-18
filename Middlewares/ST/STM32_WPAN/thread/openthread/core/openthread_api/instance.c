/**
 ******************************************************************************
 * @file    instance.c
 * @author  MCD Application Team
 * @brief   This file contains the Instance interface shared between M0 and
 *          M4.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"

/* Include definition of compilation flags requested for OpenThread configuration */
#include OPENTHREAD_CONFIG_FILE

#include "instance.h"

extern otStateChangedCallback otStateChangedCb;


#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
otInstance *otInstanceInit(void *aInstanceBuffer, size_t *aInstanceBufferSize)
{
  otInstance * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_INIT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aInstanceBuffer;
  p_ot_req->Data[1] = (uint32_t) aInstanceBufferSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otInstance *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#else
otInstance *otInstanceInitSingle(void)
{
  otInstance * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_INIT_SINGLE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otInstance *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif /* #if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE */

uint32_t otInstanceGetId(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_GET_ID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otInstanceIsInitialized(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_IS_INITIALIZED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otInstanceFinalize(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_FINALIZE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otInstanceReset(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_RESET;

  p_ot_req->Size=0;

  Ot_Cmd_TransferWithNotif();

  Post_OtCmdProcessing();
}

#if OPENTHREAD_CONFIG_UPTIME_ENABLE
uint64_t otInstanceGetUptime(otInstance *aInstance)
{
  uint64_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_GET_UP_TIME;

  p_ot_req->Size=0;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  /* 64bit word is returned with two 32bits words (MSB are at offset 1 and LSB are offset 0) */
  rspData = (uint64_t)(((uint64_t)p_ot_req->Data[1] << 32) | p_ot_req->Data[0]);
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otInstanceGetUptimeAsString(otInstance *aInstance, char *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_GET_UP_TIME_AS_STRING;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aBuffer;
  p_ot_req->Data[1] = (uint32_t)aSize;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}
#endif

#if OPENTHREAD_MTD || OPENTHREAD_FTD
otError otSetStateChangedCallback(otInstance *aInstance, otStateChangedCallback aCallback,
    void *aContext)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* Store the callback function */
  otStateChangedCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SET_STATE_CHANGED_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otRemoveStateChangeCallback(otInstance *aInstance, otStateChangedCallback aCallback,
    void *aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* RESET the callback function */
  otStateChangedCb = NULL;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_REMOVE_STATE_CHANGED_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otInstanceFactoryReset(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_FACTORY_RESET;

  p_ot_req->Size=0;

  Ot_Cmd_TransferWithNotif();

  Post_OtCmdProcessing();
}

otError otInstanceErasePersistentInfo(otInstance *aInstance)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_ERASE_PERSISTENT_INFO;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif // OPENTHREAD_MTD || OPENTHREAD_FTD

#if OPENTHREAD_RADIO
void otInstanceResetRadioStack(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_RESET_RADIO_STACK;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}
#endif

const char *otGetVersionString(void)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_OPENTHREAD_GET_VERSION_STRING;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const char *otGetRadioVersionString(otInstance *aInstance)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_OPENTHREAD_GET_RADIO_VERSION_STRING;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
