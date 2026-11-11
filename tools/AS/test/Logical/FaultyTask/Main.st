
PROGRAM _INIT

END_PROGRAM

PROGRAM _CYCLIC

	IF trigger_div_dint THEN
		trigger_div_dint := FALSE;
		result := integerNumber / DINT#0;
	ELSIF trigger_div_udint THEN
		trigger_div_udint := FALSE;
		result := DINT_TO_UDINT(integerNumber) / UDINT#0;
	ELSIF trigger_div_int THEN
		trigger_div_int := FALSE;
		result := integerNumber / DINT_TO_INT(0);
	ELSIF trigger_div_uint THEN
		trigger_div_uint := FALSE;
		result := DINT_TO_UDINT(integerNumber) / UINT#0;
	ELSIF trigger_div_sint THEN
		trigger_div_sint := FALSE;
		result := integerNumber / SINT#0;
	ELSIF trigger_div_usint THEN
		trigger_div_usint := FALSE;
		result := DINT_TO_UDINT(integerNumber) / UINT#0;
	ELSIF trigger_div_real THEN
		trigger_div_real := FALSE;
		result := floatingPointNumber / REAL#0.0;
	ELSIF trigger_div_lreal THEN
		trigger_div_lreal := FALSE;
		result := floatingPointNumber / LREAL#0.0;
	ELSIF trigger_check_bounds1 THEN
		trigger_check_bounds1 := FALSE;
		integerNumber := 10; 
		result := array[integerNumber]; (* limited to 3..9 *)
	ELSIF trigger_check_bounds2 THEN
		trigger_check_bounds2 := FALSE;
		integerNumber := 2;
		result := array[integerNumber]; (* limited to 3..9 *)
	ELSIF trigger_check_range THEN
		trigger_check_range := FALSE;
		integerNumber := 10;
		enum := integerNumber;
	ELSIF trigger_check_signed_subrange THEN
		trigger_check_signed_subrange := FALSE;
		integerNumber := 11;
		limited_sint := DINT_TO_SINT(integerNumber);  (* limited to -10..+10 *)
	ELSIF trigger_check_unsigned_subrange THEN
		trigger_check_unsigned_subrange := FALSE;
		integerNumber := 11;
		limited_usint := DINT_TO_USINT(integerNumber); (* limited to 0..+10 *)
	ELSIF trigger_bad_read_access THEN
		trigger_bad_read_access := FALSE;
		dynamicVariable ACCESS 0;
		result := dynamicVariable;		
	ELSIF trigger_bad_write_access THEN
		trigger_bad_write_access := FALSE;
		dynamicVariable ACCESS 0;
		dynamicVariable := 1;
	ELSIF trigger_watchdog THEN
		trigger_watchdog := FALSE;	
		MCH_Trigger9206();	
	END_IF
	 
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

