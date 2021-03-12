#1
memory test trigger~
0 i 100
~
* assign this to a mob, force the mob to mremember you, then enter the
* room the mob is in while visible (not via goto)
say I remember you, %actor.name%!
~
#2
mob greet test~
0 g 100
~
if %direction%
  say Hello, %actor.name%, how are things to the %direction%?
else
* if the character popped in (word of recall, etc) this will be hit
  say Where did YOU come from, %actor.name%?
end
~
#3
obj get test~
1 g 100
~
%echo% You hear, 'Please put me down, %actor.name%'
~
#4
room test~
2 g 100
~
wait 50
wsend %actor% you enter a room
~
#5
car/cdr test~
0 d 100
test~
say speech: %speech%
say car: %speech.car%
say cdr: %speech.cdr%
~
#6
subfield test~
0 c 100
test~
* test to make sure %actor.skill(skillname)% works
say your hide ability is %actor.skill(hide)% percent.
*
* make sure %actor.eq(name)% works too
eval headgear %actor.eq(head)%
if %headgear%
  say You have some sort of helmet on
else
  say Where's your headgear?
  halt
end
say Fix your %headgear.name%
~
#7
object otransform test~
1 jl 7
test~
* test of object transformation (and remove trigger)
* test is designed for objects 3020 and 3021
* assign the trigger then wear/remove the item
* repeatedly.
%echo% Beginning object transform.
if %self.vnum% == 3020
  otransform 3021
else
  otransform 3020
end
%echo% Transform complete.
~
#8
makeuid and remote testing~
2 c 100
testnow~
* makeuid test ---- assuming your MOBOBJ_ID_BASE is 200000,
* this will display the names of the first 10 mobs loaded on your MUD,
* if they are still around.
eval counter 0
while (%counter% < 10)
  makeuid mob 200000+%counter%
  %echo% #%counter%      %mob.id%   %mob.name%
  eval counter %counter% + 1
done
%echoaround% %actor% %actor.name% cannot see this line.
*
*
* this will also serve as a test of getting a remote mob's globals.
* we know that puff, when initially loaded, is id 200000. We'll use remote
* to give her a global, then %mob.globalname% to read it.
makeuid mob 200000
eval globalname 12345
remote globalname %mob.id%
%echo% %mob.name%'s "globalname" value is %mob.globalname%
~
#9
mtransform test~
0 g 100
~
* mtransform test
* as a greet trigger, entering the room will cause
* the mob this is attached to, to toggle between mob 1 and 99.
%echo% Beginning transform.
if %self.vnum%==1
  mtransform -99
else
  mtransform -1
end
%echo% Transform complete.
~
#10
attach test~
0 d 100
attach~
attach 9 %self.id%
~
#11
attach test~
0 d 100
detach~
detach 9 %self.id%
~
#12
spellcasting test~
0 c 100
kill~
* This command trigger will disallow anyone from trying to
* use the kill command, and will toss a magic missile at them
* for trying.
dg_cast 'magic missile' %actor%
return 0
~
#13
MakeUID Test~
0 g 100
~
if (%actor.entrance% == 0)
  say Hello, %actor.name%, long time no see :)
  say How are things to the %direction%?
  makeuid person %actor.id%
  eval entrance 1
  remote entrance %actor.id%
else 
  smile %actor.name%
end
~
#14
Another MakeUID test~
0 g 100
~
eval entrance 1
makeuid person %actor.id%
remote entrance %actor.id%
~
#15
New Trigger~
0 g 100
~
if (%actor.guild(9)% == No)
  emote peers at %actor.name% with narrow eyes.
else
  say May you find shade and water this day, %actor.rank(9)% %actor.name%.
  smile %actor.name%
