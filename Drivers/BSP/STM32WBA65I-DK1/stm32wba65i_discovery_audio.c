/**
  ******************************************************************************
  * @file    stm32wba65i_discovery_audio.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio driver for the STM32WBA65I_DK1 board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
   + This driver supports STM32WBAxx devices on STM32WBA65I_DK1 (MB2143 and MB2130) board.

   + Call the function BSP_AUDIO_OUT_Init() for AUDIO OUT initialization:
        Instance:  Select the output instance. It can only be 0 (SAI)
        AudioInit: Audio Out structure to select the following parameters
                   - Device: Select the output device (headphone, speaker, hdmi ..)
                   - SampleRate: Select the output sample rate (8Khz .. 96Khz)
                   - BitsPerSample: Select the output resolution (16 or 32bits per sample)
                   - ChannelsNbr: Select the output channels number(1 for mono, 2 for stereo)
                   - Volume: Select the output volume(0% .. 100%)

      This function configures all the hardware required for the audio application (codec, I2C, SAI,
      GPIOs, DMA and interrupt if needed). This function returns BSP_ERROR_NONE if configuration is OK.
      If the returned value is different from BSP_ERROR_NONE or the function is stuck then the communication with
      the codec has failed (try to un-plug the power or reset device in this case).

      User can update the SAI or the clock configurations by overriding the weak MX functions
      MX_SAI1_Init() and  MX_SAI1_ClockConfig()
      User can override the default MSP configuration and register his own MSP callbacks (defined at application level)
      by calling BSP_AUDIO_OUT_RegisterMspCallbacks() function
      User can restore the default MSP configuration by calling BSP_AUDIO_OUT_RegisterDefaultMspCallbacks()
      To use these two functions, user have to enable USE_HAL_SAI_REGISTER_CALLBACKS
      within stm32wbaxx_hal_conf.h file


   + Call the function BSP_AUDIO_OUT_Play() to play audio stream:
        Instance:  Select the output instance. It can only be 0 (SAI)
        pBuf: pointer to the audio data file address
        NbrOfBytes: Total size of the buffer to be sent in Bytes

   + Call the function BSP_AUDIO_OUT_Pause() to pause playing
   + Call the function BSP_AUDIO_OUT_Resume() to resume playing.
       Note. After calling BSP_AUDIO_OUT_Pause() function for pause, only BSP_AUDIO_OUT_Resume() should be called
          for resume (it is not allowed to call BSP_AUDIO_OUT_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).
   + Call the function BSP_AUDIO_OUT_Stop() to stop playing.
   + Call the function BSP_AUDIO_OUT_Mute() to mute the player.
   + Call the function BSP_AUDIO_OUT_UnMute() to unmute the player.
   + Call the function BSP_AUDIO_OUT_IsMute() to get the mute state(BSP_AUDIO_MUTE_ENABLED or BSP_AUDIO_MUTE_DISABLED).
   + Call the function BSP_AUDIO_OUT_SetDevice() to update the AUDIO OUT device.
   + Call the function BSP_AUDIO_OUT_GetDevice() to get the AUDIO OUT device.
   + Call the function BSP_AUDIO_OUT_SetSampleRate() to update the AUDIO OUT sample rate.
   + Call the function BSP_AUDIO_OUT_GetSampleRate() to get the AUDIO OUT sample rate.
   + Call the function BSP_AUDIO_OUT_SetBitsPerSample() to update the AUDIO OUT resolution.
   + Call the function BSP_AUDIO_OUT_GetBitPerSample() to get the AUDIO OUT resolution.
   + Call the function BSP_AUDIO_OUT_SetChannelsNbr() to update the AUDIO OUT number of channels.
   + Call the function BSP_AUDIO_OUT_GetChannelsNbr() to get the AUDIO OUT number of channels.
   + Call the function BSP_AUDIO_OUT_SetVolume() to update the AUDIO OUT volume.
   + Call the function BSP_AUDIO_OUT_GetVolume() to get the AUDIO OUT volume.
   + Call the function BSP_AUDIO_OUT_GetState() to get the AUDIO OUT state.

   + BSP_AUDIO_OUT_SetDevice(), BSP_AUDIO_OUT_SetSampleRate(), BSP_AUDIO_OUT_SetBitsPerSample() and
     BSP_AUDIO_OUT_SetChannelsNbr() cannot be called while the state is AUDIO_OUT_STATE_PLAYING.
   + For each mode, you may need to implement the relative callback functions into your code.
      The callback functions are named BSP_AUDIO_OUT_XXX_CallBack() and only their prototypes are declared in
      the stm32wba65i_discovery_audio.h file. (refer to the example for more details on the callbacks implementations)


   + Call the function BSP_AUDIO_IN_Init() for AUDIO IN initialization:
        Instance : Select the input instance. It can only be 0 (SAI)
        AudioInit: Audio In structure to select the following parameters
                   - Device: Select the input device (analog, digital micx)
                   - SampleRate: Select the input sample rate (8Khz .. 96Khz)
                   - BitsPerSample: Select the input resolution (16 or 32bits per sample)
                   - ChannelsNbr: Select the input channels number(1 for mono, 2 for stereo)
                   - Volume: Select the input volume(0% .. 100%)


      This function configures all the hardware required for the audio application (codec, I2C, SAI,
      GPIOs, DMA and interrupt if needed). This function returns BSP_ERROR_NONE if configuration is OK.
      If the returned value is different from BSP_ERROR_NONE or the function is stuck then the communication with
      the codec has failed (try to un-plug the power or reset device in this case).
      User can update the SAI or the clock configurations by overriding the weak MX functions MX_SAIx_Init() and
      MX_SAIx_ClockConfig()
      User can override the default MSP configuration and register his own MSP callbacks (defined at application level)
      by calling BSP_AUDIO_IN_RegisterMspCallbacks() function
      User can restore the default MSP configuration by calling BSP_AUDIO_IN_RegisterDefaultMspCallbacks()
      To use these two functions, user have to enable USE_HAL_SAI_REGISTER_CALLBACKS within stm32wbaxx_hal_conf.h file

   + Call the function BSP_AUDIO_IN_Record() to record audio stream. The recorded data are stored to user buffer in raw
        (L, R, L, R ...)
        Instance : Select the input instance. It can only be 0 (SAI)
        pBuf: pointer to user buffer
        NbrOfBytes: Total size of the buffer to be sent in Bytes

   + Call the function BSP_AUDIO_IN_Pause() to pause recording
   + Call the function BSP_AUDIO_IN_Resume() to resume recording.
   + Call the function BSP_AUDIO_IN_Stop() to stop recording.
   + Call the function BSP_AUDIO_IN_SetDevice() to update the AUDIO IN device.
   + Call the function BSP_AUDIO_IN_GetDevice() to get the AUDIO IN device.
   + Call the function BSP_AUDIO_IN_SetSampleRate() to update the AUDIO IN sample rate.
   + Call the function BSP_AUDIO_IN_GetSampleRate() to get the AUDIO IN sample rate.
   + Call the function BSP_AUDIO_IN_SetBitPerSample() to update the AUDIO IN resolution.
   + Call the function BSP_AUDIO_IN_GetBitPerSample() to get the AUDIO IN resolution.
   + Call the function BSP_AUDIO_IN_SetChannelsNbr() to update the AUDIO IN number of channels.
   + Call the function BSP_AUDIO_IN_GetChannelsNbr() to get the AUDIO IN number of channels.
   + Call the function BSP_AUDIO_IN_SetVolume() to update the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetVolume() to get the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetState() to get the AUDIO IN state.

   + For each mode, you may need to implement the relative callback functions into your code.
      The callback functions are named BSP_AUDIO_IN_XXX_CallBack() and only their prototypes are declared in
      the stm32wba65i_discovery_audio.h file. (refer to the example for more details on the callbacks implementations)

   + The driver API and the callback functions are at the end of the stm32wba65i_discovery_audio.h file.

  @endverbatim
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wba65i_discovery_audio.h"
#include "stm32wba65i_discovery_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WBA65I_DK1
  * @{
  */

