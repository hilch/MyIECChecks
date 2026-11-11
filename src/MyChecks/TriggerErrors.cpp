#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "MyChecks.h"
#ifdef __cplusplus
	};
#endif

/* helper function to make an entry into system logger */
UDINT MakeEntry(UINT number, UDINT errorInfo, STRING* text);

/* Simulate watchdog error 9206 */
plcbit MCH_Trigger9206(void)
{
	MakeEntry( 9206, 0, "Watchdog Error triggered by application" );
	__asm__(" CLI;");
	for(;;);
}
