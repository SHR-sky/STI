// 产生采样时钟
// 波特率为115200
module RX_BSP(
    input sys_clk,
    input rst_n,
    input Cnt_sig,

    output BSP_clk
);

    reg [11:0] Cnt_BSP;

    always @(posedge sys_clk, negedge rst_n) begin
        if(rst_n)
            Cnt_BSP <= 0;
        else if (Cnt_BSP == 12'd434) begin
            Cnt_BSP <= 12'd0;
        end
        else if (Cnt_sig) begin
            Cnt_BSP <= Cnt_BSP + 1;
        end
        else 
            Cnt_BSP <= 0;
    end

    assign BSP_clk = (Cnt_BSP == 12'd127) ? 1'b1 : 1'b0;

endmodule