/** @defgroup STM32WBA65I_DK1_AUDIO AUDIO
  * @{
  */

/** @defgroup STM32WBA65I_DK1_AUDIO_Private_Macros AUDIO Private Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32WBA65I_DK1_AUDIO_Exported_Variables AUDIO Exported Variables
  * @{
  */
/* Audio in and out context */
AUDIO_OUT_Ctx_t Audio_Out_Ctx[AUDIO_OUT_INSTANCES_NBR] = {{
    AUDIO_OUT_DEVICE_HEADPHONE,
    AUDIO_FREQUENCY_8K,
    AUDIO_RESOLUTION_16B,
    50U,
    2U,
    AUDIO_MUTE_DISABLED,
    AUDIO_OUT_STATE_RESET,
    0U
  }
};

AUDIO_IN_Ctx_t  Audio_In_Ctx[AUDIO_IN_INSTANCES_NBR] = {{
    AUDIO_IN_DEVICE_ANALOG_MIC,
    AUDIO_FREQUENCY_8K,
    AUDIO_RESOLUTION_16B,
    2U,
    NULL,
    0U,
    50U,
    AUDIO_IN_STATE_RESET,
    0U
  }
};

/* Audio component object */
void *Audio_CompObj = NULL;

/* Audio driver */
AUDIO_Drv_t *Audio_Drv = NULL;

/* Play  */
SAI_HandleTypeDef haudio_out_sai = {NULL};

/* Record  */
SAI_HandleTypeDef haudio_in_sai = {NULL};

/* Audio in and out DMA handles used by SAI */
DMA_HandleTypeDef hDmaSaiTx = {NULL}, hDmaSaiRx = {NULL};

/* Queue variables declaration */
static DMA_QListTypeDef SAITxQueue, SAIRxQueue;

/**
  * @}
  */

/** @defgroup STM32WBA65I_DK1_AUDIO_Private_Variables AUDIO Private Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32WBA65I_DK1_AUDIO_Private_Function_Prototypes AUDIO Private Function Prototypes
  * @{
  */
static int32_t WM8904_Probe(void);

static void SAI_MspInit(SAI_HandleTypeDef *hsai);
static void SAI_MspDeInit(SAI_HandleTypeDef *hsai);

/* SAI callbacks */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
static void SAI_TxCpltCallback(SAI_HandleTypeDef *hsai);
static void SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai);
static void SAI_ErrorCallback(SAI_HandleTypeDef *hsai);
static void SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai);
static void SAI_RxCpltCallback(SAI_HandleTypeDef *hsai);
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/** @addtogroup STM32WBA65I_DK1_AUDIO_OUT_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the audio out peripherals.
  * @param  Instance Audio out instance.
  * @param  AudioInit Audio out init structure.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((AudioInit->BitsPerSample == AUDIO_RESOLUTION_32B)
           || (AudioInit->BitsPerSample == AUDIO_RESOLUTION_8B))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RESET) &&
           ((Audio_In_Ctx[Instance].SampleRate != AudioInit->SampleRate) ||
            (Audio_In_Ctx[Instance].BitsPerSample != AudioInit->BitsPerSample)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Fill audio out context structure */
    Audio_Out_Ctx[Instance].Device         = AudioInit->Device;
    Audio_Out_Ctx[Instance].SampleRate     = AudioInit->SampleRate;
    Audio_Out_Ctx[Instance].BitsPerSample  = AudioInit->BitsPerSample;
    Audio_Out_Ctx[Instance].ChannelsNbr    = AudioInit->ChannelsNbr;
    Audio_Out_Ctx[Instance].Volume         = AudioInit->Volume;

    /* Probe the audio codec */
    if ((Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET) &&
        (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET))
    {
      if (WM8904_Probe() != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }

    if (status == BSP_ERROR_NONE)
    {
      /* Set SAI instance */
      haudio_out_sai.Instance = AUDIO_OUT_SAI;

      /* Configure the SAI PLL according to the requested audio frequency if not already done by other instances */
      if (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
      {
        if (MX_SAI1_ClockConfig(&haudio_out_sai, AudioInit->SampleRate) != HAL_OK)
        {
          status = BSP_ERROR_CLOCK_FAILURE;
        }
      }
      if (status == BSP_ERROR_NONE)
      {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
        SAI_MspInit(&haudio_out_sai);
#else
        /* Register the SAI MSP Callbacks */
        if (Audio_Out_Ctx[Instance].IsMspCallbacksValid == 0U)
        {
          if (BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
        }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Prepare SAI peripheral initialization */
        MX_SAI_Config_t mxSaiInit;
        mxSaiInit.AudioFrequency    = AudioInit->SampleRate;
        mxSaiInit.AudioMode         = SAI_MODEMASTER_TX;
        mxSaiInit.ClockStrobing     = SAI_CLOCKSTROBING_FALLINGEDGE;
        mxSaiInit.MonoStereoMode    = (AudioInit->ChannelsNbr == 1U) ? SAI_MONOMODE : SAI_STEREOMODE;
        if (AudioInit->BitsPerSample == AUDIO_RESOLUTION_24B)
        {
          mxSaiInit.DataSize          = SAI_DATASIZE_24;
          mxSaiInit.FrameLength       = 64;
          mxSaiInit.ActiveFrameLength = 32;
        }
        else
        {
          mxSaiInit.DataSize          = SAI_DATASIZE_16;
          mxSaiInit.FrameLength       = 32;
          mxSaiInit.ActiveFrameLength = 16;
        }
        mxSaiInit.OutputDrive       = SAI_OUTPUTDRIVE_ENABLE;
        mxSaiInit.Synchro           = SAI_ASYNCHRONOUS;
        mxSaiInit.SynchroExt        = SAI_SYNCEXT_DISABLE;
        mxSaiInit.SlotActive        = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;

        /* SAI peripheral initialization */
        if (MX_SAI1_Init(&haudio_out_sai, &mxSaiInit) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
        /* Register SAI TC, HT and Error callbacks */
        else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_COMPLETE_CB_ID, SAI_TxCpltCallback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_HALFCOMPLETE_CB_ID, SAI_TxHalfCpltCallback)
                 != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
        else
        {
          /* Initialize audio codec */
          WM8904_Init_t codec_init;
          if (Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET)
          {
            if (Audio_In_Ctx[0].Device == AUDIO_IN_DEVICE_ANALOG_MIC)
            {
              codec_init.InputDevice = WM8904_IN_MIC1;
            }
            else if (Audio_In_Ctx[0].Device == AUDIO_IN_DEVICE_LINE_IN)
            {
              codec_init.InputDevice = WM8904_IN_LINE2;
            }
            else
            {
              codec_init.InputDevice = WM8904_IN_DIGITAL_MIC2;
            }
          }
          codec_init.OutputDevice = WM8904_OUT_HEADPHONE;
          codec_init.Resolution   = WM8904_RESOLUTION_16B;
          codec_init.Frequency    = AudioInit->SampleRate;
          codec_init.Volume       = AudioInit->Volume;
          if (Audio_Drv->Init(Audio_CompObj, &codec_init) < 0)
          {
            status = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Update audio out context state */
            Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_STOP;
          }
        }
      }
    }
  }
  return status;
}

/**
  * @brief  De-initialize the audio out peripherals.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_RESET)
  {
    /* SAI peripheral de-initialization */
    if (HAL_SAI_DeInit(&haudio_out_sai) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    /* De-initialize audio codec if not currently used by audio in instance 0 */
    else
    {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
      SAI_MspDeInit(&haudio_out_sai);
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
      if (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
      {
        if (Audio_Drv->DeInit(Audio_CompObj) < 0)
        {
          status = BSP_ERROR_COMPONENT_FAILURE;
        }
      }
    }

    if (status == BSP_ERROR_NONE)
    {
      /* Update audio out context */
      Audio_Out_Ctx[Instance].State  = AUDIO_OUT_STATE_RESET;
      Audio_Out_Ctx[Instance].IsMute = 0U;
    }
  }
  else
  {
    /* Nothing to do */
  }
  return status;
}

/**
  * @brief  Start playing audio stream from a data buffer for a determined size.
  * @param  Instance Audio out instance.
  * @param  pData Pointer on data buffer.
  * @param  NbrOfBytes Size of buffer in bytes. Maximum size is 65535 bytes.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Play(uint32_t Instance, uint8_t *pData, uint32_t NbrOfBytes)
{
  int32_t  status = BSP_ERROR_NONE;
  uint16_t NbrOfDmaDatas;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || (pData == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Compute number of DMA data to tranfser according resolution */
    if (Audio_Out_Ctx[Instance].BitsPerSample == AUDIO_RESOLUTION_16B)
    {
      NbrOfDmaDatas = (uint16_t)(NbrOfBytes / 2U);
    }
    else /* AUDIO_RESOLUTION_24b */
    {
      NbrOfDmaDatas = (uint16_t)(NbrOfBytes / 4U);
    }
    /* Initiate a DMA transfer of audio samples towards the serial audio interface */
    if (HAL_SAI_Transmit_DMA(&haudio_out_sai, pData, NbrOfDmaDatas) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Call the audio codec play function */
    else if (Audio_Drv->Play(Audio_CompObj) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Update audio out state */
      Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PLAYING;
    }
  }
  return status;
}

