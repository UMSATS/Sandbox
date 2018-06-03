// UMSATS - CDH Scheduler 
//
// File Description:
//  Main entry-point for the scheduler. This file should contain the bare minimum w.r.t. working code. 
//  Should only contain Arduino specific code.
//
// The due CAN bus library is used for this file: https://github.com/collin80/due_can
// - Requires this library as well: https://github.com/collin80/can_common
//
// History
// 2018-05-12 by Tamkin Rahman
// - Updated with CAN monitor.

// -----------------------------------------------------------------------------------------------
// ----------------------- INCLUDES --------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
#include "Generic_FreeRTOS.h"

#include <due_can.h>
  
extern "C" 
{
  #include "CANManager.h"
  #include "PayloadData.h"
  #include "SerialPrint.h"
}
// -----------------------------------------------------------------------------------------------
// ----------------------- GLOBALS ---------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// Use a lock to prevent multiple tasks from hogging the serial bus.
SemaphoreHandle_t printLock;

SemaphoreHandle_t canRxQueueLock;
SemaphoreHandle_t canTxQueueLock;

SemaphoreHandle_t payloadQueueLock;
SemaphoreHandle_t taskQueueLock;

// -----------------------------------------------------------------------------------------------
// ----------------------- ARDUINO FUNCTIONS -----------------------------------------------------
// -----------------------------------------------------------------------------------------------
// The setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(9600);

  printLock = xSemaphoreCreateMutex();
  canRxQueueLock = xSemaphoreCreateMutex();
  canTxQueueLock = xSemaphoreCreateMutex();
  payloadQueueLock = xSemaphoreCreateMutex();
  taskQueueLock = xSemaphoreCreateMutex();

  if (   printLock        != NULL
      && canRxQueueLock   != NULL
      && canTxQueueLock   != NULL
      && payloadQueueLock != NULL
      && taskQueueLock    != NULL                                                                                                                                             
     )
  {
    // Initialize CAN0 and CAN1, Set the proper baud rates here
    Can0.begin(CAN_BPS_250K);
  
    //By default there are 7 mailboxes for each device that are RX boxes
    //This sets each mailbox to have an open filter that will accept extended
    //or standard frames
    int filter;
    
    //extended
    for (filter = 0; filter < 3; filter++) {
      Can0.setRXFilter(filter, 0, 0, true);
    }  
    //standard
    for (filter = 3; filter < 7; filter++) {
      Can0.setRXFilter(filter, 0, 0, false);
    }  
    
    Can0.watchFor();
  
    startPeriodicTasks();
    SerialPrint("Starting Scheduler.\n");
  
    // Start FreeRTOS
    vTaskStartScheduler();
  }
  else
  {
    Serial.println("Error, 1 or more locks are NULL.");
  }
  Serial.println("Insufficent RAM.\n");
  while(1){};
}

void loop()
{
  // Empty. Things are done in Tasks.
}

