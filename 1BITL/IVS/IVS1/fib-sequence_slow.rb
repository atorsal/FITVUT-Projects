
# indexovani clenu posloupnosti vychazi z http://en.wikipedia.org/wiki/Fibonacci_sequence
# tj. prvni clen ma index 0

class FibonacciSequence
  # konstruktor, hodnoty prvnich dvou clenu lze nastavit (implicitne 0, 1)
  def initialize( f0 = 0, f1 = 1 )
    @seq = [f0, f1]
    @count = 0
    self.reset
  end
  
  # nastavi radu do vychoziho stavu 
  def reset
    @idx = -1
    
    return true
  end
  
  # vrati dalsi fibonacciho cislo
  def next
    @idx += 1
    if @idx < 2
      return @seq[@idx]
    end
    
    i = 2
    while i <= @idx do
      @count += 1
      @seq[i] = @seq[i-1] + @seq[i-2]
      i += 1
    end
    
    return @seq[i-1]
  end
  
  # aktualni fibonacciho cislo
  def current
    if @idx < 0
      return nil
    end
    tmp = @idx
    @idx = 0

    while @idx < tmp
      self.next
    end

    return @seq[@idx]
  end
  
  # aktualni index (vraci nil, pokud se jeste nezacalo generovat)
  def current_idx
    if @idx < 0
      return nil
    end
    return @idx
  end
  
  # vrati fibonacciho cislo s danym indexem
  def [](n)
    return nil if n < 0
    @idx = 0
    while @idx < n
      self.next
    end
    
    return self.current
  end
end