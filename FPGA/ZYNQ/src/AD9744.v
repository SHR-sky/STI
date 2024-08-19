`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/12 22:45:29
// Design Name: 
// Module Name: AD9744
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: DAC
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module AD9744(
    input sys_clk,
    input signed [15:0]data_in,
    output reg signed [13:0]DAC_out,
    output clk_p,
    output clk_n
    );
    initial begin
        DAC_out <=0;
    end

    assign clk_p = sys_clk;
    assign clk_n =~sys_clk;

    always @(posedge sys_clk) begin
        DAC_out<=data_in[15:2];
    end
endmodule
