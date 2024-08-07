/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcdHandler.h"
#include "ctpHandler.h"
#include "layout.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
QueueHandle_t adcMailBox;
uint8_t adcMailBoxBuffer[4];
StaticQueue_t adcMailBoxControlBlock;
/* USER CODE END Variables */
osThreadId screenRefreshHandle;
uint32_t screenRefreshTaskBuffer[ 512 ];
osStaticThreadDef_t screenRefreshTaskControlBlock;
osThreadId initHandle;
osThreadId TouchProcessHandle;
uint32_t TouchProcessBuffer[ 128 ];
osStaticThreadDef_t TouchProcessControlBlock;
osTimerId feedWDGTimerHandle;
osStaticTimerDef_t feedWDGTimerControlBlock;
osSemaphoreId spi2DmaSemaphoreHandle;
osStaticSemaphoreDef_t spi2DmaSemaphoreControlBlock;
osSemaphoreId lcdBusySemaphoreHandle;
osStaticSemaphoreDef_t lcdBusySemaphoreControlBlock;
osSemaphoreId ctpBusySemaphoreHandle;
osStaticSemaphoreDef_t ctpBusySemaphoreControlBlock;
osSemaphoreId TouchInitSemaphoreHandle;
osStaticSemaphoreDef_t TouchInitSemaphoreControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void screenRefreshTask(void const * argument);
void initTask(void const * argument);
void TouchProcessTask(void const * argument);
void feedWDGCallback(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of spi2DmaSemaphore */
  osSemaphoreStaticDef(spi2DmaSemaphore, &spi2DmaSemaphoreControlBlock);
  spi2DmaSemaphoreHandle = osSemaphoreCreate(osSemaphore(spi2DmaSemaphore), 1);

  /* definition and creation of lcdBusySemaphore */
  osSemaphoreStaticDef(lcdBusySemaphore, &lcdBusySemaphoreControlBlock);
  lcdBusySemaphoreHandle = osSemaphoreCreate(osSemaphore(lcdBusySemaphore), 1);

  /* definition and creation of ctpBusySemaphore */
  osSemaphoreStaticDef(ctpBusySemaphore, &ctpBusySemaphoreControlBlock);
  ctpBusySemaphoreHandle = osSemaphoreCreate(osSemaphore(ctpBusySemaphore), 1);

  /* definition and creation of TouchInitSemaphore */
  osSemaphoreStaticDef(TouchInitSemaphore, &TouchInitSemaphoreControlBlock);
  TouchInitSemaphoreHandle = osSemaphoreCreate(osSemaphore(TouchInitSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of feedWDGTimer */
  osTimerStaticDef(feedWDGTimer, feedWDGCallback, &feedWDGTimerControlBlock);
  feedWDGTimerHandle = osTimerCreate(osTimer(feedWDGTimer), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  osTimerStart(feedWDGTimerHandle, 400);
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    adcMailBox = xQueueCreateStatic(1, sizeof(uint16_t*), adcMailBoxBuffer, &adcMailBoxControlBlock);
  /* add queues, ... */    
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of screenRefresh */
  osThreadStaticDef(screenRefresh, screenRefreshTask, osPriorityHigh, 0, 512, screenRefreshTaskBuffer, &screenRefreshTaskControlBlock);
  screenRefreshHandle = osThreadCreate(osThread(screenRefresh), NULL);

  /* definition and creation of init */
  osThreadDef(init, initTask, osPriorityRealtime, 0, 128);
  initHandle = osThreadCreate(osThread(init), NULL);

  /* definition and creation of TouchProcess */
  osThreadStaticDef(TouchProcess, TouchProcessTask, osPriorityRealtime, 0, 128, TouchProcessBuffer, &TouchProcessControlBlock);
  TouchProcessHandle = osThreadCreate(osThread(TouchProcess), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_screenRefreshTask */
/**
  * @brief  Function implementing the screenRefresh thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_screenRefreshTask */
void screenRefreshTask(void const * argument)
{
  /* USER CODE BEGIN screenRefreshTask */
    int displayRow = 32;
    int displayCol = 32;
    uint8_t head[2] = { 0xff,0xff };
    uint16_t* adcBuffer = NULL;

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    GridView* gridView = GridViewConstruct(
        NULL, 10, 20, 240, 240, 
        displayRow, displayCol, 0, 0, adcBuffer);

  /* Infinite loop */
  for(;;)
  {
      xQueuePeek(adcMailBox, &adcBuffer, 0);
      if (NULL == adcBuffer)
      {
          osDelay(100);
          continue;
      }
      gridView->data = adcBuffer;
      gridView->interface.draw(gridView);
      adcBuffer = NULL;
  }
  /* USER CODE END screenRefreshTask */
}

/* USER CODE BEGIN Header_initTask */
/**
* @brief Function implementing the init thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_initTask */
void initTask(void const * argument)
{
  /* USER CODE BEGIN initTask */
    LL_TIM_EnableIT_UPDATE(TIM4);
    LL_TIM_EnableCounter(TIM4);
    LL_TIM_GenerateEvent_UPDATE(TIM4);

    osSemaphoreRelease(*(SPI2Instance.dmaSemaphore));
    lcdInstance.init(&lcdInstance, &st7789vInstance, &lcdBusySemaphoreHandle, 100);
    xTaskNotifyGive(screenRefreshHandle);

    ctpHandler.init(&ctpHandler, &cst816tInstance, &ctpBusySemaphoreHandle);

    osThreadTerminate(NULL);
  /* USER CODE END initTask */
}

/* USER CODE BEGIN Header_TouchProcessTask */
/**
* @brief Function implementing the TouchProcess thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TouchProcessTask */
void TouchProcessTask(void const * argument)
{
  /* USER CODE BEGIN TouchProcessTask */
    uint16_t x, y;
  /* Infinite loop */
  for(;;)
  {
      osSemaphoreWait(TouchInitSemaphoreHandle, osWaitForever);
      GESTURE gesture = ctpHandler.getGesture(&ctpHandler);
      ctpHandler.getPos(&ctpHandler, &x, &y);
      printf("Touch detected. x: %d, y: %d, gesture: %d\n", x, y, gesture);
    osDelay(1);
  }
  /* USER CODE END TouchProcessTask */
}

/* feedWDGCallback function */
void feedWDGCallback(void const * argument)
{
  /* USER CODE BEGIN feedWDGCallback */
    LL_IWDG_ReloadCounter(IWDG);
  /* USER CODE END feedWDGCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

