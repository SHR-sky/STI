`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/13 15:05:53
// Design Name: 
// Module Name: Pword_set
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


module Pword_set(
    input clk,
    input rst_n,
    input [1:0] key_PINC,

    output reg [23:0] Pword
    );

    always @(*) begin
        case(key_PINC)
        0:  Pword <= 'd1048576 / 'd8    // pi/4 
        1:  Pword <= 'd1048576 / 'd4    // pi/2   
        endcase
    end

endmodule
