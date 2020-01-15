#25605
Greeter1~
0 b 1
~
emote gazes around, looking for any sign of wrongdoing.
remove billet
~
#25606
Greeter2~
0 b 1
~
emote peers around, clapping a hand on his billet as he walks. "&WLight blast me.. something odd is going on around here...&n"
remove billet
~
#25607
Greeter3~
0 g 15
~
pemote %actor.name% 's gaze passes over you as he enters the area from the %direction%.| 's gaze passes over %actor.name% as he enters the area from the %direction%, his eyes looking about for any signs of trouble.
remove billet
~
#25608
Exiter~
0 i 15
~
emote moves along, his eyes flicking and darting about as he walks, occasionally fingering his billet as he exits from the area.
remove billet
~
#25609
Thief~
0 i 5
~
emote peers around looking for a victim to stick his knife into.
~
#25610
Thief2~
0 g 50
~
if (%actor.guild(21) == Member)
 backstab %actor.name%
else 
 mremember %actor.name%
end
~
#25611
Thief3~
0 o 100
~
pemote %actor.name% frowns at you in contemplation. "&WHaven't I seen you around here before?&n"
~
#25620
Enter~
2 c 100
enter~
if (%cmd% == enter)
%send% %actor.name% You have entered the Armory.
%teleport% %actor.name% 25686
end
~
#25621
exit~
2 c 100
*~
if (%cmd == leave || %cmd% == exit)
%send% %actor.name% You have left the Armory.&n
%teleport% %actor.name% 25631
 else
return 0
end
~
#25631
Ter'angreal~
1 d 100
~
eval cloak %actor.eq(about)%
 if (!%actor.isnpc%)
  if (%actor.level% > 199 || %cloak.vnum% == 1007 || %cloak.vnum% == 3607)
   %send% %self.worn_by% &RSomeone has entered the room!&n
 elseif (!%actor.isnpc% && %actor.sneaking%)
   %send% %self.worn_by% &R%actor.name% has entered the room from the %direction%!&n
else
 end
  end
   end
~
#25632
Tester~
1 d 100
~
eval cloak %actor.eq(about)%
if (!%actor.isnpc%)
 if (%actor.level% > 199 || %cloak.vnum% == 1007 || %cloak.vnum% == 3607)
   %send% %self.worn_by% &RSomeone has entered the room!&n
elseif (!%actor.isnpc% && %actor.sneaking%)
   %send% %self.worn_by% &R%actor.name% has entered the room!&n
else
 end
  end
    end
~
#25689
Greeter~
2 c 100
look~
wait 10 s
%echo% There is a certain muffled noise coming from behind one of the various weapon racks.
wait 5 s
%echo% Wait, it seems as if you can catch a glimpse of a short, fat man behind the weapon rack. Odd.
wait 5 s
%echo% The man speaks from behind the weapon rack, "Wow, you've found me.. perhaps you would like to see my specially forged weapon? One of a kind it is.. the sharpest of blades... perfect for a swordsman like yourself..."
~
#25690
Guarder~
2 g 100
~
\c00
~
#25700
Greeter~
0 g 100
~
mremember %actor.name% smirk
wait 5 s
*If there is a way for the mob to remember said player and not perform the actions on the greet, (The following actions that is) Lemme know and this mob can be fixed.
pemote %actor.name% nods his head at you and says to you in an oddly, italian sounding accent, "&WGreetings there, if you want to enter into the room behind me, well, I'll be charging you 500 Coins.&n"
wait 5
pemote %actor.name% smirks at you as he puts his hand out towards you, apparently waiting for you to fill his hand with coins.
~
#25701
Briber~
0 m 100
500~
if (%amount% >= 500)
 pemote %actor.name% nods his head towards you, smirking for a moment before turning about towards the door set into the eastern wall.
wait 10
unlock door east
open door east
wait 15 s
close door east
lock door east
else
 give %amount% coins %actor.name%
 pemote %actor.name% shakes his head at you after handing the coins back to you. "&WThis isn't enough for you to get into the room. Maybe you should go get the right amount before you come back.&n" His voice and italian accent trailing away.
mremember %actor.name% smirk
end
~
#25702
Guido's Suit~
1 j 100
~
if (%actor.name% == Guido)
 return 1
else
 return 0
end
~
#25703
Guido~
0 k 100
~
bash %actor.name%
stun %actor.name%
disarm %actor.name%
trip %actor.name%
dirtkick %actor.name%
push %actor.name% south
~
#25704
Entry~
2 g 100
~
%send% %actor.name% The sound of music echoes from the north, emanating from one of the rooms along the eastern side of the hallway, the music would surely be deafening if the door to it were opened, however with the door being closed the music sounds only a bit loud perhaps.
~
#25705
RoomTrig~
2 c 100
look~
%send% %actor.name% As you look about the room.. you notice oddly how, the mirror seems to refract light oddly. So, your curiousity wins the better of you, and.. you find yourself drawn to stare into its depths!
%force% %actor.name% emote gazes deeply into the depths of the odd mirror.
%force% %actor.name% sleep
%send% %actor.name% \\c08It feels as if someone is trying to draw you into the dream!\\c00
wait 2 s
%send% %actor.name% Suddenly, with a shriek you sit up!
%damage% %actor.name% 2500
wait 2 s
%force% %actor.name% wake
wait 2 s
%force% %actor.name% stand
~
$~
