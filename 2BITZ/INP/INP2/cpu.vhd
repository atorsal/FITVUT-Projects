-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Dávid Bolvanský xbolva00
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is


signal reg_pc_addr : std_logic_vector(11 downto 0);
signal reg_pc_inc : std_logic; 
signal reg_pc_dec : std_logic;

 
signal ptr_data_address : std_logic_vector(9 downto 0);
signal reg_ptr_inc : std_logic;
signal reg_ptr_dec : std_logic;
 

signal reg_cnt_counter : std_logic_vector(7 downto 0);
signal reg_cnt_inc : std_logic;
signal reg_cnt_dec : std_logic;
signal set_cnt : std_logic;

signal tmp_value : std_logic_vector(7 downto 0);
signal reg_tmp : std_logic_vector(7 downto 0);
signal load_from_tmp : std_logic; 

signal memory_sel : std_logic_vector(1 downto 0); 

-- BrainLove <3
type instruction_type is (
pc_inc, pc_dec, ptr_inc,ptr_dec, 
while_begin,while_end, 
write_char,read_char,
tmp_save, tmp_write, 
halt, nop
);
signal instruction : instruction_type; 


-- FSM states
type fsm_state is (idle,fetch,decode,pc_inc,pc_dec,ptr_inc,ptr_inc_exec,
ptr_dec,ptr_dec_exec, while_begin,while_1,while_2,while_3,while_4,
while_end,e_while_1,e_while_2,e_while_3,e_while_4,e_while_5,e_while_6,
write_char,write_char_do,write_char_exec,read_char,read_char_exec,tmp_save,tmp_save_exec,tmp_write,  
halt, nop);

signal pstate : fsm_state;
signal nstate : fsm_state;


begin

CODE_ADDR <= reg_pc_addr;
DATA_ADDR <= ptr_data_address;
-- output to LCD
OUT_DATA <= DATA_RDATA;

