#11800
Tempus' Room Trig~
2 g 100
~
if %actor.level% < 200
 return 0
else
 return 1
~
#11801
Teddy#2~
0 g 100
~
if %actor.level% < 200
%send% %actor.name% &RYOU ARE NOT ALLOWED TO BE HERE.&n
%teleport% %actor.name% 6299
end
~
#11811
Teddy~
0 c 100
*~
if %arg% == recall
 %damage% %actor.name% 25000
else
 return 0
end
~
#11812
Daene's to Tempus-room~
2 d 100
Pat~
wdoor 12 up room 11889 flags ab name door north
%echo% \c08A bright blazing light creates a vertical slash in the air above you, and an odd anti-gravitational field seems to draw you upwards.\c00
%force% %actor.name% up
wait 10 s
wdoor 12 up purge
~
#11813
Teleporter~
1 c 3
gate~
if %arg% == tempus
 osend %actor.name% &RYou feel a strange, almost sickening sensation until you suddenly realize that you have been transported.&n
%tteleport% %actor.name% 11889
end
if %arg% == oval
 osend %actor.name% &RYou feel a strange, almost sickening sensation until you suddenly realize that you have been transported.&n
 %teleport% %actor.name% 10186
end
if %arg% == ooc
 osend %actor.name% &RYou feel a strange, almost sickening sensation until you suddenly realize that you have been transported.&n
 %teleport% %actor.name% 29906
end
if %arg% == platinum
 osend %actor.name% &RYou feel a strange, almost sickening sensation until you suddenly realize that you have been transported.&n
 %teleport% %actor.name% 848
end
~
#11850
Test~
0 c 100
*~
   if (%cmd% == Version)
if %arg%
%echoaround% %actor.name% %actor.name% fades from view!
%send% %actor.name% Suddenly you feel strange.
%teleport% %actor.name% %arg%
%send% %actor.name% You feel fine now.
%echoaround% %actor.name% %actor.name% slips in from outside the Pattern!
else
return 0
end
else
return 0
end
~
#11854
Ring~
1 ghijn 100
~
if (%actor.name% != Tempus)
 return 0
else
 return 1
end
~
#11855
Ring#2~
1 c 9
look~
if %actor.name% != Tempus
return 1
else 
return 0
end
~
#11875
Enterer~
2 g 100
~
if %direction% == up
wait 5
 %send% %actor.name% Now that you have entered through the secret trapdoor and entered into the dark depths of the tunnels beneath Voran's smithy, an odd sound comes from down the hallway, odd mainly because it is something that one would probably not expect to hear.
 wait 5
 %send% %actor.name% The sound of a lashing whip can be heard, cracking and snapping while only a moment after the sound of the whip crack a loud, almost animalistic scream can be heard, the victim of the lashing quite obviously a woman.
~
#11876
Enterer#2~
2 g 100
~
if %direction% == north
wait 5
 %send% %actor.name% The whip cracks again, and once more that animalistic, moaning scream can be heard from closer now, only a ways to the south. One might call the screem blood-curdling, others might call the scream music to one's ears. However the scream is loud and torturous, pain seeming to exude from every bit of the scream!
wait 5
 %send% %actor.name% Again, the whip cracks, lashing at the unseen woman who again screams in agony, pain evident in the long, drawn out lashing of the scream and as well, her voice can be made out after the scream, begging for mercy. "Please.... please... please... stop.. stop.. I'll do whatever you want.. just stop....!"
~
#11889
Room~
2 d 100
slartibarfast~
if %actor.name% == Koltur
 %teleport% %actor.name% 10186
elseif %actor.name% == Llewellyn
 %teleport% %actor.name% 10186
elseif %actor.name% == Terjon
 %teleport% %actor.name% 10186
elseif %actor.name% == Mat
 %teleport% %actor.name% 10186
else
return 1
end
~
#11890
Elsewise~
2 d 100
d~
wdoor 11889 north room 12 flags ab name door north
%force% %actor.name% north
wait 10 s
wdoor 11889 north purge
~
#11891
Elsewise2~
2 d 100
oval~
wdoor 11889 north room 10186 flags ab name door north
%force% %actor.name% north
wait 10 s
wdoor 11889 north purge
~
#11892
Test~
2 d 1
on~
attach 11890 %self.id%
%send% %actor.name% 11890 has been attached.
wait 5
detach 11890 %self.id%
%send% %actor.name% 11890 has been detached.
~
#11893
Protection~
2 c 100
*~
if (%actor.name% == Koltur || %actor.name% == Tempus || %actor.name% == Llewellyn)
return 0
elseif (%actor.name% == Daene || %actor.name% == Anibelle || %actor.name% == Ciramina)
return 0
elseif (%actor.name% == Deran || %actor.name% == Alyse || %actor.name% == Feithen)
return 0
else
return 1
%send% %actor.name% &RYou are not allowed to do that here!&n
%damage% %actor.name% 25000
wait 5 s
%damage% %actor.name% 25000
end
~
#11894
Protector2~
2 g 100
~
if (%actor.name% == Koltur || %actor.name% == Tempus || %actor.name% == Llewellyn)
return 1
elseif (%actor.name% == Daene || %actor.name% == Anibelle || %actor.name% == Ciramina)
return 1
elseif (%actor.name% == Deran || %actor.name% == Alyse || %actor.name% == Feithen)
return 1
else
return 0
%send% %actor.name% &RYou are not allowed to enter here!&n
~
#11898
Lightning~
1 c 7
trap~
   if (%cmd% == trap)
%send% %arg% &RA bolt of lightning strikes from the sky!&n
%echoaround% %arg% &rA bolt of lightning strikes down from the sky, arcing straight towards %arg%!&n
%damage% %arg% 25000
return 1
else
return 0
end
~
#11899
My Objects~
1 ghijlm 100
~
if (%actor.name% == Tempus) || (%actor.name% == Eilifir) || (%actor.name% == Patrick) || (%actor.name% == Korwyn) || (%actor.name% == slaadi) || (%actor.name% == Deran)
  return 1
else
  return 0
~
$~
