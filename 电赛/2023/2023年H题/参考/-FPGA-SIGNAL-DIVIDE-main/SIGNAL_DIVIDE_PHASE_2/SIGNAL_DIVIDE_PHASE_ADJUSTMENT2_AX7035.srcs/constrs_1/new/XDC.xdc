############## NET - IOSTANDARD ######################
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property BITSTREAM.CONFIG.UNUSEDPIN PULLUP [current_design]
#############SPI Configurate Setting##################
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design] 
set_property CONFIG_MODE SPIx4 [current_design] 
set_property BITSTREAM.CONFIG.CONFIGRATE 50 [current_design] 
############## clock and reset define##################
set_property PACKAGE_PIN Y18 [get_ports {CLK}]
set_property IOSTANDARD LVCMOS33 [get_ports {CLK}]

########ad9238 ON AX7035 J9#################################
set_property PACKAGE_PIN D19  [get_ports {ADCLK}]
set_property IOSTANDARD LVCMOS33  [get_ports {ADCLK}]
set_property SLEW FAST [get_ports {ADCLK}]

set_property PACKAGE_PIN B16  [get_ports {ADDB[1]}]
set_property PACKAGE_PIN B15  [get_ports {ADDB[0]}]
set_property PACKAGE_PIN B18  [get_ports {ADDB[3]}]
set_property PACKAGE_PIN B17  [get_ports {ADDB[2]}]
set_property PACKAGE_PIN A19  [get_ports {ADDB[5]}]
set_property PACKAGE_PIN A18  [get_ports {ADDB[4]}]
set_property PACKAGE_PIN C19  [get_ports {ADDB[7]}]
set_property PACKAGE_PIN C18  [get_ports {ADDB[6]}]
set_property PACKAGE_PIN A20  [get_ports {ADDB[9]}]
set_property PACKAGE_PIN B20  [get_ports {ADDB[8]}]
set_property PACKAGE_PIN C17  [get_ports {ADDB[11]}]
set_property PACKAGE_PIN D17  [get_ports {ADDB[10]}]
set_property IOSTANDARD LVCMOS33  [get_ports {ADDB[*]}]


##############J10 on ax7035 ###########################
set_property PACKAGE_PIN Y21 [get_ports {DACLK_0}]
set_property IOSTANDARD LVCMOS33 [get_ports {DACLK_0}]
set_property SLEW FAST [get_ports {DACLK_0}]

