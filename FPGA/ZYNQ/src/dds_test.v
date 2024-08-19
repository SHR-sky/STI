`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/10 18:52:48
// Design Name: 
// Module Name: dds_test
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module dds_test(
    input sys_clk,
    input rst_n
    );

// VIO频率控制字
wire [1:0] key_PINC;

vio_0 vio_0_inst (
  .clk(sys_clk),                // input wire clk
  .probe_out0(key_PINC)  // output wire [1 : 0] probe_out0
);

// 频率控制模块
wire [23:0] Fword;  // 频率字
Fword_set Fword_set_inst(
    .clk(sys_clk),
    .rst_n(rst_n),
    .key_PINC(key_PINC),

    .Fword(Fword)
);

wire fre_ctrl_word_en;

wire m_axis_data_tvalid;
wire [47:0]  m_axis_data_tdata;
wire m_axis_phase_tvalid;
wire [23:0]  m_axis_phase_tdata;

assign fre_ctrl_word_en=1'b1;

dds_compiler_0 dds_compiler_0_inst (
  .aclk(sys_clk),                                  // input wire aclk
  .s_axis_config_tvalid(fre_ctrl_word_en),  // input wire s_axis_config_tvalid
  .s_axis_config_tdata(Fword),    // input wire [23 : 0] s_axis_config_tdata
  .m_axis_data_tvalid(m_axis_data_tvalid),      // output wire m_axis_data_tvalid
  .m_axis_data_tdata(m_axis_data_tdata),        // output wire [47 : 0] m_axis_data_tdata
  .m_axis_phase_tvalid(m_axis_phase_tvalid),    // output wire m_axis_phase_tvalid
  .m_axis_phase_tdata(m_axis_phase_tdata)      // output wire [23 : 0] m_axis_phase_tdata
);

ila_0 ila_0_inst(
    .clk(sys_clk),

    .probe0(key_PINC),
    .probe1(Fword),
    .probe2(m_axis_data_tdata)
);


endmodule