/**
  * @brief  Pause playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Pause(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PLAYING)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec pause function */
  else if (Audio_Drv->Pause(Audio_CompObj) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Pause DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_SAI_DMAPause(&haudio_out_sai) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PAUSE;
  }
  return status;
}

/**
  * @brief  Resume playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Resume(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PAUSE)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec resume function */
  else if (Audio_Drv->Resume(Audio_CompObj) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Resume DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_SAI_DMAResume(&haudio_out_sai) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PLAYING;
  }
  return status;
}

/**
  * @brief  Stop playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Stop(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_STOP)
  {
    /* Nothing to do */
  }
  else if ((Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PLAYING) &&
           (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PAUSE))
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec stop function */
  else if (Audio_Drv->Stop(Audio_CompObj, WM8904_PDWN_SW) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Stop DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_SAI_DMAStop(&haudio_out_sai) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_STOP;
  }
  return status;
}

/**
  * @brief  Mute playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Mute(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check audio out mute status */
  else if (Audio_Out_Ctx[Instance].IsMute == 1U)
  {
    /* Nothing to do */
  }
  /* Call the audio codec mute function */
  else if (Audio_Drv->SetMute(Audio_CompObj, WM8904_MUTE_ON) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Update audio out mute status */
    Audio_Out_Ctx[Instance].IsMute = 1U;
  }
  return status;
}

/**
  * @brief  Unmute playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_UnMute(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check audio out mute status */
  else if (Audio_Out_Ctx[Instance].IsMute == 0U)
  {
    /* Nothing to do */
  }
  /* Call the audio codec mute function */
  else if (Audio_Drv->SetMute(Audio_CompObj, WM8904_MUTE_OFF) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Update audio out mute status */
    Audio_Out_Ctx[Instance].IsMute = 0U;
  }
  return status;
}

/**
  * @brief  Check audio out mute status.
  * @param  Instance Audio out instance.
  * @param  IsMute Pointer to mute status. Value is 1 for mute, 0 for unmute status.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_IsMute(uint32_t Instance, uint32_t *IsMute)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out mute status */
  else
  {
    *IsMute = Audio_Out_Ctx[Instance].IsMute;
  }
  return status;
}

/**
  * @brief  Set audio out volume.
  * @param  Instance Audio out instance.
  * @param  Volume Volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || (Volume > 100U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Call the audio codec volume control function */
    if (Audio_Drv->SetVolume(Audio_CompObj, VOLUME_OUTPUT, (uint8_t) Volume) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Store volume on audio out context */
      Audio_Out_Ctx[Instance].Volume = Volume;
    }
  }
  return status;
}

/**
  * @brief  Get audio out volume.
  * @param  Instance Audio out instance.
  * @param  Volume Pointer to volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out volume */
  else
  {
    *Volume = Audio_Out_Ctx[Instance].Volume;
  }
  return status;
}

/**
  * @brief  Set audio out sample rate.
  * @param  Instance Audio out instance.
  * @param  SampleRate Sample rate of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetSampleRate(uint32_t Instance, uint32_t SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check if record on instance 0 is on going and corresponding sample rate */
  else if ((Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET) &&
           (Audio_In_Ctx[0].SampleRate != SampleRate))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check if sample rate is modified */
  else if (Audio_Out_Ctx[Instance].SampleRate == SampleRate)
  {
    /* Nothing to do */
  }
  else
  {
    /* Update SAI1 clock config */
    haudio_out_sai.Init.AudioFrequency = SampleRate;
    if (MX_SAI1_ClockConfig(&haudio_out_sai, SampleRate) != HAL_OK)
    {
      status = BSP_ERROR_CLOCK_FAILURE;
    }
    /* Re-initialize SAI1 with new sample rate */
    else if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Store new sample rate on audio out context */
    else
    {
      Audio_Out_Ctx[Instance].SampleRate = SampleRate;
    }
  }
  return status;
}

/**
  * @brief  Get audio out sample rate.
  * @param  Instance Audio out instance.
  * @param  SampleRate Pointer to sample rate of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out sample rate */
  else
  {
    *SampleRate = Audio_Out_Ctx[Instance].SampleRate;
  }
  return status;
}

/**
  * @brief  Set audio out device.
  * @param  Instance Audio out instance.
  * @param  Device Device of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetDevice(uint32_t Instance, uint32_t Device)
{
  int32_t status = BSP_ERROR_NONE;

  UNUSED(Device);

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because there is only one device (AUDIO_OUT_HEADPHONE) */
  }
  return status;
}

/**
  * @brief  Get audio out device.
  * @param  Instance Audio out instance.
  * @param  Device Pointer to device of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out device */
  else
  {
    *Device = Audio_Out_Ctx[Instance].Device;
  }
  return status;
}

/**
  * @brief  Set bits per sample for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  BitsPerSample Bits per sample of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && ((BitsPerSample == AUDIO_RESOLUTION_32B) || (BitsPerSample == AUDIO_RESOLUTION_8B)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET) &&
           (Audio_In_Ctx[0].BitsPerSample != BitsPerSample))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Store new bits per sample on audio out context */
    Audio_Out_Ctx[Instance].BitsPerSample = BitsPerSample;

    /* Update data size, frame length and active frame length parameters of SAI handle */
    if (BitsPerSample == AUDIO_RESOLUTION_24B)
    {
      haudio_out_sai.Init.DataSize               = SAI_DATASIZE_24;
      haudio_out_sai.FrameInit.FrameLength       = 64;
      haudio_out_sai.FrameInit.ActiveFrameLength = 32;
    }
    else
    {
      haudio_out_sai.Init.DataSize               = SAI_DATASIZE_16;
      haudio_out_sai.FrameInit.FrameLength       = 32;
      haudio_out_sai.FrameInit.ActiveFrameLength = 16;
    }

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
    SAI_MspInit(&haudio_out_sai);
#else
    /* Update SAI state only to keep current MSP functions */
    haudio_out_sai.State = HAL_SAI_STATE_RESET;
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */

    /* Re-initialize SAI1 with new parameters */
    if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
    /* Register SAI TC, HT and Error callbacks */
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_COMPLETE_CB_ID, SAI_TxCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_HALFCOMPLETE_CB_ID, SAI_TxHalfCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
    else
    {
      /* Nothing to do */
    }
  }
  return status;
}

/**
  * @brief  Get bits per sample for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  BitsPerSample Pointer to bits per sample of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current bits per sample of audio out stream */
  else
  {
    *BitsPerSample = Audio_Out_Ctx[Instance].BitsPerSample;
  }
  return status;
}

