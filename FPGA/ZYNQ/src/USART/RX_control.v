module RX_control(
    input sys_clk,
    input rst_n,
    input H2L_Sig,
    input RX_En_Sig,
    input RX_Pin_In,
    input BSP_clk,

    output Cnt_sig,
    output [7;0] RX_data,
    output RX_Done_Sig
);

    reg [3:0] i;
    reg [7:0] rData;
    reg isCnt;
    reg isDone;

    always @(posedge sys_clk, negedge rst_n) begin
        if(!rst_n) begin
            i <= 0;
            rData <= 0;
            isCnt <= 0;
            isDone <= 0;
        end
        else if(RX_En_Sig) begin
            case (i)
                4'd0:
                if(H2L_Sig) begin 
                    i <= i + 1'b1;
                    isCnt <= 1'b1;
                end

                4'd1:
                if(BSP_clk) begin
                    i <= i + 1'b1;
                end

                4'd2, 4'd3, 4'd4, 4'd5, 4'd6, 4'd7, 4'd8, 4'd9:
                if(BSP_clk) begin
                    i <= i + 1'b1;
                    rData[i-2] = RX_Pin_In;
                end

                4'd10:
                if(BSP_clk) begin
                    i <= i + 1'b1;
                end

                4'd11:
                if(BSP_clk) begin
                    i <= i + 1'b1;
                end

                4'd12:
                begin
                    i <= i + 1'b1;
                    isDone <= 1'b1;
                    isCnt <= 1'b0;
                end

                4'd13:
                begin
                    i <= 1'b0;
                    isDone <= 1'b0;
                end
                default: 
                begin
                    i <= 1'b0;
                    isDone <= 1'b0;
                end
            endcase
        end
    end

    assign Cnt_sig = isCnt;
    assign RX_data = rData;
    assign RX_Done_Sig = isDone;

endmodule