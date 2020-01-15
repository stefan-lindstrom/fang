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
1 c 100
look~
if %actor.name% != Tempus
 return 0
else 
return 1
end
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
Greeter~
0 g 100
~
if (%actor.name% == Tempus)
  curtsey %actor.name%
  pemote %actor.name% smiles lasciviously at you before saying in a soft voice, "&WIt is good to see you my Lord.&n"| smiles lasciviously at %actor.name%.
elseif (%actor.name% == Daene)
 curtsey %actor.name%
 pemote %actor.name% smiles lightly at you before saying, "&WGreetings, my lady.&n"| smiles lightly at %actor.name% before saying, "&WGreetings, my lady&n." to her.
elseif (%actor.name% == Chirion)
 spank %actor.name%
 wink %actor.name%
end
~
#11891
Tester trigger.~
0 c 100
at~
return 0
 %send% Tempus %actor.name% has attempted to %cmd% %arg%.
end
~
$~
