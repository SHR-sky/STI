// 探测下降沿
module detect(
    input sys_clk,
    input rst_n,
    input RX_Pin_In,

    output H2L_Sig
);

    reg H2L_F1;
    reg H2L_F2;

    // 利用赋值的延迟，将下降沿转化为脉冲
    always @(posedge sys_clk, negedge rst_n) begin
        if(!rst_n) begin
            H2L_F1 <= 1;
            H2L_F2 <= 1;
        end
        else begin
            H2L_F1 <= RX_Pin_In;
            H2L_F2 <= H2L_F1;
        end
    end

    assign H2L_Sig = H2L_F2 & !H2L_F1;

endmodule