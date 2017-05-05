-- INP Projekt 1: Maticovy displej
-- Oktober 2016
-- Autor: David Bolvansky (xbolva00)

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
		ROW		: out std_logic_vector (0 to 7);
		LED		: out std_logic_vector (0 to 7);
		RESET	: in std_logic;
        SMCLK : in std_logic

);
end ledc8x8;

architecture main of ledc8x8 is

    signal lights: std_logic_vector(7 downto 0);
    signal rows: std_logic_vector(7 downto 0);
    signal ce_cnt: std_logic_vector(7 downto 0);
    signal ce: std_logic;
    
    signal sec: std_logic;
    signal period_cnt: std_logic_vector(22 downto 0);

begin

    -- znizenie frekvencie
    ce_gen: process(SMCLK, RESET)
    begin
		if RESET = '1' then
			ce_cnt <= "00000000";
        elsif SMCLK'event and SMCLK = '1' then
            ce_cnt <= ce_cnt + 1;
        end if;
    end process ce_gen;
    ce <= '1' when ce_cnt = X"FF" else '0';  -- aktivne pri 256 (FF) periode
    
    
    rotate_rows: process(RESET, ce, SMCLK)
	begin	
		if RESET = '1' then
			rows <= "10000000"; 
		elsif SMCLK'event and SMCLK = '1' and ce = '1' then
			rows <= rows(0) & rows(7 downto 1); -- posun riadkov
		end if;
    end process rotate_rows;
    
    sec_gen: process(SMCLK,RESET)
    begin
        if RESET = '1' then
           period_cnt <= "00000000000000000000000";
        elsif SMCLK'event and SMCLK = '1' then
           period_cnt <= period_cnt + 1;
            if period_cnt = "11100001000000000000000" then
                sec <= not sec; 
                period_cnt <= "00000000000000000000000";   
         end if;    
      end if;  
    end process sec_gen; 
    
    decode: process(rows, sec)
	begin
    
        if sec = '1' then
            case rows is --- pre D
				when "10000000" => lights <= "00000011";
				when "01000000" => lights <= "01111101";
				when "00100000" => lights <= "01111101";
				when "00010000" => lights <= "01111101";
				when "00001000" => lights <= "01111101";
				when "00000100" => lights <= "01111101";
				when "00000010" => lights <= "00000011";
                when "00000001" => lights <= "11111111";
                when others =>  lights <= "11111111";
            end case;
        
        else 
            case rows is --- pre B
				when "10000000" => lights <= "00000011";
				when "01000000" => lights <= "01111101";
				when "00100000" => lights <= "01111101";
				when "00010000" => lights <= "00000011";
				when "00001000" => lights <= "01111101";
				when "00000100" => lights <= "01111101";
				when "00000010" => lights <= "00000011";
                when "00000001" => lights <= "11111111";
                when others =>  lights <= "11111111";
            end case;
        end if;
    end process decode;
    
    ROW <= rows;
    LED <= lights;

end main;
