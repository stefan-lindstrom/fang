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
#3502
Rebel Sedai Board Guardian~
0 gi 100
~
if (%actor.level% < 200)
     if ((%actor.guild(50)% == Member) || (%actor.guild(50)% == GL))
     wait 1
     emote curtseys to %actor.name% and says,"&WLight Illumine.&n"
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
#3503
Enter Trig~
2 g 100
~
 if (%actor.level% < 200)
   if (%actor.guild(50)% == Member || %actor.guild(50)% == GL)
         return 1
      else
         %send% %actor.name% &R&uYou are not a allowed here.&n
         return 0
      end
   else
      return 1
   end
~
#3504
Guard~
2 g 100
~
    if (%actor.level% < 200)
if (%actor.guild(51)% == Member || %actor.guild(51)% == GL)
return 1
else
%send% %actor.name% &R&uYou are not a allowed here.&n
return 0
end
else
return 1
end 
~
#3550
Writhing snake trigger~
1 b 5
~
%echo% The snakes writhe and flail about, the miscellaneous tentacles whipping about at random!
wait 20
detach 3552 %self.id%
~
#3551
Writhing Snake Trig Part 2~
1 ghijl 10
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
1 j 100
~
\c00
~
#3622
room~
2 g 100
~
if %actor.name% == Koltur || %actor.name% == Llewellyn
wait 2 s
   wsend %actor.name% Greetings %actor.name%. You were welcome.
return 1
else
   %send% %actor.name% \\c08You are not allowed to be here.\\c00
   %send% %actor.name% \\c08Pain fills your being, your heart suddenly stops.\\c00
   %damage% %actor.name% 25000
wait 50
   %teleport% %actor.name% 6299
return 0
end
~
#3623
RoomTrig~
2 c 100
say~
if (%arg% == tempus)
 wdoor 3622 west room 11889
 %force% %actor.name% west
 wdoor 3622 west purge
end
if (%arg% == daene)
 wdoor 3622 west room 12
 %force% %actor.name% west
wdoor 3622 west purge
end
if (%arg% == oval)
 wdoor 3622 north room 10186
 %force% %actor.name% north
 wdoor 3622 north purge
end
if (%arg% == baerlon)
 wdoor 3622 west room 25600
 %force% %actor.name% west
 wdoor 3622 west purge
end
if (%arg% == whitebridge)
 wdoor 3622 west room 11800
 %force% %actor.name% west
 wdoor 3622 west purge
end
~
#3630
Rebel Board Blocker~
2 g 100
~
    if (%actor.level% < 200)
      if (%actor.guild(5)% == Member || %actor.guild(5)% == GL)
            return 1
         else
            %send% %actor.name% &u&BYou are not a allowed here.&n
            return 0
         end
      else
         return 1
      end
~
#3650
WarderBoard Guard Trigger~
0 g 100
~
if (%actor.level% < 200)
  if (%actor.guild(51)% == Member)
  wait 1
sayto %actor.name% &WLight Illumine %actor.rank(51)%.&n
  else
  wait 1
  frown %actor.name%
  wait 1
  %force% %actor.name% west
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
