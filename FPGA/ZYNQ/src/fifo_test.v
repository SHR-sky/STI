`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/08/10 10:13:58
// Design Name: 
// Module Name: fifo_test
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


module fifo_test(
    input clk,
    input rst_n
    );

reg [15:0] w_data;
wire wr_en;
wire rd_en;
wire [15:0] r_data;
wire full;
wire empty;
wire [8:0] rd_data_count;
wire [8:0] wr_data_count;

wire clk_100M;
wire clk_75M;
wire locked;
wire fifo_rst_n;

wire wr_clk;
wire rd_clk;
reg [7:0] wcnt;
reg [7:0] rcnt;

wire wr_rst_busy;
wire rd_rst_busy;

clk_wiz_0 fifo_pll
   (
    // Clock out ports
    .clk_out1(clk_100M),     // output clk_out1
    .clk_out2(clk_75M),     // output clk_out2
    // Status and control signals
    .reset(~rst_n), // input reset
    .locked(locked),       // output locked
   // Clock in ports
    .clk_in1(clk)      // input clk_in1
);

assign fifo_rst_n = locked;
assign wr_clk = clk_100M;
assign rd_clk = clk_75M;

localparam W_IDLE = 1;
localparam W_FIFO = 2;

reg [2:0] write_state;
reg [2:0] next_write_state;

always @(posedge wr_clk, negedge fifo_rst_n) begin
    if(!fifo_rst_n)
        write_state <= W_IDLE;
    else
        write_state <= next_write_state;
end

always @(*) begin
    case (write_state)
        W_IDLE:
            begin
                if(wcnt == 8'd79)
                    next_write_state <= W_FIFO;
                else
                    next_write_state <= W_IDLE;
            end
        W_FIFO:
            next_write_state <= W_FIFO;
        default: 
            next_write_state <= W_IDLE;
    endcase
end

always @(posedge wr_clk, negedge fifo_rst_n) begin
    if(!fifo_rst_n)
        wcnt <= 8'd0;
    else if (write_state == W_IDLE)
        wcnt <= wcnt + 1;
    else
        wcnt <= 8'd0;
end

assign wr_en = (write_state == W_FIFO) ? ~full : 1'b0;

always @(posedge wr_clk, negedge fifo_rst_n) begin
    if(!fifo_rst_n)
        w_data <= 16'd1;
    else if (wr_en)
        w_data <= w_data + 1;
end

localparam R_IDLE = 1;
localparam R_FIFO = 2;

reg [2:0] read_state;
reg [2:0] next_read_state;

always @(posedge rd_clk, negedge fifo_rst_n) begin
    if(!fifo_rst_n)
        read_state <= W_IDLE;
    else
        read_state <= next_read_state;
end

always @(*) begin
    case (write_state)
        R_IDLE:
            begin
                if(wcnt == 8'd59)
                    next_read_state <= R_FIFO;
                else
                    next_read_state <= R_IDLE;
            end
        R_FIFO:
            next_read_state <= R_FIFO;
        default: 
            next_read_state <= R_IDLE;
    endcase
end

always @(posedge rd_clk, negedge fifo_rst_n) begin
    if(!fifo_rst_n)
        rcnt <= 8'd0;
    else if (write_state == W_IDLE)
        rcnt <= rcnt + 1;
    else
        rcnt <= 8'd0;
end

assign rd_en = (read_state == R_FIFO) ? ~empty : 1'b0;

fifo_ip your_instance_name (
  .rst(~fifo_rst_n),                      // input wire rst
  .wr_clk(wr_clk),                // input wire wr_clk
  .rd_clk(rd_clk),                // input wire rd_clk
  .din(w_data),                      // input wire [15 : 0] din
  .wr_en(wr_en),                  // input wire wr_en
  .rd_en(rd_en),                  // input wire rd_en
  .dout(r_data),                    // output wire [15 : 0] dout
  .full(full),                    // output wire full
  .empty(empty),                  // output wire empty
  .rd_data_count(rd_data_count),  // output wire [8 : 0] rd_data_count
  .wr_data_count(wr_data_count),  // output wire [8 : 0] wr_data_count
  .wr_rst_busy(wr_rst_busy),      // output wire wr_rst_busy
  .rd_rst_busy(rd_rst_busy)      // output wire rd_rst_busy
);

ila_0 ila_wfifo (
	.clk(wr_clk), // input wire clk


	.probe0(w_data), // input wire [15:0]  probe0  
	.probe1(wr_en), // input wire [0:0]  probe1 
	.probe2(full), // input wire [0:0]  probe2 
	.probe3(wr_data_count) // input wire [8:0]  probe3
);

ila_1 ila_rfifo (
	.clk(rd_clk), // input wire clk


	.probe0(r_data), // input wire [15:0]  probe0  
	.probe1(rd_en), // input wire [0:0]  probe1 
	.probe2(empty), // input wire [0:0]  probe2 
	.probe3(rd_data_count) // input wire [8:0]  probe3
);

endmodule
