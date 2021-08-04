#5000
Squirrel Nuts~
0 g 100
~
   if (%actor.name% == a daydreaming novice)
halt
   elseif (%actor.name% == a chattering accepted)
halt
elseif (%actor.vnum% == 5000)
halt
   else
    %echo% The \\c01c\\c03o\\c01p\\c08p\\c01e\\c03r\\c14-\\c01t\\c03a\\c08i\\c03l\\c01e\\c03d\\c00 squirrel chitters from the treetops, and tosses a nut at the intruder!  
end
~
#5001
Amyrlin's Guard~
0 c 100
open~
if (%actor.guild(3)% == No)
frown %actor.name%
wait 5
pemote %actor.name% says to you, "You have not been invited to enter."| says to %actor.name% "You have not been invited to enter."
shout %actor.name% has attempted entry into the Amyrlin's Office!
unsheathe %actor.name%
elseif (%actor.ranknum(3)% == 11)
wait 5
salute %actor.name%
say Light Illuminate, Mother.
open door south
wait 150
close door south
return 0
elseif (%actor.ranknum(3)% == 10)
wait 5
salute %actor.name%
say Light Illuminate, Keeper.
open door south
wait 150
close door south
return 0
else
wait 5
frown %actor.name%
say Sorry, you don't have an appointment.
end
~
#5002
Squirrel Follow~
0 c 100
scratch~
sit
~
#5005
WT Board Guardian~
0 g 100
~
         if (%actor.level% < 200)
if (%actor.guild(3)% == Member) || (%actor.guild(3)% == GL)
           wait 1
           pemote %actor.name% says to you, "Light Illumine, %actor.rank(3)%."
           else
           wait 1
           pemote %actor.name% frowns at you and says,"You don't belong here."
           wait 1
           pemote %actor.name% weaves a Gate and tosses you through it.
           wait 1
           %teleport% %actor.name% 10228
           end
         else
           wait 1
           smooch %actor.name%
         end
~
#5050
WT Guide - Library~
0 d 0
to the library~
wait 1
bow %actor.name%
wait 1
if (%actor.guild(3)% == Member)
  sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
else
  if (%actor.sex% == Male)
    sayto %actor.name% If you would follow me, sir?
  elseif (%actor.sex% == Female)
    sayto %actor.name% If you would follow me, miss?
  else
    sayto %actor.name% If you would follow me?
  end
end
wait 1
%force% %actor.name% follow tourguide
9w
sw
2s
e
bow %actor.name%
wait 2
if (%actor.guild(3)% == Member)
  sayto %actor.name% Here you are, %actor.rank(3)%.
else
  if (%actor.sex% == Male)
    sayto %actor.name% Here you are, sir.
  elseif (%actor.sex% == Female)
    sayto %actor.name% Here you are, miss.
  else
    sayto %actor.name% Here you are.
  end
end
%force% %actor.name% follow self
w
2n
ne
9e
~
#5051
Tower Guide - Grove~
0 d 0
to the grove~
wait 1
   bow %actor.name%
   wait 1
   if (%actor.guild(3)% == Member)
     sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
   else
     if (%actor.sex% == Male)
       sayto %actor.name% If you would follow me, sir?
     elseif (%actor.sex% == Female)
       sayto %actor.name% If you would follow me, miss?
     else
       sayto %actor.name% If you would follow me?
     end
   end
   wait 1
   %force% %actor.name% follow tourguide
   9w
   sw
   2s
   sw
   bow %actor.name%
   wait 2
   if (%actor.guild(3)% == Member)
     sayto %actor.name% Here you are, %actor.rank(3)%.
   else
     if (%actor.sex% == Male)
       sayto %actor.name% Here you are, sir.
     elseif (%actor.sex% == Female)
       sayto %actor.name% Here you are, miss.
     else
       sayto %actor.name% Here you are.
     end
   end
   %force% %actor.name% follow self
   ne
   2n
   ne
   9e
