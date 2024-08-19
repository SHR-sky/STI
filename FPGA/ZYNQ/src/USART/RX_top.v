module rx_module
(
	input sys_clk,
    input rst_n,
	input RX_Pin_In,
    input RX_En_Sig,
	output [7:0] RX_data,
    output RX_Done_Sig
	
); 
	
	wire H2L_Sig;
	
	detect U1
	(
		.sys_clk(sys_clk),  	 // 顶层的 CLK
		.rst_n(rst_n),	 // 顶层的 复位信号
		.RX_Pin_In(RX_Pin_In),  //顶层的 输入信号线 
		.H2L_Sig(H2L_Sig)		// 发送给 rx_control_module 模块中的 开始信号
	);

	wire BPS_CLK;
	
	RX_BSP U2
	(
		.sys_clk(sys_clk),		// 顶层的 CLK
		.rst_n(rst_n),		// 顶层的 复位信号
		.Cnt_Sig(Cnt_Sig),  //来自 rx_control_module 模块中 正在接受数据的信号 （isCount）
		.BSP_CLK(BSP_CLK)   //发送给rx_control_module 模块用于状态机变换的 信号
	);

	wire Cnt_Sig;
		
	RX_control U3
	(
		.sys_clk(sys_clk),		// 顶层的 CLK
		.rst_n(rst_n),		// 顶层的 复位信号
		
		.H2L_Sig(H2L_Sig),  // 来自detect_module 模块中的开始的信号
		.RX_En_Sig(RX_En_Sig), // 来自顶层的使能信号
		.RX_Pin_In( RX_Pin_In ),//顶层的 输入信号线 
		.BSP_clk(BSP_CLK),		//来自rx_bps_module 模块中的 采集脉冲
		
		.Cnt_Sig(Cnt_Sig),  //发送给 rx_bps_module 模块 表示是否正在传输的 信号线
		.RX_data(RX_data),      //对数据位的数据进行串转并 之后输出的并口数据
		.RX_Done_Sig(RX_Done_Sig) //产生的传输完成的脉冲
		
	);	
    
endmodule	