/**
  * @brief  Set channels number for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  ChannelNbr Channels number of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || ((ChannelNbr != 1U) && (ChannelNbr != 2U)))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Update mono or stereo mode of SAI handle */
    haudio_out_sai.Init.MonoStereoMode = (ChannelNbr == 1U) ? SAI_MONOMODE : SAI_STEREOMODE;

    /* Re-initialize SAI1 with new parameter */
    if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      /* Store new channels number on audio out context */
      Audio_Out_Ctx[Instance].ChannelsNbr = ChannelNbr;
    }
  }
  return status;
}

/**
  * @brief  Get channels number for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  ChannelNbr Pointer to channels number of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current channels number of audio out stream */
  else
  {
    *ChannelNbr = Audio_Out_Ctx[Instance].ChannelsNbr;
  }
  return status;
}

/**
  * @brief  Get current state for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  State Pointer to state of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Get the current state of audio out stream */
  else
  {
    *State = Audio_Out_Ctx[Instance].State;
  }
  return status;
}

/**
  * @brief  BSP AUDIO OUT interrupt handler.
  * @param  Instance Audio out instance.
  * @param  Device Device of the audio out stream.
  * @retval None.
  */
void BSP_AUDIO_OUT_IRQHandler(uint32_t Instance, uint32_t Device)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  UNUSED(Device);

  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)  || !defined (USE_HAL_SAI_REGISTER_CALLBACKS)
/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hsai SAI handle
  * @retval None
  */
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Manage the remaining file size and new address offset: This function
     should be coded by user (its prototype is already declared in stm32wba65i_discovery_audio.h) */
  BSP_AUDIO_OUT_TransferComplete_CallBack(0);
}

/**
  * @brief  Tx Half Transfer completed callbacks.
  * @param  hsai  SAI handle
  * @retval None
  */
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Manage the remaining file size and new address offset: This function
     should be coded by user (its prototype is already declared in stm32wba65i_discovery_audio.h) */
  BSP_AUDIO_OUT_HalfTransfer_CallBack(0);
}

/**
  * @brief  SAI error callbacks.
  * @param  hsai  SAI handle
  * @retval None
  */
void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == AUDIO_OUT_SAI)
  {
    BSP_AUDIO_OUT_Error_CallBack(0);
  }
  else
  {
    BSP_AUDIO_IN_Error_CallBack(0);
  }
}
#endif /* SAI Register Callbacks */

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register default BSP AUDIO OUT msp callbacks.
  * @param  Instance AUDIO OUT Instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPINIT_CB_ID, SAI_MspInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPDEINIT_CB_ID, SAI_MspDeInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Audio_Out_Ctx[Instance].IsMspCallbacksValid = 1U;
    }
  }
  /* Return BSP status */
  return status;
}

/**
  * @brief  Register BSP AUDIO OUT msp callbacks.
  * @param  Instance AUDIO OUT Instance.
  * @param  CallBacks Pointer to MspInit/MspDeInit callback functions.
  * @retval BSP status
  */
int32_t BSP_AUDIO_OUT_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_OUT_Cb_t *CallBacks)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPINIT_CB_ID, CallBacks->pMspSaiInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPDEINIT_CB_ID, CallBacks->pMspSaiDeInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Audio_Out_Ctx[Instance].IsMspCallbacksValid = 1U;
    }
  }
  /* Return BSP status */
  return status;
}
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Manage the BSP audio out transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manage the BSP audio out half transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manages the BSP audio out error event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  SAI1 clock Config.
  * @param  hsai SAI handle.
  * @param  SampleRate Audio sample rate used to play the audio stream.
  * @note   The SAI PLL configuration done within this function assumes that
  *         the SAI PLL input is HSE clock and that HSE is already enabled at 32 MHz.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_SAI1_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  HAL_StatusTypeDef ret = HAL_OK;
  RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;
  RCC_OscInitTypeDef       rcc_osc_init_struct;

  rcc_osc_init_struct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
  rcc_osc_init_struct.HSEState            = RCC_HSE_ON;
  rcc_osc_init_struct.HSEDiv              = RCC_HSE_DIV1;
  rcc_osc_init_struct.PLL1.PLLState       = RCC_PLL_ON;
  rcc_osc_init_struct.PLL1.PLLSource      = RCC_PLLSOURCE_HSE;
  rcc_osc_init_struct.PLL1.PLLM           = 8;
  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) || (SampleRate == AUDIO_FREQUENCY_44K))
  {
    /* SAI clock config:
    PLL1_VCO Input = HSE/PLLM = 4 Mhz
    PLL1_VCO Output = PLL1_VCO Input * PLLN = 192 Mhz
    SAI_CLK_x = PLL1_VCO Output/PLLP = 192/17 = 11.294 Mhz */
    rcc_osc_init_struct.PLL1.PLLN         = 48;
    rcc_osc_init_struct.PLL1.PLLP         = 17;
  }
  else  /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_32K, AUDIO_FREQUENCY_48K, AUDIO_FREQUENCY_96K */
  {
    /* SAI clock config:
    PLL1_VCO Input = HSE/PLLM = 4 Mhz
    PLL1_VCO Output = PLL1_VCO Input * PLLN = 344 Mhz
    SAI_CLK_x = PLL1_VCO Output/PLLP = 344/7 = 49.142 Mhz */
    rcc_osc_init_struct.PLL1.PLLN         = 86;
    rcc_osc_init_struct.PLL1.PLLP         = 7;
  }
  rcc_osc_init_struct.PLL1.PLLQ           = 28;
  rcc_osc_init_struct.PLL1.PLLR           = 28;
  rcc_osc_init_struct.PLL1.PLLFractional  = 0;
  if (HAL_RCC_OscConfig(&rcc_osc_init_struct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  rcc_ex_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  rcc_ex_clk_init_struct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL1P;

  if (HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  __HAL_RCC_SAI1_CLK_ENABLE();

  return ret;
}

/**
  * @brief  Initialize SAI1.
  * @param  hsai SAI handle.
  * @param  MXInit SAI configuration structure.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_SAI1_Init(SAI_HandleTypeDef *hsai, MX_SAI_Config_t *MXInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Disable SAI peripheral to allow access to SAI internal registers */
  __HAL_SAI_DISABLE(hsai);

  /* Configure SAI1_Block_X */
  hsai->Init.MonoStereoMode       = MXInit->MonoStereoMode;
  hsai->Init.AudioFrequency       = MXInit->AudioFrequency;
  hsai->Init.AudioMode            = MXInit->AudioMode;
  hsai->Init.NoDivider            = SAI_MASTERDIVIDER_ENABLE;
  hsai->Init.Protocol             = SAI_FREE_PROTOCOL;
  hsai->Init.DataSize             = MXInit->DataSize;
  hsai->Init.FirstBit             = SAI_FIRSTBIT_MSB;
  hsai->Init.ClockStrobing        = MXInit->ClockStrobing;
  hsai->Init.Synchro              = MXInit->Synchro;
  hsai->Init.OutputDrive          = MXInit->OutputDrive;
  hsai->Init.FIFOThreshold        = SAI_FIFOTHRESHOLD_1QF;
  hsai->Init.SynchroExt           = MXInit->SynchroExt;
  hsai->Init.CompandingMode       = SAI_NOCOMPANDING;
  hsai->Init.TriState             = SAI_OUTPUT_NOTRELEASED;
  hsai->Init.Mckdiv               = 0U;
  hsai->Init.MckOutput            = SAI_MCK_OUTPUT_ENABLE;
  hsai->Init.MckOverSampling      = SAI_MCK_OVERSAMPLING_DISABLE;
  hsai->Init.PdmInit.Activation   = DISABLE;

  /* Configure SAI1_Block_X Frame */
  hsai->FrameInit.FrameLength       = MXInit->FrameLength;
  hsai->FrameInit.ActiveFrameLength = MXInit->ActiveFrameLength;
  hsai->FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai->FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  hsai->FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  /* Configure SAI1_Block_X Slot */
  hsai->SlotInit.FirstBitOffset     = 0;
  if (MXInit->DataSize == SAI_DATASIZE_24)
  {
    hsai->SlotInit.SlotSize         = SAI_SLOTSIZE_32B;
  }
  else
  {
    hsai->SlotInit.SlotSize         = SAI_SLOTSIZE_16B;
  }
  hsai->SlotInit.SlotNumber         = 2;
  hsai->SlotInit.SlotActive         = MXInit->SlotActive;

  if (HAL_SAI_Init(hsai) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  return status;
}
/**
  * @}
  */

/** @defgroup STM32WBA65I_DK1_AUDIO_OUT_Private_Functions AUDIO OUT Private Functions
  * @{
  */
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hsai SAI handle
  * @retval None
  */
static void SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Manage the remaining file size and new address offset: This function
     should be coded by user (its prototype is already declared in stm32wba65i_discovery_audio.h) */
  BSP_AUDIO_OUT_TransferComplete_CallBack(0);
}

/**
  * @brief  Tx Half Transfer completed callbacks.
  * @param  hsai  SAI handle
  * @retval None
  */
static void SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Manage the remaining file size and new address offset: This function
     should be coded by user (its prototype is already declared in stm32wba65i_discovery_audio.h) */
  BSP_AUDIO_OUT_HalfTransfer_CallBack(0);
}

