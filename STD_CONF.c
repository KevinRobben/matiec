/*******************************************/
/*     FILE GENERATED BY iec2c             */
/* Editing this file is not recommended... */
/*******************************************/

#include "iec_std_lib.h"

#include "accessor.h"

#include "POUS.h"

// CONFIGURATION STD_CONF

void STD_RESSOURCE_init__(void);

void config_init__(void) {
  BOOL retain;
  retain = 0;
  
  STD_RESSOURCE_init__();
}

void STD_RESSOURCE_run__(unsigned long tick);

void config_run__(unsigned long tick) {
  STD_RESSOURCE_run__(tick);
}
unsigned long long common_ticktime__ = 50000000ULL * 1ULL; /*ns*/
unsigned long greatest_tick_count__ = (unsigned long)0UL; /*tick*/
