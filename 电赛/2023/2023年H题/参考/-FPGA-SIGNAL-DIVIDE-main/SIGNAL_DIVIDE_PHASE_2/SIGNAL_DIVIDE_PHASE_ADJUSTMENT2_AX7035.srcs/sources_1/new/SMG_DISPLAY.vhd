LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY SMG_DISPLAY IS 
PORT(
  ACLK:IN STD_LOGIC;
  ARESETN:IN STD_LOGIC;
  DIN:IN STD_LOGIC_VECTOR(15 DOWNTO 0);
  SMG_WX:OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
  SMG_DX:OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
);
END ENTITY;

ARCHITECTURE BEHAVE OF SMG_DISPLAY IS 

TYPE SMG_VALUE_LIST IS ARRAY (0 TO 15) OF STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL SMG_VALUE:SMG_VALUE_LIST:= (X"3F",
                                   X"06",
                                   X"5B",
                                   X"4F",
                                   X"66",
                                   X"6D",
                                   X"7D",
                                   X"07",
                                   X"7F",
                                   X"6F",
                                   X"00",
                                   X"00",
                                   X"00",
                                   X"00",
                                   X"00",
                                   X"00"
                                   );

SIGNAL WX:STD_LOGIC_VECTOR(2 DOWNTO 0):= "000";

SIGNAL DX:STD_LOGIC_VECTOR(7 DOWNTO 0):= X"00";

SIGNAL WX_CNT:INTEGER RANGE 0 TO 7:= 0;

SIGNAL BIT0:INTEGER RANGE 0 TO 9:= 0;
SIGNAL BIT1:INTEGER RANGE 0 TO 9:= 0;
SIGNAL BIT2:INTEGER RANGE 0 TO 9:= 0;

SIGNAL NUM:INTEGER:= 0;

BEGIN
  
  NUM <= CONV_INTEGER(DIN) WHEN DIN >= 0 AND DIN <= 180 ELSE
         0 WHEN DIN < 0 ELSE
         180 WHEN DIN > 180;
  
  PROCESS IS BEGIN
  WAIT UNTIL RISING_EDGE(ACLK);
    BIT0 <= NUM MOD 10;
    BIT1 <= (NUM / 10) MOD 10;
    BIT2 <= (NUM / 100) MOD 10;
  END PROCESS;
    

  PROCESS IS 
  VARIABLE CNT:INTEGER:= 0;
  BEGIN
  WAIT UNTIL RISING_EDGE(ACLK);
    IF CNT >= 99999 THEN
      CNT := 0;
      IF WX_CNT >= 3 THEN
        WX_CNT <= 0;
      ELSE
        WX_CNT <= WX_CNT + 1;
      END IF;
    ELSE
      CNT := CNT + 1;
    END IF;
    
    CASE WX_CNT IS 
      WHEN 0 => 
        WX <= "001";
        DX <= SMG_VALUE(BIT0);
      WHEN 1 =>
        WX <= "010";
        IF BIT2 = 0 AND BIT1 = 0 THEN
          DX <= SMG_VALUE(10);
        ELSE
          DX <= SMG_VALUE(BIT1);
        END IF;
      WHEN 2 =>
        WX <= "100";
        IF BIT2 = 0 THEN
          DX <= SMG_VALUE(10);
        ELSE
          DX <= SMG_VALUE(BIT2);
        END IF;
      WHEN OTHERS =>
        WX <= "000"; 
        DX <= X"00";
    END CASE;
  END PROCESS;
  
  SMG_WX <= NOT WX;
  SMG_DX <= NOT DX;

END ARCHITECTURE;