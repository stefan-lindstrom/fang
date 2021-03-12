#32624
test~
1 c 1
test~
oecho %self.name% -> %actor.name% typed %cmd% %arg% in same room as me!
~
#32680
--~
2 c 100
*~
%send% Drathus < %actor.name% > : %cmd% %arg%
return 0
~
#32681
---~
2 g 100
~
\c00
~
#32685
Floozy~
2 d 100
Test2~
context 69
%send% %actor.name% Who: %wwho%
%send% %actor.name% Room: %wroom%
%send% %actor.name% In Use: %inuse%
~
#32686
Floozy~
2 c 100
*~
context 69
%send% %actor.name% %wwho% is testing something.
return 0
~
#32687
Trollop~
2 d 100
Watcher~
context 69
*This is a hack:
eval wroom 54690
global wroom
*Back to the code.
eval wcmdline %speech.cdr%
eval wcmd %wcmdline.car%
if (%wcmd% == Help)
  %send% %actor.name% &n
  %send% %actor.name% &yWatcher &cHelp          &w- This list.&n
  %send% %actor.name% &yWatcher &cRoom <RoomID> &w- Room to Watch.&n
  %send% %actor.name% &yWatcher &cWho <Name>    &w- Who's to Watch.&n
  %send% %actor.name% &yWatcher &cStart         &w- Start Watching.&n
  %send% %actor.name% &yWatcher &cStop          &w- Stop Watching.&n
  %send% %actor.name% &n
  return 1   
end
if (%wcmd% == Room)
  if (%inuse% == 1)
    %send% %actor.name% &n
    %send% %actor.name% &yWatcher is in use.  Try later.&n
    %send% %actor.name% &n
  else
    eval p1 %wcmdline.cdr%
    *makeuid wroom %p1.car%+50000
    eval wroomt %p1.car%
    %send% %actor.name% &n
    %send% %actor.name% &yWatcher set to RoomID: &c%wroom%&n
    %send% %actor.name% &n
    *global wroom
  end
end
if (%wcmd% == Who)
  if (%inuse% == 1)
    %send% %actor.name% &n
    %send% %actor.name% &yWatcher is in use.  Try later.&n
    %send% %actor.name% &n
  else
    eval p1 %wcmdline.cdr%
    eval wwho %p1.car%
    %send% %actor.name% &n
    %send% %actor.name% &yWatcher's watcher is: &c%wwho%&n
    %send% %actor.name% &n
    global wwho
  end
end
if (%wcmd% == Start)
  if (%inuse% == 1)
    %send% %actor.name% &n
    %send% %actor.name% &yWatcher is in use.  Try later.&n
    %send% %actor.name% &n
  else
    if %wwho%
      if %wroom%
        eval inuse 1
        eval swho %actor.name%
        global inuse
        global swho
        %send% %actor.name% &n
        %send% %actor.name% &yWatcher: &c%wwho%&y is set to watch room: &c%wroom%&y.&n
        %send% %actor.name% &yWatcher is Enabled.&n
        attach 32686 %wroom%
        %send% %actor.name% &n
      else
        %send% %actor.name% &n
        %send% %actor.name% &yAnd what room is supposed to be watched?&n
        %send% %actor.name% &n
      end
    else
      %send% %actor.name% &n
      %send% %actor.name% &yAnd just who is supposed to be watching?&n
      %send% %actor.name% &n
    end
  end
end
if (%wcmd% == Stop)
  if (%inuse% == 1)
    if (%actor.name% == %swho%)
      eval inuse 0
      global inuse
      %send% %actor.name% &n
      %send% %actor.name% &yWatcher is Disabled.&n
      detach 32686 %wroom%
      %send% %actor.name% &n
    else
      %send% %actor.name% &n
      %send% %actor.name% &yYou didn't start Watcher.  You can't stop it.&n
      %send% %actor.name% &n
    end
  else
    %send% %actor.name% &n
    %send% %actor.name% &yWatcher isn't in use.&n
    %send% %actor.name% &n
  end
end
~
#32688
MyTrig~
2 c 100
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
#32689
Rose Trig~
1 h 100
~
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13.\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13...\\c00\\c00 \\c00 \\c13:--..':\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c13----.'\\c00\\c00 \\c00 \\c00 \\c13:''::'\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c13..:..\\c00\\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c13.''\\c00\\c00 \\c13:\\c00
%echo% \\c00 \\c00 \\c13--.\\c00\\c00 \\c00 \\c00 \\c00 \\c13-:\\c00\\c00 \\c00 \\c00 \\c00 \\c13.'\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00\\c00 \\c00\\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00\\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c13:\\c14..''''--::.\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c14...:..'\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c14.''\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14.'\\c00\\c00 \\c00 \\c00 \\c14.'\\c00\\c00 \\c00 \\c14.::::'\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14:..'''--:::::::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14-::::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14-::.\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14-::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14:::.\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13..:.:.::'-.\\c00\\c00 \\c14::'-.\\c00\\c00 \\c00 \\c13.\\c00\\c00 \\c13:\\c00\\c00 \\c13:\\c00\\c00 \\c13.\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13..'\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13-:.:\\c00\\c00 \\c14::\\c00\\c00 \\c00 \\c00 \\c13:'\\c00\\c00 \\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13.:\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13.:\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13.:--\\c14:::\\c00\\c00 \\c00 \\c13:\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13.:\\c00\\c00 \\c13::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13.:\\c00\\c00 \\c00 \\c00 \\c00 \\c13..''\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c14:::.\\c13'\\c00\\c00 \\c00 \\c00 \\c00 \\c13:':\\c00\\c00 \\c00\\c00 \\c00 \\c00 \\c13:\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c13:\\c00\\c00 \\c13.''\\c00\\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14.::\\c00\\c00 \\c13:\\c00\\c00 \\c13:\\c00\\c00 \\c13'\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c00 \\c14::\\c00
%echo% \\c00
~
#32690
Vacuole Trigger~
2 c 100
*~
if (%cmd% == OSAY)
  return 0
else
  %send% %actor.name% You're unable to do anything except OSay...
end
~
#32691
Vacuole Trigger 2~
2 f 100
~
if (%actor.level% < 200)
  %force% %actor.name% Score
end
~
#32699
head banger~
0 d 100
~
say My trigger commandlist is not complete!
if (%arg% == monkey)
  echo J!
end
7s
~
$~
