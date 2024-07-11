LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;
--USE IEEE.NUMERIC_STD.ALL;

ENTITY FFT_CTRL IS 
GENERIC(
  AD_DATA_WIDTH:INTEGER:= 12;  --ADC数据位宽
  RE_IM_WIDTH:INTEGER:= 23;     --FFT输出实部、虚部位宽
  FFT_DATA_WIDTH:INTEGER:= 48
);
PORT(
  ACLK:IN STD_LOGIC;
  ARESETN:IN STD_LOGIC;
  
  --与ADC SAMP模块交互
  READ_BEGIN: IN STD_LOGIC;  --来自ADC SAMP模块，告诉FFT CTRL模块可以读取ADC数据
  AD_RAM_ADDR:OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
  AD_RAM_DATA:IN STD_LOGIC_VECTOR(AD_DATA_WIDTH - 1 DOWNTO 0);
  
  --FFT数据输出
  FFT_M_DATA_TDATA:OUT STD_LOGIC_VECTOR(47 DOWNTO 0);
  FFT_M_DATA_TUSER:OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
  FFT_M_DATA_TVALID:OUT STD_LOGIC;
  
  FFT_FINISH_FLAG:OUT STD_LOGIC    --一次FFT运算完成信号
);
END ENTITY;

ARCHITECTURE BEHAVE OF FFT_CTRL IS 

COMPONENT xfft_0
  PORT (
    aclk : IN STD_LOGIC;
    aresetn : IN STD_LOGIC;
    s_axis_config_tdata : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    s_axis_config_tvalid : IN STD_LOGIC;
    s_axis_config_tready : OUT STD_LOGIC;
    s_axis_data_tdata : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    s_axis_data_tvalid : IN STD_LOGIC;
    s_axis_data_tready : OUT STD_LOGIC;
    s_axis_data_tlast : IN STD_LOGIC;
    m_axis_data_tdata : OUT STD_LOGIC_VECTOR(47 DOWNTO 0);
    m_axis_data_tuser : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
    m_axis_data_tvalid : OUT STD_LOGIC;
    m_axis_data_tready: IN STD_LOGIC;
    m_axis_data_tlast : OUT STD_LOGIC;
    event_frame_started : OUT STD_LOGIC;
    event_tlast_unexpected : OUT STD_LOGIC;
    event_tlast_missing : OUT STD_LOGIC;
    event_data_in_channel_halt : OUT STD_LOGIC 
  );
END COMPONENT;

SIGNAL FFT_S_CONFIG_TREADY:STD_LOGIC:= '0';

SIGNAL FFT_S_DATA_TDATA:STD_LOGIC_VECTOR(31 DOWNTO 0):= (OTHERS => '0');
SIGNAL FFT_S_DATA_TVALID:STD_LOGIC:= '0';
SIGNAL FFT_S_DATA_TREADY:STD_LOGIC:= '0';
SIGNAL FFT_S_DATA_TLAST:STD_LOGIC:= '0';

SIGNAL FFT_M_DATA_TREADY:STD_LOGIC:= '0';
SIGNAL FFT_M_DATA_TLAST:STD_LOGIC:= '0';

SIGNAL AD_RAM_CNT:INTEGER RANGE 0 TO 1023:= 0;  
SIGNAL DATA_CNT:INTEGER RANGE 0 TO 1023:= 0;

TYPE FFT_STATES IS (IDLE,DATA_WRITE,DATA_READ,FFT_END);
SIGNAL STATE:FFT_STATES:= IDLE;

SIGNAL FFT_M_DATA_TVALID_R:STD_LOGIC:= '0';

