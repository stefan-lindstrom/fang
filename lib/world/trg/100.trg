#10019
Aiel Trainer~
0 g 100
~
if (%actor.level% <= 20)
%send% %actor.name% The Aiel trainer exclaims, 'Greetings, %actor.name%. Please type '&Rgain&n' to see what new you can learn right now!'
wait 3 s
%send% %actor.name% The Aiel trainer exclaims, 'Or, if you want to improve what you already know, type '&RPractice&n'!'
wait 3 s
%send% %actor.name% The Aiel trainer exclaims, 'To convert trains to practices, just type '&Rread sign&n'!'
wait 3 s
bow %actor.name%
smile %actor.name%
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
%bow% %actor.name%
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
%bow% %actor.name%
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
%bow% %actor.name%
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
%bow% %actor.name%
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
%bow% %actor.name%
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
%bow% %actor.name%
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
%bow% %actor.name%
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
$~
