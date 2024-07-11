--Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2022.2 (win64) Build 3671981 Fri Oct 14 05:00:03 MDT 2022
--Date        : Fri Nov 17 21:27:48 2023
--Host        : DESKTOP-PDI3DR2 running 64-bit major release  (build 9200)
--Command     : generate_target TOP_wrapper.bd
--Design      : TOP_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity TOP_wrapper is
  port (
    ADCLK : out STD_LOGIC;
    ADDB : in STD_LOGIC_VECTOR ( 11 downto 0 );
    AWRT : out STD_LOGIC;
    BWRT : out STD_LOGIC;
    CLK : in STD_LOGIC;
    DACLK_0 : out STD_LOGIC;
    DACLK_1 : out STD_LOGIC;
    DADB_0 : out STD_LOGIC_VECTOR ( 13 downto 0 );
    DADB_1 : out STD_LOGIC_VECTOR ( 13 downto 0 );
    KEY0 : in STD_LOGIC;
    KEY1 : in STD_LOGIC;
    KEY2 : in STD_LOGIC;
    KEY3 : in STD_LOGIC;
    KEY4 : in STD_LOGIC;
    SMG_DX : out STD_LOGIC_VECTOR ( 7 downto 0 );
    SMG_WX : out STD_LOGIC_VECTOR ( 2 downto 0 )
  );
end TOP_wrapper;

architecture STRUCTURE of TOP_wrapper is
  component TOP is
  port (
    CLK : in STD_LOGIC;
    ADDB : in STD_LOGIC_VECTOR ( 11 downto 0 );
    DADB_0 : out STD_LOGIC_VECTOR ( 13 downto 0 );
    DADB_1 : out STD_LOGIC_VECTOR ( 13 downto 0 );
    DACLK_0 : out STD_LOGIC;
    DACLK_1 : out STD_LOGIC;
    AWRT : out STD_LOGIC;
    BWRT : out STD_LOGIC;
    ADCLK : out STD_LOGIC;
    KEY0 : in STD_LOGIC;
    KEY1 : in STD_LOGIC;
    KEY2 : in STD_LOGIC;
    KEY3 : in STD_LOGIC;
    KEY4 : in STD_LOGIC;
    SMG_WX : out STD_LOGIC_VECTOR ( 2 downto 0 );
    SMG_DX : out STD_LOGIC_VECTOR ( 7 downto 0 )
  );
  end component TOP;
begin
TOP_i: component TOP
     port map (
      ADCLK => ADCLK,
      ADDB(11 downto 0) => ADDB(11 downto 0),
      AWRT => AWRT,
      BWRT => BWRT,
      CLK => CLK,
      DACLK_0 => DACLK_0,
      DACLK_1 => DACLK_1,
      DADB_0(13 downto 0) => DADB_0(13 downto 0),
      DADB_1(13 downto 0) => DADB_1(13 downto 0),
      KEY0 => KEY0,
      KEY1 => KEY1,
      KEY2 => KEY2,
      KEY3 => KEY3,
      KEY4 => KEY4,
      SMG_DX(7 downto 0) => SMG_DX(7 downto 0),
      SMG_WX(2 downto 0) => SMG_WX(2 downto 0)
    );
end STRUCTURE;
