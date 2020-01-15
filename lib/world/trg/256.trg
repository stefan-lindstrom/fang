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
2 b 100
~
%send% %actor.name% The sound of music echoes from the north, emanating from one of the rooms along the eastern side of the hallway, the music would surely be deafening if the door to it were opened, however with the door being closed the music sounds only a bit loud perhaps.
~
$~
