# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "AD_DATA_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "FFT_DATA_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "RE_IM_WIDTH" -parent ${Page_0}


}

proc update_PARAM_VALUE.AD_DATA_WIDTH { PARAM_VALUE.AD_DATA_WIDTH } {
	# Procedure called to update AD_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.AD_DATA_WIDTH { PARAM_VALUE.AD_DATA_WIDTH } {
	# Procedure called to validate AD_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.FFT_DATA_WIDTH { PARAM_VALUE.FFT_DATA_WIDTH } {
	# Procedure called to update FFT_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.FFT_DATA_WIDTH { PARAM_VALUE.FFT_DATA_WIDTH } {
	# Procedure called to validate FFT_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.RE_IM_WIDTH { PARAM_VALUE.RE_IM_WIDTH } {
	# Procedure called to update RE_IM_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.RE_IM_WIDTH { PARAM_VALUE.RE_IM_WIDTH } {
	# Procedure called to validate RE_IM_WIDTH
	return true
}


proc update_MODELPARAM_VALUE.AD_DATA_WIDTH { MODELPARAM_VALUE.AD_DATA_WIDTH PARAM_VALUE.AD_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.AD_DATA_WIDTH}] ${MODELPARAM_VALUE.AD_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.RE_IM_WIDTH { MODELPARAM_VALUE.RE_IM_WIDTH PARAM_VALUE.RE_IM_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.RE_IM_WIDTH}] ${MODELPARAM_VALUE.RE_IM_WIDTH}
}

proc update_MODELPARAM_VALUE.FFT_DATA_WIDTH { MODELPARAM_VALUE.FFT_DATA_WIDTH PARAM_VALUE.FFT_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.FFT_DATA_WIDTH}] ${MODELPARAM_VALUE.FFT_DATA_WIDTH}
}

