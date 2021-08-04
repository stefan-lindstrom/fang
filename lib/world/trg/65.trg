#6500
Machin Shin (damage trig)~
0 k 100
~
if (%actor.level%  < 200)
  %send% %actor.name% The Black Wind howls, winding itself around you and wringing the life from your body.
  %echoaround% %actor.name% The Black Wind howls, winding itself around %actor.name%.
  %damage% %actor.name% 5000
else
  %send% %actor.name% You're lucky you an IMM.  *grin*
end
~
$~