~
#5052
Tower Guide - Bridge~
0 d 0
to the bridge~
wait 1
   bow %actor.name%
   wait 1
   if (%actor.guild(3)% == Member)
     sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
   else
     if (%actor.sex% == Male)
       sayto %actor.name% If you would follow me, sir?
     elseif (%actor.sex% == Female)
       sayto %actor.name% If you would follow me, miss?
     else
       sayto %actor.name% If you would follow me?
     end
   end
   wait 1
   %force% %actor.name% follow tourguide
   9w
   nw
   n
   bow %actor.name%
   wait 2
   if (%actor.guild(3)% == Member)
     sayto %actor.name% Here you are, %actor.rank(3)%.
   else
     if (%actor.sex% == Male)
       sayto %actor.name% Here you are, sir.
     elseif (%actor.sex% == Female)
       sayto %actor.name% Here you are, miss.
     else
       sayto %actor.name% Here you are.
     end
   end
   %force% %actor.name% follow self
   s
   se
   9e
~
#5053
Tower Guide - Training Grounds~
0 d 0
to the training grounds~
wait 1
   bow %actor.name%
   wait 1
   if (%actor.guild(3)% == Member)
     sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
   else
     if (%actor.sex% == Male)
       sayto %actor.name% If you would follow me, sir?
     elseif (%actor.sex% == Female)
       sayto %actor.name% If you would follow me, miss?
     else
       sayto %actor.name% If you would follow me?
     end
   end
   wait 1
   %force% %actor.name% follow tourguide
      2n
      w
      n
      w
      2n
      e  
      2n
      2e
      bow %actor.name%
   wait 2
   if (%actor.guild(3)% == Member)
     sayto %actor.name% Here you are, %actor.rank(3)%.
   else
     if (%actor.sex% == Male)
       sayto %actor.name% Here you are, sir.
     elseif (%actor.sex% == Female)
       sayto %actor.name% Here you are, miss.
     else
       sayto %actor.name% Here you are.
     end
   end
   %force% %actor.name% follow self
  2w
2s
w
2s
e
s
e
2s
~
#5054
Tower Guide - Kitchen~
0 d 0
to the kitchen~
wait 1
   bow %actor.name%
   wait 1
   if (%actor.guild(3)% == Member)
     sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
   else
     if (%actor.sex% == Male)
       sayto %actor.name% If you would follow me, sir?
     elseif (%actor.sex% == Female)
       sayto %actor.name% If you would follow me, miss?
     else
       sayto %actor.name% If you would follow me?
     end
   end
   wait 1
   %force% %actor.name% follow tourguide
   3w
   s
   bow %actor.name%
   wait 2
   if (%actor.guild(3)% == Member)
     sayto %actor.name% Here you are, %actor.rank(3)%.
   else
     if (%actor.sex% == Male)
       sayto %actor.name% Here you are, sir.
     elseif (%actor.sex% == Female)
       sayto %actor.name% Here you are, miss.
     else
       sayto %actor.name% Here you are.
     end
   end
   %force% %actor.name% follow self
   n
  3e
  
~
#5055
Tower Guide - Dining Room~
0 d 0
to the dining room~
wait 1
   bow %actor.name%
   wait 1
   if (%actor.guild(3)% == Member)
     sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
   else
     if (%actor.sex% == Male)
       sayto %actor.name% If you would follow me, sir?
     elseif (%actor.sex% == Female)
       sayto %actor.name% If you would follow me, miss?
     else
       sayto %actor.name% If you would follow me?
     end
   end
   wait 1
   %force% %actor.name% follow tourguide
   3w
   n
   bow %actor.name%
   wait 2
   if (%actor.guild(3)% == Member)
     sayto %actor.name% Here you are, %actor.rank(3)%.
   else
     if (%actor.sex% == Male)
       sayto %actor.name% Here you are, sir.
     elseif (%actor.sex% == Female)
       sayto %actor.name% Here you are, miss.
     else
       sayto %actor.name% Here you are.
     end
   end
   %force% %actor.name% follow self
   s
  3e
  
