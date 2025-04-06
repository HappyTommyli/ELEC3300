#ifndef _MGC3X30_H
#define _MGC3X30_H

#include "stm32f1xx_hal.h"
#include "main.h"



typedef struct
{
  uint32_t gestureInfo;  /**< Gestures information */
  uint32_t touchInfo;    /**< Touch information */
  uint32_t airWheelInfo; /**< airWheel information */
  uint16_t xPosition;    /**< X-axis position information */
  uint16_t yPosition;    /**< Y-axis position information */
  uint16_t zPosition;    /**< Z-axis position information */
} sInfo_t;

/**
 * @enum eErrorCode_t
 * @brief The state of information interaction in communication
 */
typedef enum
{
  eNoError = 0x0000,             /**< OK */
  eUnknownCommand = 0x0001,      /**< Message ID is unknown */
  eWrongParameterValue = 0x0014, /**< The value of the Argument/Parameter of a RuntimeParameter command is out of the valid range */
  eUnknownParameterID = 0x0015,  /**< The MessageID or RuntimeParameterID is unknown or out of the valid range */
  eWakeupHappend = 0x001A,       /**< A wake-up by Host was detected */
} eErrorCode_t;

/**
 * @enum eGestureInfo_t
 * @brief Gesture Info
 */
typedef enum
{
  eNoGesture = 0,          /**< No gesture */
  eGarbageModel,           /**< Garbage model */
  eFilckR,                 /**< Flick Left to Right */
  eFilckL,                 /**< Flick Right to Left */
  eFilckU,                 /**< Flick Down to Up */
  eFilckD,                 /**< Flick Up to Down */
  eCircleClockwise,        /**< Circle clockwise (only active if AirWheel disabled) */
  eCircleCounterclockwise, /**< Circle counterclockwise (only active if AirWheel disabled) */
} eGestureInfo_t;



/**
 * @enum eTouchInfo_t
 * @brief Touch Info
 */
typedef enum
{
  eTouchDown = 1,           /**< Touch Down electrode */
  eTouchLeft = 2,           /**< Touch Left electrode */
  eTouchUp = 4,             /**< Touch Up electrode */
  eTouchRight = 8,          /**< Touch Right electrode */
  eTouchCenter = 16,        /**< Touch Center electrode */
  eTapDown = 32,            /**< Tap Down electrode*/
  eTapLeft = 64,            /**< Tap Left electrode */
  eTapUp = 128,             /**< Tap Up electrode */
  eTapRight = 256,          /**< Tap Right electrode */
  eTapCenter = 512,         /**< Tap Center electrode */
  eDoubleTapDown = 1024,    /**< Double Tap Down electrode*/
  eDoubleTapLeft = 2048,    /**< Double Tap Left electrode */
  eDoubleTapUp = 4096,      /**< Double Tap Up electrode */
  eDoubleTapRight = 8192,   /**< Double Tap Right electrode */
  eDoubleTapCenter = 16384, /**< Double Tap Center electrode */
} eTouchInfo_t;




  void MGC3X30_task();

  void handle_data(sInfo_t *info,uint8_t *pbuf ,uint16_t *nowTimeStamp,uint16_t *nowTouch);

  void reset_info(sInfo_t *info);
  /**
   * @fn tsInput
   * @brief set the ts_pin of the master computer to input mode(TS:transfer status line)
   */
  void tsInput(void);

  /**
   * @fn tsOutput
   * @brief set the ts_pin of the master computer to output mode(TS:transfer status line)
   */
  void tsOutput(void);

  /**
   * @fn tsWrite
   * @brief set the ts_pin of the master computer to output status(TS:transfer status line)
   * @param mode output status HIGH/LOW
   * @n HIGH  Move the Transfer Status line higher
   * @n LOW   Lower the Transfer Status line
   */
  void tsWrite(uint8_t mode);

  /**
   * @fn tsRead
   * @brief get the status of transfer status line
   * @return  return the status of transfer status line
   * @retval HIGH
   * @retval LOW
   */
  uint8_t tsRead();

