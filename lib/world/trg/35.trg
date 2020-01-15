#3500
Joshine~
0 g 100
~
if (%actor.guild(5)% == No)
frown %actor.name%
pemote %actor.name% moves to stand before you.|moves to stand before %actor.name%.
elseif (%direction% == South)
nod %actor.name%
wait 5
pemote %actor.name% says in an impassive voice to you, "\c14Light Illumine you, \c23%actor.rank(5)%\c14 %actor.name%. It is well that you are returned to Salidar.\c00"
%echoaround% %actor.name% Joshine, the Salidar Guard says to %actor.name% in an impassive voice, "\\c14Light Illumine you \\c23%actor.rank(5)%\\c14    %actor.name%. It is well that you are returned to Salidar.\\c00"
elseif (%direction% == North)
nod %actor.name%
wait 5
pemote %actor.name% says in an impassive voice to you, "\c14Light Illumine you, \c23%actor.rank(5)%\c14 %actor.name%. Have a nice journey from Salidar.\c00"
%echoaround% %actor.name% Joshine, the Salidar Guard says to %actor.name% in an impassive voice, "\\c14Light Illumine you \\c23%actor.rank(5)%\\c14 %actor.name%. Have a nice journey from Salidar.\\c00"
end
~
#3501
Joshine#2~
0 c 100
n~
   if (%actor.guild(5)% == No)
frown %actor.name%
wait 5
pemote %actor.name% says to you, "\c14You are not allowed to pass.\c00"| says to %actor.name% "\c14You are \c00\c14not allowed to pass.\c00"
shout &WSomeone has attempted entry into Salidar! Guards!
&n unsheathe %actor.name%
return 0
 else
%force% %actor.name% north
~
#3550
Writhing snake trigger~
1 b 50
~
%echo% The snakes writhe and flail about, the miscellaneous tentacles whipping about at random!
wait 20
detach 3552 %self.id%
~
#3551
Writhing Snake Trig Part 2~
1 ghijl 100
~
%echoaround% %actor.name% The snakes writhe and flail about, the miscellaneous tentacles whipping about at random!
%send% %actor.name% You suddenly notice as one of the snakes nibbles on your bum!
%damage% %actor.name% 2
~
#3552
Detacher~
1 b 100
~
detach 3600 %self.id%
detach 6700 %self.id%
detach 6701 %self.id%
detach 12000 %self.id%
detach 12001 %self.id%
detach 1000 %self.id%
detach 18100 %self.id%
~
#3600
RW Guild EQ~
1 gj 100
~
   if (%actor.guild(51)% == Member)
return 1
else
return 0
   wait 2
   %echo% %actor.name% is \\c08NOT\\c00 Guilded in \\c14Rebel Warders!\\c00
   otransform 3580
attach 3550 %self.id%
attach 3551 %self.id%
attach 3552 %self.id%
~
#3622
room~
2 g 100
~
if (%actor.name% != Koltur)
wait 2 s
%echo% You are not allowed to be here %actor.name%!
wait 5
%damage% %actor.name% 2000
%teleport% %actor.name% 6252
end
if (%actor.name% == Koltur)
wait 2 s
%echo% Greetings %actor.rank(51)% %actor.name% it is good you have come.
wait 10
%echo% Welcome to your Inner Sanctum.
end
~
#3650
WarderBoard Guard Trigger~
0 g 100
~
if (%actor.level% < 200)
  if (%actor.guild(51)% == Member)
  wait 1
  sayto %actor.name% &WLight Illumine %actor.rank(51)% %actor.name%&n
  else
  wait 1
  frown %actor.name%
  wait 1
  %force% %actor.name% east
  end
else
  wait 1
  emote whimpers loudly when the all powerful %actor.name% approaches.
end
~
#3651
warderboard~
0 c 100
l~
if %actor.guild(51)% == Member
 return 1
else
 return 0
end
~
$~
