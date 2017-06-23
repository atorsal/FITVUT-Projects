class Equation
  def self.solve_quadratic(a, b, c)
    if a == 0
    	if b == 0
            # premenne a, b su nuly
    		if c != 0
                # konstantna funkcia
    			return nil
            end
    	else
            # linearna funkcia
            x1 = (-c)/(b.to_f)
    		return [x1]
        end
    else
        # vypocet diskriminatu
    	d = (b*b) - (4*a*c)

        # diskriminat mensi ako 0, riesenie v komplexnych cislach
    	if d < 0
    		return nil
    	elsif d == 0
            #  d sa rovna 0, jedno riesenie
            x1 = (-b)/(2*a)
    		return [x1]
    	else
            # d je vacsie ako 0, rovnica ma dve riesenia
            d = Math.sqrt(d)
    		x1 = (-b+d)/(2*a)
    		x2 = (-b-d)/(2*a)
    		return [x1, x2]
        end
    end
  end
end
