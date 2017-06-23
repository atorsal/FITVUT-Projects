-- fsm.vhd: Finite State Machine
-- Author(s): Dávid Bolvanský (xbolva00)
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST_1, TEST_2, TEST_3, TEST_4, TEST_A_5, TEST_A_6, TEST_A_7, TEST_A_8, TEST_A_9, TEST_B_5, TEST_B_6, TEST_B_7, TEST_B_8, TEST_B_9, TEST_B_10, FAILED, WAIT_TEST, PRINT_MSG_WRONG, PRINT_MSG_OK, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;
-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_1 =>
      next_state <= TEST_1;
      if (KEY(9) = '1') then
		   next_state <= TEST_2;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_2 =>
      next_state <= TEST_2;
      if (KEY(8) = '1') then
         next_state <= TEST_3;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_3 =>
      next_state <= TEST_3;
      if (KEY(5) = '1') then
         next_state <= TEST_4;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_4 =>
      next_state <= TEST_4;
      if (KEY(7) = '1') then
         next_state <= TEST_A_5;
		elsif (KEY(0) = '1') then
			next_state <= TEST_B_5;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_A_5 =>
      next_state <= TEST_A_5;
      if (KEY(2) = '1') then
         next_state <= TEST_A_6;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_A_6 =>
      next_state <= TEST_A_6;
      if (KEY(3) = '1') then
         next_state <= TEST_A_7;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_A_7 =>
      next_state <= TEST_A_7;
      if (KEY(2) = '1') then
         next_state <= TEST_A_8;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_A_8 =>
      next_state <= TEST_A_8;
      if (KEY(6) = '1') then
         next_state <= TEST_A_9;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_A_9 =>
      next_state <= TEST_A_9;
      if (KEY(6) = '1') then
         next_state <= WAIT_TEST;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_B_5 =>
      next_state <= TEST_B_5;
      if (KEY(0) = '1') then
         next_state <= TEST_B_6;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_B_6 =>
      next_state <= TEST_B_6;
      if (KEY(3) = '1') then
         next_state <= TEST_B_7;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_B_7 =>
      next_state <= TEST_B_7;
      if (KEY(1) = '1') then
         next_state <= TEST_B_8;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_B_8 =>
      next_state <= TEST_B_8;
      if (KEY(4) = '1') then
         next_state <= TEST_B_9;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_B_9 =>
      next_state <= TEST_B_9;
      if (KEY(3) = '1') then
         next_state <= TEST_B_10;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_B_10 =>
      next_state <= TEST_B_10;
      if (KEY(2) = '1') then
         next_state <= WAIT_TEST;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WAIT_TEST =>
      next_state <= WAIT_TEST;
      if (KEY(15) = '1') then
         next_state <= PRINT_MSG_OK;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= FAILED;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FAILED =>
      next_state <= FAILED;
      if (KEY(15) = '1') then
         next_state <= PRINT_MSG_WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MSG_WRONG =>
      next_state <= PRINT_MSG_WRONG;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MSG_OK =>
      next_state <= PRINT_MSG_OK;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST_1;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST_1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
    -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MSG_WRONG =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '0';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MSG_OK =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	  FSM_MX_MEM		<= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
			FSM_LCD_WR     <= '1';
      elsif (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;