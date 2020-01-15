#1000
White Tower Guild EQ~
1 gij 100
~
      if (%actor.guild(3)% == Member)
return 1
else
return 0
wait 2
%echo% %actor.name% is \\c08NOT\\c00 Guilded in the \\c14White Tower!\\c00
otransform 3580
attach 3550 %self.id%
attach 3551 %self.id%
attach 3552 %self.id%
~
$~
