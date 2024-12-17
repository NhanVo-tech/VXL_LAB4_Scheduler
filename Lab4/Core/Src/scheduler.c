/*
 * scheduler.c
 *
 *  Created on: DEC 12, 2024
 *      Author: nhanvo
 */

# include "scheduler.h"
# include "main.h"

int count = 0;
void SCH_Init ( void ) {
	unsigned char i ;
	for ( i = 0; i < SCH_MAX_TASKS ; i ++) {
		SCH_Delete_Task ( i) ;
	}
}

void SCH_Update ( void ) {
	if ( SCH_tasks_G [0]. pTask ) {
		if ( SCH_tasks_G [0]. Delay == 0) {
			SCH_tasks_G [0]. RunMe += 1;
			if ( SCH_tasks_G [0]. Period ) {
				SCH_tasks_G [0]. Delay = SCH_tasks_G [0]. Period ;
			}
		} else {
			SCH_tasks_G [0]. Delay -= 1;
		}
	}
}

unsigned char SCH_Add_Task ( void (* pFunction ) () , unsigned int DELAY , unsigned int PERIOD ){
	unsigned char Index = 0;
	if ( count == 0)
	{
		SCH_tasks_G [0]. pTask = pFunction ;
		SCH_tasks_G [0]. Delay = DELAY ;
		SCH_tasks_G [0]. Period = PERIOD ;
		SCH_tasks_G [0]. RunMe = 0;
		count ++;
	}
	else if ( count > 0)
	{
		while (( Index < count ) && count < SCH_MAX_TASKS )
		{
			if ( SCH_tasks_G [ Index ]. Delay > DELAY )
			{
				for ( int i = count ; i > Index ; i--)
				{
					// Shift tasks
					SCH_tasks_G [i ] = SCH_tasks_G [i-1];
				}
				SCH_tasks_G [ Index ]. pTask = pFunction ;
				SCH_tasks_G [ Index ]. Delay = DELAY ;
				SCH_tasks_G [ Index ]. Period = PERIOD ;
				SCH_tasks_G [ Index ]. RunMe = 0;
				count ++;
				for ( int j = Index + 1; j < count ; j ++) {
					SCH_tasks_G [j ]. Delay -= DELAY ;
				}
				break ;
			}
			else {
				DELAY -= SCH_tasks_G [ Index ]. Delay ;
			}
			Index++;
		}
		if( Index == count && ( count + 1 < SCH_MAX_TASKS ) ) {
			SCH_tasks_G [ Index ]. pTask = pFunction ;
			SCH_tasks_G [ Index ]. Delay = DELAY ;
			SCH_tasks_G [ Index ]. Period = PERIOD ;
			SCH_tasks_G [ Index ]. RunMe = 0;
			count ++;
		}
	}
	return Index ;
}

void SCH_Dispatch_Tasks ( void )
{
	if ( SCH_tasks_G [0]. RunMe > 0) {
		(* SCH_tasks_G [0]. pTask ) () ;
		SCH_tasks_G [0]. RunMe -= 1;
		if ( SCH_tasks_G [0]. Period == 0)
		{
			SCH_Delete_Task (0) ;
		}
		else
		{
			sTask temp = SCH_tasks_G [0];
			SCH_Delete_Task (0) ;
			SCH_Shift_Task (0) ;
			SCH_Add_Task ( temp . pTask , temp . Delay , temp . Period ) ;
		}
	}
}

unsigned char SCH_Delete_Task ( const int TASK_INDEX ) {
	unsigned char Return_code ;
	Return_code = 0;
	SCH_tasks_G [ TASK_INDEX ]. pTask = 0x0000 ;
	SCH_tasks_G [ TASK_INDEX ]. Delay = 0;
	SCH_tasks_G [ TASK_INDEX ]. Period = 0;
	SCH_tasks_G [ TASK_INDEX ]. RunMe = 0;
	return Return_code ;
}

void SCH_Shift_Task ( const int TASK_INDEX ) {
	if ( TASK_INDEX != count -1 ) {
		for ( int i = TASK_INDEX ; i < count -1; i ++) {
			SCH_tasks_G [i ] = SCH_tasks_G [ i +1];
		}
		SCH_tasks_G [ count -1]. pTask = 0x0000 ;
		SCH_tasks_G [ count -1]. Delay = 0;
		SCH_tasks_G [ count -1]. Period = 0;
		SCH_tasks_G [ count -1]. RunMe = 0;
	}
	count--;
}

