LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

LIBRARY XPM;
USE XPM.VCOMPONENTS.ALL;

ENTITY IFFT_CTRL IS 
GENERIC(
  RE_IM_WIDTH:INTEGER:= 23;     --FFT输出实部、虚部位宽
  FFT_DATA_WIDTH:INTEGER:= 48;
  DA_DATA_WIDTH:INTEGER:= 14
);
PORT(
  ACLK:IN STD_LOGIC;
  ARESETN:IN STD_LOGIC;
  
  READ_BEGIN:IN STD_LOGIC;   --告诉IFFT模块可以读取FFT数据
  FFT_RAM_ADDR:OUT STD_LOGIC_VECTOR(15 DOWNTO 0);  --读取RAM里的FFT数据
  FFT_RAM_DATA:IN STD_LOGIC_VECTOR(31 DOWNTO 0);

  DA_CLK:IN STD_LOGIC;
  DA_CLK_EN:IN STD_LOGIC;
  DADB:OUT STD_LOGIC_VECTOR(11 DOWNTO 0)
  
);
END ENTITY;

ARCHITECTURE BEHAVE OF IFFT_CTRL IS 

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
    m_axis_data_tready : IN STD_LOGIC;
    m_axis_data_tlast : OUT STD_LOGIC;
    event_frame_started : OUT STD_LOGIC;
    event_tlast_unexpected : OUT STD_LOGIC;
    event_tlast_missing : OUT STD_LOGIC;
    event_data_in_channel_halt : OUT STD_LOGIC 
  );
END COMPONENT;

COMPONENT fifo_generator_0
  PORT (
    wr_clk : IN STD_LOGIC;
    rd_clk : IN STD_LOGIC;
    din : IN STD_LOGIC_VECTOR(47 DOWNTO 0);
    wr_en : IN STD_LOGIC;
    rd_en : IN STD_LOGIC;
    dout : OUT STD_LOGIC_VECTOR(47 DOWNTO 0);
    full : OUT STD_LOGIC;
    empty : OUT STD_LOGIC 
  );
END COMPONENT;

SIGNAL IFFT_S_CONFIG_TREADY:STD_LOGIC:= '0';

SIGNAL IFFT_S_DATA_TDATA:STD_LOGIC_VECTOR(31 DOWNTO 0):= (OTHERS => '0');
SIGNAL IFFT_S_DATA_TVALID:STD_LOGIC:= '0';
SIGNAL IFFT_S_DATA_TREADY:STD_LOGIC:= '0';
SIGNAL IFFT_S_DATA_TLAST:STD_LOGIC:= '0';

SIGNAL IFFT_M_DATA_TDATA:STD_LOGIC_VECTOR(47 DOWNTO 0):= (OTHERS => '0');
SIGNAL IFFT_M_DATA_TUSER:STD_LOGIC_VECTOR(15 DOWNTO 0):= (OTHERS => '0');
SIGNAL IFFT_M_DATA_TVALID:STD_LOGIC:= '0';
SIGNAL IFFT_M_DATA_TREADY:STD_LOGIC:= '0';
SIGNAL IFFT_M_DATA_TLAST:STD_LOGIC:= '0';

SIGNAL FFT_RAM_CNT:INTEGER RANGE 0 TO 1023:= 0;
SIGNAL DATA_CNT:INTEGER RANGE 0 TO 1023:= 0;

TYPE RAMS IS ARRAY (0 TO 4095) OF STD_LOGIC_VECTOR(47 DOWNTO 0);
SIGNAL DAC_RAM:RAMS:= (OTHERS => (OTHERS => '0'));
SIGNAL DAC_RAM_WR_ADDR:INTEGER RANGE 0 TO 4095:= 2048;
SIGNAL DAC_RAM_RD_ADDR:INTEGER RANGE 0 TO 4095:= 0;

TYPE FFT_STATES IS (IDLE,DATA_WRITE,DATA_READ,IFFT_END);
SIGNAL STATE:FFT_STATES:= IDLE;

-- IFFT输出RAM

SIGNAL DADB_R:STD_LOGIC_VECTOR(47 DOWNTO 0):= (OTHERS => '0');

SIGNAL DA_RAM_CNT:INTEGER:= 0;

SIGNAL RD_FLAG:STD_LOGIC:= '0';
SIGNAL RD_FLAG_R:STD_LOGIC:= '0';