/**
  * @brief  SAI error callbacks.
  * @param  hsai  SAI handle
  * @retval None
  */
static void SAI_ErrorCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == AUDIO_OUT_SAI)
  {
    BSP_AUDIO_OUT_Error_CallBack(0);
  }
  else
  {
    BSP_AUDIO_IN_Error_CallBack(0);
  }
}
#endif /* SAI Register callbacks */
/**
  * @}
  */

/** @addtogroup STM32WBA65I_DK1_AUDIO_IN_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize wave recording.
  * @param  Instance  Audio IN instance: 0 for SAI
  * @param  AudioInit Init structure
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RESET)
  {
    ret = BSP_ERROR_BUSY;
  }
  else if (AudioInit->BitsPerSample != AUDIO_RESOLUTION_16B)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((AudioInit->ChannelsNbr != 1U) && (AudioInit->Device == AUDIO_IN_DEVICE_ANALOG_MIC))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((AudioInit->ChannelsNbr != 2U) && (AudioInit->Device == AUDIO_IN_DEVICE_LINE_IN))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((AudioInit->ChannelsNbr != 1U) && (AudioInit->Device == AUDIO_IN_DEVICE_DIGITAL_MIC))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET) && (Audio_Out_Ctx[0].SampleRate != AudioInit->SampleRate))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    /* Store the audio record context */
    Audio_In_Ctx[Instance].Device          = AudioInit->Device;
    Audio_In_Ctx[Instance].ChannelsNbr     = AudioInit->ChannelsNbr;
    Audio_In_Ctx[Instance].SampleRate      = AudioInit->SampleRate;
    Audio_In_Ctx[Instance].BitsPerSample   = AudioInit->BitsPerSample;
    Audio_In_Ctx[Instance].Volume          = AudioInit->Volume;
    Audio_In_Ctx[Instance].State           = AUDIO_IN_STATE_INIT;

    /* Un-reset audio codec if not currently used by audio out instances */
    if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
    {
      /* Initialize the codec internal registers */
      if (WM8904_Probe() != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }

    if (ret == BSP_ERROR_NONE)
    {
      /* Set SAI instances (SAI1_Block_A needed for MCLK, FS and CLK signals) */
      haudio_in_sai.Instance  = AUDIO_IN_SAI;
      haudio_out_sai.Instance = AUDIO_OUT_SAI;

      /* Configure the SAI PLL according to the requested audio frequency if not already done by other instances */
      if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
      {
        if (MX_SAI1_ClockConfig(&haudio_in_sai, AudioInit->SampleRate) != HAL_OK)
        {
          ret = BSP_ERROR_CLOCK_FAILURE;
        }
      }

      if (ret == BSP_ERROR_NONE)
      {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
        SAI_MspInit(&haudio_in_sai);
        if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
        {
          SAI_MspInit(&haudio_out_sai);
        }
#else
        /* Register the default SAI MSP callbacks */
        if (Audio_In_Ctx[Instance].IsMspCallbacksValid == 0U)
        {
          if (BSP_AUDIO_IN_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
        }
        if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
        {
          if (Audio_Out_Ctx[0].IsMspCallbacksValid == 0U)
          {
            if (BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(0) != BSP_ERROR_NONE)
            {
              ret = BSP_ERROR_PERIPH_FAILURE;
            }
          }
        }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
      }

      if (ret == BSP_ERROR_NONE)
      {
        MX_SAI_Config_t mx_config;

        /* Prepare haudio_in_sai handle */
        mx_config.AudioFrequency        = Audio_In_Ctx[Instance].SampleRate;
        mx_config.AudioMode             = SAI_MODESLAVE_RX;
        mx_config.ClockStrobing         = SAI_CLOCKSTROBING_FALLINGEDGE;
        mx_config.MonoStereoMode        = (AudioInit->Device != AUDIO_IN_DEVICE_LINE_IN) ? SAI_MONOMODE :
                                          SAI_STEREOMODE;
        mx_config.DataSize              = SAI_DATASIZE_16;
        mx_config.FrameLength           = 32;
        mx_config.ActiveFrameLength     = 16;
        mx_config.OutputDrive           = SAI_OUTPUTDRIVE_ENABLE;
        mx_config.Synchro               = SAI_SYNCHRONOUS;
        mx_config.SynchroExt            = SAI_SYNCEXT_DISABLE;
        mx_config.SlotActive            = (AudioInit->Device != AUDIO_IN_DEVICE_LINE_IN) ? SAI_SLOTACTIVE_0 :
                                          (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

        if (MX_SAI1_Init(&haudio_in_sai, &mx_config) != HAL_OK)
        {
          /* Return BSP_ERROR_PERIPH_FAILURE when operations are not correctly done */
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
        else
        {
          if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
          {
            /* Prepare haudio_out_sai handle */
            mx_config.AudioMode         = SAI_MODEMASTER_TX;
            mx_config.Synchro           = SAI_ASYNCHRONOUS;
            mx_config.SlotActive        = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;

            Audio_Out_Ctx[0].BitsPerSample = AUDIO_RESOLUTION_16B;

            if (MX_SAI1_Init(&haudio_out_sai, &mx_config) != HAL_OK)
            {
              /* Return BSP_ERROR_PERIPH_FAILURE when operations are not correctly done */
              ret = BSP_ERROR_PERIPH_FAILURE;
            }
          }
        }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
        if (ret == BSP_ERROR_NONE)
        {
          /* Register SAI TC, HT and Error callbacks */
          if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_RX_COMPLETE_CB_ID, SAI_RxCpltCallback) != HAL_OK)
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_RX_HALFCOMPLETE_CB_ID, SAI_RxHalfCpltCallback)
                   != HAL_OK)
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
          else
          {
            if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
            {
              ret = BSP_ERROR_PERIPH_FAILURE;
            }
          }
        }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
        if (ret == BSP_ERROR_NONE)
        {
          WM8904_Init_t codec_init;
          uint32_t      wm8904InputDevice;

          if (AudioInit->Device == AUDIO_IN_DEVICE_ANALOG_MIC)
          {
            wm8904InputDevice = WM8904_IN_MIC1;
          }
          else if (AudioInit->Device == AUDIO_IN_DEVICE_LINE_IN)
          {
            wm8904InputDevice = WM8904_IN_LINE2;
          }
          else
          {
            wm8904InputDevice = WM8904_IN_DIGITAL_MIC2;
          }

          /* Fill codec_init structure */
          codec_init.OutputDevice = (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET) ? WM8904_OUT_NONE :
                                    WM8904_OUT_HEADPHONE;
          codec_init.Frequency    = AudioInit->SampleRate;
          codec_init.Resolution   = WM8904_RESOLUTION_16B; /* Not used */
          codec_init.Volume       = AudioInit->Volume;
          codec_init.InputDevice  = wm8904InputDevice;

          /* Initialize the codec internal registers */
          if (Audio_Drv->Init(Audio_CompObj, &codec_init) < 0)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Update audio in context state */
            Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
          }
        }
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Deinit the audio IN peripherals.
  * @param  Instance  AUDIO IN Instance. It can be 0 when SAI is used or 1 if SAI PDM is used
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RESET)
  {
    /* SAI peripheral de-initialization */
    if (HAL_SAI_DeInit(&haudio_in_sai) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    /* De-initialize audio codec if not currently used by audio out instance 0 */
    else
    {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
      SAI_MspDeInit(&haudio_in_sai);
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
      if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
      {
        if (Audio_Drv->DeInit(Audio_CompObj) < 0)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
      }
    }

    if (ret == BSP_ERROR_NONE)
    {
      /* Update audio in context */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RESET;
    }
  }
  else
  {
    /* Nothing to do */
  }


  /* Return BSP status */
  return ret;
}