BEGIN

  AD_RAM_ADDR <= CONV_STD_LOGIC_VECTOR(AD_RAM_CNT,16);
  
  FFT_M_DATA_TREADY <= '1';
  
  FFT_M_DATA_TVALID <= FFT_M_DATA_TVALID_R;
  
  PROCESS IS BEGIN
  WAIT UNTIL RISING_EDGE(ACLK);
    IF ARESETN = '0' THEN
      FFT_FINISH_FLAG <= '0';
      DATA_CNT <= 0;
      AD_RAM_CNT <= 0;
      FFT_S_DATA_TVALID <= '0';
      FFT_S_DATA_TDATA <= (OTHERS => '0');
      FFT_S_DATA_TLAST <= '0';
    ELSE
      CASE STATE IS 
        WHEN IDLE => 
          FFT_FINISH_FLAG <= '0';
          DATA_CNT <= 0;
          AD_RAM_CNT <= 0;
          FFT_S_DATA_TVALID <= '0';
          FFT_S_DATA_TDATA <= (OTHERS => '0');
          FFT_S_DATA_TLAST <= '0';
          IF READ_BEGIN = '1' THEN
            STATE <= DATA_WRITE;
          END IF;
        WHEN DATA_WRITE =>
          IF FFT_S_DATA_TREADY = '1' THEN
            IF AD_RAM_CNT < 1023 THEN
              AD_RAM_CNT <= AD_RAM_CNT + 1;
            ELSE
              AD_RAM_CNT <= AD_RAM_CNT;
            END IF;
            
            IF AD_RAM_CNT /= 0 THEN
              FFT_S_DATA_TVALID <= '1';
              IF DATA_CNT = 1023 THEN
                FFT_S_DATA_TLAST <= '1';
                FFT_S_DATA_TDATA(27 DOWNTO 16) <= (OTHERS => '0');
                FFT_S_DATA_TDATA(11 DOWNTO 0) <= AD_RAM_DATA;
                STATE <= DATA_READ;
                DATA_CNT <= 0;
              ELSE 
                FFT_S_DATA_TLAST <= '0';
                FFT_S_DATA_TDATA(27 DOWNTO 16) <= (OTHERS => '0');
                FFT_S_DATA_TDATA(11 DOWNTO 0) <= AD_RAM_DATA;
                DATA_CNT <= DATA_CNT + 1;
              END IF;
            ELSE
              FFT_S_DATA_TVALID <= '0';
              FFT_S_DATA_TLAST <= '0';
              FFT_S_DATA_TDATA(27 DOWNTO 16) <= (OTHERS => '0');
              FFT_S_DATA_TDATA(11 DOWNTO 0) <= (OTHERS => '0');
            END IF;
          ELSE
            FFT_S_DATA_TLAST <= '0';
            FFT_S_DATA_TDATA(27 DOWNTO 16) <= (OTHERS => '0');
            FFT_S_DATA_TDATA(11 DOWNTO 0) <= (OTHERS => '0');
          END IF;
        WHEN DATA_READ =>
          FFT_S_DATA_TVALID <= '0';
          FFT_S_DATA_TLAST <= '0';
          IF FFT_M_DATA_TVALID_R = '1' THEN
            IF FFT_M_DATA_TLAST = '1' THEN
              STATE <= FFT_END;
            END IF;
          END IF;
        WHEN FFT_END =>
          FFT_FINISH_FLAG <= '1';
          STATE <= IDLE;
        WHEN OTHERS => 
          STATE <= IDLE;
      END CASE;
    END IF;
  END PROCESS;
                    
              
  FFT_GEN : xfft_0
  PORT MAP (
    aclk => aclk,
    aresetn => aresetn,
    s_axis_config_tdata => X"01",
    s_axis_config_tvalid => '1',
    
    s_axis_config_tready => FFT_S_CONFIG_TREADY,
    
    s_axis_data_tdata => FFT_S_DATA_TDATA,
    s_axis_data_tvalid => FFT_S_DATA_TVALID,
    s_axis_data_tready => FFT_S_DATA_TREADY,
    s_axis_data_tlast => FFT_S_DATA_TLAST,
    
    m_axis_data_tdata => FFT_M_DATA_TDATA,
    m_axis_data_tuser => FFT_M_DATA_TUSER,
    m_axis_data_tvalid => FFT_M_DATA_TVALID_R,
    m_axis_data_tready => '1',
    m_axis_data_tlast => FFT_M_DATA_TLAST,
    
    event_frame_started => OPEN,
    event_tlast_unexpected => OPEN,
    event_tlast_missing => OPEN,
    event_data_in_channel_halt => OPEN
  );

END ARCHITECTURE;