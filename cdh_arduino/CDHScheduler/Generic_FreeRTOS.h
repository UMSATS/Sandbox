#ifndef GENERIC_FREERTOS_H
#define GENERIC_FREERTOS_H
// UMSATS - CDH Scheduler 
//
// File Description:
//   Generic include file that is compatible between Arduino Due and Arduino Uno.
//
// History
// 2018-05-12 by Tamkin Rahman
// - Created.


#ifndef __arm__
  #include <Arduino_FreeRTOS.h>
#else
  // For SAM3X8E (i.e. Arduino Due) chips, the stock Arduino library (v9.0.0) isn't compatible.
  // This library can be obtained from: https://github.com/greiman/FreeRTOS-Arduino
  #include <FreeRTOS_ARM.h>
#endif  // ifndef __arm__

#endif // GENERIC_FREERTOS_H
