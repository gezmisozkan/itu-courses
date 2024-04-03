`timescale 1ns / 1ps
// havva eda körpe, yusuf emir sezgin, özkan gezmi?

module Memory(
    input wire[7:0] address,
    input wire[7:0] data,
    input wire wr, //Read = 0, Write = 1
    input wire cs, //Chip is enable when cs = 0
    input wire clock,
    output reg[7:0] o // Output
);
    //Declaration oýf the RAM Area
    reg[7:0] RAM_DATA[0:255];
    //Read Ram data from the file
    initial $readmemh("RAM.mem", RAM_DATA);
    //Read the selected data from RAM
    always @(*) begin
        o = ~wr && ~cs ? RAM_DATA[address] : 8'hZ;
    end
    
    //Write the data to RAM
    always @(posedge clock) begin
        if (wr && ~cs) begin
            RAM_DATA[address] <= data; 
        end
    end
endmodule


module register #(parameter N=4)(
  input clk,
  input [1:0]funsel,
  input enable,
  input [N-1:0] data_in,
  output reg [N-1:0] data_out
);
  always @(posedge clk) begin
        if (enable) begin
            case(funsel)
                2'b00: data_out <= 0;
                2'b01: data_out <= data_in;           
                2'b10: data_out <= data_out -1; 
                2'b11: data_out <= data_out +1;
             endcase
        end
   end
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
    
    register #(.N(16)) R1(.clk(clk),.funsel(funsel),.enable(enable),.data_in(reg_data),.data_out(data_out));
    
   
    always @* begin
        if(enable && (funsel==2'b01))begin
            if(!LH) // Load LSB
                reg_data <= {data_out[15:8],data_in};
            else if(LH) // Load MSB
                reg_data <= {data_in,data_out[7:0]};
        end
    end  
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
    // enable for R1,R2,R3,R4
    wire ER1, ER2, ER3, ER4;
    assign ER1 = Rsel[3];
    assign ER2 = Rsel[2];
    assign ER3 = Rsel[1];
    assign ER4 = Rsel[0];

    wire [7:0] reg_data1;wire [7:0] reg_data2;wire [7:0] reg_data3;wire [7:0] reg_data4;
    register #(.N(8)) R1(.clk(clk),.funsel(funsel),.enable(ER1),.data_in(data_in),.data_out(reg_data1));
    register #(.N(8)) R2(.clk(clk),.funsel(funsel),.enable(ER2),.data_in(data_in),.data_out(reg_data2));
    register #(.N(8)) R3(.clk(clk),.funsel(funsel),.enable(ER3),.data_in(data_in),.data_out(reg_data3));
    register #(.N(8)) R4(.clk(clk),.funsel(funsel),.enable(ER4),.data_in(data_in),.data_out(reg_data4));
    
    
    // enable for T1,T2,T3,T4
    wire ET1, ET2, ET3, ET4;
    assign ET1 = Tsel[3];
    assign ET2 = Tsel[2];
    assign ET3 = Tsel[1];
    assign ET4 = Tsel[0];
        
    wire [7:0] temp_data1;wire [7:0] temp_data2;wire [7:0] temp_data3;wire [7:0] temp_data4;
    reg enablet1=0,enablet2=0,enablet3=0,enablet4=0;
    register #(.N(8)) T1(.clk(clk),.funsel(funsel),.enable(ET1),.data_in(data_in),.data_out(temp_data1));
    register #(.N(8)) T2(.clk(clk),.funsel(funsel),.enable(ET2),.data_in(data_in),.data_out(temp_data2));
    register #(.N(8)) T3(.clk(clk),.funsel(funsel),.enable(ET3),.data_in(data_in),.data_out(temp_data3));
    register #(.N(8)) T4(.clk(clk),.funsel(funsel),.enable(ET4),.data_in(data_in),.data_out(temp_data4));
    
    reg [7:0] o1;
    reg [7:0] o2;
    always @(*) begin               
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
    // enable for PC,AR,SP,PCPast
    wire E1, E2, E3, E4;
    assign E1 = Rsel[3];
    assign E2 = Rsel[2];
    assign E3 = Rsel[1];
    assign E4 = Rsel[0];
    
    
    wire [7:0] PCPast;wire [7:0] SP;wire [7:0] AR;wire [7:0] PC;
    register #(.N(8)) PC_R(.clk(clk),.funsel(funsel),.enable(E1),.data_in(data_in),.data_out(PC));
    register #(.N(8)) AR_R(.clk(clk),.funsel(funsel),.enable(E2),.data_in(data_in),.data_out(AR));
    register #(.N(8)) SP_R(.clk(clk),.funsel(funsel),.enable(E3),.data_in(data_in),.data_out(SP));
    register #(.N(8)) PCPast_R(.clk(clk),.funsel(funsel),.enable(E4),.data_in(data_in),.data_out(PCPast));


    reg [7:0]o1;
    reg [7:0]o2;
    always @(*) begin                  
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

module ALUSystem(
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
    output reg [7:0]MuxAOut,
    output reg [7:0]MuxBOut,
    output reg [7:0]MuxCOut
);
    
    always@* begin
        case (MuxASel)
            2'd0: MuxAOut<=ALUOut;
            2'd1: MuxAOut<=MemoryOut;
            2'd2: MuxAOut<=IROut[7:0];
            2'd3: MuxAOut<=ARF_AOut;
        endcase
        case (MuxBSel)
            2'd0: MuxBOut<=ALUOut;
            2'd1: MuxBOut<=MemoryOut;
            2'd2: MuxBOut<=IROut[7:0];
            2'd3: MuxBOut<=ARF_AOut;
        endcase
        case (MuxCSel)
            1'd0: MuxCOut<=AOut; 
            1'd1: MuxCOut<=ARF_AOut;
        endcase    
    end
    
    IR_16 IR1(.clk(Clock),.funsel(IR_Funsel),.enable(IR_Enable),.LH(IR_LH),.data_in(MemoryOut),.data_out(IROut));
    
    part2c part2c_1(.clk(Clock),.data_in(MuxBOut),.OutAsel(ARF_OutCSel),.OutBsel(ARF_OutDSel),.funsel(ARF_FunSel),.Rsel(ARF_RegSel),.OutA(ARF_AOut),.OutB(Address));
    
    part2b part2b_1(.clk(Clock),.data_in(MuxAOut),.O1sel(RF_OutASel),.O2sel(RF_OutBSel),.funsel(RF_FunSel),.Rsel(RF_RSel),.Tsel(RF_TSel),.O1(AOut),.O2(BOut)); 
    
    ALU ALU_1(.funsel(ALU_FunSel),.A(MuxCOut),.B(BOut),.flag(ALUOutFlag),.OutALU(ALUOut));    
    
    Memory mem1(.address(Address),.data(ALUOut),.wr(Mem_WR),.cs(Mem_CS),.clock(Clock),.o(MemoryOut));    

endmodule

module CPUSystem(
    input reset,
    input clock,    
    output [3:0]T
);
    reg Mem_WR, Mem_CS;
    reg [1:0] MuxASel;
    reg [1:0] MuxBSel;
    reg MuxCSel;    
    
    reg [2:0] O1Sel;
    reg [2:0] O2Sel;
    reg [1:0] RF_FunSel;
    reg [3:0] RF_RegSel;
    
    reg [3:0] ALU_FunSel;
    
    reg [1:0] ARF_OutASel;
    reg [1:0] ARF_OutBSel;
    reg [1:0] ARF_FunSel;
    reg [3:0] ARF_RegSel;
    
    reg IR_LH, IR_Enable;
    reg [1:0] IR_FunSel;

    wire [15:0] IROut;
    wire [3:0] ALUFlag;
    reg [3:0] LastFlag;

    reg [3:0] counter;
    
    wire Addressing_Mode = IROut[10]; // 10th bit of instruction is addressing mode
    wire instr_type = IROut[6]; // IROut[6] is 3rd bit of SREG1, and if it is 0 instruction is in form of figure 1 else figure 2
    wire opcode = IROut[15:12]; // IROut[15:12] is opcode
    wire RSEL = IROut[9:8]; // According to IROut[9:8] appropriate registers will be selected


    always@(posedge reset) begin
        counter <= 4'h0;


        RF_FunSel <= 2'b00;
        RF_RegSel <= 4'b1111;


        ARF_FunSel <= 2'b00;
        ARF_RegSel <= 4'b1111;


        IR_Enable <= 1'b1;
        IR_FunSel <= 2'b00;
    end


    always @(posedge clock) begin  
        if (!reset) begin
            counter <= counter + 1;
        end
    end

    always @(*) begin
        if (!reset) begin
            if (counter == 4'h0) begin
                LastFlag <= ALUFlag;


                Mem_CS <= 1'b0;
                Mem_WR <= 1'b0;
                IR_FunSel <= 2'b01;
                IR_Enable <= 1'b1;
                IR_LH <= 1'b0; //Load LSB

                ARF_OutBSel <= 2'b11; //Address is read from PC


                ARF_FunSel <= 2'b11;
                ARF_RegSel <= 4'b1000;


                RF_RegSel <= 4'b0000;
            end
            else if (counter == 4'h1) begin

                Mem_CS <= 1'b0;
                Mem_WR <= 1'b0;
                IR_FunSel <= 2'b01;
                IR_Enable <= 1'b1;
                IR_LH <= 1'b1; //Load MSB

                ARF_OutBSel <= 2'b11; //Address is read from Program Counter

                ARF_FunSel <= 2'b11;
                ARF_RegSel <= 4'b1000;

                RF_RegSel <= 4'b0000;
            end
            else begin
                if (counter == 4'h2)
                    IR_Enable <= 1'b0;

                case (opcode) // according to opcode, microoperations will be done
                    4'h0: begin
                        // AND
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b0: begin
                                    RF_RegSel <= 4'b0000;
                                    case (RSEL)
                                        2'b00: ARF_RegSel <= 4'b1000;
                                        2'b01: ARF_RegSel <= 4'b1000;
                                        2'b10: ARF_RegSel <= 4'b0100;
                                        2'b11: ARF_RegSel <= 4'b0010;
                                    endcase
                                    ARF_FunSel <= 2'b01;
                                end
                                1'b1: begin
                                    ARF_RegSel <= 4'b0000;
                                    case (RSEL)
                                        2'b00: RF_RegSel <= 4'b1000;
                                        2'b01: RF_RegSel <= 4'b0100;
                                        2'b10: RF_RegSel <= 4'b0010;
                                        2'b11: RF_RegSel <= 4'b0001;
                                    endcase
                                    RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            O2Sel <= IROut[1:0];
                            ALU_FunSel <= 4'b0111;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'h1: begin
                        // OR
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                    RF_RegSel <= 4'b0000;
                                    case (RSEL)
                                        2'b00: RF_RegSel <= 4'b1000;
                                        2'b01: RF_RegSel <= 4'b0100;
                                        2'b10: RF_RegSel <= 4'b0010;
                                        2'b11: RF_RegSel <= 4'b0001;
                                    endcase
                                    ARF_FunSel <= 2'b01;
                                end
                                1'b0: begin
                                    ARF_RegSel <= 4'b0000;
                                    case (RSEL)
                                        2'b00: RF_RegSel <= 4'b1000;
                                        2'b01: RF_RegSel <= 4'b0100;
                                        2'b10: RF_RegSel <= 4'b0010;
                                        2'b11: RF_RegSel <= 4'b0001;
                                    endcase
                                    RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            O2Sel <= IROut[1:0];
                            ALU_FunSel <= 4'b1000;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'h2: begin
                        // NOT
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0010;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'h3: begin
                        // ADD
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            O2Sel <= IROut[1:0];
                            ALU_FunSel <= 4'b0100;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'h4: begin
                        // SUB
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            O2Sel <= IROut[1:0];
                            ALU_FunSel <= 4'b0101;
                        end
                        if (counter == 4'h3) begin
                            MuxCSel <= Addressing_Mode;
                            case (Addressing_Mode)
                                1'b1: ARF_OutASel <= RSEL;
                                1'b0: O1Sel <= RSEL;
                            endcase
                            ALU_FunSel <= 4'b0010;
                        end
                        if (counter == 4'h4) begin
                            case (Addressing_Mode)
                                1'b1: ARF_FunSel <= 2'b00;
                                1'b0: RF_FunSel <= 2'b00;
                            endcase
                        end
                        if (counter == 4'h5) counter <= 0;
                    end
                    4'h5: begin
                        // LSR
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b1100;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'h6: begin
                        // LSL
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b1011;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'h7: begin
                        // INC
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0000;
                        end
                        if (counter == 4'h3) begin
                            case (Addressing_Mode)
                                1'b1: ARF_FunSel <= 2'b11;
                                1'b0: RF_FunSel <= 2'b11;
                            endcase
                        end
                        if (counter == 4'h4) begin
                            ARF_RegSel <= 4'b0000;
                            ARF_OutASel <= RSEL;
                            RF_RegSel <= 4'b0000;
                            O1Sel <= RSEL;
                            MuxCSel <= !Addressing_Mode;
                            ALU_FunSel <= 4'b0000;
                        end
                        if (counter == 4'h5) counter <= 0;
                    end
                    4'h8: begin
                        // DEC
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (RSEL)
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0000;
                        end
                        if (counter == 4'h3) begin
                            case (Addressing_Mode)
                                1'b1: ARF_FunSel <= 2'b10;
                                1'b0: RF_FunSel <= 2'b10;
                            endcase
                        end
                        if (counter == 4'h4) begin
                            ARF_RegSel <= 4'b1111;
                            ARF_OutASel <= RSEL;
                            RF_RegSel <= 4'b1111;
                            O1Sel <= RSEL;
                            MuxCSel <= !Addressing_Mode;
                            ALU_FunSel <= 4'b0000;
                        end
                        if (counter == 4'h5) counter <= 0;
                    end
                    4'h9: begin
                        // BRA
                        if (counter == 4'h2) begin
                            RF_RegSel <= 4'b0000;
                            ARF_RegSel <= 4'b1000;
                            ARF_FunSel <= 2'b01;
                            MuxBSel <= 2'b01;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'hA: begin
                        // BNE
                        if (counter == 4'h2) begin
                            RF_RegSel <= 4'b0000;
                            MuxBSel = 2'b01;
                            ARF_FunSel <= 2'b01;
                            if (LastFlag[3] == 0) begin
                                ARF_RegSel <= 4'b1000;
                            end
                            else begin
                                ARF_RegSel <= 4'b0000;
                            end
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'hB: begin 
                        // MOV
                        if (counter == 4'h2) begin
                            case (Addressing_Mode)
                                1'b1: begin
                                    RF_RegSel <= 4'b1111;
                                    case (RSEL)
                                        2'b00: ARF_RegSel <= 4'b1000;
                                        2'b01: ARF_RegSel <= 4'b1000;
                                        2'b10: ARF_RegSel <= 4'b0100;
                                        2'b11: ARF_RegSel <= 4'b0010;
                                    endcase
                                    ARF_FunSel <= 2'b01;
                                end
                                1'b0: begin
                                    ARF_RegSel <= 4'b0000;
                                    case (RSEL)
                                        2'b00: RF_RegSel <= 4'b0111;
                                        2'b01: RF_RegSel <= 4'b1011;
                                        2'b10: RF_RegSel <= 4'b1101;
                                        2'b11: RF_RegSel <= 4'b1110;
                                    endcase
                                    RF_FunSel <= 2'b01;
                                end
                            endcase
                            MuxASel <= 2'b11;
                            MuxBSel <= 2'b11;
                            MuxCSel <= instr_type;
                            case (instr_type)
                                1'b0: O1Sel <= IROut[5:4];
                                1'b1: ARF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0000;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'hC: begin
                        // LD
                        if (counter == 4'h2) begin
                            ARF_RegSel <= 4'b0000;
                            case (RSEL)
                                2'b00: RF_RegSel <= 4'b1000;
                                2'b01: RF_RegSel <= 4'b0100;
                                2'b10: RF_RegSel <= 4'b0010;
                                2'b11: RF_RegSel <= 4'b0001;
                            endcase
                            RF_FunSel <= 2'b01;
                            case (Addressing_Mode)
                                1'b1: begin
                                    MuxASel <= 2'b01;
                                    ARF_OutBSel <= 2'b00;
                                end
                                1'b0: MuxASel <= 2'b00;
                            endcase
                            Mem_WR <= 0;
                            Mem_CS <= 0;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'hD: begin
                        // ST
                        if (counter == 4'h2) begin
                            RF_RegSel <= 4'b0000;
                            ARF_RegSel <= 4'b0000;
                            ARF_OutBSel <= 2'b00;
                            O2Sel <= RSEL;
                            ALU_FunSel <= 4'b0001;
                            Mem_WR <= 1;
                            Mem_CS <= 0;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end                
                    4'hE: begin
                        // PUL
                        if (counter == 4'h2) begin
                            MuxASel <= 2'b01;
                            case (RSEL)
                                2'b00: RF_RegSel <= 4'b1000;
                                2'b01: RF_RegSel <= 4'b0100;
                                2'b10: RF_RegSel <= 4'b0010;
                                2'b11: RF_RegSel <= 4'b0001;
                            endcase
                            ARF_OutBSel <= 2'b01;
                            ARF_FunSel <= 2'b11;
                            ARF_RegSel <= 3'b0010;
                            Mem_CS <= 0;
                            Mem_WR <= 0;
                        end
                        if (counter == 4'h3) counter <= 0;
                    end
                    4'hF: begin
                        // PSH
                        if (counter == 4'h2) begin
                            ARF_FunSel <= 2'b10;
                            ARF_RegSel <= 3'b0010;
                            RF_RegSel <= 4'b0000;
                        end
                        if (counter == 4'h3) begin
                            ALU_FunSel <= 4'b0001;
                            O2Sel <= RSEL;
                            ARF_OutBSel <= 2'b11;
                            ARF_RegSel <= 4'b0000;
                            Mem_CS <= 0;
                            Mem_WR <= 1;
                        end
                        if (counter == 4'h4) counter <= 0;
                    end
                endcase
            end
        end
    end
    assign T = counter;
    ALUSystem _ALUSystem(
    .RF_OutASel(O1Sel), 
    .RF_OutBSel(O2Sel), 
    .RF_FunSel(RF_FunSel),
    .RF_RSel(RF_RegSel),
    .ALU_FunSel(ALU_FunSel),
    .ARF_OutCSel(ARF_OutASel), 
    .ARF_OutDSel(ARF_OutBSel), 
    .ARF_FunSel(ARF_FunSel),
    .ARF_RegSel(ARF_RegSel),
    .IR_LH(IR_LH),
    .IR_Enable(IR_Enable),
    .IR_Funsel(IR_FunSel),
    .Mem_WR(Mem_WR),
    .Mem_CS(Mem_CS),
    .MuxASel(MuxASel),
    .MuxBSel(MuxBSel),
    .MuxCSel(MuxCSel),
    .Clock(clock),
    .ALUOutFlag(ALUFlag),
    .IROut(IROut)
    );
    
        always @(negedge clock)
        if (~reset) // skip during reset
        begin
            $display("Input Values:");
            //$display("Operation: %d", Operation);
            $display("Register File: O1Sel: %d, O2Sel: %d, FunSel: %d, RSel: %d", O1Sel, O2Sel, RF_FunSel, RF_RegSel);            
            $display("ALU FunSel: %d", ALU_FunSel);
            $display("Addres Register File: OutASel: %d, OutBSel: %d, FunSel: %d, Regsel: %d", ARF_OutASel, ARF_OutBSel, ARF_FunSel, ARF_RegSel);            
            $display("Instruction Register: LH: %d, Enable: %d, FunSel: %d", IR_LH, IR_Enable, IR_FunSel);            
            $display("Memory: WR: %d, CS: %d", Mem_WR, Mem_CS);
            $display("MuxASel: %d, MuxBSel: %d, MuxCSel: %d", MuxASel, MuxBSel, MuxCSel);
            
            $display("");
            $display("Output Values:");
            $display("Register File: AOut: %d, BOut: %d", _ALUSystem.AOut, _ALUSystem.BOut);            
            $display("ALUOut: %d, ALUFlag: %d, ALUFlags: Z:%d, C:%d, N:%d, O:%d,", _ALUSystem.ALUOut, _ALUSystem.ALUOutFlag, _ALUSystem.ALUOutFlag[3],_ALUSystem.ALUOutFlag[2],_ALUSystem.ALUOutFlag[1],_ALUSystem.ALUOutFlag[0]);
            $display("Address Register File: AOut: %d, BOut (Address): %d", _ALUSystem.AOut, _ALUSystem.Address);            
            $display("Memory Out: %d", _ALUSystem.MemoryOut);            
            $display("Instruction Register: IROut: %d", _ALUSystem.IROut);            
            $display("MuxAOut: %d, MuxBOut: %d, MuxCOut: %d", _ALUSystem.MuxAOut, _ALUSystem.MuxBOut, _ALUSystem.MuxCOut);

        end
    
endmodule