/**
  * @brief  Start audio recording.
  * @param  Instance  AUDIO IN Instance. It can be 0 (SAI is used)
  * @param  pBuf     Main buffer pointer for the recorded data storing
  * @param  NbrOfBytes  Size of the record buffer
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Record(uint32_t Instance, uint8_t *pBuf, uint32_t NbrOfBytes)
{
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_IN_INSTANCES_NBR) || (pBuf == NULL))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* If no playback is on going, transmit some bytes on audio out stream to generate SAI clock and synchro signals */
    if ((Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_PLAYING) && (Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_PAUSE))
    {
      uint8_t TxData[2] = {0x00U, 0x00U};
      if (HAL_SAI_Transmit(&haudio_out_sai, TxData, 2, 1000) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }

    if (ret == BSP_ERROR_NONE)
    {
      /* Call the audio Codec Play function */
      if (Audio_Drv->Play(Audio_CompObj) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Initiate a DMA transfer of audio samples from the serial audio interface */
        /* Because only 16 bits per sample is supported, DMA transfer is in halfword size */
        if (HAL_SAI_Receive_DMA(&haudio_in_sai, (uint8_t *) pBuf, (uint16_t) NbrOfBytes / 2U) != HAL_OK)
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }

    if (ret == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Stop audio recording.
  * @param  Instance  AUDIO IN Instance. It can be 0 (SAI is used)
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Stop(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_STOP)
  {
    /* Nothing to do */
  }
  else if ((Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RECORDING) &&
           (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_PAUSE))
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* Call the Media layer stop function */
    if (Audio_Drv->Stop(Audio_CompObj, WM8904_PDWN_SW) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if (HAL_SAI_DMAStop(&haudio_in_sai) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        /* Update BSP AUDIO IN state */
        Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Pause the audio file stream.
  * @param  Instance  AUDIO IN Instance. It can be 0 (SAI is used)
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Pause(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RECORDING)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if (Audio_Drv->Pause(Audio_CompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Pause DMA transfer of audio samples from the serial audio interface */
      if (HAL_SAI_DMAPause(&haudio_in_sai) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        /* Update BSP AUDIO IN state */
        Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_PAUSE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Resume the audio file stream.
  * @param  Instance  AUDIO IN Instance. It can be 0 (SAI is used)
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_Resume(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_PAUSE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if (Audio_Drv->Resume(Audio_CompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Resume DMA transfer of audio samples from the serial audio interface */
      if (HAL_SAI_DMAResume(&haudio_in_sai) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        /* Update BSP AUDIO IN state */
        Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set Audio In device
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  Device    The audio input device to be used
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if (Audio_In_Ctx[Instance].Device != Device)
    {
      MX_SAI_Config_t mx_config;

      /* Prepare haudio_in_sai handle */
      mx_config.AudioFrequency        = Audio_In_Ctx[Instance].SampleRate;
      mx_config.AudioMode             = SAI_MODESLAVE_RX;
      mx_config.ClockStrobing         = SAI_CLOCKSTROBING_FALLINGEDGE;
      mx_config.MonoStereoMode        = (Device != AUDIO_IN_DEVICE_LINE_IN) ? SAI_MONOMODE : SAI_STEREOMODE;
      mx_config.DataSize              = SAI_DATASIZE_16;
      mx_config.FrameLength           = 32;
      mx_config.ActiveFrameLength     = 16;
      mx_config.OutputDrive           = SAI_OUTPUTDRIVE_ENABLE;
      mx_config.Synchro               = SAI_SYNCHRONOUS;
      mx_config.SynchroExt            = SAI_SYNCEXT_DISABLE;
      mx_config.SlotActive            = (Device != AUDIO_IN_DEVICE_LINE_IN) ? SAI_SLOTACTIVE_0 :
                                        (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

      if (MX_SAI1_Init(&haudio_in_sai, &mx_config) != HAL_OK)
      {
        /* Return BSP_ERROR_PERIPH_FAILURE when operations are not correctly done */
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        WM8904_Init_t codec_init;
        uint32_t      wm8904InputDevice;

        if (Device == AUDIO_IN_DEVICE_ANALOG_MIC)
        {
          wm8904InputDevice = WM8904_IN_MIC1;
        }
        else if (Device == AUDIO_IN_DEVICE_LINE_IN)
        {
          wm8904InputDevice = WM8904_IN_LINE2;
        }
        else
        {
          wm8904InputDevice = WM8904_IN_DIGITAL_MIC2;
        }

        /* Fill codec_init structure */
        codec_init.OutputDevice = (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET) ? WM8904_OUT_NONE :
                                  WM8904_OUT_HEADPHONE;
        codec_init.Frequency    = Audio_In_Ctx[Instance].SampleRate;
        codec_init.Resolution   = WM8904_RESOLUTION_16B; /* Not used */
        codec_init.Volume       = Audio_In_Ctx[Instance].Volume;
        codec_init.InputDevice  = wm8904InputDevice;

        /* Initialize the codec internal registers */
        if (Audio_Drv->Init(Audio_CompObj, &codec_init) < 0)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          /* Update audio in context device */
          Audio_In_Ctx[Instance].Device = Device;
        }
      }
    }
    else
    {
      /* Nothing to do because there is only one device for each instance or device is the same as already used */
    }
  }
  return ret;
}

/**
  * @brief  Get Audio In device
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  Device    The audio input device used
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* Return audio Input Device */
    *Device = Audio_In_Ctx[Instance].Device;
  }
  return ret;
}

/**
  * @brief  Set Audio In frequency
  * @param  Instance    Audio IN instanc. It can be 0 for SAI
  * @param  SampleRate  Input frequency to be set
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t  SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    ret = BSP_ERROR_BUSY;
  }
  /* Check if playback on instance 0 is on going and corresponding sample rate */
  else if ((Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET) &&
           (Audio_Out_Ctx[0].SampleRate != SampleRate))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check if sample rate is modified */
  else if (Audio_In_Ctx[Instance].SampleRate == SampleRate)
  {
    /* Nothing to do */
  }
  else
  {
    /* Update SAI1 clock config */
    haudio_in_sai.Init.AudioFrequency = SampleRate;
    haudio_out_sai.Init.AudioFrequency = SampleRate;
    if (MX_SAI1_ClockConfig(&haudio_in_sai, SampleRate) != HAL_OK)
    {
      ret = BSP_ERROR_CLOCK_FAILURE;
    }
    /* Re-initialize SAI1 with new sample rate */
    else if (HAL_SAI_Init(&haudio_in_sai) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Store new sample rate on audio in context */
    else
    {
      Audio_In_Ctx[Instance].SampleRate = SampleRate;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In frequency
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  SampleRate  Audio Input frequency to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* Return audio in frequency */
    *SampleRate = Audio_In_Ctx[Instance].SampleRate;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set Audio In Resolution
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  BitsPerSample  Input resolution to be set
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (BitsPerSample != AUDIO_RESOLUTION_16B)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* nothing to do because only 16 bits or 8 bits per sample are supported */
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In Resolution
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  BitsPerSample  Input resolution to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    ret = BSP_ERROR_BUSY;
  }
  /* Get the current bits per sample of audio in stream */
  else
  {
    *BitsPerSample = Audio_In_Ctx[Instance].BitsPerSample;
  }
  return ret;
}

/**
  * @brief  Set Audio In Channel number
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  ChannelNbr  Channel number to be used
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Audio_In_Ctx[Instance].Device == AUDIO_IN_DEVICE_ANALOG_MIC) && (ChannelNbr != 1U))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Audio_In_Ctx[Instance].Device == AUDIO_IN_DEVICE_LINE_IN) && (ChannelNbr != 2U))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Audio_In_Ctx[Instance].Device == AUDIO_IN_DEVICE_DIGITAL_MIC) && (ChannelNbr != 1U))
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because channels are already configurated and can't be modified */
  }
  return ret;
}

/**
  * @brief  Get Audio In Channel number
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  ChannelNbr  Channel number to be used
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    ret = BSP_ERROR_BUSY;
  }
  /* Get the current channels number of audio in stream */
  else
  {
    *ChannelNbr = Audio_In_Ctx[Instance].ChannelsNbr;
  }
  return ret;
}

/**
  * @brief  Set the current audio in volume level.
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  Volume    Volume level to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t ret;

  if ((Instance >= AUDIO_IN_INSTANCES_NBR) || (Volume > 100U))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Feature not supported */
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the current audio in volume level.
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  Volume    Volume level to be returned
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t ret;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Feature not supported */
  else
  {
    *Volume = 0U;
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In device
  * @param  Instance  AUDIO IN Instance. It can be 0 for SAI
  * @param  State     Audio Out state
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Input State to be returned */
    *State = Audio_In_Ctx[Instance].State;
  }
  return ret;
}

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register default BSP AUDIO IN msp callbacks.
  * @param  Instance AUDIO IN Instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPINIT_CB_ID, SAI_MspInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPDEINIT_CB_ID, SAI_MspDeInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Audio_In_Ctx[Instance].IsMspCallbacksValid = 1U;
    }
  }
  /* Return BSP status */
  return status;
}

/**
  * @brief  Register BSP AUDIO IN msp callbacks.
  * @param  Instance AUDIO IN Instance.
  * @param  CallBacks Pointer to MspInit/MspDeInit callback functions.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_IN_Cb_t *CallBacks)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPINIT_CB_ID, CallBacks->pMspSaiInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPDEINIT_CB_ID, CallBacks->pMspSaiDeInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Audio_In_Ctx[Instance].IsMspCallbacksValid = 1U;
    }
  }
  /* Return BSP status */
  return status;
}
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS == 1 */

/**
  * @brief  This function handles Audio Out DMA interrupt requests.
  * @param  Instance Audio IN instance: It can be 0 for SAI
  * @param  InputDevice Can be:
  *         - AUDIO_IN_DEVICE_ANALOG_MIC
  *         - AUDIO_IN_DEVICE_LINE_IN
  *         - AUDIO_IN_DEVICE_DIGITAL_MIC
  * @retval None
  */
void BSP_AUDIO_IN_IRQHandler(uint32_t Instance, uint32_t InputDevice)
{
  if ((Instance == 0U) &&
      ((InputDevice == AUDIO_IN_DEVICE_ANALOG_MIC) || (InputDevice == AUDIO_IN_DEVICE_LINE_IN)))
  {
    HAL_DMA_IRQHandler(haudio_in_sai.hdmarx);
  }
}

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0) || !defined (USE_HAL_SAI_REGISTER_CALLBACKS)
/**
  * @brief  Half reception complete callback.
  * @param  hsai   SAI handle.
  * @retval None
  */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Call the record update function to get the first half */
  BSP_AUDIO_IN_HalfTransfer_CallBack(0);
}