set_property PACKAGE_PIN P17 [get_ports {DADB_0[13]}]
set_property PACKAGE_PIN N17 [get_ports {DADB_0[12]}]
set_property PACKAGE_PIN R19 [get_ports {DADB_0[11]}]
set_property PACKAGE_PIN P19 [get_ports {DADB_0[10]}]
set_property PACKAGE_PIN T18 [get_ports {DADB_0[9]}]
set_property PACKAGE_PIN R18 [get_ports {DADB_0[8]}]
set_property PACKAGE_PIN U21 [get_ports {DADB_0[7]}]
set_property PACKAGE_PIN T21 [get_ports {DADB_0[6]}]
set_property PACKAGE_PIN V22 [get_ports {DADB_0[5]}]
set_property PACKAGE_PIN U22 [get_ports {DADB_0[4]}]
set_property PACKAGE_PIN V20 [get_ports {DADB_0[3]}]
set_property PACKAGE_PIN U20 [get_ports {DADB_0[2]}]
set_property PACKAGE_PIN W22 [get_ports {DADB_0[1]}]
set_property PACKAGE_PIN W21 [get_ports {DADB_0[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {DADB_0[*]}]
set_property PACKAGE_PIN Y22 [get_ports {AWRT}]
set_property IOSTANDARD LVCMOS33 [get_ports {AWRT}]

set_property PACKAGE_PIN AA21 [get_ports {DACLK_1}]
set_property IOSTANDARD LVCMOS33 [get_ports {DACLK_1}]
set_property SLEW FAST [get_ports {DACLK_1}]

set_property PACKAGE_PIN AB22 [get_ports {DADB_1[13]}]
set_property PACKAGE_PIN AB21 [get_ports {DADB_1[12]}]
set_property PACKAGE_PIN AB20 [get_ports {DADB_1[11]}]
set_property PACKAGE_PIN AA19 [get_ports {DADB_1[10]}]
set_property PACKAGE_PIN W20 [get_ports  {DADB_1[9]}]
set_property PACKAGE_PIN W19 [get_ports  {DADB_1[8]}]
set_property PACKAGE_PIN AB18 [get_ports {DADB_1[7]}]
set_property PACKAGE_PIN AA18 [get_ports {DADB_1[6]}]
set_property PACKAGE_PIN V19 [get_ports  {DADB_1[5]}]
set_property PACKAGE_PIN V18 [get_ports  {DADB_1[4]}]
set_property PACKAGE_PIN W17 [get_ports  {DADB_1[3]}]
set_property PACKAGE_PIN V17 [get_ports  {DADB_1[2]}]
set_property PACKAGE_PIN U18 [get_ports  {DADB_1[1]}]
set_property PACKAGE_PIN U17 [get_ports  {DADB_1[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {DADB_1[*]}]

set_property PACKAGE_PIN AA20 [get_ports {BWRT}]
set_property IOSTANDARD LVCMOS33 [get_ports {BWRT}]


######################  Key  ##########################
set_property PACKAGE_PIN F20 [get_ports {KEY0}]
set_property IOSTANDARD LVCMOS33 [get_ports {KEY0}]

set_property PACKAGE_PIN M13 [get_ports {KEY1}]
set_property IOSTANDARD LVCMOS33 [get_ports {KEY1}]

set_property PACKAGE_PIN K14 [get_ports {KEY2}]
set_property IOSTANDARD LVCMOS33 [get_ports {KEY2}]

set_property PACKAGE_PIN K13 [get_ports {KEY3}]
set_property IOSTANDARD LVCMOS33 [get_ports {KEY3}]

set_property PACKAGE_PIN L13 [get_ports {KEY4}]
set_property IOSTANDARD LVCMOS33 [get_ports {KEY4}]



################## SMG Display ########################
set_property PACKAGE_PIN J5 [get_ports {SMG_DX[0]}]     
set_property PACKAGE_PIN M3 [get_ports {SMG_DX[1]}]     
set_property PACKAGE_PIN J6 [get_ports {SMG_DX[2]}]     
set_property PACKAGE_PIN H5 [get_ports {SMG_DX[3]}]     
set_property PACKAGE_PIN G4 [get_ports {SMG_DX[4]}]     
set_property PACKAGE_PIN K6 [get_ports {SMG_DX[5]}]     
set_property PACKAGE_PIN K3 [get_ports {SMG_DX[6]}]     
set_property PACKAGE_PIN H4 [get_ports {SMG_DX[7]}]     

set_property PACKAGE_PIN N4 [get_ports {SMG_WX[0]}]     
set_property PACKAGE_PIN L5 [get_ports {SMG_WX[1]}]     
set_property PACKAGE_PIN L4 [get_ports {SMG_WX[2]}]     

set_property IOSTANDARD LVCMOS33 [get_ports {SMG_DX[*]}]
set_property IOSTANDARD LVCMOS33 [get_ports {SMG_WX[*]}]










##Pmod Header JA
#set_property -dict { PACKAGE_PIN V12   IOSTANDARD LVCMOS33 } [get_ports { JA1 }]; 
#set_property -dict { PACKAGE_PIN T14   IOSTANDARD LVCMOS33 } [get_ports { JA2 }]; 
#set_property -dict { PACKAGE_PIN T11   IOSTANDARD LVCMOS33 } [get_ports { JA3 }]; 
#set_property -dict { PACKAGE_PIN P14   IOSTANDARD LVCMOS33 } [get_ports { JA4 }]; 
#set_property -dict { PACKAGE_PIN W13   IOSTANDARD LVCMOS33 } [get_ports { JA7 }]; 
#set_property -dict { PACKAGE_PIN T15   IOSTANDARD LVCMOS33 } [get_ports { JA8 }]; 
#set_property -dict { PACKAGE_PIN T10   IOSTANDARD LVCMOS33 } [get_ports { JA9 }]; 
#set_property -dict { PACKAGE_PIN R14   IOSTANDARD LVCMOS33 } [get_ports { JA10 }];

##Pmod Header JB
#set_property -dict { PACKAGE_PIN V16   IOSTANDARD LVCMOS33 } [get_ports { JB1 }]; 
#set_property -dict { PACKAGE_PIN V15   IOSTANDARD LVCMOS33 } [get_ports { JB2 }]; 
#set_property -dict { PACKAGE_PIN U14   IOSTANDARD LVCMOS33 } [get_ports { JB3 }]; 
#set_property -dict { PACKAGE_PIN U13   IOSTANDARD LVCMOS33 } [get_ports { JB4 }]; 
#set_property -dict { PACKAGE_PIN W16   IOSTANDARD LVCMOS33 } [get_ports { JB7 }]; 
#set_property -dict { PACKAGE_PIN W15   IOSTANDARD LVCMOS33 } [get_ports { JB8 }]; 
#set_property -dict { PACKAGE_PIN U15   IOSTANDARD LVCMOS33 } [get_ports { JB9 }]; 
#set_property -dict { PACKAGE_PIN V13   IOSTANDARD LVCMOS33 } [get_ports { JB10 }];

##Pmod Header JC
#set_property -dict { PACKAGE_PIN W18   IOSTANDARD LVCMOS33 } [get_ports { JC1 }]; 
#set_property -dict { PACKAGE_PIN Y18   IOSTANDARD LVCMOS33 } [get_ports { JC2 }]; 
#set_property -dict { PACKAGE_PIN Y16   IOSTANDARD LVCMOS33 } [get_ports { JC3 }]; 
#set_property -dict { PACKAGE_PIN V17   IOSTANDARD LVCMOS33 } [get_ports { JC4 }]; 
#set_property -dict { PACKAGE_PIN W19   IOSTANDARD LVCMOS33 } [get_ports { JC7 }]; 
#set_property -dict { PACKAGE_PIN Y19   IOSTANDARD LVCMOS33 } [get_ports { JC8 }]; 
#set_property -dict { PACKAGE_PIN Y17   IOSTANDARD LVCMOS33 } [get_ports { JC9 }]; 
#set_property -dict { PACKAGE_PIN V18   IOSTANDARD LVCMOS33 } [get_ports { JC10 }];

##Pmod Header JD
#set_property -dict { PACKAGE_PIN N17   IOSTANDARD LVCMOS33 } [get_ports { JD1 }]; 
#set_property -dict { PACKAGE_PIN R16   IOSTANDARD LVCMOS33 } [get_ports { JD2 }]; 
#set_property -dict { PACKAGE_PIN T17   IOSTANDARD LVCMOS33 } [get_ports { JD3 }]; 
#set_property -dict { PACKAGE_PIN T16   IOSTANDARD LVCMOS33 } [get_ports { JD4 }]; 
#set_property -dict { PACKAGE_PIN P18   IOSTANDARD LVCMOS33 } [get_ports { JD7 }]; 
#set_property -dict { PACKAGE_PIN R17   IOSTANDARD LVCMOS33 } [get_ports { JD8 }]; 
#set_property -dict { PACKAGE_PIN R18   IOSTANDARD LVCMOS33 } [get_ports { JD9 }]; 
#set_property -dict { PACKAGE_PIN U17   IOSTANDARD LVCMOS33 } [get_ports { JD10 }];

##Pmod Header JE
#set_property -dict { PACKAGE_PIN H15   IOSTANDARD LVCMOS33 } [get_ports { JE1 }]; 
#set_property -dict { PACKAGE_PIN F16   IOSTANDARD LVCMOS33 } [get_ports { JE2 }]; 
#set_property -dict { PACKAGE_PIN E17   IOSTANDARD LVCMOS33 } [get_ports { JE3 }]; 
#set_property -dict { PACKAGE_PIN E18   IOSTANDARD LVCMOS33 } [get_ports { JE4 }]; 
#set_property -dict { PACKAGE_PIN G15   IOSTANDARD LVCMOS33 } [get_ports { JE7 }]; 
#set_property -dict { PACKAGE_PIN F17   IOSTANDARD LVCMOS33 } [get_ports { JE8 }]; 
#set_property -dict { PACKAGE_PIN D18   IOSTANDARD LVCMOS33 } [get_ports { JE9 }]; 
#set_property -dict { PACKAGE_PIN E19   IOSTANDARD LVCMOS33 } [get_ports { JE10 }];

##Pmod Header JF
#set_property -dict { PACKAGE_PIN G17   IOSTANDARD LVCMOS33 } [get_ports { JF1 }]; 
#set_property -dict { PACKAGE_PIN H16   IOSTANDARD LVCMOS33 } [get_ports { JF2 }]; 
#set_property -dict { PACKAGE_PIN B19   IOSTANDARD LVCMOS33 } [get_ports { JF3 }]; 
#set_property -dict { PACKAGE_PIN C20   IOSTANDARD LVCMOS33 } [get_ports { JF4 }]; 
#set_property -dict { PACKAGE_PIN G18   IOSTANDARD LVCMOS33 } [get_ports { JF7 }]; 
#set_property -dict { PACKAGE_PIN H17   IOSTANDARD LVCMOS33 } [get_ports { JF8 }]; 
#set_property -dict { PACKAGE_PIN A20   IOSTANDARD LVCMOS33 } [get_ports { JF9 }]; 
#set_property -dict { PACKAGE_PIN B20   IOSTANDARD LVCMOS33 } [get_ports { JF10 }];

##Pmod Header JG
#set_property -dict { PACKAGE_PIN F19   IOSTANDARD LVCMOS33 } [get_ports { JG1 }]; 
#set_property -dict { PACKAGE_PIN G19   IOSTANDARD LVCMOS33 } [get_ports { JG2 }]; 
#set_property -dict { PACKAGE_PIN J20   IOSTANDARD LVCMOS33 } [get_ports { JG3 }]; 
#set_property -dict { PACKAGE_PIN K16   IOSTANDARD LVCMOS33 } [get_ports { JG4 }]; 
#set_property -dict { PACKAGE_PIN F20   IOSTANDARD LVCMOS33 } [get_ports { JG7 }]; 
#set_property -dict { PACKAGE_PIN G20   IOSTANDARD LVCMOS33 } [get_ports { JG8 }]; 
#set_property -dict { PACKAGE_PIN H20   IOSTANDARD LVCMOS33 } [get_ports { JG9 }]; 
#set_property -dict { PACKAGE_PIN J16   IOSTANDARD LVCMOS33 } [get_ports { JG10 }];

##Pmod Header JH
#set_property -dict { PACKAGE_PIN K19   IOSTANDARD LVCMOS33 } [get_ports { JH1 }]; 
#set_property -dict { PACKAGE_PIN L16   IOSTANDARD LVCMOS33 } [get_ports { JH2 }]; 
#set_property -dict { PACKAGE_PIN M19   IOSTANDARD LVCMOS33 } [get_ports { JH3 }]; 
#set_property -dict { PACKAGE_PIN L19   IOSTANDARD LVCMOS33 } [get_ports { JH4 }]; 
#set_property -dict { PACKAGE_PIN J19   IOSTANDARD LVCMOS33 } [get_ports { JH7 }]; 
#set_property -dict { PACKAGE_PIN L17   IOSTANDARD LVCMOS33 } [get_ports { JH8 }]; 
#set_property -dict { PACKAGE_PIN M20   IOSTANDARD LVCMOS33 } [get_ports { JH9 }]; 
#set_property -dict { PACKAGE_PIN L20   IOSTANDARD LVCMOS33 } [get_ports { JH10 }];

