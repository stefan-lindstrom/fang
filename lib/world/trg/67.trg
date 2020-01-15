#6700
&KDF GuildEq&n~
1 gij 100
~
      if (%actor.guild(21)% == Member)
return 1
else
return 0
wait 2
%echo% %actor.name% is \\c08NOT\\c00 Guilded in \\c23DarkFriends\\c00!
otransform 3580
attach 3550 %self.id%
attach 3551 %self.id%
attach 3552 %self.id%
~
#6701
&KChosen GeQ&n~
1 gij 100
~
      if (%actor.guild(7)% == Member)
return 1
else
return 0
wait 2
%echo% %actor.name% is \\c08NOT\\c00 Guilded in \\c23Chosen\\c00!
otransform 3580
attach 3550 %self.id%
attach 3551 %self.id%
attach 3552 %self.id%
~
$~