/**
  * @brief  Reception complete callback.
  * @param  hsai   SAI handle.
  * @retval None
  */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Call the record update function to get the second half */
  BSP_AUDIO_IN_TransferComplete_CallBack(0);
}
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) || !defined (USE_HAL_SAI_REGISTER_CALLBACKS) */

/**
  * @brief  User callback when record buffer is filled.
  * @retval None
  */
__weak void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* This function should be implemented by the user application.
     It is called into this driver when the current buffer is filled
     to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @retval None
  */
__weak void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* This function should be implemented by the user application.
     It is called into this driver when the current buffer is filled
     to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Audio IN Error callback function.
  * @retval None
  */
__weak void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* This function is called when an Interrupt due to transfer error on or peripheral
     error occurs. */
}

/**
  * @}
  */

/** @defgroup STM32WBA65I_DK1_AUDIO_IN_Private_Functions AUDIO IN Private Functions
  * @{
  */

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
/**
  * @brief  Half reception complete callback.
  * @param  hsai   SAI handle.
  * @retval None
  */
static void SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Call the record update function to get the first half */
  BSP_AUDIO_IN_HalfTransfer_CallBack(0);
}

/**
  * @brief  Reception complete callback.
  * @param  hsai   SAI handle.
  * @retval None
  */
static void SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  /* Call the record update function to get the second half */
  BSP_AUDIO_IN_TransferComplete_CallBack(0);
}
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */

/** @defgroup STM32WBA65I_DK1_AUDIO_Private_Functions AUDIO Private Functions
  * @{
  */

/**
  * @brief  Probe the WM8904 audio codec.
  * @retval BSP status.
  */
