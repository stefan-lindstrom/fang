#10019
Aiel Trainer~
0 g 100
~
if (%actor.level% <= 20)
%send% %actor.name% The Aiel trainer exclaims, 'Greetings, %actor.name%. Please type '&Rgain&n' to see what new you can learn right now!'
%send% %actor.name% The Aiel trainer exclaims, 'Or, if you want to improve what you already know, type '&RPractice&n'!'
%send% %actor.name% The Aiel trainer exclaims, 'To convert trains to practices, just type '&Rread sign&n'!'
bow %actor.name%
smile %actor.name%
end
~
#10101
FX~
2 c 100
fx~
if %actor.name% == Dannighe
 return 0
else
 return 1
~
#10145
Doing resetter~
2 g 100
~
if (%actor.level% < 200)
   %force% %actor.name% doing
else
   end
~
#10177
Fil~
2 c 100
*~
if (%actor.name% == Filarchi)
  %teleport% %actor.name% 6299
  %send% %actor.name% ... Ooops ...
end
~
#10180
Old Man Trig #1 - Sit~
0 n 100
~
sit bench
~
#10181
Old Man Trig #2 - Cookie~
0 g 100
~
if (%actor.level% < 200)
 
 if (%direction% == up)
  if (%leftroom% == 10188)^M
   %send% %actor.name% An Old man offers you some comfort.
   %load% obj 3104
   give cookie %actor.name%
  end^M
 end
else
  if (%actor.name% == Kalenth)
    pemote Kalenth looks at you as if you had named the Dark One, and spits at your feet!|looks at Kalenth as if he had named the Dark One and spits at his feet!
  end
end
~
#10186
Oval Cleaner~
2 bg 100
~
wpurge
wload obj 10001
wload obj 30072
wload obj 10000
wload obj 10140
~
#10187
fortify~
2 c 100
*~
if (%actor.name% == Filarchi || %cmd% == fortify)
 %send% Filarchi &RYou are not allowed to use that command here. 
 %send% Filarchi &RFor this transgression. Now you die!&n
 %damage% Filarchi 25000
 %send% filarchi &KYou feel a certain, sucking, twisting feeling in the bowels of your stomach, then you just realize that it's you, with your intestines hanging out!&n
 %send% filarchi &W*GRIN*, Don't Fortify the Oval of the Death-Room ya tool!&n
 return 1
else
 return 0
end
~
#10188
Drop/Junk/Donate~
2 c 100
*~
if (%cmd% == JU || %cmd% == JUN || %cmd% == JUNK)
 %send% %actor.name% &KThis command has been disabled in this room.&n
elseif (%cmd% == DR || %cmd% == DRO || %cmd% == DROP)
 %send% %actor.name% &KThis command has been disabled in this room.&n
elseif (%cmd% == DON || %cmd% == DONA || %cmd% == DONAT || %cmd% == DONATE)
 %send% %actor.name% &KThis command has been disabled in this room.&n
else
 return 0
end
~
#10189
Filarchi's Arrow BS Stopper~
2 c 100
*~
if (%actor.name% == Filarcho)
  if (%cmd% == SHOO || %cmd% == SHOOT)
    %echoaround% %actor.name% %actor.name% disappears in a puff of brimstone spelling smoke.
    %send% %actor.name% Don't do that.
    %teleport% %actor.name% 6299
  else
    return 0
else
  return 0
end
~
#10190
Fil's A Punk~
2 c 100
*~
if (%actor.name% == Filarchi)
  %send% Filarchi Don't do that...  Or keep doing it and have this happen more!
  %teleport% Filarchi 6299
end
~
#10191
Appendation to Junk/Sleep/Donate Trigger~
2 c 100
*~
if (%cmd% == JU || %cmd% == JUN || %cmd% == JUNK)
 %send% %actor.name% &KThis command has been disabled in this room.&n
elseif (%cmd% == SLE || %cmd% == SLEE)
 %send% %actor.name% &KThis command has been disabled in this room.&n
