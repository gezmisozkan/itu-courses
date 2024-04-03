`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/01/2023 11:32:50 AM
// Design Name: 
// Module Name: sources
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


module register #(parameter N=4)(
  input clk,
  input [1:0]funsel,
  input enable,
  input [N-1:0] data_in,
  output [N-1:0] data_out
);
  reg [N-1:0] reg_data;
  always @(posedge clk) begin
        if(!enable)begin
            reg_data <= data_out;
        end
        else if (enable) begin
            case(funsel)
                2'b00: reg_data <= 0;
                2'b01: reg_data <= data_in;           
                2'b10: reg_data <= data_out -1; 
                2'b11: reg_data <= data_out +1;
             endcase
        end
   end
   assign data_out = reg_data;
endmodule


module IR_16 (
   input clk,
   input [1:0]funsel,
   input enable,
   input LH,
   input [7:0] data_in,
   output [15:0] data_out
);
    reg [15:0] reg_data; 
    always @(posedge clk) begin
            if (enable) begin
                case(funsel)
                    2'b00: reg_data <= 0; 
                    2'b01:
                          if(!LH) begin
                              reg_data[7:0] <= data_in;
                              reg_data[15:8] <= data_out[15:8];
                          end
                          else if(LH) begin
                              reg_data[7:0] <= data_out[7:0];
                              reg_data[15:8] <= data_in;
                          end            
                    2'b10: reg_data <= reg_data -1;
                    2'b11: reg_data <= reg_data +1;           
                 endcase
            end
     end  
     assign data_out = reg_data;
endmodule

module part2b (
    input  clk,
    input  [7:0] data_in,
    input  [2:0] O1sel,
    input  [2:0] O2sel,
    input  [1:0] funsel,
    input  [3:0] Rsel,
    input  [3:0] Tsel,
    output [7:0] O1,
    output [7:0] O2
); 
    reg [31:0] regs;
    wire [7:0] reg_data1;wire [7:0] reg_data2;wire [7:0] reg_data3;wire [7:0] reg_data4;
    reg enable1=0,enable2=0,enable3=0,enable4=0;
    register #(.N(8)) R1(.clk(clk),.funsel(funsel),.enable(enable1),.data_in(data_in),.data_out(reg_data1));
    register #(.N(8)) R2(.clk(clk),.funsel(funsel),.enable(enable2),.data_in(data_in),.data_out(reg_data2));
    register #(.N(8)) R3(.clk(clk),.funsel(funsel),.enable(enable3),.data_in(data_in),.data_out(reg_data3));
    register #(.N(8)) R4(.clk(clk),.funsel(funsel),.enable(enable4),.data_in(data_in),.data_out(reg_data4));
    
    always@* begin
        regs[7:0]   <= reg_data4;
        regs[15:8]  <= reg_data3;
        regs[23:16] <= reg_data2;
        regs[31:24] <= reg_data1;
    end
    
    reg [31:0] temp;
    wire [7:0] temp_data1;wire [7:0] temp_data2;wire [7:0] temp_data3;wire [7:0] temp_data4;
    reg enablet1=0,enablet2=0,enablet3=0,enablet4=0;
    register #(.N(8)) T1(.clk(clk),.funsel(funsel),.enable(enablet1),.data_in(data_in),.data_out(temp_data1));
    register #(.N(8)) T2(.clk(clk),.funsel(funsel),.enable(enablet2),.data_in(data_in),.data_out(temp_data2));
    register #(.N(8)) T3(.clk(clk),.funsel(funsel),.enable(enablet3),.data_in(data_in),.data_out(temp_data3));
    register #(.N(8)) T4(.clk(clk),.funsel(funsel),.enable(enablet4),.data_in(data_in),.data_out(temp_data4));
    
    always@* begin
        temp[7:0]   <= temp_data4; 
        temp[15:8]  <= temp_data3;    
        temp[23:16] <= temp_data2;   
        temp[31:24] <= temp_data1;
    end
    reg [7:0] o1;
    reg [7:0] o2;
    always @(*) begin
            case(Rsel)
                4'd0:     begin enable1<=0;  enable2<=0;   enable3<=0;  enable4<=0;         end
                4'd1:     begin enable1<=0;  enable2<=0;   enable3<=0;  enable4<=1;         end
                4'd2:     begin enable1<=0;  enable2<=0;   enable3<=1;  enable4<=0;         end
                4'd3:     begin enable1<=0;  enable2<=0;   enable3<=1;  enable4<=1;         end
                4'd4:     begin enable1<=0;  enable2<=1;   enable3<=0;  enable4<=0;         end
                4'd5:     begin enable1<=0;  enable2<=1;   enable3<=0;  enable4<=1;         end
                4'd6:     begin enable1<=0;  enable2<=1;   enable3<=1;  enable4<=0;         end
                4'd7:     begin enable1<=0;  enable2<=1;   enable3<=1;  enable4<=1;         end
                4'd8:     begin enable1<=1;  enable2<=0;   enable3<=0;  enable4<=0;         end
                4'd9:     begin enable1<=1;  enable2<=0;   enable3<=0;  enable4<=1;         end
                4'd10:    begin enable1<=1;  enable2<=0;   enable3<=1;  enable4<=0;         end
                4'd11:    begin enable1<=1;  enable2<=0;   enable3<=1;  enable4<=1;         end
                4'd12:    begin enable1<=1;  enable2<=1;   enable3<=0;  enable4<=0;         end
                4'd13:    begin enable1<=1;  enable2<=1;   enable3<=0;  enable4<=1;         end
                4'd14:    begin enable1<=1;  enable2<=1;   enable3<=1;  enable4<=0;         end
                4'd15:    begin enable1<=1;  enable2<=1;   enable3<=1;  enable4<=1;         end
            endcase
            
            case(Tsel)
                4'd0:     begin enablet1<=0;  enablet2<=0;   enablet3<=0;  enablet4<=0;     end
                4'd1:     begin enablet1<=0;  enablet2<=0;   enablet3<=0;  enablet4<=1;     end
                4'd2:     begin enablet1<=0;  enablet2<=0;   enablet3<=1;  enablet4<=0;     end
                4'd3:     begin enablet1<=0;  enablet2<=0;   enablet3<=1;  enablet4<=1;     end
                4'd4:     begin enablet1<=0;  enablet2<=1;   enablet3<=0;  enablet4<=0;     end
                4'd5:     begin enablet1<=0;  enablet2<=1;   enablet3<=0;  enablet4<=1;     end
                4'd6:     begin enablet1<=0;  enablet2<=1;   enablet3<=1;  enablet4<=0;     end
                4'd7:     begin enablet1<=0;  enablet2<=1;   enablet3<=1;  enablet4<=1;     end
                4'd8:     begin enablet1<=1;  enablet2<=0;   enablet3<=0;  enablet4<=0;     end
                4'd9:     begin enablet1<=1;  enablet2<=0;   enablet3<=0;  enablet4<=1;     end
                4'd10:    begin enablet1<=1;  enablet2<=0;   enablet3<=1;  enablet4<=0;     end
                4'd11:    begin enablet1<=1;  enablet2<=0;   enablet3<=1;  enablet4<=1;     end
                4'd12:    begin enablet1<=1;  enablet2<=1;   enablet3<=0;  enablet4<=0;     end
                4'd13:    begin enablet1<=1;  enablet2<=1;   enablet3<=0;  enablet4<=1;     end
                4'd14:    begin enablet1<=1;  enablet2<=1;   enablet3<=1;  enablet4<=0;     end
                4'd15:    begin enablet1<=1;  enablet2<=1;   enablet3<=1;  enablet4<=1;     end
            endcase            
            

            case(O1sel)
                3'd0: o1 <= temp_data1;
                3'd1: o1 <= temp_data2;
                3'd2: o1 <= temp_data3;
                3'd3: o1 <= temp_data4;
                3'd4: o1 <= reg_data1;
                3'd5: o1 <= reg_data2;
                3'd6: o1 <= reg_data3; 
                3'd7: o1 <= reg_data4;
            endcase
            
            case(O2sel)
                3'd0: o2 <= temp_data1;
                3'd1: o2 <= temp_data2;
                3'd2: o2 <= temp_data3;
                3'd3: o2 <= temp_data4;
                3'd4: o2 <= reg_data1;
                3'd5: o2 <= reg_data2;
                3'd6: o2 <= reg_data3; 
                3'd7: o2 <= reg_data4;
            endcase
            
    end
    assign O1=o1;
    assign O2=o2;
    
endmodule

module part2c (
    input  clk,
    input  [7:0] data_in,
    input  [1:0] OutAsel,
    input  [1:0] OutBsel,
    input  [1:0] funsel,
    input  [3:0] Rsel,
    output [7:0] OutA,
    output [7:0] OutB
);

    reg [31:0] regs;
    wire [7:0] PCPast;wire [7:0] SP;wire [7:0] AR;wire [7:0] PC;
    reg enable1=0,enable2=0,enable3=0,enable4=0; 
    reg OA,OB;
    register #(.N(8)) R4(.clk(clk),.funsel(funsel),.enable(enable4),.data_in(data_in),.data_out(PCPast));
    register #(.N(8)) R3(.clk(clk),.funsel(funsel),.enable(enable3),.data_in(data_in),.data_out(SP));
    register #(.N(8)) R2(.clk(clk),.funsel(funsel),.enable(enable2),.data_in(data_in),.data_out(AR));
    register #(.N(8)) R1(.clk(clk),.funsel(funsel),.enable(enable1),.data_in(data_in),.data_out(PC));
    
    always@* begin
        regs[7:0]   <= PCPast;
        regs[15:8]  <= SP;
        regs[23:16] <= AR;
        regs[31:24] <= PC;
    end
    reg [7:0]o1;
    reg [7:0]o2;
    always @(*) begin
            case(Rsel)
                 4'd0:     begin enable1<=0;  enable2<=0;   enable3<=0;  enable4<=0;         end
                 4'd1:     begin enable1<=0;  enable2<=0;   enable3<=0;  enable4<=1;         end
                 4'd2:     begin enable1<=0;  enable2<=0;   enable3<=1;  enable4<=0;         end
                 4'd3:     begin enable1<=0;  enable2<=0;   enable3<=1;  enable4<=1;         end
                 4'd4:     begin enable1<=0;  enable2<=1;   enable3<=0;  enable4<=0;         end
                 4'd5:     begin enable1<=0;  enable2<=1;   enable3<=0;  enable4<=1;         end
                 4'd6:     begin enable1<=0;  enable2<=1;   enable3<=1;  enable4<=0;         end
                 4'd7:     begin enable1<=0;  enable2<=1;   enable3<=1;  enable4<=1;         end
                 4'd8:     begin enable1<=1;  enable2<=0;   enable3<=0;  enable4<=0;         end
                 4'd9:     begin enable1<=1;  enable2<=0;   enable3<=0;  enable4<=1;         end
                 4'd10:    begin enable1<=1;  enable2<=0;   enable3<=1;  enable4<=0;         end
                 4'd11:    begin enable1<=1;  enable2<=0;   enable3<=1;  enable4<=1;         end
                 4'd12:    begin enable1<=1;  enable2<=1;   enable3<=0;  enable4<=0;         end
                 4'd13:    begin enable1<=1;  enable2<=1;   enable3<=0;  enable4<=1;         end
                 4'd14:    begin enable1<=1;  enable2<=1;   enable3<=1;  enable4<=0;         end
                 4'd15:    begin enable1<=1;  enable2<=1;   enable3<=1;  enable4<=1;         end
            endcase          
            
            case(OutAsel)
                3'd0: o1 <= AR;
                3'd1: o1 <= SP;
                3'd2: o1 <= PCPast;
                3'd3: o1 <= PC;
            endcase
                   
            case(OutBsel)
                3'd0: o2 <= AR;
                3'd1: o2 <= SP;
                3'd2: o2 <= PCPast;
                3'd3: o2 <= PC;
            endcase
    end
    assign OutA=o1;
    assign OutB=o2;
endmodule

module ALU (
    input [3:0]funsel,
    input  [7:0]A,
    input  [7:0]B,
    output [3:0] flag,
    output [7:0]OutALU
);
    reg [3:0]flag_reg=0;
    reg [7:0]OutputALU=0;
    always @(*) begin
        case(funsel)
             4'd0:     begin OutputALU<=A;                                                    end
             4'd1:     begin OutputALU<=B;                                                    end
             4'd2:     begin OutputALU<=~A;                                                   end
             4'd3:     begin OutputALU<=~B;                                                   end
             4'd4:     begin 
                            {flag_reg[2],OutputALU}<=A+B; 
                            if((A[7]==B[7])& A[7]!=OutputALU[7])   //Overflow                       
                                   flag_reg[0]<=1;       
                            else   flag_reg[0]<=0;      
                       end
             4'd5:     begin 
                            {flag_reg[2],OutputALU}<=A+~B+1; 
                            if((A[7]==~B[7])& A[7]!=OutputALU[7])   //Overflow
                                     flag_reg[0]<=1;        
                            else     flag_reg[0]<=0;                                            
                       end
             4'd6:     begin
                            flag_reg[0]<=0;
                            if(A>B)  OutputALU<=A;       
                            else     OutputALU<=8'd0;    
                       end
             4'd7:     begin OutputALU<=A&B;                                                  end
             4'd8:     begin OutputALU<=A|B;                                                  end
             4'd9:     begin OutputALU<=~(A&B);                                               end
             4'd10:    begin OutputALU<=(((~A)&B)|(A&(~B)));                                  end
             4'd11:    begin flag_reg[2]<=A[7];   OutputALU<={A[6:0],1'b0};                   end
             4'd12:    begin flag_reg[2]<=A[0];   OutputALU<={1'b0,A[7:1]};                   end
             4'd13:    begin OutputALU<={A[6:0],1'b0}; 
                            if(A[7]!=A[6]) flag_reg[0]<=1; 
                            else flag_reg[0]<=0;                                   
                       end
             4'd14:    begin OutputALU<={A[7],A[7:1]};                                        end
             4'd15:    begin flag_reg[2]<=A[0]; OutputALU<={flag_reg[2],A[7:1]};              end
        endcase      
        
        if(OutputALU==8'd0) flag_reg[3]<=1;
        else flag_reg[3]<=0;   
            
        if(OutputALU[7]==1)  flag_reg[1]<=1;    
        else flag_reg[1]<=0;
   end 
   assign OutALU=OutputALU;
   assign flag=flag_reg;
endmodule

module ALU_System(
    input [2:0]RF_OutASel, 
    input [2:0]RF_OutBSel, 
    input [1:0]RF_FunSel,
    input [3:0]RF_RSel,
    input [3:0]RF_TSel,
    input [3:0]ALU_FunSel,
    input [1:0]ARF_OutCSel, 
    input [1:0]ARF_OutDSel, 
    input [1:0]ARF_FunSel,
    input [3:0]ARF_RegSel,
    input      IR_LH,
    input      IR_Enable,
    input [1:0]IR_Funsel,
    input      Mem_WR,
    input      Mem_CS,
    input [1:0]MuxASel,
    input [1:0]MuxBSel,
    input      MuxCSel,
    input      Clock,
    output [3:0]ALUOutFlag,
    output [7:0]ALUOut,
    output [7:0]AOut,
    output [7:0]BOut,
    output [7:0]ARF_AOut,
    output [7:0]Address,
    output [7:0]MemoryOut,
    output [15:0]IROut,
    output [7:0]MuxAOut,
    output [7:0]MuxBOut,
    output [7:0]MuxCOut
);
    
    reg [7:0]MUX_A_Out;
    reg [7:0]MUX_B_Out;
    reg [7:0]MUX_C_Out;
    
    always@* begin
        case (MuxASel)
            2'd0: MUX_A_Out<=ALUOut;
            2'd1: MUX_A_Out<=MemoryOut;
            2'd2: MUX_A_Out<=IROut[7:0];
            2'd3: MUX_A_Out<=ARF_AOut;
        endcase
        case (MuxBSel)
            2'd0: MUX_B_Out<=ALUOut;
            2'd1: MUX_B_Out<=MemoryOut;
            2'd2: MUX_B_Out<=IROut[7:0];
            2'd3: MUX_B_Out<=ARF_AOut;
        endcase
        case (MuxCSel)
            1'd0: MUX_C_Out<=AOut; 
            1'd1: MUX_C_Out<=ARF_AOut;
        endcase    
    end
    
    IR_16 IR1(.clk(Clock),.funsel(IR_Funsel),.enable(IR_Enable),.LH(IR_LH),.data_in(MemoryOut),.data_out(IROut));
    
    part2c part2c_1(.clk(Clock),.data_in(MUX_B_Out),.OutAsel(ARF_OutCSel),.OutBsel(ARF_OutDSel),.funsel(ARF_FunSel),.Rsel(ARF_RegSel),.OutA(ARF_AOut),.OutB(Address));
    
    part2b part2b_1(.clk(Clock),.data_in(MUX_A_Out),.O1sel(RF_OutASel),.O2sel(RF_OutBSel),.funsel(RF_FunSel),.Rsel(RF_RSel),.Tsel(RF_TSel),.O1(AOut),.O2(BOut)); 
    
    ALU ALU_1(.funsel(ALU_FunSel),.A(MUX_C_Out),.B(BOut),.flag(ALUOutFlag),.OutALU(ALUOut));    
    
    Memory mem1(.address(Address),.data(ALUOut),.wr(Mem_WR),.cs(Mem_CS),.clock(Clock),.o(MemoryOut));    

    assign MuxAOut=MUX_A_Out;
    assign MuxBOut=MUX_B_Out;
    assign MuxCOut=MUX_C_Out;

endmodule