static int32_t WM8904_Probe(void)
{
  int32_t                  status = BSP_ERROR_NONE;
  WM8904_IO_t              IOCtx;
  uint32_t                 wm8904_id;
  static WM8904_Object_t   WM8904Obj;

  /* Configure the audio driver */
  IOCtx.Address     = AUDIO_I2C_ADDRESS;
  IOCtx.Init        = BSP_I2C1_Init;
  IOCtx.DeInit      = BSP_I2C1_DeInit;
  IOCtx.ReadReg     = BSP_I2C1_ReadReg;
  IOCtx.WriteReg    = BSP_I2C1_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  if (WM8904_RegisterBusIO(&WM8904Obj, &IOCtx) != WM8904_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  else if (WM8904_ReadID(&WM8904Obj, &wm8904_id) != WM8904_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if ((wm8904_id & WM8904_ID_MASK) != WM8904_ID)
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Audio_Drv = (AUDIO_Drv_t *) &WM8904_Driver;
    Audio_CompObj = &WM8904Obj;
  }

  return status;
}

/**
  * @brief  Initialize BSP_AUDIO_OUT MSP.
  * @param  hsai  SAI handle
  * @retval None
  */
static void SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  gpio_init_structure;
  static DMA_NodeTypeDef TxNode, RxNode;
  static DMA_NodeConfTypeDef dmaNodeConfig;

  /* Enable SAI clock */
  AUDIO_OUT_SAI_CLK_ENABLE();

  /* Enable GPIO clock */
  AUDIO_OUT_SAI_MCLK_ENABLE();
  AUDIO_OUT_SAI_SCK_ENABLE();
  AUDIO_OUT_SAI_SD_ENABLE();
  AUDIO_OUT_SAI_FS_ENABLE();
  /* CODEC_SAI pins configuration: FS, SCK, MCK and SD pins ------------------*/
  gpio_init_structure.Pin = AUDIO_OUT_SAI_FS_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = AUDIO_OUT_SAI_FS_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAI_FS_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = AUDIO_OUT_SAI_SCK_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_SAI_SCK_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAI_SCK_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin =  AUDIO_OUT_SAI_SD_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_SAI_SD_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAI_SD_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = AUDIO_OUT_SAI_MCLK_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_SAI_MCLK_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAI_MCLK_GPIO_PORT, &gpio_init_structure);

  if (hsai->Instance == AUDIO_OUT_SAI)
  {
    /* Configure the hDmaSaiTx handle parameters */
    AUDIO_OUT_SAI_DMA_CLK_ENABLE();

    if (SAITxQueue.Head != NULL)
    {
      /* Reset the DMA Channel configuration*/
      if (HAL_DMAEx_List_DeInit(&hDmaSaiTx) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }

      /* Reset TxQueue */
      if (HAL_DMAEx_List_ResetQ(&SAITxQueue) != HAL_OK)
      {
        BSP_AUDIO_OUT_Error_CallBack(0);
      }
    }

    /* DMA for Tx */
    /* Set node type */
    dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
    /* Set common node parameters */
    dmaNodeConfig.Init.Request                        = AUDIO_OUT_SAI_DMA_REQUEST;
    dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
    dmaNodeConfig.Init.Direction                      = DMA_MEMORY_TO_PERIPH;
    dmaNodeConfig.Init.SrcInc                         = DMA_SINC_INCREMENTED;
    dmaNodeConfig.Init.DestInc                        = DMA_DINC_FIXED;
    if (Audio_Out_Ctx[0].BitsPerSample == AUDIO_RESOLUTION_16B)
    {
      dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_HALFWORD;
      dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_HALFWORD;
    }
    else /* AUDIO_RESOLUTION_24b */
    {
      dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_WORD;
      dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_WORD;
    }
    dmaNodeConfig.Init.SrcBurstLength                 = 1;
    dmaNodeConfig.Init.DestBurstLength                = 1;
    dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
    dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
    dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    /* Set node data handling parameters */
    dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
    dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    /* Set node trigger parameters */
    dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

    /* Build NodeTx */
    if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &TxNode) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* Insert NodeTx to SAI queue */
    if (HAL_DMAEx_List_InsertNode_Tail(&SAITxQueue, &TxNode) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* Set queue circular mode for sai queue */
    if (HAL_DMAEx_List_SetCircularMode(&SAITxQueue) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* DMA for Tx */
    hDmaSaiTx.Instance                         = AUDIO_OUT_SAI_DMA_CHANNEL;

    hDmaSaiTx.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
    hDmaSaiTx.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
    hDmaSaiTx.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
    hDmaSaiTx.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
    hDmaSaiTx.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

    /* DMA linked list init */
    if (HAL_DMAEx_List_Init(&hDmaSaiTx) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* Link SAI queue to DMA channel */
    if (HAL_DMAEx_List_LinkQ(&hDmaSaiTx, &SAITxQueue) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmatx, hDmaSaiTx);

    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_OUT_SAI_DMA_IRQ, BSP_AUDIO_OUT_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(AUDIO_OUT_SAI_DMA_IRQ);

  }

  /* Audio In Msp initialization */
  if (hsai->Instance == AUDIO_IN_SAI)
  {
    /* Enable SAI clock */
    AUDIO_IN_SAI_CLK_ENABLE();

    /* Enable SD GPIO clock */
    AUDIO_IN_SAI_SD_ENABLE();
    /* CODEC_SAI pin configuration: SD pin */
    gpio_init_structure.Pin = AUDIO_IN_SAI_SD_PIN;
    gpio_init_structure.Mode = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Alternate = AUDIO_IN_SAI_AF;
    HAL_GPIO_Init(AUDIO_IN_SAI_SD_GPIO_PORT, &gpio_init_structure);

    /* Enable the DMA clock */
    AUDIO_IN_SAI_DMA_CLK_ENABLE();

    if (SAIRxQueue.Head != NULL)
    {
      /* Reset the DMA Channel configuration*/
      if (HAL_DMAEx_List_DeInit(&hDmaSaiRx) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Reset RxQueue */
      if (HAL_DMAEx_List_ResetQ(&SAIRxQueue) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }
    }

    /* DMA for Rx */
    /* Set node type */
    dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
    /* Set common node parameters */
    dmaNodeConfig.Init.Request                        = AUDIO_IN_SAI_DMA_REQUEST;
    dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
    dmaNodeConfig.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
    dmaNodeConfig.Init.SrcInc                         = DMA_SINC_FIXED;
    dmaNodeConfig.Init.DestInc                        = DMA_DINC_INCREMENTED;
    if (Audio_In_Ctx[0].BitsPerSample == AUDIO_RESOLUTION_16B)
    {
      dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_HALFWORD;
      dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_HALFWORD;
    }
    else /* AUDIO_RESOLUTION_24b */
    {
      dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_WORD;
      dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_WORD;
    }
    dmaNodeConfig.Init.SrcBurstLength                 = 1;
    dmaNodeConfig.Init.DestBurstLength                = 1;
    dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
    dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
    dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    /* Set node data handling parameters */
    dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
    dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    /* Set node trigger parameters */
    dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

    /* Build NodeRx */
    if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &RxNode) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Insert NodeTx to SAI queue */
    if (HAL_DMAEx_List_InsertNode_Tail(&SAIRxQueue, &RxNode) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Set queue circular mode for sai queue */
    if (HAL_DMAEx_List_SetCircularMode(&SAIRxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* DMA for Rx */
    hDmaSaiRx.Instance                         = AUDIO_IN_SAI_DMA_CHANNEL;

    hDmaSaiRx.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
    hDmaSaiRx.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
    hDmaSaiRx.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
    hDmaSaiRx.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
    hDmaSaiRx.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

    /* DMA linked list init */
    if (HAL_DMAEx_List_Init(&hDmaSaiRx) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Link SAI queue to DMA channel */
    if (HAL_DMAEx_List_LinkQ(&hDmaSaiRx, &SAIRxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmarx, hDmaSaiRx);

    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_IN_SAI_DMA_IRQ, BSP_AUDIO_IN_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(AUDIO_IN_SAI_DMA_IRQ);
  }
}

/**
  * @brief  Deinitializes SAI MSP.
  * @param  hsai  SAI handle
  * @retval HAL status
  */
static void SAI_MspDeInit(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == AUDIO_OUT_SAI)
  {
    /* Disable SAI DMA Channel IRQ */
    HAL_NVIC_DisableIRQ(AUDIO_OUT_SAI_DMA_IRQ);

    /* Reset the DMA Channel configuration*/
    if (HAL_DMAEx_List_DeInit(&hDmaSaiTx) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* Reset TxQueue */
    if (HAL_DMAEx_List_ResetQ(&SAITxQueue) != HAL_OK)
    {
      BSP_AUDIO_OUT_Error_CallBack(0);
    }

    /* De-initialize FS, SCK, MCK and SD pins */
    HAL_GPIO_DeInit(AUDIO_OUT_SAI_FS_GPIO_PORT, AUDIO_OUT_SAI_FS_PIN);
    HAL_GPIO_DeInit(AUDIO_OUT_SAI_SCK_GPIO_PORT, AUDIO_OUT_SAI_SCK_PIN);
    HAL_GPIO_DeInit(AUDIO_OUT_SAI_SD_GPIO_PORT, AUDIO_OUT_SAI_SD_PIN);
    HAL_GPIO_DeInit(AUDIO_OUT_SAI_MCLK_GPIO_PORT, AUDIO_OUT_SAI_MCLK_PIN);
  }
  if (hsai->Instance == AUDIO_IN_SAI)
  {
    /* Disable SAI DMA Channel IRQ */
    HAL_NVIC_DisableIRQ(AUDIO_IN_SAI_DMA_IRQ);

    /* Reset the DMA Channel configuration*/
    if (HAL_DMAEx_List_DeInit(&hDmaSaiRx) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Reset RxQueue */
    if (HAL_DMAEx_List_ResetQ(&SAIRxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* De-initialize SD pin */
    HAL_GPIO_DeInit(AUDIO_IN_SAI_SD_GPIO_PORT, AUDIO_IN_SAI_SD_PIN);

    /* Disable SAI clock */
    AUDIO_IN_SAI_CLK_DISABLE();
  }
}


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


