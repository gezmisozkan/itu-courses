`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/16/2023 03:43:10 PM
// Design Name: 
// Module Name: HW2
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



module CPUSystem(
    input RESET,
    input CLOCK,    
    output [3:0]T
);
    reg Mem_WR, Mem_CS;
    reg [1:0] MuxASel;
    reg [1:0] MuxBSel;
    reg MuxCSel;    
    
    reg [2:0] RF_OutASel;
    reg [2:0] RF_OutBSel;
    reg [1:0] RF_FunSel;
    reg [3:0] RF_RegSel;
    
    reg [3:0] ALU_FunSel;
    
    reg [1:0] ARF_OutCSel;
    reg [1:0] ARF_OutDSel;
    reg [1:0] ARF_FunSel;
    reg [3:0] ARF_RegSel;
    
    reg IR_LH, IR_Enable;
    reg [1:0] IR_FunSel;

    wire [15:0] IROut;
    wire [3:0] ALUOutFlag;
    reg [3:0] LastALUOutFlag;

    reg [3:0] seq_counter;

    //Reset registers
    //Reset must be held in one clock cycle
    always@(posedge RESET) begin
        seq_counter <= 4'h0;

        //Reset Register File
        RF_FunSel <= 2'b00;
        RF_RegSel <= 4'b1111;

        //Reset Address Register File
        ARF_FunSel <= 2'b00;
        ARF_RegSel <= 4'b1111;

        //Reset Instruction Register
        IR_Enable <= 1'b1;
        IR_FunSel <= 2'b00;
    end

    //Increment sequence counter with every clock cycle, reset if operation is done 
    always @(posedge CLOCK) begin  
        if (!RESET) begin
            seq_counter <= seq_counter + 1;
        end
    end

    always @(*) begin
        if (!RESET) begin
            if (seq_counter == 4'h0) begin
                LastALUOutFlag <= ALUOutFlag;

                //Fetch: Load LSB of the instruction from RAM to IR
                Mem_CS <= 1'b0;
                Mem_WR <= 1'b0;
                IR_FunSel <= 2'b01;
                IR_Enable <= 1'b1;
                IR_LH <= 1'b0; //Load LSB

                ARF_OutDSel <= 2'b11; //Address is read from Program Counter

                //Increment PC
                ARF_FunSel <= 2'b11;
                ARF_RegSel <= 4'b1000;

                //Disable unused registers
                RF_RegSel <= 4'b0000;
            end
            else if (seq_counter == 4'h1) begin
                //Fetch: Load MSB of the instruction from RAM to IR
                Mem_CS <= 1'b0;
                Mem_WR <= 1'b0;
                IR_FunSel <= 2'b01;
                IR_Enable <= 1'b1;
                IR_LH <= 1'b1; //Load MSB

                ARF_OutDSel <= 2'b11; //Address is read from Program Counter

                //Increment PC
                ARF_FunSel <= 2'b11;
                ARF_RegSel <= 4'b1000;

                //Disable unused registers
                RF_RegSel <= 4'b0000;
            end
            else begin
                if (seq_counter == 4'h2)
                    IR_Enable <= 1'b0;

                case (IROut[15:12])
                    4'h0: begin
                        // AND
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b0: begin
                                    RF_RegSel <= 4'b0000;
                                    case (IROut[9:8])
                                        2'b00: ARF_RegSel <= 4'b1000;
                                        2'b01: ARF_RegSel <= 4'b1000;
                                        2'b10: ARF_RegSel <= 4'b0100;
                                        2'b11: ARF_RegSel <= 4'b0010;
                                    endcase
                                    ARF_FunSel <= 2'b01;
                                end
                                1'b1: begin
                                    ARF_RegSel <= 4'b0000;
                                    case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            RF_OutBSel <= IROut[1:0];
                            ALU_FunSel <= 4'b0111;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'h1: begin
                        // OR
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                    RF_RegSel <= 4'b0000;
                                    case (IROut[9:8])
                                        2'b00: RF_RegSel <= 4'b1000;
                                        2'b01: RF_RegSel <= 4'b0100;
                                        2'b10: RF_RegSel <= 4'b0010;
                                        2'b11: RF_RegSel <= 4'b0001;
                                    endcase
                                    ARF_FunSel <= 2'b01;
                                end
                                1'b0: begin
                                    ARF_RegSel <= 4'b0000;
                                    case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            RF_OutBSel <= IROut[1:0];
                            ALU_FunSel <= 4'b1000;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'h2: begin
                        // NOT
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (IROut[9:8])
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0010;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'h3: begin
                        // ADD
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (IROut[9:8])
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            RF_OutBSel <= IROut[1:0];
                            ALU_FunSel <= 4'b0100;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'h4: begin
                        // SUB
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (IROut[9:8])
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            RF_OutBSel <= IROut[1:0];
                            ALU_FunSel <= 4'b0101;
                        end
                        // Now we have SRCREG1 - SRCREG2 in DESTREG
                        if (seq_counter == 4'h3) begin
                            MuxCSel <= IROut[10];
                            case (IROut[10])
                                1'b1: ARF_OutCSel <= IROut[9:8]; // ???????????????? neden 2 bit
                                1'b0: RF_OutASel <= IROut[9:8];
                            endcase
                            ALU_FunSel <= 4'b0010;
                        end
                        // 1's Complement of DESTREG --> -DESTREG
                        if (seq_counter == 4'h4) begin
                            case (IROut[10])
                                1'b1: ARF_FunSel <= 2'b00;
                                1'b0: RF_FunSel <= 2'b00;
                            endcase
                        end
                        if (seq_counter == 4'h5) seq_counter <= 0;
                    end
                    4'h5: begin
                        // LSR
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (IROut[9:8])
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b1100;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'h6: begin
                        // LSL
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (IROut[9:8])
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b1011;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'h7: begin
                        // INC
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (IROut[9:8])
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0000;
                        end
                        if (seq_counter == 4'h3) begin
                            case (IROut[10])
                                1'b1: ARF_FunSel <= 2'b11;
                                1'b0: RF_FunSel <= 2'b11;
                            endcase
                        end
                        if (seq_counter == 4'h4) begin
                            // Additional cycle required to update the ALUOutFlag as this operation is done on the register instead of ALU.
                            ARF_RegSel <= 4'b0000;
                            ARF_OutCSel <= IROut[9:8];
                            RF_RegSel <= 4'b0000;
                            RF_OutASel <= IROut[9:8];
                            MuxCSel <= !IROut[10];
                            ALU_FunSel <= 4'b0000;
                        end
                        if (seq_counter == 4'h5) seq_counter <= 0;
                    end
                    4'h8: begin
                        // DEC
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                RF_RegSel <= 4'b0000;
                                case (IROut[9:8])
                                    2'b00: RF_RegSel <= 4'b1000;
                                    2'b01: RF_RegSel <= 4'b0100;
                                    2'b10: RF_RegSel <= 4'b0010;
                                    2'b11: RF_RegSel <= 4'b0001;
                                endcase
                                ARF_FunSel <= 2'b01;
                            end
                            1'b0: begin
                                ARF_RegSel <= 4'b0000;
                                case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0000;
                        end
                        if (seq_counter == 4'h3) begin
                            case (IROut[10])
                                1'b1: ARF_FunSel <= 2'b10;
                                1'b0: RF_FunSel <= 2'b10;
                            endcase
                        end
                        if (seq_counter == 4'h4) begin
                            // Additional cycle required to update the ALUOutFlag as this operation is done on the register instead of ALU.
                            ARF_RegSel <= 4'b1111;
                            ARF_OutCSel <= IROut[9:8];
                            RF_RegSel <= 4'b1111;
                            RF_OutASel <= IROut[9:8];
                            MuxCSel <= !IROut[10];
                            ALU_FunSel <= 4'b0000;
                        end
                        if (seq_counter == 4'h5) seq_counter <= 0;
                    end
                    4'h9: begin
                        // BRA
                        if (seq_counter == 4'h2) begin
                            RF_RegSel <= 4'b0000;
                            ARF_RegSel <= 4'b1000;
                            ARF_FunSel <= 2'b01;
                            MuxBSel <= 2'b01;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'hA: begin
                        // BNE
                        if (seq_counter == 4'h2) begin
                            RF_RegSel <= 4'b0000;
                            MuxBSel = 2'b01;
                            ARF_FunSel <= 2'b01;
                            if (LastALUOutFlag[3] == 0) begin
                                ARF_RegSel <= 4'b1000;
                            end
                            else begin
                                ARF_RegSel <= 4'b0000;
                            end
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'hB: begin 
                        // MOV
                        if (seq_counter == 4'h2) begin
                            case (IROut[10])
                                1'b1: begin
                                    RF_RegSel <= 4'b1111;
                                    case (IROut[9:8])
                                        2'b00: ARF_RegSel <= 4'b1000;
                                        2'b01: ARF_RegSel <= 4'b1000;
                                        2'b10: ARF_RegSel <= 4'b0100;
                                        2'b11: ARF_RegSel <= 4'b0010;
                                    endcase
                                    ARF_FunSel <= 2'b01;
                                end
                                1'b0: begin
                                    ARF_RegSel <= 4'b0000;
                                    case (IROut[9:8])
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
                            MuxCSel <= !IROut[6];// !!!!!!!!!!!!!!!!!!
                            case (IROut[6])
                                1'b0: ARF_OutCSel <= IROut[5:4];
                                1'b1: RF_OutASel <= IROut[5:4];
                            endcase
                            ALU_FunSel <= 4'b0000;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'hC: begin
                        // LD
                        if (seq_counter == 4'h2) begin
                            ARF_RegSel <= 4'b0000;
                            case (IROut[9:8])
                                2'b00: RF_RegSel <= 4'b1000;
                                2'b01: RF_RegSel <= 4'b0100;
                                2'b10: RF_RegSel <= 4'b0010;
                                2'b11: RF_RegSel <= 4'b0001;
                            endcase
                            RF_FunSel <= 2'b01;
                            case (IROut[10])
                                1'b1: begin
                                    MuxASel <= 2'b01;
                                    ARF_OutDSel <= 2'b00;
                                end
                                1'b0: MuxASel <= 2'b00;
                            endcase
                            Mem_WR <= 0;
                            Mem_CS <= 0;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'hD: begin
                        // ST
                        if (seq_counter == 4'h2) begin
                            RF_RegSel <= 4'b0000;
                            ARF_RegSel <= 4'b0000;
                            ARF_OutDSel <= 2'b00;
                            RF_OutBSel <= IROut[9:8];
                            ALU_FunSel <= 4'b0001;
                            Mem_WR <= 1;
                            Mem_CS <= 0;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end                
                    4'hE: begin
                        // PUL
                        if (seq_counter == 4'h2) begin
                            MuxASel <= 2'b01;
                            case (IROut[9:8])
                                2'b00: RF_RegSel <= 4'b1000;
                                2'b01: RF_RegSel <= 4'b0100;
                                2'b10: RF_RegSel <= 4'b0010;
                                2'b11: RF_RegSel <= 4'b0001;
                            endcase
                            ARF_OutDSel <= 2'b01;
                            ARF_FunSel <= 2'b11;
                            ARF_RegSel <= 3'b0010;
                            Mem_CS <= 0;
                            Mem_WR <= 0;
                        end
                        if (seq_counter == 4'h3) seq_counter <= 0;
                    end
                    4'hF: begin
                        // PSH
                        if (seq_counter == 4'h2) begin
                            ARF_FunSel <= 2'b10;
                            ARF_RegSel <= 3'b0010;
                            RF_RegSel <= 4'b0000;
                        end
                        if (seq_counter == 4'h3) begin
                            ALU_FunSel <= 4'b0001;
                            RF_OutBSel <= IROut[9:8];
                            ARF_OutDSel <= 2'b11;
                            ARF_RegSel <= 4'b0000;
                            Mem_CS <= 0;
                            Mem_WR <= 1;
                        end
                        if (seq_counter == 4'h4) seq_counter <= 0;
                    end
                endcase
            end
        end
    end
    assign T = seq_counter;
    ALUSystem _ALUSystem(
    .RF_OutASel(RF_OutASel), 
    .RF_OutBSel(RF_OutBSel), 
    .RF_FunSel(RF_FunSel),
    .RF_RSel(RF_RegSel),
    .ALU_FunSel(ALU_FunSel),
    .ARF_OutCSel(ARF_OutCSel), 
    .ARF_OutDSel(ARF_OutDSel), 
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
    .Clock(CLOCK),
    .ALUOutFlag(ALUOutFlag),
    .IROut(IROut)
    );
    
        always @(negedge CLOCK)
        if (~RESET) // skip during reset
        begin
            $display("Input Values:");
            //$display("Operation: %d", Operation);
            $display("Register File: O1Sel: %d, O2Sel: %d, FunSel: %d, RSel: %d", RF_OutASel, RF_OutBSel, RF_FunSel, RF_RegSel);            
            $display("ALU FunSel: %d", ALU_FunSel);
            $display("Addres Register File: OutASel: %d, OutBSel: %d, FunSel: %d, Regsel: %d", ARF_OutCSel, ARF_OutDSel, ARF_FunSel, ARF_RegSel);            
            $display("Instruction Register: LH: %d, Enable: %d, FunSel: %d", IR_LH, IR_Enable, IR_FunSel);            
            $display("Memory: WR: %d, CS: %d", Mem_WR, Mem_CS);
            $display("MuxASel: %d, MuxBSel: %d, MuxCSel: %d", MuxASel, MuxBSel, MuxCSel);
            
            $display("");
            $display("Output Values:");
            $display("Register File: AOut: %d, BOut: %d", _ALUSystem.AOut, _ALUSystem.BOut);            
            $display("ALUOut: %d, ALUOutFlag: %d, ALUOutFlags: Z:%d, C:%d, N:%d, O:%d,", _ALUSystem.ALUOut, _ALUSystem.ALUOutFlag, _ALUSystem.ALUOutFlag[3],_ALUSystem.ALUOutFlag[2],_ALUSystem.ALUOutFlag[1],_ALUSystem.ALUOutFlag[0]);
            $display("Address Register File: AOut: %d, BOut (Address): %d", _ALUSystem.AOut, _ALUSystem.Address);            
            $display("Memory Out: %d", _ALUSystem.MemoryOut);            
            $display("Instruction Register: IROut: %d", _ALUSystem.IROut);            
            $display("MuxAOut: %d, MuxBOut: %d, MuxCOut: %d", _ALUSystem.MuxAOut, _ALUSystem.MuxBOut, _ALUSystem.MuxCOut);

        end
    
endmodule
