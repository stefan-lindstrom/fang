#12000
&RBand of the Red Hand GEQ&n~
1 gij 100
~
   if (%actor.guild(2)% == Member)
return 1
else
return 0
wait 2
%echo% %actor.name% is \\c08NOT\\c00 Guilded in the \\c14Band of the Red Hand!\\c00
otransform 3580
attach 3550 %self.id%
attach 3551 %self.id%
attach 3552 %self.id%
~
#12001
Geq#2~
1 gij 100
~
   if (%actor.name% == Mat)
return 1
else
return 0
wait 2
%send% %actor.name% You can \\c08NOT\\c00 use this equipment.
otransform 3580
attach 3550 %self.id%
attach 3551 %self.id%
attach 3552 %self.id%
~
#12199
Mat's Medallion - Wear Trigger~
1 gj 100
~
if (%actor.level% < 200)
   if (%actor.name% == Mat)
      return 1
   else
      %send% %actor.name% You can't do that.
      return 0
   end
else
   return 1
end
~
$~
