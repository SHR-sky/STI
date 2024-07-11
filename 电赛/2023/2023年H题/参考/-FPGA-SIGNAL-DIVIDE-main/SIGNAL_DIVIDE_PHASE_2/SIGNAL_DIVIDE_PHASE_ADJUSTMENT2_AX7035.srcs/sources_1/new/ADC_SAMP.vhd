LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

LIBRARY XPM;
USE XPM.VCOMPONENTS.ALL;

LIBRARY XPM;
USE XPM.VCOMPONENTS.ALL;

ENTITY ADC_SAMP IS 
PORT(
  AD_CLK:IN STD_LOGIC;
  AD_CLK_EN:IN STD_LOGIC;
  AD_RSTN:IN STD_LOGIC;
  AD_DATA:IN STD_LOGIC_VECTOR(11 DOWNTO 0);
  
  ACLK:IN STD_LOGIC;
  ARESETN:IN STD_LOGIC;
  
  RAM_ADDR:IN STD_LOGIC_VECTOR(15 DOWNTO 0);
  RAM_DATA:OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
  
  SAMP_FINISH:OUT STD_LOGIC
);
END ENTITY;

ARCHITECTURE BEHAVE OF ADC_SAMP IS 

TYPE RAMS IS ARRAY(0 TO 2047) OF STD_LOGIC_VECTOR(11 DOWNTO 0);
SIGNAL RAM0:RAMS:= (OTHERS => (OTHERS => '0'));

ATTRIBUTE RAM_STYLE:STRING;
ATTRIBUTE RAM_STYLE OF RAM0:SIGNAL IS "TRUE";

SIGNAL SAMP_FLAG:STD_LOGIC:= '0';

SIGNAL RAM_FLAG:STD_LOGIC:= '0';
SIGNAL RAM_FLAG_T:STD_LOGIC:= '0';
--SIGNAL RAM_FLAG_T2:STD_LOGIC:= '0';

SIGNAL SAMP_CNT:INTEGER RANGE 0 TO 1023:= 0;

BEGIN
  
  PROCESS IS BEGIN
  WAIT UNTIL RISING_EDGE(AD_CLK);
    IF AD_RSTN = '0' THEN
      SAMP_CNT <= 0;
      SAMP_FLAG <= '0';
      RAM_FLAG <= '0';
    ELSE
      IF AD_CLK_EN = '1' THEN
        IF SAMP_CNT >= 1023 THEN
          SAMP_CNT <= 0;
          SAMP_FLAG <= '1';
          RAM_FLAG <= NOT RAM_FLAG;
        ELSE
          SAMP_CNT <= SAMP_CNT + 1;
          SAMP_FLAG <= '0';
          RAM_FLAG <= RAM_FLAG;
        END IF;
        
        IF RAM_FLAG = '0' THEN
          RAM0(SAMP_CNT) <= AD_DATA;
        ELSE
          RAM0(SAMP_CNT + 1024) <= AD_DATA;
        END IF;
        
      END IF;
    END IF;
  END PROCESS;

   xpm_cdc_pulse_inst : xpm_cdc_pulse
   generic map (
      DEST_SYNC_FF => 4,   -- DECIMAL; range: 2-10
      INIT_SYNC_FF => 0,   -- DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      REG_OUTPUT => 0,     -- DECIMAL; 0=disable registered output, 1=enable registered output
      RST_USED => 0,       -- DECIMAL; 0=no reset, 1=implement reset
      SIM_ASSERT_CHK => 0  -- DECIMAL; 0=disable simulation messages, 1=enable simulation messages
   )
   port map (
      dest_pulse => RAM_FLAG_T, -- 1-bit output: Outputs a pulse the size of one dest_clk period when a pulse
                                -- transfer is correctly initiated on src_pulse input. This output is
                                -- combinatorial unless REG_OUTPUT is set to 1.

      dest_clk => ACLK,     -- 1-bit input: Destination clock.
      dest_rst => '0',     -- 1-bit input: optional; required when RST_USED = 1
      src_clk => AD_CLK,       -- 1-bit input: Source clock.
      src_pulse => RAM_FLAG,   -- 1-bit input: Rising edge of this signal initiates a pulse transfer to the
                                -- destination clock domain. The minimum gap between each pulse transfer must
                                -- be at the minimum 2*(larger(src_clk period, dest_clk period)). This is
                                -- measured between the falling edge of a src_pulse to the rising edge of the
                                -- next src_pulse. This minimum gap will guarantee that each rising edge of
                                -- src_pulse will generate a pulse the size of one dest_clk period in the
                                -- destination clock domain. When RST_USED = 1, pulse transfers will not be
                                -- guaranteed while src_rst and/or dest_rst are asserted.

      src_rst => '0'        -- 1-bit input: optional; required when RST_USED = 1
   );
  
  PROCESS IS BEGIN
  WAIT UNTIL RISING_EDGE(ACLK);
    IF ARESETN = '0' THEN
      RAM_DATA <= (OTHERS => '0');
    ELSE
      IF RAM_FLAG_T = '0' THEN
        RAM_DATA <= RAM0(CONV_INTEGER(RAM_ADDR + 1024));
      ELSE
        RAM_DATA <= RAM0(CONV_INTEGER(RAM_ADDR));
      END IF;
    END IF;
  END PROCESS;  
    
  ASYNC_PULSE_GEN: xpm_cdc_pulse
  generic map (
    DEST_SYNC_FF => 4,
    INIT_SYNC_FF => 0,
    REG_OUTPUT => 0,  
    RST_USED => 1, 
    SIM_ASSERT_CHK => 0 
  )
  port map (
    dest_pulse => SAMP_FINISH,
    dest_clk => ACLK,
    dest_rst => NOT ARESETN,
    src_clk => AD_CLK, 
    src_pulse => SAMP_FLAG,
    src_rst => NOT AD_RSTN
  );

END ARCHITECTURE;