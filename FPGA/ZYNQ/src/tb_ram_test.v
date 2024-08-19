`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/10 00:49:59
// Design Name: 
// Module Name: tb_ram_test
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


module tb_ram_test(

    );

reg clk;
reg rst_n;

ram_test uut(
    .clk(clk),
    .rst_n(rst_n)
);

initial begin
    clk = 0;
    rst_n = 0;

    #100;

    rst_n = 1;
end

always begin
    #10
    clk = ~clk;
end

endmodule
