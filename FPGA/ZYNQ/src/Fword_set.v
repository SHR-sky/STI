`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/13 11:42:20
// Design Name: 
// Module Name: Fword_set
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


module Fword_set(
    input               clk         ,
    input               rst_n       ,
    input  [1:0]        key_PINC    ,
    
    output reg [23:0]   Fword
    );
          
always@(*)
begin
    case(key_PINC)
        0:  Fword <= 'h51eb;     //1Mhz 20971.52 取整20971
        1:  Fword <= 'ha3d7;     //2Mhz 41943.04 取整41943
        2:  Fword <= 'hf5c2;     //3Mhz 62914.56 取整62914
        //3:  Fword <= 'h33333;    //10Mhz 209715.2 取整209715
    endcase
end

endmodule