-- PC register
pc_reg: process(CLK, RESET, reg_pc_addr)
begin
    if(RESET = '1') then
        reg_pc_addr <= (others => '0');
    elsif (CLK'event) and (CLK = '1') then
        if (reg_pc_inc = '1') then
            reg_pc_addr <= reg_pc_addr + 1;
        elsif (reg_pc_dec = '1') then
            reg_pc_addr <= reg_pc_addr - 1;
        end if;
     end if;
end process; 

ptr_reg: process(CLK, RESET, ptr_data_address)
begin
    if(RESET = '1') then
        ptr_data_address <= (others => '0');
    elsif (CLK'event) and (CLK = '1') then
        if (reg_ptr_inc = '1') then
            ptr_data_address <= ptr_data_address + 1;
        elsif (reg_ptr_dec = '1') then
           ptr_data_address <= ptr_data_address - 1;
        end if;
     end if;
end process; 

-- counting brackets
cnt_reg: process(CLK,RESET,set_cnt)
begin
    if(RESET = '1') then
        reg_cnt_counter <= (others => '0');
    elsif (CLK'event) and (CLK = '1') then
        if (reg_cnt_inc = '1') then
            reg_cnt_counter <= reg_cnt_counter + 1;
        elsif (reg_cnt_dec = '1') then
            reg_cnt_counter <= reg_cnt_counter - 1;
        end if;
    end if;
    if(set_cnt = '1') then
        reg_cnt_counter(0) <= '1';
    end if;   
end process; 

tmp_reg: process(CLK,RESET,tmp_value)
begin
    if(RESET = '1') then
        tmp_value <= (others => '0');
    elsif (CLK'event) and (CLK = '1') then
        if(load_from_tmp = '1') then
            tmp_value <=  DATA_RDATA;
        end if;
     end if;
     reg_tmp <= tmp_value;
end process; 

mpx_memory: process (DATA_RDATA,IN_DATA,memory_sel,CLK,reg_tmp)
begin
    case memory_sel is
        when "00" => DATA_WDATA <= IN_DATA;
        when "01" => DATA_WDATA <= DATA_RDATA - 1;
        when "10" => DATA_WDATA <= DATA_RDATA + 1;
        when "11" => DATA_WDATA <= reg_tmp;
        when others =>
     end case;
end process;

instruction_decoder: process (CODE_DATA)
begin
    case CODE_DATA is
        when X"3E"  => instruction <= pc_inc; -- >
        when X"3C"  => instruction <= pc_dec; -- <
        when X"2B"  => instruction <= ptr_inc; -- +
        when X"2D"  => instruction <= ptr_dec; -- -
        when X"5B"  => instruction <= while_begin; -- [
        when X"5D"  => instruction <= while_end; -- ]
        when X"2E"  => instruction <= write_char; -- .
        when X"2C"  => instruction <= read_char; -- ,
        when X"24"  => instruction <= tmp_save; -- $
        when X"21"  => instruction <= tmp_write; -- !
        when X"00"  => instruction <= halt; -- null
        when others => instruction <= nop; -- other 
      end case;
end process;


fsm_pstate : process(CLK, RESET)
begin
    if(RESET = '1') then
        pstate <= idle;
    elsif (CLK'event) and (CLK = '1') then
        if(EN = '1') then
            pstate <= nstate;
        end if;
    end if;
end process;
 

-- FSM
fsm : process (pstate,instruction,CODE_DATA, DATA_RDATA, IN_VLD, OUT_BUSY, memory_sel, reg_cnt_counter)
begin

-- INITIAL SETTINGS
CODE_EN <= '0';
DATA_EN <= '0';
DATA_RDWR <= '0';
IN_REQ <= '0'; 
OUT_WE <= '0'; 

reg_pc_inc <= '0';
reg_pc_dec <= '0';
reg_ptr_inc <= '0';
reg_ptr_dec <= '0';
reg_cnt_inc <= '0';
reg_cnt_dec <= '0';
load_from_tmp <= '0';

memory_sel <= "00";
set_cnt <= '0';


case pstate is
when idle => 
    nstate <= fetch;

when fetch =>
    nstate <= decode;
    CODE_EN <= '1';   

when decode =>
    case instruction is
        when pc_inc =>
            nstate <= pc_inc;
        when pc_dec =>
            nstate <= pc_dec;
        when ptr_inc =>
            nstate <= ptr_inc;
        when ptr_dec =>
            nstate <= ptr_dec;
        when while_begin =>
            nstate <= while_begin;
        when while_end =>
            nstate <= while_end;
        when write_char =>
            nstate <= write_char;
        when read_char =>
            nstate <= read_char;
        when tmp_save =>
            nstate <= tmp_save;
        when tmp_write =>
            nstate <= tmp_write;
        when halt =>
            nstate <= halt;
        when nop =>
            nstate <= nop;
     end case;   
when pc_inc =>
     nstate <= fetch;
     reg_ptr_inc <= '1';
     reg_pc_inc <= '1'; 
   

when pc_dec =>
     nstate <= fetch;
     reg_ptr_dec <= '1';  
     reg_pc_inc <= '1';  
       
when ptr_inc => 
    nstate <= ptr_inc_exec;
    DATA_EN <= '1'; 
    DATA_RDWR <= '1'; 
    memory_sel <= "10";
  
when ptr_inc_exec =>
    nstate <= fetch;
    DATA_EN <= '1';
    DATA_RDWR <= '0';
    memory_sel <= "10";  
    reg_pc_inc <= '1';
    
when ptr_dec =>
    nstate <= ptr_dec_exec;
    DATA_EN <= '1';
    DATA_RDWR <= '1';
    
when ptr_dec_exec =>
    nstate <= fetch;
    DATA_EN <= '1';
    DATA_RDWR <= '0'; 
    memory_sel <= "01";
    reg_pc_inc <= '1';
   
-- write char
when write_char =>
    if(OUT_BUSY = '0') then
        nstate <= write_char_do;
     else 
        nstate <= write_char;   
    end if;
      
when write_char_do => 
        nstate <= write_char_exec;  
        DATA_EN <= '1';
        DATA_RDWR <= '1';
 
when write_char_exec =>
        nstate <= fetch;    
        OUT_WE <= '1';
        reg_pc_inc <= '1';
     
-- read char     
when read_char =>
    IN_REQ <= '1';
    if(IN_VLD = '1') then
        nstate <= read_char_exec;
    else
        nstate <= read_char;
    end if;

when read_char_exec =>    
       nstate <= fetch;
       DATA_EN <= '1';
       DATA_RDWR <= '0';
       memory_sel <= "00";
       reg_pc_inc <= '1';
    
-- while cycle        
when while_begin =>
   nstate <= while_1;
   reg_pc_inc <= '1'; 
   DATA_EN <= '1'; 
   DATA_RDWR <= '1';

when while_1 =>
    if(DATA_RDATA = "00000000") then
      set_cnt <= '1';
      nstate <= while_2;
    else
      nstate <= fetch;
    end if; 

when while_2 =>
   if(reg_cnt_counter = "00000000" ) then 
       nstate <= fetch;   
   else
       nstate <= while_3;
   end if;    

when while_3 =>
    nstate <= while_4;
    CODE_EN <= '1'; 
    
    
when while_4 =>     
    nstate <= while_2;  
    if(CODE_DATA = X"5B") then
      reg_cnt_inc <= '1';      
    elsif(CODE_DATA = X"5D") then
      reg_cnt_dec <= '1';         
    end if; 
    reg_pc_inc <= '1';
   
when while_end => 
    nstate <= e_while_1;
    DATA_EN <= '1'; 
    DATA_RDWR <= '1'; 

when e_while_1 =>
    if (DATA_RDATA = "00000000") then
      nstate <= fetch;
      reg_pc_inc <= '1';
    else
      nstate <= e_while_2;   
    end if;

when e_while_2 =>
    nstate <= e_while_3;
    set_cnt <= '1';
    reg_pc_dec <= '1';

when e_while_3 =>
    if (reg_cnt_counter = "00000000") then 
       nstate <= fetch;
    else
       nstate <= e_while_4;
    end if;           
      
when e_while_4 =>
     nstate <= e_while_5;
     CODE_EN <= '1';    

when e_while_5 =>
    nstate <= e_while_6;
    if (CODE_DATA = X"5D") then
      reg_cnt_inc <= '1';      
    elsif (CODE_DATA = X"5B") then
      reg_cnt_dec <= '1';    
    end if;
    
when e_while_6 =>
    nstate <= e_while_3;
   if (reg_cnt_counter = "00000000") then
      reg_pc_inc <= '1';   
   else
      reg_pc_dec <= '1';
   end if;

-- save temp      
when tmp_save =>
    nstate <= tmp_save_exec;
    DATA_EN <= '1';
    DATA_RDWR <= '1';

    
when tmp_save_exec =>
    nstate <= fetch;    
    load_from_tmp <= '1';
    reg_pc_inc <= '1';

-- write temp    
when tmp_write =>
    nstate <= fetch;
    DATA_EN <= '1';
    memory_sel <= "11";
    reg_pc_inc <= '1';
       
when halt => 
    nstate <= halt;

when nop =>
    nstate <= fetch;
    reg_pc_inc <= '1';
      
end case;

end process;

end behavioral;
 
