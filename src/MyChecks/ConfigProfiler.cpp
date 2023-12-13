
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "MyChecks.h"
#ifdef __cplusplus
	};
#endif


#include <cstdio>

/* Configure Profiler */
void MycConfigProfiler(struct MycConfigProfiler* inst)
{
	MycProfilingInternalType * i = &inst->internal;
	PROFILER_DEFINITION *def = &inst->Definition;

	if( !inst->Enable )
		i->step = MYC_PROF_DISABLED;

	switch( i->step ){
		case MYC_PROF_DISABLED:
			i->fbLogStateGet.enable = false;

			if( inst->Enable ){
				inst->ErrorID = 0;
				inst->Error = false;
				inst->Valid = false;
				inst->Busy = true;
				/* set defaults if necessary */
				/*Event grout to be logged. profLOGGRP_ALL = log all groups */
				if( def->logGroup == 0 ) def->logGroup = profLOGGRP_ALL;  

 				/* starts the profiling then continous recording*/
				if( def->logMode == 0 ) def->logMode = profLOGMOD_STARTUPINST | profLOGMOD_CONTINUOUS; 

 				/*Automatic archiving in case of error*/
				if( def->archMode == 0 ) def->archMode =  profARCHMOD_OPT_ERROR;

				/*Store definition module to USERROM*/
				if( def->memTypeDef == 0 ) def->memTypeDef = profMEMTYPE_USERROM ;  

				/* storing to profMEMTYPE_USERRAM allows to find watchdog errors, but causes a high system load !! */
				if( def->memTypeDat == 0 ) def->memTypeDat = profMEMTYPE_USERROM; 
				
				/*Size of the log buffer in bytes*/
				if( def->bufferSize == 0 ) def->bufferSize = 1024*100;  
				
				if( def->backupLevel == 0 ) def->backupLevel = profBKPLEVEL_SHUTDOWN;  /* profiling is backed up only at shutdown */

				
				i->fbLogStateGet.enable = true;
				i->step = MYC_PROF_GET_STATE;
			}
			break;


		case MYC_PROF_GET_STATE:  /* Get Profiler/Profiling Status */

			if( i->fbLogStateGet.status == 0 ){  /*LogStateGet successful*/
				switch( i->fbLogStateGet.logstate ){
					case profSTATE_RUNNING: /* Data being recorded */
					case profSTATE_READY: /* Data recording completed */
						i->step = MYC_PROF_DEINSTALLING;
					break;
				}
			}
			else if( i->fbLogStateGet.status == ERR_FUB_BUSY ){  /* busy */
			}
			else {  /* error */
				inst->ErrorID = i->fbLogStateGet.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}	
			break;


		case MYC_PROF_STOPPING:  /* Stop Profiling */
			i->fbLogStop.enable = true;
			LogStop( &i->fbLogStop );  /* Call the Functionblock*/
			
			if( i->fbLogStop.status == 0 ){  /*LogStop successful*/
				i->step = MYC_PROF_GET_STATE;  
			}
			else if( i->fbLogStop.status == ERR_FUB_BUSY ){ /* busy */
			}
			else { /* error */
				inst->ErrorID = i->fbLogStop.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_DEINSTALLING: /* deinstalling profiler configuration */
			i->fbLogDeInstall.enable = true;
			i->fbLogDeInstall.option = 0;
			LogDeInstall( &i->fbLogDeInstall );  /* Call the Functionblock */
		
			if( i->fbLogDeInstall.status == 0 ){ /* successful */
				i->step = MYC_PROF_CHECK_DEINSTALLED; 				
			}
			else if( i->fbLogDeInstall.status == ERR_FUB_BUSY ){ /* busy */
			}
			else { /* error */
				inst->ErrorID = i->fbLogDeInstall.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_CHECK_DEINSTALLED: /* check whether profiler is deinstalled */
//			i->fbLogStateGet.enable = true;
//			LogStateGet( &i->fbLogStateGet );

			if( i->fbLogStateGet.status == 0 ) {  /* LogStateGet successful */
				switch( i->fbLogStateGet.logstate ){
					case profSTATE_ACTIVE : /* Profiler active. AR profiler is ready */
						i->step = MYC_PROF_INSTALLING;
					break;
				}
			}
			else if( i->fbLogStateGet.status == ERR_FUB_BUSY ){ /* busy */
			}
			else {  /* error */
				inst->ErrorID = i->fbLogStateGet.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_INSTALLING:  /*Install Profiling*/
			i->fbLogInstall.enable = 1;
			i->fbLogInstall.pVersion =  0;  /*Current version is used */
			i->fbLogInstall.pDefinition = (UDINT) &inst->Definition;  /*Pointer to the profiler definition structure */
			LogInstall( &i->fbLogInstall );  /* Call the Functionblock*/
		
			if( i->fbLogInstall.status == 0 ){  /*LogInstall successful*/
				i->step = MYC_PROF_STARTING;  
			}
			else if( i->fbLogInstall.status == ERR_FUB_BUSY ){ /* busy */
			}
			else { /* error */
				inst->ErrorID = i->fbLogInstall.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;

		
		case MYC_PROF_STARTING:  /*Start Profiling*/
			i->fbLogStart.enable = true;
			LogStart( &i->fbLogStart );  /* Call the Functionblock*/
			
			if( i->fbLogStart.status == 0 ){  /*LogStart successful*/
				i->step = MYC_PROF_CHECK_RUNNING;
			}
			else if( i->fbLogStart.status == ERR_FUB_BUSY ){ /* busy */
			}
			else {  /*Go to Error Step*/
				inst->ErrorID = i->fbLogStart.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;
		

		case MYC_PROF_CHECK_RUNNING: /* check whether profiler is running */
//			i->fbLogStateGet.enable = true;
//			LogStateGet( &i->fbLogStateGet );

			if( i->fbLogStateGet.status == 0 ){  /*LogStateGet successful*/
				switch( i->fbLogStateGet.logstate ){
					case profSTATE_RUNNING : /* = 5 Data being recorded */
						i->step = MYC_PROF_RUNNING;
					break;
				}
			}
			else if( i->fbLogStateGet.status == ERR_FUB_BUSY ){ /* busy */
			}
			else { /* error */
				inst->ErrorID = i->fbLogStateGet.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_RUNNING: /* profiler is running */
			inst->Busy = false;
			inst->Valid = true;
			if( inst->CreateArchive ){
				if( !i->createArchiveOld ){
					i->createArchiveOld = true;
					i->step = MYC_PROF_CREATE_ARCHIVE;
				}
			}
			else {
				i->createArchiveOld = false;
			}

			if( i->fbLogStateGet.status == 0 ){  /*LogStateGet successful*/
				switch( i->fbLogStateGet.logstate ){
					case profSTATE_RUNNING : /* = 5 Data being recorded */
						i->step = MYC_PROF_RUNNING;
						break;
					case profSTATE_READY: /* stopped outside this FB ? */
						i->step = MYC_PROF_NOT_RUNNING;
						break;
				}
			}
			else if( i->fbLogStateGet.status == ERR_FUB_BUSY ){ /* busy */
			}
			else { /* error */
				inst->ErrorID = i->fbLogStateGet.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_NOT_RUNNING: /* profiler is not running, probably stopped by SDM */
			inst->Busy = true;
			inst->Valid = false;
			if( inst->CreateArchive ){
				if( !i->createArchiveOld ){
					i->createArchiveOld = true;
					i->step = MYC_PROF_CREATE_ARCHIVE;
				}
			}
			else {
				i->createArchiveOld = false;
			}

			if( i->fbLogStateGet.status == 0 ){  /*LogStateGet successful*/
				switch( i->fbLogStateGet.logstate ){
					case profSTATE_RUNNING : /* = 5 Data being recorded */
						i->step = MYC_PROF_RUNNING;
					break;
				}
			}
			else if( i->fbLogStateGet.status == ERR_FUB_BUSY ){ /* busy */
			}
			else { /* error */
				inst->ErrorID = i->fbLogStateGet.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_CREATE_ARCHIVE:
			i->fbGetTime.enable = true;
			DTGetTime( &i->fbGetTime );
			std::sprintf( i->archiveName, "%d", i->fbGetTime.DT1 );
			inst->Busy = true;
			inst->Valid = false;
			i->step = MYC_PROF_ARCH_STOP;
			break;


		case MYC_PROF_ARCH_STOP:  /*Stop Profiling*/
			i->fbLogStop.enable = true;
			LogStop( &i->fbLogStop );  /* Call the Functionblock */
			
			if( i->fbLogStop.status == 0 ){  /*LogStop successful*/
				i->step = MYC_PROF_ARCH_CREATE;  
			}
			else if( i->fbLogStop.status == ERR_FUB_BUSY ){
			}
			else {  /* busy */
				inst->ErrorID = i->fbLogStop.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_ARCH_CREATE: /* create archive */
			i->fbLogArchCreate.enable = 1;
			i->fbLogArchCreate.pDevice = 0;  /*no file device*/
			i->fbLogArchCreate.pName = (UDINT) &i->archiveName;
			i->fbLogArchCreate.option = 0;   
			LogArchCreate( &i->fbLogArchCreate );  /* Call the Functionblock*/
			
			if( i->fbLogArchCreate.status == 0 ){  /* LogArchCreate successful */
				i->step = MYC_PROF_STARTING;  
			}
			else if( i->fbLogArchCreate.status == ERR_FUB_BUSY ){ /* busy */
			}
			else { /* error */ 
				inst->ErrorID = i->fbLogArchCreate.status;
				i->errorStep = i->step;
				i->step = MYC_PROF_ERROR;
			}
			break;


		case MYC_PROF_ERROR:
			inst->Busy = false;
			inst->Error = true;
			inst->Valid = true;
		
			if( !inst->Enable ){
				inst->ErrorID = 0;
				inst->Error = false;
				i->step = MYC_PROF_DISABLED;
			}
			break;

	}

	LogStateGet( &i->fbLogStateGet );  	

}