~
#5056
Tower Guide - Amyrlin~
0 d 0
to the amyrlin~
wait 1
bow %actor.name%
wait 1
if (%actor.guild(3)% == Member)
sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
else
if (%actor.sex% == Male)
sayto %actor.name% If you would follow me, sir?
elseif (%actor.sex% == Female)
sayto %actor.name% If you would follow me, miss?
else
sayto %actor.name% If you would follow me?
end
end
wait 1
%force% %actor.name% follow tourguide
6w
2s
bow %actor.name%
wait 2
if (%actor.guild(3)% == Member)
sayto %actor.name% Here you are, %actor.rank(3)%.
else
if (%actor.sex% == Male)
sayto %actor.name% Here you are, sir.
elseif (%actor.sex% == Female)
sayto %actor.name% Here you are, miss.
else
sayto %actor.name% Here you are.
end
end
%force% %actor.name% follow self
2n
6e
~
#5057
Tower Guide - Stable~
0 d 0
to the stable~
wait 1
   bow %actor.name%
   wait 1
   if (%actor.guild(3)% == Member)
     sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
   else
     if (%actor.sex% == Male)
       sayto %actor.name% If you would follow me, sir?
     elseif (%actor.sex% == Female)
       sayto %actor.name% If you would follow me, miss?
     else
       sayto %actor.name% If you would follow me?
     end
   end
   wait 1
   %force% %actor.name% follow tourguide
   4w
   5s
   e
   bow %actor.name%
   wait 2
   if (%actor.guild(3)% == Member)
     sayto %actor.name% Here you are, %actor.rank(3)%.
   else
     if (%actor.sex% == Male)
       sayto %actor.name% Here you are, sir.
     elseif (%actor.sex% == Female)
       sayto %actor.name% Here you are, miss.
     else
       sayto %actor.name% Here you are.
     end
   end
   %force% %actor.name% follow self
   w
5n
  4e
  
~
#5058
Tower Guide - Pond~
0 d 0
to the pond~
wait 1
   bow %actor.name%
   wait 1
   if (%actor.guild(3)% == Member)
     sayto %actor.name% If you would follow me, %actor.rank(3)% %actor.name%?
   else
     if (%actor.sex% == Male)
       sayto %actor.name% If you would follow me, sir?
     elseif (%actor.sex% == Female)
       sayto %actor.name% If you would follow me, miss?
     else
       sayto %actor.name% If you would follow me?
     end
   end
   wait 1
   %force% %actor.name% follow tourguide
   9w
   nw
   4n
   bow %actor.name%
   wait 2
   if (%actor.guild(3)% == Member)
     sayto %actor.name% Here you are, %actor.rank(3)%.
   else
     if (%actor.sex% == Male)
       sayto %actor.name% Here you are, sir.
     elseif (%actor.sex% == Female)
       sayto %actor.name% Here you are, miss.
     else
       sayto %actor.name% Here you are.
     end
   end
   %force% %actor.name% follow self
   4s
se
  9e
  
~
#5059
Board Guard~
0 c 100
read~
if (%actor.guild(3)% == No)
wait 5
frown %actor.name%
shake %actor.name%
else
%force% %actor.name% read board
~
#5060
Tower Board Guardian~
0 g 100
~
   if (%actor.level% < 200)
     if (%actor.guild(3)% == Member) || (%actor.guild(3)% == GL) || (%actor.guild(3)% == Seeking)
     wait 1
     pemote %actor.name% says to you, "Light Illumine, %actor.rank(3)%."
     else
     wait 1
     frown %actor.name%
     wait 1
     %force% %actor.name% north
     end
   else
     wait 1
     emote whimpers loudly when the all powerful %actor.name% approaches.
   end
~
#5194
White Tower Board Blocker~
2 g 100
~
    if (%actor.level% < 200)
      if (%actor.guild(3)% == Member || %actor.guild(3)% == GL)
            return 1
         else
            %send% %actor.name% &u&WYou are not a allowed here.&n
            return 0
         end
      else
         return 1
      end
~
#5235
Ward Test Kinda Thing~
0 g 100
~
if (%actor.guild(3)% == GL)
wait 1
sayt %actor.name% it works!
end
~
$~