BEGIN

  
  FFT_RAM_ADDR <= CONV_STD_LOGIC_VECTOR(FFT_RAM_CNT,16);
  
  PROCESS IS BEGIN
  WAIT UNTIL RISING_EDGE(ACLK);
    IF ARESETN = '0' THEN
      DATA_CNT <= 0;
      FFT_RAM_CNT <= 0;
      IFFT_S_DATA_TVALID <= '0';
      IFFT_S_DATA_TDATA <= (OTHERS => '0');
      IFFT_S_DATA_TLAST <= '0';
      DAC_RAM_WR_ADDR <= 2048;
      RD_FLAG <= '0';
    ELSE
      CASE STATE IS 
        WHEN IDLE =>
          DATA_CNT <= 0;
          FFT_RAM_CNT <= 0;
          IFFT_S_DATA_TVALID <= '0';
          IFFT_S_DATA_TDATA <= (OTHERS => '0');
          IFFT_S_DATA_TLAST <= '0';
          IF READ_BEGIN = '1' THEN
            STATE <= DATA_WRITE;
          END IF;
        WHEN DATA_WRITE =>
          IF IFFT_S_DATA_TREADY = '1' THEN
            IF FFT_RAM_CNT < 1023 THEN
              FFT_RAM_CNT <= FFT_RAM_CNT + 1;
            ELSE
              FFT_RAM_CNT <= FFT_RAM_CNT;
            END IF;
            
            IF FFT_RAM_CNT /= 0 THEN
              IFFT_S_DATA_TVALID <= '1';
              IFFT_S_DATA_TDATA(27 DOWNTO 16) <= CONV_STD_LOGIC_VECTOR(CONV_INTEGER(FFT_RAM_DATA(27 DOWNTO 16)), 12);
              IFFT_S_DATA_TDATA(11 DOWNTO 0) <= CONV_STD_LOGIC_VECTOR(CONV_INTEGER(FFT_RAM_DATA(11 DOWNTO 0)), 12);
              IF DATA_CNT = 1023 THEN
                IFFT_S_DATA_TLAST <= '1';
                STATE <= DATA_READ;
                DATA_CNT <= 0;
              ELSE
                IFFT_S_DATA_TLAST <= '0';
                DATA_CNT <= DATA_CNT + 1;
              END IF;
            ELSE
              IFFT_S_DATA_TLAST <= '0';
              IFFT_S_DATA_TDATA <= (OTHERS => '0');
              IFFT_S_DATA_TVALID <= '0';
            END IF;
          ELSE
            IFFT_S_DATA_TLAST <= '0'; 
            IFFT_S_DATA_TDATA <= (OTHERS => '0');
          END IF;
        WHEN DATA_READ =>
          IFFT_S_DATA_TVALID <= '0';
          IFFT_S_DATA_TLAST <= '0';
          IF IFFT_M_DATA_TVALID = '1' THEN
            IF DAC_RAM_WR_ADDR >= 4095 THEN
              DAC_RAM_WR_ADDR <= 0;
            ELSE
              DAC_RAM_WR_ADDR <= DAC_RAM_WR_ADDR + 1;
            END IF;
            
            DAC_RAM(DAC_RAM_WR_ADDR) <= IFFT_M_DATA_TDATA;
            
            IF IFFT_M_DATA_TLAST = '1' THEN
              RD_FLAG <= '1';
              STATE <= IFFT_END;
            END IF;
          END IF;
        WHEN IFFT_END =>
          STATE <= IDLE;
        WHEN OTHERS => 
          STATE <= IDLE;
      END CASE;
    END IF;
  END PROCESS;

  DADB <= CONV_STD_LOGIC_VECTOR(CONV_INTEGER((DADB_R(22 DOWNTO 0) & '0')),12);
  
  PROCESS IS BEGIN
  WAIT UNTIL RISING_EDGE(DA_CLK);
    IF ARESETN = '0' THEN
      DAC_RAM_RD_ADDR <= 0;
    ELSE
      IF DA_CLK_EN = '1' THEN
        IF RD_FLAG_R = '0' THEN
          DAC_RAM_RD_ADDR <= 0;
        ELSE
          IF DAC_RAM_RD_ADDR >= 4095 THEN
            DAC_RAM_RD_ADDR <= 0;
          ELSE
            DAC_RAM_RD_ADDR <= DAC_RAM_RD_ADDR + 1;
          END IF;
        END IF;
        
        DADB_R <= DAC_RAM(DAC_RAM_RD_ADDR);
        
      END IF;
    END IF;
  END PROCESS;

   xpm_cdc_single_inst : xpm_cdc_single
   generic map (
      DEST_SYNC_FF => 4,   -- DECIMAL; range: 2-10
      INIT_SYNC_FF => 0,   -- DECIMAL; 0=disable simulation init values, 1=enable simulation init values
      SIM_ASSERT_CHK => 0, -- DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      SRC_INPUT_REG => 1   -- DECIMAL; 0=do not register input, 1=register input
   )
   port map (
      dest_out => RD_FLAG_R, -- 1-bit output: src_in synchronized to the destination clock domain. This output
                            -- is registered.

      dest_clk => DA_CLK, -- 1-bit input: Clock signal for the destination clock domain.
      src_clk => ACLK,   -- 1-bit input: optional; required when SRC_INPUT_REG = 1
      src_in => RD_FLAG      -- 1-bit input: Input signal to be synchronized to dest_clk domain.
   );

  IFFT_GEN : xfft_0
  PORT MAP (
    aclk => aclk,
    aresetn => aresetn,
    s_axis_config_tdata => X"00",
    s_axis_config_tvalid => '1',
    
    s_axis_config_tready => IFFT_S_CONFIG_TREADY,
    
    s_axis_data_tdata => IFFT_S_DATA_TDATA,
    s_axis_data_tvalid => IFFT_S_DATA_TVALID,
    s_axis_data_tready => IFFT_S_DATA_TREADY,
    s_axis_data_tlast => IFFT_S_DATA_TLAST,
    
    m_axis_data_tdata => IFFT_M_DATA_TDATA,
    m_axis_data_tuser => IFFT_M_DATA_TUSER,
    m_axis_data_tvalid => IFFT_M_DATA_TVALID,
    m_axis_data_tready => '1',
    m_axis_data_tlast => IFFT_M_DATA_TLAST,
    
    event_frame_started => OPEN,
    event_tlast_unexpected => OPEN,
    event_tlast_missing => OPEN,
    event_data_in_channel_halt => OPEN
  );

END ARCHITECTURE;