// -------------------------------------------------------------------------------------------
// ----------------------- FUNCTIONS ---------------------------------------------------------
// -------------------------------------------------------------------------------------------
int SerialRead(int default_value) 
{
  int result = default_value;
  int received = 0x00;
  CAN_Message message;
  
  message.id = POWER_ID;
  message.length = 8;
  message.data.bytes[0] = 0x50;
  message.data.bytes[1] = 0x00;
  message.data.bytes[2] = 0x00;
  message.data.bytes[3] = 0x00;
  
  while( xSemaphoreTake( printLock, portMAX_DELAY ) != pdTRUE ){}

  if (Serial.available() > 0)
  {
    received = Serial.read();
    
    if (received == 'A') 
    {
      result = 10;
    } 
    else if (received == 'B') 
    {
      result = 90;
    }
    else if (received == 'C') 
    {
      AddToTXQueue(&message);
      Serial.println("Added to TX queue");
    }
    else if (received == 'D') 
    {
      message.id = POWER_ID;
      message.length = 8;
      message.data.bytes[0] = 0x50;
      message.data.bytes[1] = 0x00;
      message.data.bytes[2] = 0x00;
      message.data.bytes[3] = 0x00;
      
      AddToRXQueue(&message);
      Serial.println("Added to RX queue");
    }
    else if (received == 'E')
    {
      message.id = PAYLOAD_ID;
      message.length = 8;
      message.data.bytes[0] = 1;  // Well number
      // Reading and reserved bytes not set.
      
      AddToRXQueue(&message);
      Serial.println("Added to RX queue");
    }
    else if (received == 'F')
    {
      message.id = GROUND_STATION;
      message.length = 8;
      message.data.GroundStationData.command = TURN_ON_WELL; // Command.
      message.data.GroundStationData.dataBytes.payLoadCommand.wellNumber = 1; // Well number.

      // Wait 1 second until using this command.
      message.data.GroundStationData.dataBytes.payLoadCommand.secondsUntilCommandLSB[0] = 1;
      message.data.GroundStationData.dataBytes.payLoadCommand.secondsUntilCommandLSB[1] = 0;
      message.data.GroundStationData.dataBytes.payLoadCommand.secondsUntilCommandLSB[2] = 0;
      message.data.GroundStationData.dataBytes.payLoadCommand.secondsUntilCommandLSB[3] = 0;
      
      // Reading and reserved bytes not set.
      
      AddToRXQueue(&message);
      Serial.println("Added to RX queue");
    }    
  }    
  xSemaphoreGive( printLock );
  
  return result;
}

// --------------------------------------------------------------------------------
void SerialPrint(const char * text) 
{
  while( xSemaphoreTake( printLock, portMAX_DELAY ) != pdTRUE ){}
  Serial.print(text); 
  xSemaphoreGive( printLock );
}

// --------------------------------------------------------------------------------
void SerialPrintInt(int text) 
{
  while( xSemaphoreTake( printLock, portMAX_DELAY ) != pdTRUE ){} 
  Serial.print(text); 
  xSemaphoreGive( printLock );
}

// --------------------------------------------------------------------------------
void CANMonitor(void *pvParameters)
{
    UNUSED(pvParameters);

    int ix;
    TickType_t lastWakeTime;

    CAN_FRAME incoming;
    CAN_FRAME outgoing;
    CAN_Message currentTxMessage;
    CAN_Message currentRxMessage;
    
    const TickType_t frequency = pdMS_TO_TICKS( 1 ); // Run every 1 ms.

    lastWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        // SerialPrint("Monitoring CAN bus\n");
        
        while (0 != GetNextCANTXMessage(&currentTxMessage))
        {
          // Transmit the message.
          SerialPrint("Transmitted message with\n    ID: ");
          SerialPrintInt(currentTxMessage.id);
          SerialPrint("\n    Length: ");
          SerialPrintInt(currentTxMessage.length);
          SerialPrint("\n    Bytes : ");
          for (ix = 0; ix < currentTxMessage.length; ix++)
          {
            SerialPrintInt(currentTxMessage.data.bytes[ix]);
            SerialPrint(" ");
          }
          SerialPrint("\n");

          outgoing.id = currentTxMessage.id;
          outgoing.length = currentTxMessage.length;
          outgoing.extended = false;

          for (ix = 0; ix < outgoing.length; ix++)
          {
            outgoing.data.bytes[ix] = currentTxMessage.data.bytes[ix];
          }

          Can0.sendFrame(outgoing);
        }
        
        // Also, listen for messages here, and pass them onto the CAN manager, e.g.
        if (Can0.available() > 0) {
          Can0.read(incoming);

          currentRxMessage.id = incoming.id;
          currentRxMessage.length = incoming.length;

          for(ix = 0; ix < currentRxMessage.length; ix++)
          {
            currentRxMessage.data.bytes[ix] = incoming.data.bytes[ix];
          }

          while (AddToRXQueue(&currentRxMessage) == 0)
          {
            vTaskDelay(0);
          }
        }
        
        vTaskDelayUntil(&lastWakeTime, frequency);
    }
}

