#30200
Sewers trig 1 - Anit Escape (&RNOT WORKING&n)~
2 b 0
*~
* this should have been a no-escape trigger, but it's too much of a f-ing bitch to code, so it's not.  Tada.
~
#30201
Sewers Trig #2 - Door slammer~
2 g 100
~
if (%direction% == South )
  %echo% As you move into the sewers the wall behind you starts to crumble!  Oh no! The way back seems to be blocked!  It looks like you're going to have to find another way out.
else
return 1
end
~
#30202
Additional Mob Damage~
0 g 90
*~
if (%actor.level% == 199)
%send% %actor.name% &KThe horrible beast looks up as you arrive, the look in it's eyes illiciting a fear so strong it seems to suck the very life from your body.&n
%damage% %actor.name% 1000
else
end
~
#30203
Random Slipping~
2 g 10
~
 if (%actor.level% < 200)
if (%actor.vnum% == -1)
wait 5
%echoaround% %actor.name% %actor.name% suddenly slips on a puddle of slime, landing flat on his back with a loud -thud-.  That looked sore!
%send% %actor.name% Whoops!  One minute you're stepping in an innocent-looking puddle of slime, the next you're flat on your back staring at the ceiling.  That must have hurt!
%damage% %actor.name% 100
else
end
else
end
~
#30204
Guide-Bribe~
0 m 1
~
   if ( %amount% <= 10000 )
wait 1
shake
pemote %actor.name% smirks at you.  You obviously need to try a little more. | doesn't seem all that impressed.
else
wait 1
pemote %actor.name% smiles at you and beckons you closer.  He leans close to whisper softly, with a waft of foul-smelling breath, '&WDo you realise, you just gave me %amount% in coin for absolutely nothing?&n' | whispers something to %actor.name%.
~
#30205
Random Rocks~
2 g 10
~
    if (%actor.level% < 200)
if (%actor.vnum% == -1)
wait 1
%echoaround% %actor.name% %actor.name% suddenly trips over an outcrop of rock, falling face-first into the cave floor.  That looked sore!
%send% %actor.name% Whoops!  One minute you're walking innocently along the tunnel, the next you're slamming face first into the cave floor.  Those protruding rocks really are dangerous, that must have hurt!
%damage% %actor.name% 300
else
end
else
end
~
#30212
Collapsing Floor to 30202~
2 g 80
~
    if (%actor.level% < 200)
if (%actor.vnum% == -1)
wait 2
%teleport% %actor.name% 30202
%echoaround% %actor.name% %actor.name% suddenly slips into the channel of sewage and vanishes!
%send% %actor.name% Look out!  You don't see exactly what happens, but between one heartbeat and the next the floor seems to vanish from beneath you, and you find yourself falling.. to land with a thud somewhere on the next level.  You also now seem to be covered in sewage.  Lovely.
%damage% %actor.name% 1000
else
end
else
end
~
#30240
Collapsing Floor to 30253 ~
2 g 80
~
 if (%actor.level% < 200)
        if (%actor.vnum% == -1)
   wait 2
%teleport% %actor.name% 30253
   %echoaround% %actor.name% %actor.name% suddenly slips into the channel of sewage and vanishes!
   %send% %actor.name% Look out!  You don't see exactly what happens, but between one heartbeat and the next the floor seems to vanish from beneath you, and you find yourself falling.. to land with a thud somewhere on the next level.  You also now seem to be covered in sewage.  Lovely.
   %damage% %actor.name% 1000
   else
   end
   else
   end
~
#30266
Collapsing Floor to 30289~
2 g 99
~
 if (%actor.level% < 200)
        if (%actor.vnum% == -1)
   wait 2
%teleport% %actor.name% 30289
   %echoaround% %actor.name% %actor.name% suddenly slips on a rock and vanishes!
   %send% %actor.name% Look out!  You don't see exactly what happens, but between one heartbeat and the next the floor seems to vanish from beneath you, and you find yourself falling.. to land with a thud somewhere on the next level.  You also now seem to be covered in dust and mud.
   %damage% %actor.name% 1000
   else
   end
   else
   end
~
$~
