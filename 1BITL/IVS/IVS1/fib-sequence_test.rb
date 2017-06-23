require 'test/unit'
require_relative 'fib-sequence'

class FibonacciSequenceTest < Test::Unit::TestCase
  def setup
      @fib =  FibonacciSequence.new
      @fib2 =  FibonacciSequence.new(3,5)
  end
  
  # Overenie spravnosti funkcie current_idx, ktora vracia aktualney clen fib postupnosti
  def test_current
     for i in 0..9
         @fib.next
     end
     assert_equal(@fib.[](9), @fib.current)
     assert_equal(34, @fib.current)
  end

  # Overenie spravnosti inicializacie
  def test_initialization
     assert_equal(nil, @fib.current)
     assert_equal(nil, @fib.current_idx)
  end
  
  # Overenie spravnosti funkcie current_idx, ktora vracia index dalsieho clena fib postupnosti
  def test_current_idx
     @fib.next # idx = 0, value should be 0
     @fib2.next
     assert_equal(0, @fib.current_idx)
     assert_equal(0, @fib2.current_idx)
     @fib.[](11)
     @fib2.[](11)
     assert_equal(11, @fib.current_idx)
     assert_equal(11, @fib2.current_idx)
     
  end

  # Overenie spravnosti funkcie next, ktora vracia dalsi clen fib postupnosti
  def test_next
     for i in 0..15
         @fib.next 
         @fib2.next
     end 
     assert_equal(610, @fib.current)
     assert_equal(15, @fib.current_idx)
     #assert_equal(4181, @fib2.current)
     assert_equal(15, @fib2.current_idx)
  end
 
  # Overene spravnosti funkcie [], ktora vracia hodnotu fib postupnost
  # pre dany n clen
  def test_brackets
     assert_equal(nil, @fib.[](-1))
     assert_equal(13, @fib.[](7))
     assert_equal(89, @fib2.[](7))
     assert_equal(0, @fib.[](0))
  end
end