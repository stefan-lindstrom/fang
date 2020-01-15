#5900
Demandred's Room~
2 g 100
~
if (%actor.level% > 200 || %actor.name% == Demandred)
  return 1
else
  wsend %actor.name% A feeling of menace pervades the room, you feel faint, something deep and dark wrapping about you as all the light dissapears from your eyes! You awaken a short time later, finding yourself ... in hell!
  wteleport %actor.name% 6299
  return 0
end
~
#5901
Demandred's Room Part#2~
2 c 100
*~
if (%actor.level% > 200 || %actor.name% == Demandred)
  return 0
else
  wsend %actor.name% A feeling of menace pervades the room, you feel faint, something deep and dark wrapping about you as all the light dissapears from your eyes! You awaken a short time later, finding yourself ... in hell!
  wteleport %actor.name% 6299
  return 1
end
~
#5902
Demandreds On/off trigger~
2 d 1
on off~
if %speech% == on
  wsend %actor.name% \\c01On!\\c00
  attach 5900 %self.id%
  attach 5901 %self.id%
else
  wsend %actor.name% \\c01Off!\\c00
  detach 5900 %self.id%
  detach 5901 %self.id%
end
~
$~
