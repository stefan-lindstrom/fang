#2707
Sewing Box Trigger~
1 n 100
~
   oload in obj 2709
   oload in obj 2710
   oload in obj 2712
~
#2717
Ice Trigger~
2 g 100
~
if (%actor.level% < 200)
  if (%actor.vnum% == -1)
    wait 5
    %echoaround% %actor.name% %actor.name% slips on a treacherous patch of ice and lands on %actor.hisher% butt!
       %send% %actor.name% You lose your footing on the ice and land hard on your butt. Ouch!
       %damage% %actor.name% 50
    %force% %actor.name% sit
    if (%random.10% < 6)
      wait 2
              %echoaround% %actor.name% %actor.name% can't find anything to grab to stop %actor.himher%self and skids down the slope.
              %send% %actor.name% You start to slide and can't grab anything to halt your descent. You skid down the slope and land with a thud!
              %damage% %actor.name% 100
%teleport% %actor.name% 2718
    else
      end
   else
   end
   else
   end
~
#2718
Slippery Ice~
2 e 25
*~
   if (%actor.level% < 200)
     if (%actor.vnum% == -1)
return 0
       wait 5
       %echoaround% %actor.name% %actor.name% slips on a treacherous patch of ice and lands on %actor.hisher% butt!
          %send% %actor.name% You lose your footing on the ice and land hard on your butt. Ouch!
          %damage% %actor.name% 50
       %force% %actor.name% sit
       if (%random.10% < 6)
         wait 2
                 %echoaround% %actor.name% %actor.name% can't find anything to grab to stop %actor.himher%self and skids down the slope.
                 %send% %actor.name% You start to slide and can't grab anything to halt your descent. You skid down the slope and land with a thud!
                 %damage% %actor.name% 100
   %teleport% %actor.name% 2718
       else
         end
      else
      end
      else
      end
~
#2769
Candlemaker trigger~
0 j 100
~
if %object.vnum% == 2786
wait 1
say Thank you, %actor.name%. Here, have a fresh candle.
  give candle %actor.name%
else
   end
end
~
#2770
Jynthyn's Hint~
0 g 100
~
wait 1
   msend %actor.name% Jynthyn looks up from his work when you enter, nods a greeting to you, then turns his attention back to melting down old candle stubs.
mechoaround %actor.name% Jynthyn looks up from his work and nods a greeting to %actor.name%, then turns his attention back to melting down old candle stubs.
end
~
#2771
Tapered Candle trigger~
1 f 100
~
  oecho A tapered candle has burned down to a short ugly candle.
   otransform 2786
   detach 2771 %self.id%
   detach 2772 %self.id%
   attach 2786 %self.id%
   end
~
#2772
Tapered Candle trigger II~
1 j 100
~
otimer 50
~
#2786
Short Candle trigger~
1 i 100
~
   if %actor.vnum% == 2769
      otransform 2769
   detach 2786 %self.id%
   attach 2771 %self.id%
   attach 2772 %self.id%
   else
      end
   end
~
$~