/**
 * @fn begin
 * @brief initialization function
 * @return bool,returns the initialization status
 * @retval true Initialization succeeded
 * @retval fasle Initialization  failed
 */
uint8_t begin();

/**
 * @fn reset
 * @brief reset the sensor
 */
void reset();

/**
 * @fn sensorDataRecv
 * @brief get the sensor data
 */
void sensorDataRecv(sInfo_t *info,uint16_t *nowTimeStamp,uint16_t *nowTouch);

  /**
   * @fn disableTouchDetection
   * @brief turn off touch detection function
   * @return Result of disabling touch detection function
   * @retval -1  setup fails
   * @retval  0  setup succeeds
   */
  int8_t disableTouchDetection();

  /**
   * @fn enableTouchDetection
   * @brief enable touch detection function
   * @return Result of enabling touch detection function
   * @retval -1  setup fails
   * @retval  0  setup succeeds
   */
  int8_t enableTouchDetection();

/**
 * @fn enableGestures
 * @brief enable gesture recognition function
 * @return Result of enabling gesture recognition
 * @retval -1  setup fails
 * @retval  0  setup succeeds
 */
int8_t enableGestures();

int8_t enableAirWheel();
/**
 * @fn disableGestures
 * @brief turn off gesture recognition function
 * @return Result of disabling gesture recognition
 * @retval -1  setup fails
 * @retval  0  setup succeeds
 */
int8_t disableGestures();

/**
 * @fn getTouchInfo
 * @brief get touch information
 * @return touch information
 * @retval eDoubleTapCenter  Double Tap Center electrode
 * @retval eDoubleTapRight   Double Tap Right electrode
 * @retval eDoubleTapUp      Double Tap Up electrode
 * @retval eDoubleTapLeft    Double Tap Left electrode
 * @retval eDoubleTapDown    Double Tap Down electrode
 * @retval eTapCenter        Tap Center electrode
 * @retval eTapRight         Tap Right electrode
 * @retval eTapUp            Tap Up electrode
 * @retval eTapLeft          Tap Left electrode
 * @retval eTapDown          Tap Down electrode
 * @retval eTouchCenter      Touch Center electrode
 * @retval eTouchRight       Touch Right electrode
 * @retval eTouchUp          Touch Up electrode
 * @retval eTouchLeft        Touch Left electrode
 * @retval eTouchDown        Touch Down electrode
 */
uint16_t getTouchInfo(sInfo_t *info, uint16_t *lastTimeStamp,uint16_t *nowTimeStamp,uint16_t *lastTouch,uint16_t *nowTouch);



/**
 * @fn getGestureInfo
 * @brief get gesture information
 * @return gesture information
 * @retval eFilckR                   Flick Left to Right
 * @retval eFilckL                   Flick Right to Left
 * @retval eFilckU                   Flick Down to Up
 * @retval eFilckD                   Flick Up to Down
 * @retval eCircleClockwise          Circle clockwise (only active if AirWheel disabled)
 * @retval eCircleCounterclockwise   Circle counterclockwise (only active if AirWheel disabled)
 */
uint8_t getGestureInfo(sInfo_t *info);

/**
 * @fn read
 * @brief get the sensor IIC data
 * @param pBuf write the store and buffer of the data
 * @param size data length to be written
 * @return return the actually written length, returning 0 means reading data fails
 */
uint8_t read(uint8_t *pBuf, size_t size);

  /**
   * @fn lockDataOutput
   * @brief lock the output data format of the sensor
   * @return Result of setting
   * @retval -1  setup fails
   * @retval  0  setup succeeds
   */
  int8_t lockDataOutput(void);

  /**
   * @fn enableDataOutput
   * @brief set the output data format of the sensor
   * @return Result of setting
   * @retval -1  setup fails
   * @retval  0  setup succeeds
   */
  int8_t enableDataOutput(void);

/**
 * @fn setRuntimeparameter
 * @brief write the sensor IIC data
 * @param pBuf write the store and buffer of the data
 * @param size data length to be written
 * @return return the actually written length
 */
uint8_t setRuntimeparameter(uint8_t *pBuf, size_t size);

#endif
