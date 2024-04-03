`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/01/2023 11:54:03 AM
// Design Name: 
// Module Name: test
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

module register_tb;
  // Parameters
  localparam N = 4;

  // Inputs
  reg clk;
  reg [1:0] funsel;
  reg enable;
  reg [N-1:0] data_in;

  // Outputs
  wire [N-1:0] data_out;

  // Instantiate the module
  register #(.N(N)) uut (.clk(clk),.funsel(funsel),.enable(enable),.data_in(data_in),.data_out(data_out));

  // Clock generation
  always #25 clk = ~clk;

  // Stimulus
  initial begin
    clk=0; #10;
    funsel=2'b00; enable=1; data_in=4'd10; #50;
    funsel=2'b01; enable=1; data_in=4'd10; #50;
    funsel=2'b10; enable=1; data_in=4'd15; #50;
    funsel=2'b00; enable=0; data_in=4'd0; #50;
    funsel=2'b11; enable=1; data_in=4'd7; #50;
    funsel=2'b11; enable=1; data_in=4'd7; #50;
  end
endmodule


module IR_tb;
  // Inputs
  reg clk;
  reg [1:0] funsel;
  reg enable;
  reg [7:0] data_in;
  reg LH;
  // Outputs
  wire [15:0] data_out;

  // Instantiate the module
  IR_16 uut (.clk(clk),.funsel(funsel),.enable(enable),.LH(LH),.data_in(data_in),.data_out(data_out)
  );

  // Clock generation
  always #25 clk = ~clk;

  // Stimulus
  initial begin
    clk=0; LH=0; #10;
  
    funsel=2'b00; enable=1; data_in=8'b10; #50;
    funsel=2'b01; enable=1; LH=0; data_in=8'b11001001; #50;
    funsel=2'b10; enable=0;       #50;
    funsel=2'b10; enable=1;       #50;
    funsel=2'b10; enable=1;       #50;
    funsel=2'b01; enable=1; LH=1; data_in=8'b00110000; #50;
    funsel=2'b11; enable=1;       #50;
  end
endmodule


module part2b_tb ();
    reg clk;
    reg [7:0]data_in;
    reg [2:0] O1sel;
    reg [2:0] O2sel;
    reg [1:0] funsel;
    reg [3:0] Rsel;
    reg [3:0] Tsel;
    wire [7:0] O1;
    wire [7:0] O2;
        
    part2b uut(.clk(clk),.data_in(data_in),.O1sel(O1sel),.O2sel(O2sel),.funsel(funsel),.Rsel(Rsel),.Tsel(Tsel),.O1(O1),.O2(O2));
    
      // Clock generation
    always #20 clk = ~clk;
    
    initial begin
        clk=1; #15; 
        data_in=8'd18 ; O1sel=3'd5; O2sel=3'd1; funsel=2'd0; Rsel=4'd15; Tsel=4'd15; #50;
        //data_in=8'd18 ; O1sel=3'd5; O2sel=3'd1; funsel=2'd0; Rsel=4'd15; Tsel=4'd15; #50;
        data_in=8'd18 ; O1sel=3'd5; O2sel=3'd1; funsel=2'd1; Rsel=4'd5; Tsel=4'd4;   #50;
        data_in=8'd25 ; O1sel=3'd7; O2sel=3'd3; funsel=2'd3; Rsel=4'd1; Tsel=4'd8;   #50;
        data_in=8'd25 ; O1sel=3'd7; O2sel=3'd0; funsel=2'd0; Rsel=4'd0; Tsel=4'd1;   #50;
        data_in=8'd25 ; O1sel=3'd6; O2sel=3'd0; funsel=2'd3; Rsel=4'd10; Tsel=4'd1;  #50;
        //data_in=8'd ; O1sel=3'd; O2sel=3'd; funsel=2'd; Rsel=4'd; Tsel=4'd; #50;
        //data_in=8'd ; O1sel=3'd; O2sel=3'd; funsel=2'd; Rsel=4'd; Tsel=4'd; #50;
        //data_in=8'd ; O1sel=3'd; O2sel=3'd; funsel=2'd; Rsel=4'd; Tsel=4'd; #50;
    end
endmodule


module part2c_tb ();
    reg clk;
    reg [7:0]  data_in;
    reg [1:0]  OutAsel;
    reg [1:0]  OutBsel;
    reg [1:0]  funsel;
    reg [3:0]  Rsel;
    wire [7:0] O1;
    wire [7:0] O2;
    part2c uut (.clk(clk),.data_in(data_in),.OutAsel(OutAsel),.OutBsel(OutBsel),.funsel(funsel),.Rsel(Rsel),.OutA(O1),.OutB(O2));
 
      // Clock generation
    always #20 clk = ~clk;
    
    initial begin
        clk=1; #15; 
        data_in=8'd18 ; OutAsel=2'd2; OutBsel=2'd1; funsel=2'd0; Rsel=4'd15; #50;
        data_in=8'd18 ; OutAsel=2'd0; OutBsel=2'd1; funsel=2'd1; Rsel=4'd6;  #50;
        data_in=8'd0  ; OutAsel=2'd1; OutBsel=2'd2; funsel=2'd3; Rsel=4'd5;  #50;
        data_in=8'd2  ; OutAsel=2'd3; OutBsel=2'd1; funsel=2'd2; Rsel=4'd15;  #50;
        //data_in=8'd12 ; OutAsel=2'd; OutBsel=2'd; funsel=2'd3; Rsel=4'd10; #50;
    end
endmodule


module ALU_tb();
    reg [3:0]funsel;
    reg [7:0]A;
    reg [7:0]B;
    wire [3:0]flag;
    wire [7:0]OutALU;
    
    ALU uut(.funsel(funsel),.A(A),.B(B),. flag(flag),.OutALU(OutALU));
   
    initial begin
         A=8'd15 ;           B=8'd120;         funsel=4'd0;  #50;
         A=8'd15 ;           B=8'b11001001;    funsel=4'd3;  #50;
         A=8'd12 ;           B=8'd18;          funsel=4'd4;  #50;
         A=8'd127;           B=8'd2;           funsel=4'd4;  #50;
         A=8'd18 ;           B=8'd18;          funsel=4'd6;  #50;    
         A=8'b01010101 ;     B=8'b01001101;    funsel=4'd7;  #50;    
    
         A=8'b01010001 ;     B=8'b01001101;    funsel=4'd10; #50;
         A=8'd15 ;           B=8'b11001001;    funsel=4'd15; #50;
         A=8'b01010101 ;     B=8'd18;          funsel=4'd13; #50;
         A=8'b10101010 ;     B=8'd18;          funsel=4'd13; #50;
         A=8'b00101010 ;     B=8'd18;          funsel=4'd13; #50;
         A=8'd127 ;          B=8'd2;           funsel=4'd9;  #50;
         A=8'b10000001 ;     B=8'b00000010;    funsel=4'd5;  #50;    
         A=8'b01010101 ;     B=8'b01001101;    funsel=4'd14; #50;   
     
         A=8'b11001000 ;     B=8'b10001100;    funsel=4'd4;  #50;    
         A=8'b11101000 ;     B=8'd104;         funsel=4'd5;  #50;
         A=8'd100 ;          B=8'd15;          funsel=4'd4;  #50;    
         
         A=8'b10001000 ;     B=8'b10101100;    funsel=4'd8;  #50;
         A=8'b11001001 ;     B=8'd15;          funsel=4'd11; #50;    
         A=8'b11001001 ;     B=8'd15;          funsel=4'd12; #45;
    end 
endmodule

