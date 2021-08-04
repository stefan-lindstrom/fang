#17559
Trapdoor Exit Trigger~
2 c 100
leave~
if (%actor.level% < 200)
   %echoaround% %actor.name% %actor.name% slips out through a trapdoor.
   %send% %actor.name% You slip out through the trapdoor.
   %teleport% %actor.name% 17554
   %echoaround% %actor.name% %actor.name% arrives through a trap door.
end
~
$~
