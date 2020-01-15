#13000
BT GEQ Trigger - 1~
1 gj 100
~
if (%actor.level% < 200)
if (%actor.guild(8)% == Member || %actor.guild(8)% == GL)
      return 1
   else
      %send% %actor.name% You are not a member of the \\c22Black Tower\\c00!
      return 0
   end
else
   return 1
end
~
#13001
BT GEQ Trigger - 2~
1 j 100
~
if (%actor.level% < 200)
if (%actor.guild(8)% == Member || %actor.guild(8)% == GL)
    return 1
  else
    %send% %actor.name% %actor.name% isn't a member of the \\c22Black Tower\\c00!
    return 0
  end
else
  return 1
end
~
#13099
The Ways~
2 c 100
*~
*
* Waygate Trigger v.01 - September 9th, 2001
*
* Written for the Dragon's Fang MUD by Drathus (Greg Buxton)
*
* ----------------------------------------------------------
*
* This trigger works with the OPEN, CLOSE, and ENTER
* commands.
*
context 42
if (%cmd% == Open)
if (%arg% == Waygate || %arg% == Way || %arg% == Gate)
if (%gateopen% == 1)
%send% %actor.name% The Waygate is already open!
%echoaround% %actor.name% %actor.name% looks confused.
else
%send% %actor.name% You reach out and remove the Avendesora leaf from the mural faced Waygate, placing it in a different location in the scene.
%echoaround% %actor.name% %actor.name% reaches out towards the stone mural of the Waygate and removes the Avendesora leaf, and then places it in a different location in the scene.
%echo% &n
%echo% Slowly a thin line appears in the middle of the face of the Waygate.  Slowly the stone face opens revealing the Waygate beneath it.
eval gateopen 1
global gateopen
end
else
return 0
end
else
if (%cmd% == Close)
if (%arg% == Waygate || %arg% == Way || %arg% == Gate)
if (%gateopen% == 1)
%send% %actor.name% You reach out and move the Avendesora leaf to a different location in the scene.
%echoaround% %actor.name% %actor.name% reaches out and moves the Avendesora leaf to a different location in the scene.
%echo% &n
%echo% Slowly the two halves of the Waygate start to slide closed, until the unbroken scene lies in front of you.
eval gateopen 0
global gateopen
else
%send% %actor.name% The Waygate is already closed!
%echoaround% %actor.name% %actor.name% looks confused.
end  
else
return 0
end
else
if (%cmd% == Enter)
if (%arg% == Waygate || %arg% == Way || %arg% == Gate)
if (%gateopen% == 1)
* Fal Moran
if (%self.vnum% == 6451)
eval twaygate 6401
eval inout 1
end
if (%self.vnum% == 6401)
eval twaygate 6451
eval inout 0
end
* Fal Dara
if (%self.vnum% == 6452)
eval twaygate 6402
eval inout 1
end
if (%self.vnum% == 6402)
eval twaygate 6452
eval inout 0
end
* Tar Valon
if (%self.vnum% == 6403)
eval twaygate 6453
eval inout 0
end
if (%self.vnum% == 6453
eval inout 1
eval twaygate 6403
end
* Cairhien
if (%self.vnum% == 6404)
eval twaygate 6454
eval inout 0
end
if (%self.vnum% == 6454)
eval twaygate 6404
eval inout 1
end
* Aringill
if (%self.vnum% == 6405)
eval twaygate 6455
eval inout 0
end
if (%self.vnum% == 6455)
eval twaygate 6405
eval inout 1
end
* Stedding Shangtai
if (%self.vnum% == 6406)
eval twaygate 6456
eval inout 0
end
if (%self.vnum% == 6456)
eval twaygate 6406
eval inout 1
end
* Mayene
if (%self.vnum% == 6407)
eval twaygate 6457
eval inout 0
end
if (%self.vnum% == 6457)
eval twaygate 6407
eval inout 1
end
* Tear
if (%self.vnum% == 6408)
eval twaygate 6458
eval inout 0
end
if (%self.vnum% == 6458)
eval twaygate 6408
eval inout 1
end
* Illian
if (%self.vnum% == 6409)
eval twaygate 6459
eval inout 0
end
if (%self.vnum% == 6459)
eval twaygate 6409
eval inout 1
end
* Amador
if (%self.vnum% == 6410)
eval twaygate 6460
eval inout 0
end
if (%self.vnum% == 6460)
eval twaygate 6410
eval inout 1
end
* Salidar
if (%self.vnum% == 6413)
eval twaygate 6463
eval inout 0
end
if (%self.vnum% == 6463)
eval twaygate 6413
eval inout 1
end
* Two Rivers / Manetheren
if (%self.vnum% == 6414)
eval twaygate 6464
eval inout 0
end
if (%self.vnum% == 6464)
eval twaygate 6414
eval inout 1
end
* Stedding Tsofu
if (%self.vnum% == 6415)
eval twaygate 6465
eval inout 0
end
if (%self.vnum% == 6465)
eval twaygate 6415
eval inout 1
end
* Rhuidean
if (%self.vnum% == 6416)
eval twaygate 6466
eval inout 0
end
if (%self.vnum% == 6466)
eval twaygate 6416
eval inout 1
end
* Tanchico
if (%self.vnum% == 6417)
eval twaygate 6467
eval inout 0
end
if (%self.vnum% == 6467)
eval twaygate 6417
eval inout 1
end
* Falme
if (%self.vnum% == 6418)
eval twaygate 6468
eval inout 0
end
if (%self.vnum% == 6468)
eval twaygate 6418
eval inout 1
end
* Bandar Eban
if (%self.vnum% == 6419)
eval twaygate 6469
eval inout 0
end
if (%self.vnum% == 6469)
eval twaygate 6419
eval inout 1
end
* Baerlon
if (%self.vnum% == 6420)
eval twaygate 6470
eval inout 0
end
if (%self.vnum% == 6470)
eval twaygate 6420
eval inout 1
end
* Maradon
if (%self.vnum% == 6421)
eval twaygate 6471
eval inout 0
end
if (%self.vnum% == 6471)
eval twaygate 6421
eval inout 1
end
* Chachin
if (%self.vnum% == 6422)
eval twaygate 6472
eval inout 0
end
if (%self.vnum% == 6472)
eval twaygate 6422
eval inout 1
end
* Aridhol / Shadar Logoth
if (%self.vnum% == 6423)
eval twaygate 6473
eval inout 0
end
if (%self.vnum% == 6473)
eval twaygate 6423
eval inout 1
end
* Shol Arbola 
if (%self.vnum% == 6424)
eval twaygate 6474
eval inout 0
end
if (%self.vnum% == 6474)
eval twaygate 6424
eval inout 1
end
* Stedding Taijing
if (%self.vnum% == 6425)
eval twaygate 6475
eval inout 0
end
if (%self.vnum% == 6475)
eval twaygate 6425
eval inout 1
end
* Stedding Jinsuin
if (%self.vnum% == 6426)
eval twaygate 6476
eval inout 0
end
if (%self.vnum% == 6476)
eval twaygate 6426
eval inout 1
end
* Ebou Dar
if (%self.vnum% == 6411)
eval twaygate 6461
eval inout 0
end
if (%self.vnum% == 6461)
eval twaygate 6411
eval inout 1
end
* Caemlyn
if (%self.vnum% == 6412)
eval twaygate 6462
eval inout 0
end
if (%self.vnum% == 6462)
eval twaygate 6412
eval inout 1
end
* Now let's move them through the Waygate.
if (%inout% == 1)      
%send% %actor.name% Slowly you move forwards towards the shimmering silvery surface, and then slowly you walk through your reflection.  After what seems to be several minutes you stumble through into the darkness of the ways.
%echoaround% %actor.name% %actor.name% slowly walks towards the gateway and steps through it into the Ways.
else
%send% %actor.name% You move towards the dim surface of the Waygate, and step through it, leaving the darkness of the Ways behind.
%echoaround% %actor.name% %actor.name% walks through the Waygate and out of the Ways.
end
%teleport% %actor.name% %twaygate%
%echoaround% %Actor.name% %actor.name% steps out of the Waygate.
wait 10
eval gateopen 0
global gateopen
else
%send% %actor.name% You walk forward and run into the closed Waygate!
%echoaround% %actor.name% %actor.name% walks straight forward into the closed Waygate!
end
else
return 0  
end
else
return 0
end
end
end
~
#13125
M'Hael's Office Trigger~
2 c 100
*~
 if (%actor.level% < 200)
if (%actor.name% == Mazrim)
return 0
else
if (%cmd% == Grasp || %cmd% == Weave || %cmd% == Tieweave || %cmd% == Cast || %cmd% == Unknot || %cmd% == Unravel)
%send% %actor.name% Maybe that isn't a good idea...
else
return 0
end
end
end
~
#13126
Test Trig 49123847~
2 c 100
*~
if (%cmd% == Open)
  %send% %actor.name% You're trying to Open something...
  if (%arg% == Gate)
    %send% %actor.name% You're trying to Open a Gate!!
    %send% %actor.name% No!
  else
    %send% %actor.name% Ok... You're not trying to Open a Gate...
    return 0
  end
else
  return 0
end
~
#13127
M'Hael's Office - TIEWEAVE Trigger~
2 c 100
TIEWEAVE~
if (%actor.level% < 200)
if (%actor.name% == Mazrim)
return 0
else
%send% %actor.name% Perhaps you shouldn't do that here...
return 1
end
else
return 0
end
~
#13196
Test4~
2 d 100
test4~
%send% %actor.name% %self.vnum%
~
#13197
Test3~
2 d 100
Test1~
eval phaseone %speech.cdr%
%send% %actor.name% phaseone : %phaseone%, speech.cdr : %speech.cdr%
eval phasetwo %phaseone.car%
%send% %actor.name% phasetwo : %phasetwo%, phaseone.car : %phaseone.car%
%send% %actor.name% : %phasetwo% was entered.
~
#13198
Test2~
2 d 100
Test2~
context 69
%send% %actor.name% %number%
~
#13199
Test1~
2 d 100
Test1~
context 69
set number 5
%send% %actor.name% %number%
global number
~
#13201
Sphere Trigger~
1 g 100
~
%echoaround% %actor.name% pulls out a large &cc&Cr&Wyst&Ca&cl&n sphere and peers into it deeply.
%send% %actor.name% And awaaaaaaaaaay you go.
%teleport% %actor.name% 10186
%echoaround% %actor.name% appears suddenly, holding a &cc&Cr&Wyst&Ca&cl&n sphere.
%send% %actor.name% Here you are!
return 0
~
#13222
---~
0 d 100
wear~
%send% %actor.name% You're wearing %eq%
~
#13240
SayTo Trigger~
1 c 7
VERSION~
%force% %actor.name% sayto %arg.car% &B%arg.cdr%&n
~
#13241
SayTo Color Set~
1 c 7
VERSION~
%force% %actor.name% sayto %arg.car% &C%arg.cdr%&n
~
#13242
Will You Go?~
1 i 100
~
%send% %victim.name% &n
%send% %victim.name% Oh the summer time is coming, and the trees are sweetly bloomin'
%send% %victim.name% And wild mountain thyme, grows around the bloomin' heather.
%send% %victim.name% Will you go, lassie go?  And we'll all go together.
%send% %victim.name% To pick wild mountain thyme, all around the bloomin' heather.
%send% %victim.name% &n
%send% %victim.name% Will you go, lassie go?
%send% %victim.name% &n
%send% %victim.name% I will build my love a tower, by yon pure crystal fountian
%send% %victim.name% And on it I will pile, all the flowers of the mountain
%send% %victim.name% Will you go, lassie go?  And we'll all go together.
%send% %victim.name% TO pick wild mountain thyme, all around the bloomin' heather.
%send% %victim.name% &n
%send% %victim.name% Will you go, lassie go?
%send% %victim.name% &n
%send% %victim.name% If my true love she were gone, I would surely find another
%send% %victim.name% Where wild mountain thyme, grows around the bloomin' heather.
%send% %victim.name% Will you go, lassie go?  And we'll all go together.
%send% %victim.name% TO pick wild mountain thyme, all around the bloomin' heather.
%send% %victim.name% &n
%send% %victim.name% Will you go, lassie go?  And we'll all go together.
%send% %victim.name% To pick wild mountain thyme, all around the bloomin' heather.
%send% %victim.name% &n
%send% %victim.name% Will you go, lassie go?
%send% %victim.name% Will you go, lassie go?
%send% %victim.name% &n
~
#13251
Finale Firework Trigger~
1 h 100
~
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c01'\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 &K\\c00 \\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c01#.\\c08|\\c01.'\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c01-\\c08=\\c14x\\c08=\\c01-\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c01'\\c08|\\c01#\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c01'\\c07\\c00 \\c01#\\c07\\c00 \\c01#\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04|\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04#&c.:\\c07\\c00 \\c11|\\c07\\c00 &c:\\c04.'\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 &K_\\c14\/&K_\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04:&c:#\\c13.|.&c':\\c04:\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14/\\\c07
%echo% \\c00 \\c00 \\c00 \\c14+\\c07\\c00 \\c00 \\c00 \\c04--\\c11-=\\c13=\\c14+\\c13=\\c11=-\\c04--\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04:&c:.\\c13'|#&c.:\\c04:\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c01.\\c05\\\c12|\\c05/\\c01.\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04.'&c:\\c07\\c00 \\c11|\\c07\\c00 &c:\\c04#.\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c01-\\c12=\\c14x\\c12=\\c05-\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04\\c00 \\c00 &c\\c00 \\c07\\c00 \\c04|\\c07\\c00 &c\\c00 \\c04\\c00 \\c00 \\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c01'\\c05/\\c12|\\c05\\\c01#\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c02\\\c09|\\c02/\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c02-\\c09=\\c14+\\c09=\\c02-\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c02/\\c09|\\c02\\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 &K_\\c14\/&K_\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14/\\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14+\\c07\\c00 
~
#13290
Button Trigger~
1 c 4
PUSH~
if (%actor.level% < 200)
   if (%arg% == Button)
%echoaround% %actor.name% %actor.name% decides to push the button, and disappears in a puff of brimstone smelling smoke!
      %send% %actor.name% It said NOT to push it!
      %teleport% %actor.name% 6299
   end
end
~
#13291
Button Mk 2~
1 g 100
~
if (%actor.level% < 200)
%echoaround% %actor.name% %actor.name% gets a tube of red bone.
%send% %actor.name% You get a tube of red bone.
%echoaround% %actor.name% A blazing line suddenly appears before %actor.name%, splitting in two and slowly revolving outwards, forming a Gateway.
%send% %actor.name% A blazing line forms before you, slowly beginning to revolve outwards into a Gateway.
%send% %actor.name% Suddenly the tube of bone becomes too hot to hold!
%send% %actor.name% You drop a tube of red bone.
%echoaround% %actor.name% %actor.name% drops a tube of red bone.
%echoaround% %actor.name% %actor.name% mutters something softly.
%send% %actor.name% You hop around trying to cool your hand off, and stumble and fall backwards into the Gateway.
%echoaround% %actor.name% %actor.name% hops around waving a red hand, and all of a sudden stumbles and falls backwards into the open Gateway!
%send% %actor.name% The Gateway closes as you fall through, leaving you seconds to realize what you're falling towards.
%echoaround% %actor.name% A gateway dissappears in a flash of light!
%teleport% %actor.name% 6299
return 0
end
~
#13295
Test5~
2 c 100
*~
context 42
if (%cmd% == Open)
  if (%arg% == Waygate || %arg% == Way || %arg% == Gate)
    if (%gateopen% == 1)
      %send% %actor.name% The Waygate is already open!
      %echoaround% %actor.name% %actor.name% looks confused.
    else
      %send% %actor.name% You reach out and remove the Avendesora leaf from the mural faced Waygate, placing it in a different location in the scene.
      %echoaround% %actor.name% %actor.name% reaches out towards the stone mural of the Waygate and removes the Avendesora leaf, and then places it in a different location in the scene.
      %echo% &n
      %echo% Slowly a thin line appears in the middle of the face of the Waygate.  Slowly the stone face opens revealing the Waygate beneath it.
      eval gateopen 1
      global gateopen
    end
  else
    return 0
  end
else
  return 0
end
~
#13296
Drathus' Trig.~
1 j 100
~
%echoaround% %actor.name% %actor.name% falls through a hole!
%send% %actor.name% You feel a brief moment of disorientation.
%teleport% %actor.name% 32688
%echoaround% %actor.name% %actor.name% falls through a hole in the ceiling!
return 1
~
#13297
crash~
2 c 100
*~
* NO OUT!
      *
      * This trigger stops a Mort from Weave/TieWeave/Cast Gate/Skim
      *
      *
      * -written by Drathus, Daene just stole it.
      * 
      *
      if (%cmd% == weav || %cmd% == weave)
      if (%arg.car% == 'gat' || %arg.car% == 'gate || %arg.car% == 'gate')
      %send% %actor.name% You notice as you attempt to perform this weave that something seems to shatter your concentration, making it impossible to do this.
      else
      if (%arg.car% == 'sk' || %arg.car% == 'ski' || %arg.car% == 'skim || %arg.car% == 'skim')
      %send% %actor.name% You notice as you attempt to perform this weave that something seems to shatter your concentration, making it impossible to do this.
      else
      return 0
      end
      end
      else
      if (%cmd% == tiew || %cmd% == tiewe || %cmd% == tiewea || %cmd% == tieweav || %cmd% == tieweave)
      if (%arg.car% == 'gat' || %arg.car% == 'gate || %arg.car% == 'gate')
      %send% %actor.name% You notice as you attempt to perform this weave that something seems to shatter your concentration, making it impossible to do this.
      else
      if (%arg.car% == 'sk' || %arg.car% == 'ski' || %arg.car% == 'skim || %arg.car% == 'skim')   
      %send% %actor.name% You notice as you attempt to perform this weave that something seems to shatter your concentration, making it impossible to do this.
      else
      return 0
      end
      end
      else
      if (%cmd% == c || %cmd% == ca || %cmd% == cas || %cmd% == cast)
      if (%arg.car% == 'gat' || %arg.car% == 'gate || %arg.car% == 'gate')
      %send% %actor.name% You notice as you attempt to perform this weave that something seems to shatter your concentration, making it impossible to do this.
      else
      if (%arg.car% == 'sk' || %arg.car% == 'ski' || %arg.car% == 'skim || %arg.car% == 'skim')
      %send% %actor.name% You notice as you attempt to perform this weave that something seems to shatter your concentration, making it impossible to do this.
      else
      return 0
      end
      end   
      else
      if (%cmd% == newb || %cmd% == newbi || %cmd% == newbie || %cmd% == newbieg)    
      %send% %actor.name% You notice as you attempt to perform this action that something seems to shatter your concentration, making it impossible to do this.
      else
      if (%cmd% == newbiego || %cmd% == newbiegot || %cmd% == newbiegoto)
      %send% %actor.name% You notice as you attempt to perform this action that something seems to shatter your concentration, making it impossible to do this.
      else
      return 0
      end          
      else
      return 0
      end
      end
      end
      end
~
#13299
fun~
2 d 7
KILL~
if (%actor.name% != Mazrim)
  if (%actor.name% != Amarinda)
    if (%actor.level% < 210)
      %send% %actor.name% &WYou cannot do that.&n
    else
      %send% %speech.cdr% &RA bolt of lightning stabs out of the clear sky and impales you!&n
      %damage% %speech.cdr% 25000
    end
  else
    %send% %speech.cdr% &RA bolt of lightning stabs out of the clear sky and impales you!&n
    %damage% %speech.cdr% 25000
  end
else
  %send% %speech.cdr% &RA bolt of lightning stabs out of the clear sky and impales you!&n
  %damage% %speech.cdr% 25000
end
~
$~
