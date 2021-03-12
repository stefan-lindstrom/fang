#29900
Newbie Trainer #1~
0 g 100
~
wait 3
%echo% The old man smiles as you arrive, bowing, before saying, '&cAh.. greetings, my friend.. and welcome to the world of the Dragon's Fang!&n'
wait 3
%echo%  The trainer continues, '&cNow.. I am going to show you a few things here. First of all, if your terminal supports color you should type &Wcolor complete&c.&n'
wait3
%echo% The trainer stands and finishes, '&cWhy don't you go north from here? Just type &Wn&c, here, I'll come with you.&n'  The trainer smiles, and waits for you to move to the north.
~
#29901
Newbie Trainer #2~
0 g 100
~
wait 3
%echo% The trainer smiles and follows you.
Wait 3
%echo% A thug steps out of the shadows and growls as he sees you, 'What are you doing here? Gimme all your gold! And clothes! or else!'
wait 3
%echo% The trainer scratches his head.  'Hmm.. you need to defend yourself, my friend.. and it's easy to do so.'  The trainer looks serenely at you, 'In the lands of the dragon's fang, you will sometimes be attacked - all you need to do now is type &rkill thug&n, a nasty thing, but necessary my friend.  You can only kill mobs in this way, player killing is something else entirely.  For now, though, you should deal with this thug!'
wait 3
%echo% The trainer doesn't wait for you to kill the thug, but wanders off to the west to the next lesson.
~
#29902
Newbie Trainer #3~
0 g 100
~
wait 5
%echo% The trainer sits down on a rock and begins, '&cNow, let me explain what happens with levels.&n'
wait 8
%echo% The trainer says, '&cWhen you kill mobs (thugs, monsters, enemies generally) you gain &rexperience&c - as you get xps, you raise in &rlevels&c and you gain &rpractices&c and &rtrains&c.&n'
wait 8
%echo% The kind old man continues patiently, '&cWith trains you can buy new abilities for yourself - things like track, or backstab, or spells (we call them &rweaves&c, on Dragon's Fang).  As you go up in level you can gain better skills, by going to the Trainer and typing &rgain&c - you can find the trainer by going: &rrecall; down; down&c or by using &rnewbiegoto 16&c.&n'
wait 8
%echo% The kind old man says, '&cWith practices you can improve things you have already bought, up to 70%.&n'
wait 8
%echo%  The trainer stands, and says, '&cAlright.  If you want to know about channeling, go west, otherwise, continue on to the north.&n'
~
#29903
Newbie Trainer #3~
0 g 100
~
%send% %actor.name% The odd looking woman stands as you enter, and smiles brightly.  '&cWelcome! I have &n&cbeen asked to tell you a few things about the city you will soon enter. When you leave the school, you will find yourself in Caemlyn, the finest city in all Andor - although it's big, and often confusing, you can easily find your way around.&n'
%send% %actor.name% '&cWhen you are OOC, &n' the woman continues, '&cAnd even IC until level 40, you can  return &n&cto the central room in Caemlyn by typing &rrecall&c.  Many of the other important places can be found by typing &rhelp &n&rbasics&c.  One of the main places people tend to linger is the &WOval Plaza&c, found u/n/w from recall.  It's a great &n&cplace to start.'&n
%send% %actor.name% The woman smiles kindly, and gestures to the stairs leading back down to the city.  &n&c'Well done.  And may you enjoy your time on &yDragon's Fang&c!&n'
~
#29904
Newbie Trainer - Channie #1~
0 g 100
~
* Edited by Tempus to remove errors from syslog. I have removed the wait statements.
* Wait 5
%send% %actor.name%  The figure speaks quietly, '&WGreetings, seeker of knowledge.&n'
* Wait 10
%send% %actor.name%   The hooded figure continues, '&WFrom the trainer in Caemlyn you need to gain the skills &Rgrasp&W and &Rrelease&W, and practice both until they are as high as possible.&n'
* Wait 10
%send% %actor.name%    The hooded figure says, '&WTo cast spells (or perform weaves, as we say here on Fang) you must first be grasping the True Source - to grasp  simply type &Rgrasp&W, to release it, just type &Rrelease&n.'
* Wait 10
%send% %actor.name%    The hooded figure goes on, '&WWhen you have grasped, type &Rweave 'weave-name' (target)&W.  It really is that easy!&n'
* wait 10
%echo%   The hooded figure finishes, with a wolfish grin, '&WThere is much to learn, young apprentice.. and if wish to survive, type &Rhelp weave&W you will, and continue northeast.&n'
~
#29905
New Trigger~
0 g 100
~
if (!%actor.isnpc%)
  if (%actor.sneaking%)
    say Hey!  %actor.name% thinks %actor.heshe% can sneak around!
  end
end
~
#29998
Icebox Trig+2~
2 c 100
reply~
if (%actor.level% < 200)
%send% %actor.name% This function has been disabled.
return 1
 else
return 0
end
~
#29999
Icebox Trigger~
2 c 100
*~
   if (%cmd% == newb || %cmd% == newbi || %cmd% == newbie || %cmd% == newbieg)
return 1
%echo% &KThis Command has been disabled.&n
  else
return 0
end
   if (%cmd% == newbiego || %cmd% == newbiegot || %cmd% == newbiegoto)
return 1
%echo% &KThis Command has been disabled.&n
 else
return 0
end
if (%cmd% == rep || %cmd% == repl || %cmd% == reply)
 return 1
%echo% &KThis function has been disabled.&n
 else
return 0
end
~
$~