end
~
#16
Tester by Tempus~
1 d 100
*~
if %speech% == ID
%send% %actor.name% %actor.eq(3)%
end
~
#17
Nightflower Trigger~
1 h 100
~
if (%self.vnum% == 13250 || %self.vnum% == 25164)
%echo% Suddenly %actor.name% lights a large firework...
%echo% \\c00
%echo% \\c00\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c02n\\c04a\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c02w\\c03w\\c02w\\c04a\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c02w\\c03w\\c01w\\c03w\\c02w\\c04a\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c02nn\\c03nnn\\c02nn\\c04a\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04.ada.\\c07\\c00 \\c00 \\c00 \\c00 \\c04n\\c02nn\\c01!\\c02nn\\c04n\\c07\\c00 \\c00 \\c00 \\c00 \\c04.ada.\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04.aa\\c02nnnn\\c04aa\\c07\\c00 \\c00 \\c04ann\\c01!!!\\c04nna\\c07\\c00 \\c00 \\c04aa\\c02nnnn\\c04aa.\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04<aa\\c02nnn\\c03wwww\\c02wwv\\c03i.\\c01am\\c03#\\c01s\\c03#\\c01ma\\c03.i\\c02vww\\c03wwww\\c02nnn\\c04aa>\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04'-\\c02nnw\\c03w\\c01w\\c03ww\\c02wwwwa\\c07\\c00 \\c03v\\c07\\c00 \\c01-\\c07\\c00 \\c03v\\c07\\c00 \\c02awwww\\c03ww\\c01w\\c03w\\c02wnn\\c04-'\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04'-g\\c02gg\\c03ggg\\c02lv\\c07\\c00 \\c03a\\c07\\c00 \\c00 \\c03\\c00 a\\c07\\c00 \\c02vj\\c03ggg\\c02gg\\c04g-'\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04'\\c02---\\c04'\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04.add\\c02ama\\c03a\\c07\\c00 \\c01:\\c03-\\c01-\\c03-\\c01:\\c07\\c00 \\c03a\\c02ama\\c04dda.\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c02ammmmm\\c04v\\c02v\\c03i\\c07\\c00 \\c01:::\\c07\\c00 \\c03i\\c02v\\c04v\\c02mmmmm\\c04aa\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c02mm\\c03mmm\\c02m\\c04v\\c07\\c00 \\c02v\\c03v\\c07\\c00 \\c01a\\c03.\\c01c\\c07\\c00 \\c03v\\c02v\\c07\\c00 \\c04v\\c02m\\c03mmm\\c02mm\\c04c\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c046\\c02m\\c03mmm\\c02mm\\c04v\\c07\\c00 \\c00 \\c01-\\c02v\\c01v\\c03a\\c01v\\c02v\\c01-\\c07\\c00 \\c00 \\c04v\\c02mm\\c03mmm\\c02m\\c046\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04a\\c02mm\\c03m\\c02m\\c04mb\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c01-\\c07\\c00 \\c01-\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c04gm\\c02m\\c03m\\c02mm\\c04a\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04m\\c02mmm\\c04mj\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04um\\c02mmm\\c04m\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04m\\c02mm\\c04m-\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04-m\\c02mm\\c04m\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04v\\c02m\\c04v\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04v\\c02m\\c04v\\c07\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04v\\c07\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c04v\\c07\\c00
%echo% \\c00
otransform 13249
end
~
#27
New Trigger~
1 gj 100
~
     if (%actor.level% > 200)
            return 1
         else
%send% %actor.name% &R&fYou are not an IMM!&n
            return 0
         end
~
#41
Chris's trig~
2 c 100
*~
if %actor.name% != Chris
  %send% %actor.name% &KA Black Robed Myrddraal lunges from the shadows, pale hands reaching out hungrily to wrap about your throat as he slowly throttles you to death!&n
  return 1
end
~
#44
Puff's Muzzle~
2 c 100
Say~
if (%actor.name% != Puff)
  return 0
end
~
#69
An innocent trigger, really !~
0 c 100
say~
if (%arg% == suck)
wait 1 s
emote gets down on her knees, eyes fixed on %actor.name%.
wait 1 s
emote slowly takes off %actor.name%'s pants, yanking out his hard tool.
wait 3 s
emote smiles sweetly, opens her mouth and starts working.
wait 3 s
emote continues her demanding work.
wait 3 s
emote finishes %actor.name% off, licking her lips when done.
end
if (%arg% == kiss)
wait 1 s
emote steps forwards and places her body against %actor.name%
wait 1 s
emote moves closer, pressing her large breasts against %actor.name%'s chest.
wait 1 s
emote wraps her arms around %actor.name%'s shoulders, giggling as she bounces slightly.
wait 1 s
emote presses her lips against %actor.name%'s, using her tongue to full advantage.
wait 1 s
emote finishes kissing %actor.name% and steps back, smiling at %actor.name%
end
if (%arg% == strip)
wait 1 s
emote smiles happy at %actor.name%, "Of course, Master!"
wait 1 s
emote almost hurriedly begins stripping off her clothes revealing the large mounds of her breasts as well as her other womanly attributes.
wait 1 s
emote bounces up and down as she gazes at %actor.name% with lust evident in her eyes.
wait 1 s
%force% %actor.name% ogle partygirl
emote smiles lasciviously at %actor.name% before moving closer, swaying her hips all the while.
end
~
$~