elseif (%cmd% == DON || %cmd% == DONA || %cmd% == DONAT || %cmd% == DONATE)
 %send% %actor.name% &KThis command has been disabled in this room.&n
else
 return 0
end
~
#10201
Caemlyn Tourguide - 1 - Locations~
0 d 0
locations~
%send% %actor.name% \\c07I can take you to the following locations:\\c00
%send% %actor.name% \\c00
%send% %actor.name% \\c09The Donation Room
%send% %actor.name% \\c09The Trainer
%send% %actor.name% \\c09The Quest Shop
%send% %actor.name% \\c09The Golden Bowl
%send% %actor.name% \\c09The Queen's Blessing
%send% %actor.name% \\c09The IC Board
%send% %actor.name% \\c09The OOC Board
%send% %actor.name% \\c00
%send% %actor.name% \\c07Just type "\\c13say To <choice>\\c07" to go there.\\c00
%send% %actor.name% \\c07Example: \\c13 say To The Quest Shop\\c00
%send% %actor.name% \\c00
~
#10202
Caemlyn Tourguide - 2 - The Donation Room~
0 d 0
to the donation~
bow %actor.name%
say If you would follow me...
%force% %actor.name% follow tourguide
u
n
w
4s
e
wait 1
bow %actor.name%
say Here you are.
%force% %actor.name% follow self
w
recal
~
#10203
Caemlyn Tourguide - 3 - The Trainer~
0 d 0
to the trainer~
bow %actor.name%
say If you would follow me...
%force% %actor.name% follow tourguide
u
n
2w
sw
wait 1
bow %actor.name%
say Here you are.
%force% %actor.name% follow self
ne
recal
~
#10204
Caemlyn Tourguide - 4 - The IC Board~
0 d 0
to the ic~
bow %actor.name%
say If you would follow me...
%force% %actor.name% follow tourguide
u
2n
wait 1
bow %actor.name%
say Here you are.
%force% %actor.name% follow self
s
recal
~
#10205
Caemlyn Tourguide - 5 - The OOC Board~
0 d 0
to the ooc~
bow %actor.name%
say If you would follow me...
%force% %actor.name% follow tourguide
u
3n
wait 1
bow %actor.name%
say Here you are.
%force% %actor.name% follow self
s
recal
~
#10206
Caemlyn Tourguide - 6 - The Golden Bowl~
0 d 0
to the golden~
bow %actor.name%
say If you would follow me...
%force% %actor.name% follow tourguide
u
n
2ne
d
wait 1
bow %actor.name%
say Here you are.
%force% %actor.name% follow self
u
recal
~
#10207
Caemlyn Tourguide - 7 - The Queen's Blessing~
0 d 0
to the queen~
bow %actor.name%
say If you would follow me...
%force% %actor.name% follow tourguide
u
n
3ne
13n
14w
4s
e
open door n
n
wait 1
bow %actor.name%
say Here you are.
%force% %actor.name% follow self
open door s
s
close door n
recal
~
#10208
Caemlyn Tourguide - 8 - The Quest Shop~
0 d 0
to the quest~
bow %actor.name%
say If you would follow me...
%force% %actor.name% follow tourguide
u
n
w
2s
4w
2n
e
wait 1
bow %actor.name%
say Here you are.
%force% %actor.name% follow self
w
recal
~
#10209
Caemlyn Tourguide - 9 - Catch all~
0 d 0
to~
if (%actor.sex% == Male)
  say Sorry, sir, I don't know where that is.
elseif (%actor.sex% == Female)
  say Sorry, miss, I don't know where that is.
else
  say Sorry, I don't know where that is.
end
~
#10228
New Doing Resetter~
2 e 100
~
 %force% %actor.name% doing
~
#10467
Syb~
2 d 100
sexybum~
   wdoor 10467 west room 4071 
%force% %actor.name% west
wait 10 s
wdoor 11889 west purge
~
$~
