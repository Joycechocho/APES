#include "../inc/main.h"
#include "../inc/loggerThread.h"
#include "../inc/temperatureThread.h"
#include "../inc/lightThread.h"

int main()
{ 
  /* Create threads */
  if(pthread_create(&loggerThread,NULL,&LoggerThread,NULL) != 0)
  {
    printf("Cannot create logger thread successfully");
  }else if (pthread_create(&temperatureThread,NULL,&TemperatureThread,NULL) != 0)
  {
    printf("Cannot create temperature thread successfully");
  }else if (pthread_create(&lightThread,NULL,&LightThread,NULL) != 0)
  {
    printf("Cannot create light thread successfully");
  }

  /* Join threads */
  if(pthread_join(loggerThread, NULL) != 0)
  {
    printf("Cannot join logger thread successfully");
  }else if(pthread_join(temperatureThread, NULL) != 0)
  {
    printf("Cannot join temperature thread successfully");
  }else if(pthread_join(lightThread, NULL) != 0)
  {
    printf("Cannot join light thread successfully");
  }
}
