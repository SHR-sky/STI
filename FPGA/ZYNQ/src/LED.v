`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/09 20:07:22
// Design Name: 
// Module Name: Led
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


module Led(
    input sys_clk,
    input sys_rst_n,
    
    output [1:0] led
    );
    
reg [25:0] cnt;
assign led = (cnt < 26'd2500_0000) ? 2'b01:2'b10;

always @(posedge sys_clk, negedge sys_rst_n) begin
    if(!sys_rst_n)
        cnt <= 0;
    else if(cnt < 26'd5000_0000)
        cnt <= cnt + 1;
    else
        cnt <= 0;
end
    
    
endmodule
