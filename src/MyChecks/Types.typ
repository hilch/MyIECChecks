
TYPE
	MycProfilingInternalType : 	STRUCT  (*Profiler variables*)
		step : MycProfilingStepEnum; (*Profiler step variable*)
		fbLogStateGet : LogStateGet; (*AsARProf.LogStateGet() FUB*)
		fbLogInstall : LogInstall; (*AsARProf.LogInsatll() FUB*)
		fbLogDeInstall : LogDeInstall; (*AsARProf.LogDeInstall() FUB*)
		fbLogStart : LogStart; (*AsARProf.LogStart() FUB*)
		fbLogStop : LogStop; (*AsARProf.LogStop() FUB*)
		fbLogArchCreate : LogArchCreate; (*AsARProf.LogArchCreate() FUB*)
		fbLogArchDelete : LogArchDelete; (*AsARProf.LogArchDelete() FUB*)
		fbLogIdleShow : LogIdleShow; (*AsARProf.LogIdleShow() FUB*)
		fbGetTime : DTGetTime;
		libFctCnt : UDINT; (*Number of specified library functions*)
		pLibFunctions : UDINT; (*Pointer to library functions*)
		errorStep : MycProfilingStepEnum; (*shows the step where error occured*)
		archiveName : STRING[80];
		createArchiveOld : BOOL;
	END_STRUCT;
	MycProfilingStepEnum : 
		(
		MYC_PROF_DISABLED,
		MYC_PROF_GET_STATE,
		MYC_PROF_STOPPING,
		MYC_PROF_DEINSTALLING,
		MYC_PROF_CHECK_DEINSTALLED,
		MYC_PROF_INSTALLING,
		MYC_PROF_STARTING,
		MYC_PROF_CHECK_RUNNING,
		MYC_PROF_RUNNING,
		MYC_PROF_NOT_RUNNING,
		MYC_PROF_CREATE_ARCHIVE,
		MYC_PROF_ARCH_STOP,
		MYC_PROF_ARCH_CREATE,
		MYC_PROF_ERROR
		);
END_TYPE
