`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/10 00:22:06
// Design Name: 
// Module Name: ram_test
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


module ram_test(
    input clk,
    input rst_n
    );

reg [8:0] w_addr;
reg [15:0] w_data;
reg wea;
reg [8:0] r_addr;
wire [15:0] r_data;

always @(posedge clk, negedge rst_n)
begin
    if(!rst_n)
        r_addr <= 0;
    else if (|w_addr)
        r_addr <= r_addr + 1;
    else
        r_addr <= 0;
end

always @(posedge clk, negedge rst_n)
begin
    if(!rst_n)
        wea <= 0;
    else if (&w_addr)
        wea <= 0;
    else
        wea <= 1;
end

always @(posedge clk, negedge rst_n)
begin
    if(!rst_n) begin
        w_addr <= 0;
        w_data <= 0;
    end
    else if(wea) begin
        if(&wea) begin
            w_addr <= w_addr;
            w_data <= w_data;
        end
        else begin
            w_addr <= w_addr + 1;
            w_data <= w_data + 1;
        end
    end
end

ram_ip ram_ip_test(
    .clka(clk),
    .wea(wea),
    .addra(w_addr),
    .dina(w_data),
    .clkb(clk),
    .addrb(r_addr),
    .doutb(r_data)
)

ila_0 ila_0_inst (
	.clk(clk), // input wire clk


	.probe0(r_data), // input wire [15:0]  probe0  
	.probe1(r_addr) // input wire [8:0]  probe1
);

endmodule
